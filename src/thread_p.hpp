/*
 * thread_p.hpp
 *
 *  Created on: Sep 18, 2013
 *      Author: wladt
 */

#ifndef __CWT_THREAD_P_HPP__
#define __CWT_THREAD_P_HPP__

#include "../include/cwt/thread.hpp"

CWT_NS_BEGIN

class ThreadImpl
{
	CWT_IMPLEMENT_LOCKING(ThreadImpl);
protected:
	static const int PriorityMask = 0x0000000F;
	static const int StateMask    = 0x000000F0;

	enum State {
		  NotRunning
		, Running     = 0x00000010
		, Finishing   = 0x00000020
		, Finished    = 0x00000040
	};

public:
	ThreadImpl()
		: m_stackSize(0)
		, m_priority(Thread::InheritPriority)
		, m_status(NotRunning)
	{}

	bool   isRunning () const;
	bool   isFinished () const;
	Thread::Priority priority() const;
	void   setStackSize (size_t stackSize);
	size_t stackSize () const;

protected:
	bool   isRunningState () const    { return (m_status & StateMask) & Running; }
	bool   isFinishingState () const  { return (m_status & StateMask) & Finishing; }
	bool   isFinishedState () const   { return (m_status & StateMask) & Finished; }

protected:
	size_t           m_stackSize;
	Thread::Priority m_priority;
	uint32_t         m_status;
};

inline bool ThreadImpl::isFinished () const
{
	AutoLock locker(this);
    return isFinishedState() || isFinishingState();
}

inline bool ThreadImpl::isRunning () const
{
	AutoLock locker(this);
    return isRunningState() && !isFinishingState();
}

inline Thread::Priority ThreadImpl::priority() const
{
    AutoLock locker(this);
    return Thread::Priority(m_status & PriorityMask);
}


inline void ThreadImpl::setStackSize(size_t stackSize)
{
    AutoLock locker(this);
    CWT_ASSERT(!isRunning());
    m_stackSize = stackSize;
}


size_t ThreadImpl::stackSize() const
{
    AutoLock locker(this);
    return m_stackSize;
}

CWT_NS_END

#endif /* __CWT_THREAD_P_HPP__ */
