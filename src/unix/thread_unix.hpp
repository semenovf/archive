/*
 * thread_unix.hpp
 *
 *  Created on: Sep 20, 2013
 *      Author: wladt
 */

#ifndef __CWT_THREAD_UNIX_HPP__
#define __CWT_THREAD_UNIX_HPP__

#include "../thread_p.hpp"
#include <cerrno>
#include <ctime>
#include <unistd.h> // for _POSIX_PRIORITY_SCHEDULING macro

#if defined(_POSIX_THREAD_PRIORITY_SCHEDULING) && (_POSIX_THREAD_PRIORITY_SCHEDULING-0 >= 0)
#	define CWT_HAVE_THREAD_PRIORITY_SCHEDULING
#endif

// TODO add new supporting platforms
// TODO Uncomment after pthread's implementation of thread local data has been tested.
//#if defined(CWT_OS_LINUX) && defined(CWT_CC_GNUC)
//#	define CWT_HAVE_TLS
//#endif

CWT_NS_BEGIN


struct ThreadData
{
	ThreadData ();
	~ThreadData ();

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

	shared_ptr<Thread::Impl> threadImpl;
	pthread_t threadId;
};

inline void ThreadData::set (void * data)
{
#ifdef CWT_HAVE_TLS // TODO use __thread
    currentThreadData = data;
#else
    pthread_once(& threadKeyOnce, createKey);
    pthread_setspecific(threadKey, data);
#endif
}

inline void * ThreadData::get ()
{
#ifdef CWT_HAVE_TLS
    return currentThreadData;
#else
    pthread_once(& threadKeyOnce, ThreadData::createKey);
    return pthread_getspecific(threadKey);
#endif
}

inline void ThreadData::clear ()
{
#ifdef CWT_HAVE_TLS
    currentThreadData = nullptr;
#else
    pthread_setspecific(threadKey, nullptr);
#endif
}

inline void ThreadData::createKey ()
{
    pthread_key_create(& threadKey, destroy);
}

CWT_NS_END

#endif /* __CWT_THREAD_UNIX_HPP__ */
