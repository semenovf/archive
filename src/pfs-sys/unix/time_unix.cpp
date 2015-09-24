/*
 * time_unix.cpp
 *
 *  Created on: Sep 13, 2013
 *      Author: wladt
 */

#include <ctime>
#include <sys/time.h> // gettimeofday

#if ! PFS_HAVE_LOCALTIME_R
#	include <pfs/mutex.hpp>
#endif

#include <pfs/platform.hpp>

namespace pfs { namespace platform {

DLL_API pfs::time currentTime ()
{
    struct timeval tv;
    gettimeofday(& tv, nullptr);
    time_t t = tv.tv_sec;

    struct tm * tmPtr = nullptr;

#if PFS_HAVE_LOCALTIME_R
    tzset();
    struct tm res;
    tmPtr = localtime_r(& t, & res);
#else
    static pfs::mutex mtx;
    pfs::lock_guard<pfs::mutex> locker(mtx);
    tmPtr = localtime(& t);
#endif

    PFS_ASSERT(tmPtr);

    return pfs::time(tmPtr->tm_hour, tmPtr->tm_min, tmPtr->tm_sec, tv.tv_usec/1000);
}

}} // pfs::platform

