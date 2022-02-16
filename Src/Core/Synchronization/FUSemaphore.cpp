#include "../stdAfx.h"
#include "FUSemaphore.h"

#if defined(FP_LINUX) || defined(FP_QNXNTO)
#include <time.h>
#endif

cFUSemaphore::cFUSemaphore(uint32 initialValue, uint32 maximumValue)
#if defined(WIN32)
:	semaphoreHandle(NULL)
#endif // windows-only
{	
	assert(initialValue <= maximumValue);
#if defined(WIN32)
	semaphoreHandle = CreateSemaphore(NULL, initialValue, maximumValue, NULL);
	assert(semaphoreHandle != NULL);
#elif defined(FP_APPLE)
	MPCreateSemaphore(maximumValue, initialValue, &semaphoreHandle);
#elif defined(FP_LINUX) || defined(FP_QNXNTO)
	sem_init(&semaphoreHandle, 0, initialValue);
#endif
}

cFUSemaphore::~cFUSemaphore()
{
#if defined(WIN32)
	CloseHandle(semaphoreHandle);
#elif defined(FP_APPLE)
	MPDeleteSemaphore(semaphoreHandle);
#elif defined(FP_LINUX) || defined(FP_QNXNTO)
	sem_destroy(&semaphoreHandle);
#endif
}

void cFUSemaphore::Up()
{
#if defined(WIN32)
	ReleaseSemaphore(semaphoreHandle, 1, NULL);
#elif defined(FP_APPLE)
	MPSignalSemaphore(semaphoreHandle);
#elif defined(FP_LINUX) || defined(FP_QNXNTO)
	sem_post(&semaphoreHandle);
#endif
}

bool cFUSemaphore::Down(uint32 blockTime)
{
#if defined(WIN32)
	return WaitForSingleObject(semaphoreHandle, blockTime) == WAIT_OBJECT_0;
#elif defined(FP_APPLE)
	return MPWaitOnSemaphore(semaphoreHandle, (blockTime == (uint32)-1) ? kDurationForever : blockTime) == noErr;
#elif defined(FP_LINUX) || defined(FP_QNXNTO)
	if (blockTime == (uint32) -1)
		return sem_wait(&semaphoreHandle) == 0;
	else
	{
		// The basic POSIX semaphore does not support a block time, but rather a deadline.
		timespec deadline;
		clock_gettime(CLOCK_REALTIME, &deadline);
		deadline.tv_nsec += (blockTime * 1000);
		return sem_timedwait(&semaphoreHandle, &deadline) == 0;
	}
#endif
}
