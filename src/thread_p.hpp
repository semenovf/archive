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

enum ThreadState {
	  ThreadNotRunning
	, ThreadRunning
	, ThreadFinishing
	, ThreadFinished
};

class Thread::Impl
{
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

	static void sleep (ulong_t secs);
	static void msleep (ulong_t msecs);
	static void usleep (ulong_t usecs);

	static void * thread_routine (void * arg);
	static void finalize (void * arg);

private:
	bool setStackSize (pthread_attr_t & attr, size_t stackSize = 0);

private:
	size_t           m_stackSize;
	Thread::Priority m_priority;

	ThreadData *     m_threadDataPtr;

	friend class Thread;
};

CWT_NS_END

#endif /* __CWT_THREAD_P_HPP__ */
