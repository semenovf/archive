/*
 * thread_unix.hpp
 *
 *  Created on: Sep 20, 2013
 *      Author: wladt
 */

#ifndef __PFS_THREAD_UNIX_HPP__
#define __PFS_THREAD_UNIX_HPP__

#include "../thread_p.hpp"
#include <cerrno>
#include <ctime>
#include <unistd.h> // for _POSIX_PRIORITY_SCHEDULING macro

#if defined(_POSIX_THREAD_PRIORITY_SCHEDULING) && (_POSIX_THREAD_PRIORITY_SCHEDULING-0 >= 0)
#	define CWT_HAVE_THREAD_PRIORITY_SCHEDULING
#endif

// TODO add new supporting platforms
// TODO Uncomment after pthread's implementation of thread local data has been tested.
//#if defined(PFS_OS_LINUX) && defined(PFS_CC_GNUC)
//#	define CWT_HAVE_TLS
//#endif

namespace pfs {

struct thread::data
{
	data ();
	~data ();

	static void set (void * data);
	static void * get ();
	static void clear ();
    static void createKey ();
    static void destroy (void * /*pdata*/);

#ifdef CWT_HAVE_TLS
    static __thread void * currentThreadData;
#else
	static pthread_once_t threadKeyOnce;
	static pthread_key_t  threadKey;
#endif

	pfs::shared_ptr<thread_impl> threadImpl;
	pthread_t threadId;
};

inline void thread::data::set (void * data)
{
#ifdef CWT_HAVE_TLS // TODO use __thread
    currentThreadData = data;
#else
    pthread_once(& threadKeyOnce, createKey);
    pthread_setspecific(threadKey, data);
#endif
}

inline void * thread::data::get ()
{
#ifdef CWT_HAVE_TLS
    return currentThreadData;
#else
    pthread_once(& threadKeyOnce, thread::data::createKey);
    return pthread_getspecific(threadKey);
#endif
}

inline void thread::data::clear ()
{
#ifdef CWT_HAVE_TLS
    currentThreadData = nullptr;
#else
    pthread_setspecific(threadKey, nullptr);
#endif
}

inline void thread::data::createKey ()
{
    pthread_key_create(& threadKey, destroy);
}

} // pfs

#endif /* __PFS_THREAD_UNIX_HPP__ */
