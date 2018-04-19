/*
 * date_win.cpp
 *
 *  Created on: Sep 13, 2013
 *      Author: wladt
 */

#include "../../include/cwt/date.hpp"

CWT_NS_BEGIN

Date Date::currentDate () // static
{
    Date r;
    SYSTEMTIME t;
    memset(& t, 0, sizeof(SYSTEMTIME));
    GetLocalTime(& t);
    r.m_jd = julianDay(t.wYear, t.wMonth, t.wDay);
    return r;
}

CWT_NS_END
