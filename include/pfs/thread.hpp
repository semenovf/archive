/*
 * thread.hpp
 *
 *  Created on: Jun 21, 2013
 *      Author: wladt
 */

#ifndef __PFS_THREAD_HPP__
#define __PFS_THREAD_HPP__

#include <pfs.hpp>

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
    static thread * current_thread ();
    static int ideal_thread_count ();
    static void yield_current_thread ();

    explicit thread ();
    virtual ~thread ();

	enum priority_enum {
		  idle_priority         // scheduled only when no other threads are running.
		, lowest_priority       // scheduled less often than LowPriority.
		, low_priority          // scheduled less often than NormalPriority.
		, normal_priority       // the default priority of the operating system.
		, high_priority         // scheduled more often than NormalPriority.
		, highest_priority      // scheduled more often than HighPriority.
		, time_critical_priority // scheduled as often as possible.
		, inherit_priority      // use the same priority as the creating thread. This is the default.
	};

    void set_priority (priority_enum prior);
    priority_enum priority () const;

    bool is_finished () const;
    bool is_running () const;

    void request_interruption ();
    bool is_interruption_requested () const;

    void set_stack_size (size_t stackSize);
    size_t stack_size () const;

public:
    void start (priority_enum prior = inherit_priority/*, size_t stackSize = 0*/);
    void terminate ();
    void quit ();

public:
    // default argument causes thread to block indefinetely
    bool wait (uintmax_t timeout = max_value<uintmax_t>());

public:
	static void sleep  (uintmax_t secs);
	static void msleep (uintmax_t msecs);
	static void usleep (uintmax_t usecs);

protected:
    virtual void run () = 0;
    static void set_termination_enabled (bool enabled = true);


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
