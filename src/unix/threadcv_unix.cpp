/*
 * threadcv_unix.cpp
 *
 *  Created on: Sep 18, 2013
 *      Author: wladt
 */

#include "../../include/cwt/logger.hpp"
#include <pthread.h>
#include <cerrno>
#include <sys/time.h>
#include "../threadcv_p.hpp"

CWT_NS_BEGIN

static inline void __calculate_abstime (ulong_t timeout, timespec * ts)
{
    struct timeval tv;
    gettimeofday(& tv, nullptr);

    ts->tv_nsec  = (tv.tv_usec + (timeout % 1000) * 1000) * 1000;
    ts->tv_sec   = tv.tv_sec + (timeout / 1000) + (ts->tv_nsec / 1000000000);
    ts->tv_nsec %= 1000000000;
}

class ThreadCV::Impl
{
public:
	Impl();
	~Impl();
	bool wait (Mutex & lockedMutex);
	bool wait (Mutex & lockedMutex, ulong_t timeout);
	void wakeOne ();
	void wakeAll ();

private:
	pthread_mutex_t m_internalMutex;
	pthread_cond_t  m_cond;
};


inline ThreadCV::Impl::Impl ()
{
    int rc = pthread_mutex_init(& m_internalMutex, nullptr);
    if (rc)
    	CWT_SYS_FATAL_RC(rc, _Tr("Failed to initialize internal mutex for conditional variable"));

    rc = pthread_cond_init(& m_cond, nullptr);
    if (rc) {
        CWT_VERIFY(!pthread_mutex_destroy (& m_internalMutex));
        CWT_SYS_FATAL_RC(rc, _Tr("Unable to initialize condition variable"));
    }
}

inline ThreadCV::Impl::~Impl ()
{
    CWT_VERIFY(!pthread_mutex_destroy(& m_internalMutex));
    CWT_VERIFY(!pthread_cond_destroy(& m_cond));
}


inline void ThreadCV::Impl::wakeOne ()
{
	CWT_VERIFY(!pthread_mutex_lock(& m_internalMutex));
	CWT_VERIFY(!pthread_cond_signal(& m_cond));
	CWT_VERIFY(!pthread_mutex_unlock(& m_internalMutex));
}

inline void ThreadCV::Impl::wakeAll ()
{
	CWT_VERIFY(!pthread_mutex_lock(& m_internalMutex));
	CWT_VERIFY(!pthread_cond_broadcast(& m_cond));
	CWT_VERIFY(!pthread_mutex_unlock(& m_internalMutex));
}

// see section "Timed Condition Wait" in pthread_cond_timedwait(P) manual page.
//
bool ThreadCV::Impl::wait (Mutex & lockedMutex, ulong_t timeout)
{
	int rc = 0;
	{
    	// XXX Does the order of lock/unlock operations matter?
        CWT_VERIFY(!pthread_mutex_lock(& m_internalMutex));
        lockedMutex.unlock();

		timespec abstime;
		__calculate_abstime(timeout, & abstime);
		rc = pthread_cond_timedwait(& m_cond, & m_internalMutex, & abstime);

        CWT_VERIFY(!pthread_mutex_unlock(& m_internalMutex));
        lockedMutex.lock();
	}

	if (rc == ETIMEDOUT)
		return false;

	if (rc)
		CWT_SYS_FATAL_RC(rc, _Tr("Failed in pthread_cond_timedwait"));

   return (rc == 0);
}

bool ThreadCV::Impl::wait(Mutex & lockedMutex)
{
    int rc = 0;
    {
        CWT_VERIFY(!pthread_mutex_lock(& m_internalMutex));
        lockedMutex.unlock();

        rc = pthread_cond_wait(& m_cond, & m_internalMutex);

        CWT_VERIFY(!pthread_mutex_unlock(& m_internalMutex));
        lockedMutex.lock();
    }

	if (rc)
		CWT_SYS_FATAL_RC(rc, _Tr("Failed in pthread_cond_wait"));

	return true;
}


ThreadCV::ThreadCV () : pimpl (new ThreadCV::Impl) {}
bool ThreadCV::wait (Mutex & lockedMutex) { return pimpl->wait(lockedMutex); }
bool ThreadCV::wait (Mutex & lockedMutex, ulong_t timeout) { return pimpl->wait(lockedMutex, timeout); }
void ThreadCV::wakeOne () { pimpl->wakeOne(); }
void ThreadCV::wakeAll () { pimpl->wakeAll(); }

CWT_NS_END
