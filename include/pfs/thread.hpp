/*
 * thread.hpp
 *
 *  Created on: Jun 21, 2013
 *      Author: wladt
 */

#ifndef __PFS_THREAD_HPP__
#define __PFS_THREAD_HPP__

#include <pfs.hpp>
#include <pfs/pimpl.hpp>

#ifdef PFS_CC_MSVC
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif

namespace pfs {

class thread_impl;

class DLL_API thread : public nullable<thread_impl>
{
//	PFS_PIMPL_DECL(thread, protected, impl, protected);
protected:
	typedef nullable<thread_impl> base_class;
	typedef thread                self_class;
	typedef thread_impl           impl_class;

public:
	struct data;

	enum priority_type {
		  IdlePriority         // scheduled only when no other threads are running.
		, LowestPriority       // scheduled less often than LowPriority.
		, LowPriority          // scheduled less often than NormalPriority.
		, NormalPriority       // the default priority of the operating system.
		, HighPriority         // scheduled more often than NormalPriority.
		, HighestPriority      // scheduled more often than HighPriority.
		, TimeCriticalPriority // scheduled as often as possible.
		, InheritPriority      // use the same priority as the creating thread. This is the default.
	};

	// http://en.wikipedia.org/wiki/Thread-local_storage
	enum tls_implementation_type
	{
		  TlsPthreads = 1
		, TlsPosixThreads = TlsPthreads
		, TlsWin32
		, TlsCompilerSpecific // using '__thread' keyword for GCC (gcc.gnu.org/onlinedocs/gcc-3.3.1/gcc/Thread-Local.html)
		                      // or '__declspec(thread)' keywords for MSC (http://msdn.microsoft.com/en-us/library/6yh4a9k1.aspx)
	};

	static tls_implementation_type tls_implementation ();

public:
	thread ();
	virtual ~thread ();

	bool	 isFinished () const;
	bool	 isRunning () const;
	priority_type priority () const;
	void	 setPriority (priority_type priority);
	size_t	 stackSize () const;
	bool	 wait (uintegral_t timeout = PFS_UINTEGRAL_MAX);
	void	 start (priority_type prty = InheritPriority, size_t stackSize = 0);
	void	 terminate ();

	static void yieldCurrentThread ();

	// Terminate main() function with Thread::exit() this call instead of exit(3) to make
	// normal cleanup for application threads (including thread's specific data destruction)
	static void exit (); // equivalent to pthread_exit() for POSIX systems

public:
	static void sleep (uintegral_t secs);
	static void msleep (uintegral_t msecs);
	static void usleep (uintegral_t usecs);

protected:
	virtual void run () {}
};

} // pfs

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

#endif /* __PFS_THREAD_HPP__ */
