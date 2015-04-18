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

namespace pfs {

static inline void __calculate_abstime (uintegral_t timeout, timespec * ts)
{
    struct timeval tv;
    gettimeofday(& tv, nullptr);

    ts->tv_nsec  = (tv.tv_usec + (timeout % 1000) * 1000) * 1000;
    ts->tv_sec   = tv.tv_sec + (timeout / 1000) + (ts->tv_nsec / 1000000000);
    ts->tv_nsec %= 1000000000;
}

static void __initialize_pthread_cond (pthread_cond_t * cond)
{
    pthread_condattr_t condattr;

    pthread_condattr_init(& condattr);
//#if !defined(Q_OS_MAC) && !defined(Q_OS_ANDROID) && (_POSIX_MONOTONIC_CLOCK-0 >= 0)
//    if (QElapsedTimer::clockType() == QElapsedTimer::MonotonicClock)
//        pthread_condattr_setclock(& condattr, CLOCK_MONOTONIC);
//#endif
    PFS_VERIFY_ERRNO(pthread_cond_init(cond, & condattr));
    pthread_condattr_destroy(& condattr);
}

class thread_cv_impl
{
public:
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int waiters;
    int wakeups;

    int wait_relative (uintegral_t time)
    {
        timespec ti;
        __calculate_abstime(time, & ti);
        return pthread_cond_timedwait(& cond, & mutex, & ti);
    }

    bool wait (uintegral_t time)
    {
        int code;
        for(;;) {
            if (time != pfs::max_type<uintegral_t>()) {
                code = wait_relative(time);
            } else {
                code = pthread_cond_wait(& cond, & mutex);
            }
            if (code == 0 && wakeups == 0) {
                // many vendors warn of spurious wakeups from
                // pthread_cond_wait(), especially after signal delivery,
                // even though POSIX doesn't allow for it... sigh
                continue;
            }
            break;
        }

        PFS_ASSERT_X(waiters > 0, "pfs::thread_cv_impl::wait(): internal error (waiters)");

        --waiters;

        if (code == 0) {
        	PFS_ASSERT_X(wakeups > 0, "pfs::thread_cv_impl::wait(): internal error (wakeups)");
            --wakeups;
        }

        PFS_VERIFY_ERRNO(pthread_mutex_unlock(& mutex));

        if (code && code != ETIMEDOUT)
        	PFS_VERIFY_ERRNO(code);

        return (code == 0);
    }
};

thread_cv::thread_cv ()
	: _d(new thread_cv_impl)
{
	thread_cv_impl * d = _d.cast<thread_cv_impl>();
	PFS_VERIFY_ERRNO(pthread_mutex_init(& d->mutex, NULL));
    __initialize_pthread_cond(& d->cond);
    d->waiters = d->wakeups = 0;
}

thread_cv::~thread_cv ()
{
	thread_cv_impl * d = _d.cast<thread_cv_impl>();
	PFS_VERIFY_ERRNO(pthread_cond_destroy(& d->cond));
	PFS_VERIFY_ERRNO(pthread_mutex_destroy(& d->mutex));
}


void thread_cv::wakeOne ()
{
	thread_cv_impl * d = _d.cast<thread_cv_impl>();
	PFS_VERIFY_ERRNO(pthread_mutex_lock(& d->mutex));
    d->wakeups = pfs::min(d->wakeups + 1, d->waiters);
    PFS_VERIFY_ERRNO(pthread_cond_signal(& d->cond));
    PFS_VERIFY_ERRNO(pthread_mutex_unlock(& d->mutex));
}

void thread_cv::wakeAll ()
{
	thread_cv_impl * d = _d.cast<thread_cv_impl>();
	PFS_VERIFY_ERRNO(pthread_mutex_lock(& d->mutex));
    d->wakeups = d->waiters;
    PFS_VERIFY_ERRNO(pthread_cond_broadcast(&d->cond));
    PFS_VERIFY_ERRNO(pthread_mutex_unlock(&d->mutex));
}

// see section "Timed Condition Wait" in pthread_cond_timedwait(P) manual page.
bool thread_cv::wait (pfs::mutex & lockedMutex, uintegral_t time)
{
	thread_cv_impl * d = _d.cast<thread_cv_impl>();

//	if (lockedMutex.isRecursive()) {
//        qWarning("QWaitCondition: cannot wait on recursive mutexes");
//        return false;
//    }

	PFS_VERIFY_ERRNO(pthread_mutex_lock(& d->mutex));
    ++d->waiters;
    lockedMutex.unlock();

    bool returnValue = d->wait(time);

    lockedMutex.lock();

    return returnValue;
}

} // pfs
