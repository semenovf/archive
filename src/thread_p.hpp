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

struct thread_data;

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
	void setPriority (thread::priority_type priority);

	void terminate ();
	bool wait (ulong_t time = PFS_ULONG_MAX);

	static void sleep (ulong_t secs);
	static void msleep (ulong_t msecs);
	static void usleep (ulong_t usecs);

	static void * thread_routine (void * arg);
	static void finalize (void * arg);

private:
//FIXME need Win32 specific
#ifdef PFS_CC_MSVC
#else
	bool setStackSize (pthread_attr_t & attr, size_t stackSize = 0);
#endif

private:
	pfs::mutex       _mutex;
	size_t           _stackSize;
	thread::priority_type _priority;
	thread_state     _state;
	thread_cv        _threadFinished;
	thread *  		 _thread;
	thread::data *   _data;

	friend class thread;
//	friend struct thread_data;
};

} // cwt

#endif /* __CWT_THREAD_P_HPP__ */
