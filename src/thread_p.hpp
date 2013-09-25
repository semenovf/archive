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
	enum State {
		  ThreadNotRunning
		, ThreadRunning
		, ThreadFinishing
		, ThreadFinished
	};

public:
	explicit Impl (Thread * threadPtr);
	~Impl ();

	bool   isRunning () const;
	bool   isFinishing () const;
	bool   isFinished () const;
	Thread::Priority priority() const;
	size_t stackSize () const;

	void start (Thread::Priority priority = Thread::InheritPriority, size_t stackSize = 0);
	void setPriority(Thread::Priority priority);

	void terminate ();
	bool wait (ulong_t time = CWT_ULONG_MAX);

	static void sleep (ulong_t secs);
	static void msleep (ulong_t msecs);
	static void usleep (ulong_t usecs);

	static void * start_routine (void * arg);
	static void finish_routine (void * arg);

private:
	bool setStackSize (pthread_attr_t & attr, size_t stackSize = 0);

/*
	bool isRunningState () const    { return (m_status & StateMask) & Running; }
	bool isFinishingState () const  { return (m_status & StateMask) & Finishing; }
	bool isFinishedState () const   { return (m_status & StateMask) & Finished; }

	void setState(State state)      { m_status = (m_status & ~StateMask) | state; }
*/

private:
	Mutex            m_mutex;
	size_t           m_stackSize;
	Thread::Priority m_priority;
	State            m_state;

	ThreadData *     m_threadDataPtr;

	friend class Thread;
};

inline bool Thread::Impl::isFinishing () const
{
    return m_state == ThreadFinishing;
}


inline bool Thread::Impl::isFinished () const
{
    return m_state == ThreadFinished || m_state == ThreadFinishing;
}

inline bool Thread::Impl::isRunning () const
{
    return m_state == ThreadRunning;
}

inline Thread::Priority Thread::Impl::priority() const
{
    return m_priority;
}

inline size_t Thread::Impl::stackSize() const
{
    return m_stackSize;
}

CWT_NS_END

#endif /* __CWT_THREAD_P_HPP__ */
