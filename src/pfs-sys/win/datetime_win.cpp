/*
 * datetime_win.cpp
 *
 *  Created on: Sep 13, 2013
 *      Author: wladt
 */

#include "pfs/platform.hpp"
#include <windows.h>

namespace pfs { namespace platform {

DLL_API pfs::datetime current_datetime ()
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

}} // pfs::platform
