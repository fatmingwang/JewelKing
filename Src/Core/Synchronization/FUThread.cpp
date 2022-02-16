#include "../stdAfx.h"
#include "FUThread.h"

#if defined(WIN32)
#	include <process.h>
#	define cFUThread_INVALID NULL
#elif defined(FP_APPLE)
#	define cFUThread_INVALID kInvalidID
#elif defined(FP_LINUX) || defined(FP_QNXNTO)||defined(__IPHONE__)
#	include <pthread.h>
#	define cFUThread_INVALID 0
#endif

//
// cFUThread - Local Structure.
//

//Platform-specific data to pass to the internal thread.
struct cFUThreadInternalParam
{
	cFUThread* object;
#if defined(WIN32)
	void* parameter;
	TaskProc func;
#elif defined(FP_APPLE)
	void* parameter;
	OSStatus (*func)(void*);
#elif defined(FP_LINUX) || defined(FP_QNXNTO)||defined (__IPHONE__)
	void* parameter;
	TaskProc func;
#endif
};

//
// cFUThread
//

//This class doesn't let the user change the priority of the thread. There is a danger
// of such a thing called 'Priority Inversion' which caused the Mars Lander to fail.
// However, if we choose to let the user change the priority of the thread, use
// the function MPSetTaskWeight() on Mac and SetThreadPriority() on Windows.

cFUThread::cFUThread(THREAD_ROUTINE_POINTER startAddress, void* parameter, bool suspended, const char* szName)
:	isSuspended(suspended)
,	isSuspendedCS()
,	semaphore()
,	threadStartedSemaphore()
,	terminate(false)
,	exitCode(0)
#ifdef FP_APPLE
,	queue(cFUThread_INVALID)
#endif
,	name()
,	thread(cFUThread_INVALID)
{
	if (szName != NULL)
	{
		name = szName;
	}

	//Create the structure from the function parameters.
	cFUThreadInternalParam* params = new cFUThreadInternalParam;
	params->object = this;
	params->parameter = parameter;
	params->func = startAddress;
#if defined(WIN32)
	unsigned int ThreadId;
	thread = (HANDLE)_beginthreadex(NULL, 0, InternalFunc, params, 0, &ThreadId);
	threadStartedSemaphore.Down();

#elif defined (FP_APPLE)
	if (MPLibraryIsLoaded())
	{
		OSStatus status = MPCreateQueue(&queue);
		if (status == noErr)
			status = MPCreateTask(&InternalFunc, params, 0, queue, &exitCode, NULL, 0, &thread);
	}
#elif defined(FP_LINUX) || defined(FP_QNXNTO)
	int status = pthread_create(&thread, NULL, InternalFunc, params);
	if (status == 0) // Success
		threadStartedSemaphore.Down();
#endif
}

cFUThread::~cFUThread()
{ }

void cFUThread::StartThread()
{
	isSuspendedCS.Enter();
	if (isSuspended)
	{
		semaphore.Up();
		isSuspended = false;
	}
	isSuspendedCS.Leave();
}

void cFUThread::StopThread()
{
	assert(thread != cFUThread_INVALID);
	isSuspendedCS.Enter();
	terminate = true;
	bool _isSuspended = isSuspended;
	isSuspendedCS.Leave();
	if (_isSuspended)
		StartThread(); //The real function will not start because terminate = true;
#if defined(WIN32)
	WaitForSingleObject(thread, INFINITE);
	CloseHandle(thread); // delete the thread once it's finished
#elif defined (FP_APPLE)
	OSStatus status;
	//MPTerminateTask(thread, status);
	status = MPWaitOnQueue(queue, NULL, NULL, NULL, kDurationForever);
	status = MPDeleteQueue(queue);
#elif defined(FP_LINUX) || defined(FP_QNXNTO)
	pthread_join(thread, NULL);
#endif
	thread = cFUThread_INVALID;
}

void cFUThread::YieldCurrentThread()
{
#if defined(WIN32)
	//fuck
	//
	assert(0);
//	SwitchToThread();
#elif defined (FP_APPLE)
	//Below is a CoreFoundation call that I'm not sure about.
	//CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0, true);
	MPYield();
#elif defined(FP_LINUX)
	pthread_yield();
#elif defined(FP_QNXNTO)
	sleep(0);
#endif
}

void cFUThread::SleepCurrentThread(unsigned long milliseconds)
{
	// If you trigger this assert, you might be trying to sleep negative time!
	//assert(milliseconds < 10000, milliseconds = 0);
#if defined(WIN32)
	Sleep(milliseconds);
#elif defined (FP_APPLE)
	AbsoluteTime wakeup = AddDurationToAbsolute(milliseconds, UpTime());
	MPDelayUntil(&wakeup);
#elif defined(FP_LINUX) || defined(FP_QNXNTO)
	usleep(milliseconds * 1000); // usleep is in nanoseconds
#endif
}

cFUThreadRef cFUThread::CurrentThreadID()
{
#if defined(WIN32)
	return ::GetCurrentThreadId();
#elif defined (FP_APPLE)
	return MPCurrentTaskID();
#elif defined(FP_LINUX) || defined(FP_QNXNTO)
	return pthread_self();
#endif
}

//cFUThreadRef cFUThread::GetThreadID()
//{
//	if (thread != cFUThread_INVALID)
//#if defined(WIN32)
//		return ::GetThreadId(thread);
//#elif defined(FP_APPLE)
//#error Implement me, will ya.
//#endif
//	else return NULL;
//}

#ifdef _DEBUG
#ifdef WIN32

// As found in:
// http://msdn.microsoft.com/library/default.asp?url=/library/en-us/vsdebug/html/vxtsksettingthreadname.asp

typedef struct tagTHREADNAME_INFO
{
   DWORD dwType; // must be 0x1000
   LPCSTR szName; // pointer to name (in user addr space)
   DWORD dwThreadID; // thread ID (-1=caller thread)
   DWORD dwFlags; // reserved for future use, must be zero
} THREADNAME_INFO;

void cFUThread::SetThreadName(cFUThreadRef id, const char* name)
{
	if (name == NULL || *name == 0)
		return;

	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = name;
	info.dwThreadID = (DWORD)id;
	info.dwFlags = 0;

	__try
	{
		RaiseException(0x406D1388, 0, sizeof(info)/sizeof(DWORD), (const ULONG_PTR*) &info);
	}
	__except(EXCEPTION_CONTINUE_EXECUTION)
	{
	}
}

#else // OTHER
void cFUThread::SetThreadName(cFUThreadRef, const char*){}
#endif // WIN32
#else // OPTIMIZED
void cFUThread::SetThreadName(cFUThreadRef, const char*){}
#endif // _DEBUG

void cFUThread::SetThreadName()
{
	SetThreadName(cFUThread::CurrentThreadID(), name.c_str());
}

ImplementThreadRoutine(cFUThread::InternalFunc, paramPtr)
{
	cFUThreadInternalParam* params = (cFUThreadInternalParam*) paramPtr;
	if( params == NULL )
#if defined(FP_LINUX) || defined(FP_QNXNTO)|| defined (__IPHONE__)
		return (void*)1;
#else
		return 1;
#endif

	params->object->SetThreadName();
	params->object->threadStartedSemaphore.Up();
	params->object->isSuspendedCS.Enter();
	bool suspendedSnapshot = params->object->isSuspended;
	params->object->isSuspendedCS.Leave();
	if (suspendedSnapshot)
	{
		params->object->semaphore.Down(); //Wait until we're allowed to start.
	}
	unsigned code = 0;
	if (!params->object->terminate)
	{
		code = (unsigned int)(*(params->func))(params->parameter);
		params->object->exitCode = code;
	}
	SAFE_DELETE(params);
#if defined(FP_LINUX) || defined(FP_QNXNTO)|| defined (__IPHONE__)
	return (void*)code;
#else
	return code;
#endif
}

//Another way to implement this class would be to use the 'suspended' parameter
//in the constructor to determine when the thread/task is actually created.
//If suspended is false, the thread would be created in the constructor.
//Else, the thread would be created in StartThread.
