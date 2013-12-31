/*
 * time_unix.cpp
 *
 *  Created on: Sep 13, 2013
 *      Author: wladt
 */

#include "../../include/cwt/platform.hpp"
#include <pfs/mt.hpp>
#include <ctime>
#include <sys/time.h> // gettimeofday

namespace cwt {

pfs::time platform::currentTime () // static
{
    struct timeval tv;
    gettimeofday(& tv, nullptr);
    time_t t = tv.tv_sec;

    struct tm * tmPtr = nullptr;

#if CWT_HAVE_LOCALTIME_R
    tzset();
    struct tm res;
    tmPtr = localtime_r(& t, & res);
#else
    static pfs::mutex __mutex;
    pfs::auto_lock<> locker(&__mutex);
    tmPtr = localtime(& t);
#endif

    PFS_ASSERT(tmPtr);

    return pfs::time(tmPtr->tm_hour, tmPtr->tm_min, tmPtr->tm_sec, tv.tv_usec/1000);
}

} // cwt

