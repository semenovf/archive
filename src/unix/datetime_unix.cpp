/*
 * datetime_unix.cpp
 *
 *  Created on: Sep 13, 2013
 *      Author: wladt
 */

#include "../../include/cwt/datetime.hpp"
#include <ctime>
#include <sys/time.h>

CWT_NS_BEGIN

DateTime DateTime::currentDateTime ()
{
	Date date;
	Time time;

    struct timeval tv;
    gettimeofday(& tv, 0);
    time_t t = tv.tv_sec;
    struct tm * tmPtr = nullptr;

#if CWT_HAVE_LOCALTIME_R
    tzset();
    struct tm res;
    tmPtr = localtime_r(& t, & res);
    CWT_ASSERT(tmPtr);
    time.setTime(tmPtr->tm_hour, tmPtr->tm_min, tmPtr->tm_sec, tv.tv_usec / 1000);
    date.setDate(tmPtr->tm_year + 1900, tmPtr->tm_mon + 1, tmPtr->tm_mday);
#else
    static mt_policy_t mutex;
    mutex.lock();
    tmPtr = localtime(& t);
    CWT_ASSERT(tmPtr);
    time.setTime(tmPtr->tm_hour, tmPtr->tm_min, tmPtr->tm_sec, tv.tv_usec / 1000);
    date.setDate(tmPtr->tm_year + 1900, tmPtr->tm_mon + 1, tmPtr->tm_mday);
    mutex.unlock();
#endif

    return DateTime(date, time);
}

CWT_NS_END

