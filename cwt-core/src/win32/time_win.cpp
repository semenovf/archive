/*
 * time_win.cpp
 *
 *  Created on: Sep 13, 2013
 *      Author: wladt
 */


#include "../../include/cwt/time.hpp"
//#include <ctime>

CWT_NS_BEGIN

Time Time::currentTime () // static
{
    Time r;

    SYSTEMTIME t;
    memset(& t, 0, sizeof(SYSTEMTIME));
    GetLocalTime(& t);
    r.setTime(t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
    return r;
}

CWT_NS_END

