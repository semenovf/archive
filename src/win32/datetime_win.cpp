/*
 * datetime_win.cpp
 *
 *  Created on: Sep 13, 2013
 *      Author: wladt
 */

#include "../../include/cwt/platform.hpp"
#include <windows.h>

namespace cwt {

pfs::datetime platform::currentDateTime ()
{
	SYSTEMTIME t;
    memset(& t, 0, sizeof(SYSTEMTIME));
    GetLocalTime(& t);

	pfs::date date;
	pfs::time time;

    date.setDate(t.wYear, t.wMonth, t.wDay);
    time.setTime(t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);

    return pfs::datetime(date, time);
}

} // cwt
