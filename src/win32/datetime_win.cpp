/*
 * datetime_win.cpp
 *
 *  Created on: Sep 13, 2013
 *      Author: wladt
 */

#include "../../include/cwt/datetime.hpp"

CWT_NS_BEGIN

DateTime DateTime::currentDateTime ()
{
	Date date;
	Time time;

	SYSTEMTIME t;
    memset(& t, 0, sizeof(SYSTEMTIME));
    GetLocalTime(& t);

    date.setDate(t.wYear, t.wMonth, t.wDay);
    time.setTime(t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);

    return DateTime(date, time);
}

CWT_NS_END
