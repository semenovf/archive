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

namespace cwt {

static inline void __calculate_abstime (ulong_t timeout, timespec * ts)
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
	bool wait (pfs::mutex & lockedMutex, ulong_t timeout);
	void wakeOne ();
	void wakeAll ();

private:
	pthread_mutex_t m_internalMutex;
	pthread_cond_t  m_cond;
};


inline thread_cv::impl::impl ()
{
    int rc;
    PFS_VERIFY_ERRNO(!(rc = pthread_mutex_init(& m_internalMutex, nullptr)), rc);

    PFS_ASSERT_X(!rc, _Tr("Failed to initialize internal mutex for conditional variable"));

    PFS_VERIFY_ERRNO(!(rc = pthread_cond_init(& m_cond, nullptr)), rc);
    if (rc) {
        PFS_VERIFY(!pthread_mutex_destroy (& m_internalMutex));
        PFS_ASSERT_X(!rc, _Tr("Unable to initialize condition variable"));
    }
}

inline thread_cv::impl::~impl ()
{
	int rc = 0;
	PFS_VERIFY_ERRNO(!(rc = pthread_mutex_destroy(& m_internalMutex)), rc);
	PFS_VERIFY_ERRNO(!(rc = pthread_cond_destroy(& m_cond)), rc);
}


inline void thread_cv::impl::wakeOne ()
{
	int rc = 0;
	PFS_VERIFY_ERRNO(!(rc = pthread_mutex_lock(& m_internalMutex)), rc);
	PFS_VERIFY_ERRNO(!(rc = pthread_cond_signal(& m_cond)), rc);
	PFS_VERIFY_ERRNO(!(rc = pthread_mutex_unlock(& m_internalMutex)), rc);
}

inline void thread_cv::impl::wakeAll ()
{
	int rc = 0;
	PFS_VERIFY_ERRNO(!(rc = pthread_mutex_lock(& m_internalMutex)), rc);
	PFS_VERIFY_ERRNO(!(rc = pthread_cond_broadcast(& m_cond)), rc);
	PFS_VERIFY_ERRNO(!(rc = pthread_mutex_unlock(& m_internalMutex)), rc);
}

// see section "Timed Condition Wait" in pthread_cond_timedwait(P) manual page.
//
bool thread_cv::impl::wait (pfs::mutex & lockedMutex, ulong_t timeout)
{
	int rc = 0;
	{
		int rc0 = 0;

    	// XXX Does the order of lock/unlock operations matter?
		PFS_VERIFY_ERRNO(!(rc0 = pthread_mutex_lock(& m_internalMutex)), rc0);
        lockedMutex.unlock();

		timespec abstime;
		__calculate_abstime(timeout, & abstime);
		rc = pthread_cond_timedwait(& m_cond, & m_internalMutex, & abstime);

		PFS_VERIFY_ERRNO(!(rc0 = pthread_mutex_unlock(& m_internalMutex)), rc0);
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
    	PFS_VERIFY_ERRNO(!(rc = pthread_mutex_lock(& m_internalMutex)), rc);
        lockedMutex.unlock();

        PFS_VERIFY_ERRNO(!(rc = pthread_cond_wait(& m_cond, & m_internalMutex)), rc);

        PFS_VERIFY_ERRNO(!(rc = pthread_mutex_unlock(& m_internalMutex)), rc);
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

bool thread_cv::wait (pfs::mutex & lockedMutex, ulong_t timeout)
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

} // cwt
