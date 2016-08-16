/*
 * threadcv_unix.cpp
 *
 *  Created on: Sep 18, 2013
 *      Author: wladt
 */

#include <cerrno>
#include <sys/time.h>
#include <pfs/utility.hpp>
//#include <pfs/platform.hpp>
#include "pfs/condition_variable.hpp"

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
    PFS_ASSERT(pthread_cond_init(cond, & condattr) == 0);
    pthread_condattr_destroy(& condattr);
}

// TODO DEPRECATED
inline int __wait_relative (details::condition_variable_data & cv, uintmax_t time)
{
    timespec ti;
    __calculate_abstime(time, & ti);
    return pthread_cond_timedwait(& cv.cond, & cv.mutex, & ti);
}

// TODO DEPRECATED
bool __wait (details::condition_variable_data & cv, uintmax_t time)
{
    int rc = 0;

    for(;;) {
        if (time != pfs::max_value<uintmax_t>()) {
            rc = __wait_relative(cv, time);
        } else {
            rc = pthread_cond_wait(& cv.cond, & cv.mutex);
        }

        if (rc == 0 && cv.wakeups == 0) {
            // many vendors warn of spurious wakeups from
            // pthread_cond_wait(), especially after signal delivery,
            // even though POSIX doesn't allow for it... sigh
            continue;
        }
        break;
    }

    PFS_ASSERT_X(cv.waiters > 0, "pfs::condition_variable_impl::wait(): internal error (waiters)");

    --cv.waiters;

    if (rc == 0) {
        PFS_ASSERT_X(cv.wakeups > 0, "pfs::condition_variable_impl::wait(): internal error (wakeups)");
        --cv.wakeups;
    }

    PFS_ASSERT(pthread_mutex_unlock(& cv.mutex) == 0);

//        if (code && code != ETIMEDOUT)
//        	PFS_VERIFY_ERRNO(code);

    return (rc == 0);
}

static bool __wait (details::condition_variable_data & cv)
{
    int rc = 0;

    for(;;) {
        rc = pthread_cond_wait(& cv.cond, & cv.mutex);

        if (rc == 0 && cv.wakeups == 0) {
            // many vendors warn of spurious wakeups from
            // pthread_cond_wait(), especially after signal delivery,
            // even though POSIX doesn't allow for it... sigh
            continue;
        }
        break;
    }

    PFS_ASSERT_X(cv.waiters > 0, "pfs::condition_variable_impl::wait(): internal error (waiters)");

    --cv.waiters;

    if (rc == 0) {
        PFS_ASSERT_X(cv.wakeups > 0, "pfs::condition_variable_impl::wait(): internal error (wakeups)");
        --cv.wakeups;
    }

    PFS_ASSERT(pthread_mutex_unlock(& cv.mutex) == 0);

//        if (code && code != ETIMEDOUT)
//        	PFS_VERIFY_ERRNO(code);

    return (rc == 0);
}

condition_variable::condition_variable ()
{
	PFS_ASSERT(pthread_mutex_init(& _d.mutex, NULL) == 0);
    __initialize_pthread_cond(& _d.cond);
    _d.waiters = _d.wakeups = 0;
}

condition_variable::~condition_variable ()
{
	PFS_ASSERT(pthread_cond_destroy(& _d.cond) == 0);
	PFS_ASSERT(pthread_mutex_destroy(& _d.mutex) == 0);
}

void condition_variable::notify_one ()
{
	PFS_ASSERT(pthread_mutex_lock(& _d.mutex) == 0);
    _d.wakeups = pfs::min(_d.wakeups + 1, _d.waiters);
    PFS_ASSERT(pthread_cond_signal(& _d.cond) == 0);
    PFS_ASSERT(pthread_mutex_unlock(& _d.mutex) == 0);
}

void condition_variable::notify_all ()
{
	PFS_ASSERT(pthread_mutex_lock(& _d.mutex) == 0);
    _d.wakeups = _d.waiters;
    PFS_ASSERT(pthread_cond_broadcast(& _d.cond) == 0);
    PFS_ASSERT(pthread_mutex_unlock(& _d.mutex) == 0);
}

// see section "Timed Condition Wait" in pthread_cond_timedwait(P) manual page.
//
void condition_variable::wait (unique_lock<mutex> & lock)
{
	PFS_ASSERT(pthread_mutex_lock(& _d.mutex) == 0);
    ++_d.waiters;
    lock.unlock();

    bool r = __wait(_d);

    lock.lock();
}

bool condition_variable::wait_until (unique_lock<mutex> & lock, uintmax_t timeout)
//bool condition_variable::wait (pfs::mutex & mx, uintmax_t time)
{
	PFS_ASSERT(pthread_mutex_lock(& _d.mutex) == 0);
    ++_d.waiters;
    lock.unlock();

    bool r = __wait(_d, timeout);

    lock.lock();

    return r;
}

} // pfs
