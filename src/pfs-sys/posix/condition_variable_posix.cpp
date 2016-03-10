/*
 * threadcv_unix.cpp
 *
 *  Created on: Sep 18, 2013
 *      Author: wladt
 */

#include <pfs/utility.hpp>
#include <pfs/platform.hpp>
#include "pfs/condition_variable.hpp"
#include <pthread.h>
#include <cerrno>
#include <sys/time.h>

namespace pfs {

inline void __calculate_abstime (uintmax_t timeout, timespec * ts)
{
    struct timeval tv;
    gettimeofday(& tv, 0);

    ts->tv_nsec  = (tv.tv_usec + (timeout % 1000) * 1000) * 1000;
    ts->tv_sec   = tv.tv_sec + (timeout / 1000) + (ts->tv_nsec / 1000000000);
    ts->tv_nsec %= 1000000000;
}

inline void __initialize_pthread_cond (pthread_cond_t * cond)
{
    pthread_condattr_t condattr;

    pthread_condattr_init(& condattr);
//#if !defined(Q_OS_MAC) && !defined(Q_OS_ANDROID) && (_POSIX_MONOTONIC_CLOCK-0 >= 0)
//    if (QElapsedTimer::clockType() == QElapsedTimer::MonotonicClock)
//        pthread_condattr_setclock(& condattr, CLOCK_MONOTONIC);
//#endif
    PFS_ASSERT_BT(pthread_cond_init(cond, & condattr) == 0);
    pthread_condattr_destroy(& condattr);
}

class condition_variable_impl
{
public:
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int waiters;
    int wakeups;

    int wait_relative (uintmax_t time)
    {
        timespec ti;
        __calculate_abstime(time, & ti);
        return pthread_cond_timedwait(& cond, & mutex, & ti);
    }

    bool wait (uintmax_t time)
    {
        int code = 0;

        for(;;) {
            if (time != pfs::max_value<uintmax_t>()) {
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

        PFS_ASSERT_X(waiters > 0, "pfs::condition_variable_impl::wait(): internal error (waiters)");

        --waiters;

        if (code == 0) {
        	PFS_ASSERT_X(wakeups > 0, "pfs::condition_variable_impl::wait(): internal error (wakeups)");
            --wakeups;
        }

        PFS_ASSERT_BT(pthread_mutex_unlock(& mutex) == 0);

//        if (code && code != ETIMEDOUT)
//        	PFS_VERIFY_ERRNO(code);

        return (code == 0);
    }
};

condition_variable::condition_variable ()
	: _d(new condition_variable_impl)
{
	PFS_ASSERT_BT(pthread_mutex_init(& _d->mutex, NULL) == 0);
    __initialize_pthread_cond(& _d->cond);
    _d->waiters = _d->wakeups = 0;
}

condition_variable::~condition_variable ()
{
	PFS_ASSERT_BT(pthread_cond_destroy(& _d->cond) == 0);
	PFS_ASSERT_BT(pthread_mutex_destroy(& _d->mutex) == 0);
	delete _d;
}

void condition_variable::notify_one ()
{
	PFS_ASSERT_BT(pthread_mutex_lock(& _d->mutex) == 0);
    _d->wakeups = pfs::min(_d->wakeups + 1, _d->waiters);
    PFS_ASSERT_BT(pthread_cond_signal(& _d->cond) == 0);
    PFS_ASSERT_BT(pthread_mutex_unlock(& _d->mutex) == 0);
}

void condition_variable::notify_all ()
{
	PFS_ASSERT_BT(pthread_mutex_lock(& _d->mutex) == 0);
    _d->wakeups = _d->waiters;
    PFS_ASSERT_BT(pthread_cond_broadcast(& _d->cond) == 0);
    PFS_ASSERT_BT(pthread_mutex_unlock(& _d->mutex) == 0);
}

// see section "Timed Condition Wait" in pthread_cond_timedwait(P) manual page.
//
bool condition_variable::wait (pfs::mutex & mx, uintmax_t time)
{
//	if (lockedMutex.isRecursive()) {
//        qWarning("QWaitCondition: cannot wait on recursive mutexes");
//        return false;
//    }

	PFS_ASSERT_BT(pthread_mutex_lock(& _d->mutex) == 0);
    ++_d->waiters;
    mx.unlock();

    bool r = _d->wait(time);

    mx.lock();

    return r;
}

} // pfs
