/*
 * date_unix.cpp
 *
 *  Created on: Sep 13, 2013
 *      Author: wladt
 */

#include <ctime>
#include "pfs/platform.hpp"

#if ! PFS_HAVE_LOCALTIME_R
#	include <pfs/mutex.hpp>
#endif

namespace pfs { namespace platform {

DLL_API pfs::date current_date ()
{
    time_t t;
    ::time(& t);

    struct tm * tmPtr = 0;

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
    return pfs::date(tmPtr->tm_year + 1900, tmPtr->tm_mon + 1, tmPtr->tm_mday);
}

}} // pfs::platform
