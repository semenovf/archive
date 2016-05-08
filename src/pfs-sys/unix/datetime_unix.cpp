/*
 * datetime_unix.cpp
 *
 *  Created on: Sep 13, 2013
 *      Author: wladt
 */

#include <ctime>
#include <cstring>
#include <sys/time.h> // gettimeofday

#if ! PFS_HAVE_LOCALTIME_R
#	include <pfs/mutex.hpp>
#endif

#include "pfs/platform.hpp"

namespace pfs { namespace platform {

static struct tm * __localtime (time_t const * t, struct tm * buffer)
{
    struct tm * result = 0;
    
#if PFS_HAVE_LOCALTIME_R
    tzset();
    result = localtime_r(t, buffer);
    PFS_ASSERT(result);
#else
    static pfs::mutex mtx;
    pfs::lock_guard<pfs::mutex> locker(mtx);
    result = localtime(t);
    PFS_ASSERT(result);
    memcpy(buffer, result, sizeof(*result));
    result = buffer;
#endif

    return result;
}

DLL_API pfs::datetime current_datetime ()
{
    struct timeval tv;
    gettimeofday(& tv, 0);
    time_t t = tv.tv_sec;
    struct tm buf;
    struct tm * ptm = __localtime(& t, & buf);

    pfs::date date;
	pfs::time time;

    time.set_time(ptm->tm_hour, ptm->tm_min, ptm->tm_sec, tv.tv_usec / 1000);
    date.set_date(ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday);

    return pfs::datetime(date, time);
}

DLL_API string timezone ()
{
    struct tm buf;
    time_t t = ::time(0);
    struct tm * ptm = __localtime(& t, & buf);
    
    return _u8(ptm->tm_zone);
}

DLL_API long int offset_utc ()
{
    struct tm buf;
    time_t t = ::time(0);
    struct tm * ptm = __localtime(& t, & buf);
    
    return ptm->tm_gmtoff;
}

}} // pfs::platform

