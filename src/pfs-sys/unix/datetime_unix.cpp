/*
 * datetime_unix.cpp
 *
 *  Created on: Sep 13, 2013
 *      Author: wladt
 */

#include <ctime>
#include <sys/time.h> // gettimeofday

#if ! PFS_HAVE_LOCALTIME_R
#	include <pfs/mutex.hpp>
#endif

#include "pfs/platform.hpp"

namespace pfs { namespace platform {

DLL_API pfs::datetime current_datetime ()
{
    struct timeval tv;
    gettimeofday(& tv, 0);
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

    pfs::date date;
	pfs::time time;

    time.set_time(tmPtr->tm_hour, tmPtr->tm_min, tmPtr->tm_sec, tv.tv_usec / 1000);
    date.set_date(tmPtr->tm_year + 1900, tmPtr->tm_mon + 1, tmPtr->tm_mday);

    return pfs::datetime(date, time);
}

}} // pfs::platform

