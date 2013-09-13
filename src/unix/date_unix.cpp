/*
 * date_unix.cpp
 *
 *  Created on: Sep 13, 2013
 *      Author: wladt
 */

#include "../../include/cwt/date.hpp"
#include "../../include/cwt/mt.hpp"
#include <ctime>

CWT_NS_BEGIN

Date Date::currentDate () // static
{
    Date r;

    time_t t;
    time(&t);

    struct tm * tmPtr = nullptr;

#if CWT_HAVE_LOCALTIME_R
    tzset();
    struct tm res;
    tmPtr = localtime_r(& t, & res);
    CWT_ASSERT(tmPtr);
    r.m_jd = julianDay(tmPtr->tm_year + 1900, tmPtr->tm_mon + 1, tmPtr->tm_mday);
#else
    static mt_policy_t mutex;
    mutex.lock();
    tmPtr = localtime(& t);
    CWT_ASSERT(tmPtr);
    r.m_jd = julianDay(tmPtr->tm_year + 1900, tmPtr->tm_mon + 1, tmPtr->tm_mday);
    mutex.unlock();
#endif

    return r;
}

CWT_NS_END
