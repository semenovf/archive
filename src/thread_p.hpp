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
	size_t stackSize () const;

protected:
	bool   isRunningState () const    { return (m_status & StateMask) & Running; }
	bool   isFinishingState () const  { return (m_status & StateMask) & Finishing; }
	bool   isFinishedState () const   { return (m_status & StateMask) & Finished; }

	void setRunningState (bool b)   { m_status = (m_status & ~StateMask) | b ? Running : 0; }
	void setFinishingState (bool b) { m_status = (m_status & ~StateMask) | b ? Finishing : 0; }
	void setFinishedState (bool b) { m_status = (m_status & ~StateMask) | b ? Finished : 0; }

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

size_t ThreadImpl::stackSize() const
{
    AutoLock locker(this);
    return m_stackSize;
}


#ifdef __CWT_INIT_FROM_PIMPL

bool Thread::isFinished () const { return pimpl->isFinished(); }
bool Thread::isRunning () const  { return pimpl->isRunning(); }
Thread::Priority Thread::priority () const { return pimpl->priority(); }
void Thread::setPriority (Thread::Priority priority) { pimpl->setPriority(priority); }
size_t Thread::stackSize () const { return pimpl->stackSize(); }
//void Thread::start (Thread::Priority priority) { pimpl->start(priority); }
void Thread::terminate () { pimpl->terminate(); }
bool Thread::wait (ulong_t timeout) { return pimpl->wait(timeout); }
//void	 Thread::exit (int returnCode = 0)
//void	 Thread::quit ();

#endif

CWT_NS_END

#endif /* __CWT_THREAD_P_HPP__ */
