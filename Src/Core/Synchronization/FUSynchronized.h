/**
	@file cFUSynchronized.h
	This file contains the cFUSynchronized class.
*/

#ifndef _FU_SYNCHRONIZED_H_
#define _FU_SYNCHRONIZED_H_

#ifndef _FU_CRITICAL_SECTION_H_
#include "FUCriticalSection.h"
#endif

/**
	A synchronized object.
	Has the ability to lock itself so only one thread may be using it. 
	This can be done by calling the Lock and Unlock methods of the object.
	@ingroup FUtils
*/
class  cFUSynchronized
{
private:
	cFUCriticalSection criticalSection;

public:
	/** Destructor. */
	virtual ~cFUSynchronized();

	/** Locks the object, blocking if another thread has locked it. */
	virtual void Lock();

	/** Unlocks the object, allowing other threads to lock it. */
	virtual void Unlock();
};


/** 
	This small helper will lock a synchronized object when created and hold on to the
	locked synchronized for its lifetime. This is useful for all the member functions of a synchronized object
	or concurrent accesses to a synchronized object that may include 'return', 'break' or such statements.
	@ingroup FUtils
*/
class cFUSynchronizedHold
{
private:
	/** The resource currently locked. */
	cFUSynchronized* held; 

public: 
	/** Constructor.
		@param toLock The resource to lock. */
	cFUSynchronizedHold(const cFUSynchronized* toLock) : held(const_cast<cFUSynchronized*>(toLock)) { held->Lock(); }

	/** Destructor. */
	~cFUSynchronizedHold() { held->Unlock(); }
};

//
//cFUSynchronized	l_FUSynchronized;
//cFUSynchronizedHold hold(l_FUSynchronized);
//
//
	

#endif // _FU_SYNCHRONIZED_H_
