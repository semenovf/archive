/*
 * thread_unix.hpp
 *
 *  Created on: Sep 20, 2013
 *      Author: wladt
 */

#ifndef __CWT_THREAD_UNIX_HPP__
#define __CWT_THREAD_UNIX_HPP__

#include "../thread_p.hpp"
#include "../../include/cwt/threadcv.hpp"
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
	ThreadData(Thread * thread)
		: m_thread(thread)
		, m_threadId(0)
		, m_threadFinished()
	{}

//	static ThreadData * current ();

	static void set (ThreadData * data);
	static ThreadData * get ();
	static void clear ();
    static void createKey ();
    static void destroy (void * pdata);

#ifdef CWT_HAVE_TLS
    static __thread ThreadData * currentThreadData;
#else
	static pthread_once_t threadKeyOnce;
	static pthread_key_t  threadKey;
#endif

	Thread *  m_thread;
	pthread_t m_threadId;
	ThreadCV  m_threadFinished;
};

inline void ThreadData::set (ThreadData * data)
{
#ifdef CWT_HAVE_TLS // TODO use __thread
    currentThreadData = data;
#else
    pthread_once(& threadKeyOnce, ThreadData::createKey);
    pthread_setspecific(threadKey, data);
#endif
}

inline ThreadData * ThreadData::get ()
{
#ifdef CWT_HAVE_TLS
    return currentThreadData;
#else
    pthread_once(& threadKeyOnce, ThreadData::createKey);
    return reinterpret_cast<ThreadData *>(pthread_getspecific(threadKey));
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
    pthread_key_create(& threadKey, ThreadData::destroy);
}

CWT_NS_END

#endif /* __CWT_THREAD_UNIX_HPP__ */
