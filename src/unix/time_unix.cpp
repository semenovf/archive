/*
 * time_unix.cpp
 *
 *  Created on: Sep 13, 2013
 *      Author: wladt
 */


#include "../../include/cwt/time.hpp"
#include "../../include/cwt/mt.hpp"
#include <ctime>
#include <sys/time.h>

CWT_NS_BEGIN

Time Time::currentTime () // static
{
    Time r;

    struct timeval tv;
    gettimeofday(& tv, nullptr);
    time_t t = tv.tv_sec;

    struct tm * tmPtr = nullptr;

#if CWT_HAVE_LOCALTIME_R
    tzset();
    struct tm res;
    tmPtr = localtime_r(& t, & res);
    CWT_ASSERT(tmPtr);
    r.m_millis = Time::millis(tmPtr->tm_hour, tmPtr->tm_min, tmPtr->tm_sec, tv.tv_usec/1000);
#else
    static mt_policy_t mutex;
    mutex.lock();
    tmPtr = localtime(& t);
    CWT_ASSERT(tmPtr);
    r.m_millis = Time::millis(tmPtr->tm_hour, tmPtr->tm_min, tmPtr->tm_sec, tv.tv_usec/1000);
    mutex.unlock();
#endif

    return r;
}

CWT_NS_END

