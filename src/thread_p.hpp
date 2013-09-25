/*
 * thread_p.hpp
 *
 *  Created on: Sep 18, 2013
 *      Author: wladt
 */

#ifndef __CWT_THREAD_P_HPP__
#define __CWT_THREAD_P_HPP__

#include "../include/cwt/thread.hpp"
#include "../include/cwt/mt.hpp"

CWT_NS_BEGIN

struct ThreadData;

class Thread::Impl
{
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
	explicit Impl (Thread * threadPtr);
	~Impl ();

	bool   isRunning () const;
	bool   isFinished () const;
	Thread::Priority priority() const;
	size_t stackSize () const;

	void start (Thread::Priority priority = Thread::InheritPriority, size_t stackSize = 0);
	void setPriority(Thread::Priority priority);

	void terminate ();
	bool wait (ulong_t time = CWT_ULONG_MAX);

	void sleep (ulong_t secs);
	void msleep (ulong_t msecs);
	void usleep (ulong_t usecs);

	static void * start_routine (void * arg);
	static void finish_routine (void * arg);

private:
	bool setStackSize (pthread_attr_t & attr, size_t stackSize = 0);

	bool isRunningState () const    { return (m_status & StateMask) & Running; }
	bool isFinishingState () const  { return (m_status & StateMask) & Finishing; }
	bool isFinishedState () const   { return (m_status & StateMask) & Finished; }

	void setRunningState (bool b)   { m_status = (m_status & ~StateMask) | b ? Running : 0; }
	void setFinishingState (bool b) { m_status = (m_status & ~StateMask) | b ? Finishing : 0; }
	void setFinishedState (bool b) { m_status = (m_status & ~StateMask) | b ? Finished : 0; }

private:
	Mutex            m_mutex;
	size_t           m_stackSize;
	Thread::Priority m_priority;
	uint32_t         m_status;

	ThreadData *     m_threadDataPtr;

	friend class Thread;
};

inline bool Thread::Impl::isFinished () const
{
    return isFinishedState() || isFinishingState();
}

inline bool Thread::Impl::isRunning () const
{
    return isRunningState() && !isFinishingState();
}

inline Thread::Priority Thread::Impl::priority() const
{
    return Thread::Priority(m_status & PriorityMask);
}

inline size_t Thread::Impl::stackSize() const
{
    return m_stackSize;
}

CWT_NS_END

#endif /* __CWT_THREAD_P_HPP__ */
