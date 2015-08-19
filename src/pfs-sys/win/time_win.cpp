/*
 * time_win.cpp
 *
 *  Created on: Sep 13, 2013
 *      Author: wladt
 */


#include "pfs/platform.hpp"
#include <windows.h>

namespace pfs { namespace platform {

DLL_API pfs::time currentTime () // static
{
    SYSTEMTIME t;
    memset(& t, 0, sizeof(SYSTEMTIME));
    GetLocalTime(& t);
    return pfs::time(t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
}

}} // pfs::platform

