/*
 * threadcv_unix.cpp
 *
 *  Created on: Sep 18, 2013
 *      Author: wladt
 */

#include <pfs/logger.hpp>
#include <pfs/platform.hpp>
#include <pthread.h>
#include <cerrno>
#include <sys/time.h>
#include "../threadcv_p.hpp"

namespace pfs {

static inline void __calculate_abstime (uintegral_t timeout, timespec * ts)
{
    struct timeval tv;
    gettimeofday(& tv, nullptr);

    ts->tv_nsec  = (tv.tv_usec + (timeout % 1000) * 1000) * 1000;
    ts->tv_sec   = tv.tv_sec + (timeout / 1000) + (ts->tv_nsec / 1000000000);
    ts->tv_nsec %= 1000000000;
}

class thread_cv::impl
{
public:
	impl ();
	~impl ();
	bool wait (pfs::mutex & lockedMutex);
	bool wait (pfs::mutex & lockedMutex, uintegral_t timeout);
	void wakeOne ();
	void wakeAll ();

private:
	pthread_mutex_t m_internalMutex;
	pthread_cond_t  m_cond;
};


inline thread_cv::impl::impl ()
{
    int rc;
    rc = CWT_VERIFY_ERRNO(pthread_mutex_init(& m_internalMutex, nullptr));

    PFS_ASSERT_X(!rc, _Tr("Failed to initialize internal mutex for conditional variable"));

    rc = CWT_VERIFY_ERRNO(pthread_cond_init(& m_cond, nullptr));
    if (rc) {
        PFS_VERIFY(!pthread_mutex_destroy (& m_internalMutex));
        PFS_ASSERT_X(!rc, _Tr("Unable to initialize condition variable"));
    }
}

inline thread_cv::impl::~impl ()
{
	CWT_VERIFY_ERRNO(pthread_mutex_destroy(& m_internalMutex));
	CWT_VERIFY_ERRNO(pthread_cond_destroy(& m_cond));
}


inline void thread_cv::impl::wakeOne ()
{
	CWT_VERIFY_ERRNO(pthread_mutex_lock(& m_internalMutex));
	CWT_VERIFY_ERRNO(pthread_cond_signal(& m_cond));
	CWT_VERIFY_ERRNO(pthread_mutex_unlock(& m_internalMutex));
}

inline void thread_cv::impl::wakeAll ()
{
	CWT_VERIFY_ERRNO(pthread_mutex_lock(& m_internalMutex));
	CWT_VERIFY_ERRNO(pthread_cond_broadcast(& m_cond));
	CWT_VERIFY_ERRNO(pthread_mutex_unlock(& m_internalMutex));
}

// see section "Timed Condition Wait" in pthread_cond_timedwait(P) manual page.
//
bool thread_cv::impl::wait (pfs::mutex & lockedMutex, uintegral_t timeout)
{
	int rc = 0;
	{
    	// XXX Does the order of lock/unlock operations matter?
		CWT_VERIFY_ERRNO(pthread_mutex_lock(& m_internalMutex));
        lockedMutex.unlock();

		timespec abstime;
		__calculate_abstime(timeout, & abstime);
		rc = pthread_cond_timedwait(& m_cond, & m_internalMutex, & abstime);

		CWT_VERIFY_ERRNO(pthread_mutex_unlock(& m_internalMutex));
        lockedMutex.lock();
	}

	if (rc == ETIMEDOUT)
		return false;

	return (rc == 0);
}

bool thread_cv::impl::wait (pfs::mutex & lockedMutex)
{
    int rc = 0;
    {
    	CWT_VERIFY_ERRNO(pthread_mutex_lock(& m_internalMutex));
        lockedMutex.unlock();

        CWT_VERIFY_ERRNO(pthread_cond_wait(& m_cond, & m_internalMutex));

        CWT_VERIFY_ERRNO(pthread_mutex_unlock(& m_internalMutex));
        lockedMutex.lock();
    }

    PFS_ASSERT_X(!rc, _Tr("Failed in pthread_cond_wait"));

	return true;
}


thread_cv::thread_cv () : _d (new thread_cv::impl)
{}

bool thread_cv::wait (pfs::mutex & lockedMutex)
{
	return _d.cast<impl>()->wait(lockedMutex);
}

bool thread_cv::wait (pfs::mutex & lockedMutex, uintegral_t timeout)
{
	return _d.cast<impl>()->wait(lockedMutex, timeout);
}

void thread_cv::wakeOne ()
{
	_d.cast<impl>()->wakeOne();
}

void thread_cv::wakeAll ()
{
	_d.cast<impl>()->wakeAll();
}

} // pfs
