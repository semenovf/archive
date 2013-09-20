/*
 * thread_unix.hpp
 *
 *  Created on: Sep 20, 2013
 *      Author: wladt
 */

#ifndef __CWT_THREAD_UNIX_HPP__
#define __CWT_THREAD_UNIX_HPP__

#include "../thread_p.hpp"

// TODO add new supporting platforms
// TODO Uncomment after pthread's implementation of thread local data has been tested.
//#if defined(CWT_OS_LINUX) && defined(CWT_CC_GNUC)
//#	define HAVE_TLS
//#endif
#ifdef __COMMENT__
CWT_NS_BEGIN

struct ThreadData
{
	ThreadData()
		: m_thread(nullptr)
		, m_threadId(0)
	{}

	static ThreadData * current ();

	static void set (ThreadData * data);
	static ThreadData * get ();
	static void clear ();
    static void createKey ();
    static void destroy (void * pdata);
#ifndef HAVE_TLS
	static pthread_once_t m_keyOnce;
	static pthread_key_t  m_key;
#endif

	Thread * m_thread;
	pthread_t m_threadId;
};

#ifdef HAVE_TLS
	static __thread ThreadData * __current_thread_data = nullptr;
#else
	static pthread_once_t ThreadData::m_keyOnce = PTHREAD_ONCE_INIT;
	static pthread_key_t  ThreadData::m_key;
#endif

inline void ThreadData::set (ThreadData * data)
{
#ifdef HAVE_TLS // TODO use __thread
    __current_thread_data = data;
#else
    pthread_once(& m_keyOnce, ThreadData::createKey);
    pthread_setspecific(m_key, data);
#endif
}

inline ThreadData * ThreadData::get ()
{
#ifdef HAVE_TLS
    return __current_thread_data;
#else
    pthread_once(& m_keyOnce, ThreadData::createKey);
    return reinterpret_cast<ThreadData *>(pthread_getspecific(m_key));
#endif
}

inline void ThreadData::clear ()
{
#ifdef HAVE_TLS
    __current_thread_data = nullptr;
#else
    pthread_setspecific(m_key, 0);
#endif
}

void ThreadData::destroy (void * pdata)
{
    // POSIX says the value in our key is set to zero before calling
    // this destructor function, so we need to set it back to the
    // right value...

	// TODO Check this sentence in debug
    //pthread_setspecific(m_key, pdata);

    ThreadData * data = static_cast<ThreadData *>(pdata);
    if (data->m_isAdopted) {
        Thread * thread = data->m_thread;
        CWT_ASSERT(thread);
        PosixThreadImpl * thread_p = static_cast<PosixThreadImpl *>(thread->pimpl);
        CWT_ASSERT(! thread_p->isFinished());
        thread_p->finish(thread);
    }
    data->deref();

    // ... but we must reset it to zero before returning so we aren't
    // called again (POSIX allows implementations to call destructor
    // functions repeatedly until all values are zero)
    pthread_setspecific(m_key, nullptr);
}

inline void ThreadData::createKey ()
{
    pthread_key_create(& m_key, ThreadData::destroy);
}

ThreadData * ThreadData::current ()
{
    ThreadData * data = get();

    if (! data) {
        data = new ThreadData;
        if (true) { // TODO try ?
        	ThreadData::set(data);
            data->m_thread = new QAdoptedThread(data);
        } else  { // catch(...)
        	ThreadData::clear();
            data->deref();
            data = 0;
            // QT_RETHROW; // TODO throw ?
        }
        data->deref();
        data->m_isAdopted = true;
        data->m_threadId = pthread_self();
    }
    return data;
}

CWT_NS_END

#endif

#endif /* __CWT_THREAD_UNIX_HPP__ */
