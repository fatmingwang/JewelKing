/**
	@file cFUThreadRef.h
	This file contains the cFUThreadRef type definition.

	Separated from cFUThread.h to avoid circular dependencies between
	cFUThread and cFUCriticalSection.
*/

#ifndef _FU_THREADREF_H_
#define _FU_THREADREF_H_

#if defined(WIN32)
typedef DWORD cFUThreadRef;
#elif defined(FP_APPLE) 
typedef MPTaskID cFUThreadRef;
#elif defined(FP_LINUX) || defined(FP_QNXNTO)|| defined(__IPHONE__)
typedef pthread_t cFUThreadRef;
#else
#error "cFUThreadRef is not defined  for this platform"
#endif // OS-dependent

#endif // _FU_THREADREF_H_
