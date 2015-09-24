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
class thread_data;

class DLL_API thread
{
	friend class thread_impl;
	friend class thread_data;

private:
	thread_impl * _d;

private:
    thread (const thread &);
    thread & operator = (const thread &);

public:
//    static Qt::HANDLE currentThreadId ();
    static thread * currentThread ();
    static int idealThreadCount ();
    static void yieldCurrentThread ();

    explicit thread ();
    virtual ~thread ();

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

    void setPriority (Priority priority);
    Priority priority () const;

    bool isFinished () const;
    bool isRunning () const;

    void requestInterruption ();
    bool isInterruptionRequested () const;

    void setStackSize (size_t stackSize);
    size_t stackSize () const;

public:
    void start (Priority = InheritPriority/*, size_t stackSize = 0*/);
    void terminate ();
    void quit ();

public:
    // default argument causes thread to block indefinetely
    bool wait (uintegral_t timeout = max_type<uintegral_t>());

public:
	static void sleep  (uintegral_t secs);
	static void msleep (uintegral_t msecs);
	static void usleep (uintegral_t usecs);

protected:
    virtual void run () = 0;
    static void setTerminationEnabled (bool enabled = true);


#ifdef __COMMENT__
	// Terminate main() function with Thread::exit() this call instead of exit(3) to make
	// normal cleanup for application threads (including thread's specific data destruction)
	static void exit (); // equivalent to pthread_exit() for POSIX systems

//	PFS_PIMPL_DECL(thread, protected, impl, protected);

protected:
//	typedef nullable<thread_impl> base_class;
	typedef thread                self_class;
	typedef thread_impl           impl_class;

public:
	struct data;

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
#endif // __COMMENT__
};

} // pfs

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

#endif /* __PFS_THREAD_HPP__ */
