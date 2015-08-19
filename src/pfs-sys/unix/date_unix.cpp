/*
 * date_unix.cpp
 *
 *  Created on: Sep 13, 2013
 *      Author: wladt
 */

#include "pfs/platform.hpp"
#include <pfs/mt.hpp>
#include <ctime>

namespace pfs { namespace platform {

DLL_API pfs::date currentDate ()
{
    time_t t;
    ::time(& t);

    struct tm * tmPtr = nullptr;

#if PFS_HAVE_LOCALTIME_R
    tzset();
    struct tm res;
    tmPtr = localtime_r(& t, & res);
#else
    static pfs::mutex __mutex;
    pfs::auto_lock<> locker(&__mutex);
    tmPtr = localtime(& t);
#endif

    PFS_ASSERT(tmPtr);
    return pfs::date(tmPtr->tm_year + 1900, tmPtr->tm_mon + 1, tmPtr->tm_mday);
}

}} // pfs::platform
