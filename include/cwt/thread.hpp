/*
 * thread.hpp
 *
 *  Created on: Jun 21, 2013
 *      Author: wladt
 */

#ifndef __CWT_THREAD_HPP__
#define __CWT_THREAD_HPP__

#include <cwt/cwt.hpp>
#include <cwt/pimpl.hpp>

CWT_NS_BEGIN

class DLL_API Thread
{
	CWT_PIMPL_IMPL(Thread);

public:
	enum Priority {
		  IdlePriority         // scheduled only when no other threads are running.
		, LowestPriority       // scheduled less often than LowPriority.
		, LowPriority          // scheduled less often than NormalPriority.
		, NormalPriority       // the default priority of the operating system.
		, HighPriority         // scheduled more often than NormalPriority.
		, HighestPriority      // scheduled more often than HighPriority.
		, TimeCriticalPriority // scheduled as often as possible.
		, InheritPriority      // use the same priority as the creating thread. This is the default.
	};

public:
	Thread();
	virtual ~Thread();

	bool	 isFinished () const;
	bool	 isRunning () const;
	Priority priority () const;
	void	 setPriority (Priority priority);
	size_t	 stackSize () const;
	bool	 wait (ulong_t timeout = CWT_ULONG_MAX);
	void	 start (Priority priority = InheritPriority, size_t stackSize = 0);
	void	 terminate ();

	static void yieldCurrentThread ();
protected:
	static void sleep (ulong_t secs);
	static void msleep (ulong_t msecs);
	static void usleep (ulong_t usecs);

	// To allow other threads to continue execution, the main thread should terminate by calling pthread_exit() rather than exit(3).
	//
	// main()
	// {
	// 		...
	// 		pthread_exit(nullptr);
	// }

protected:
	virtual void run () {}
};

CWT_NS_END

#endif /* __CWT_THREAD_HPP__ */
