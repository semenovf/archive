/*
 * time_win.cpp
 *
 *  Created on: Sep 13, 2013
 *      Author: wladt
 */


#include "../../include/cwt/platform.hpp"
#include <windows.h>

namespace cwt {

pfs::time platform::currentTime () // static
{
    SYSTEMTIME t;
    memset(& t, 0, sizeof(SYSTEMTIME));
    GetLocalTime(& t);
    return pfs::time(t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
}

} // cwt

