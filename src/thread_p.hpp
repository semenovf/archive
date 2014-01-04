/*
 * thread_p.hpp
 *
 *  Created on: Sep 18, 2013
 *      Author: wladt
 */

#ifndef __CWT_THREAD_P_HPP__
#define __CWT_THREAD_P_HPP__

#include <pfs/mt.hpp>
#include "../include/cwt/thread.hpp"
#include "../include/cwt/threadcv.hpp"

namespace cwt {

enum thread_state
{
	  ThreadNotRunning
	, ThreadRunning
	, ThreadFinishing
	, ThreadFinished
};

class thread::impl
{
public:
	explicit impl ();
	~impl ();

	void start (thread::priority_type priority = thread::InheritPriority, size_t stackSize = 0);
	void setPriority(thread::priority_type priority);

	void terminate ();
	bool wait (ulong_t time = PFS_ULONG_MAX);

	static void sleep (ulong_t secs);
	static void msleep (ulong_t msecs);
	static void usleep (ulong_t usecs);

	static void * thread_routine (void * arg);
	static void finalize (void * arg);

private:
	bool setStackSize (pthread_attr_t & attr, size_t stackSize = 0);

private:
	pfs::mutex       m_mutex;
	size_t           m_stackSize;
	thread::priority_type m_priority;
	thread_state     m_state;
	thread_cv        m_threadFinished;
	thread *  		 m_thread;
	thread::data *   m_data;

	friend class thread;
	friend struct thread_data;
};

} // cwt

#endif /* __CWT_THREAD_P_HPP__ */
