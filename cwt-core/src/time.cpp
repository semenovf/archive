/*
 * time.cpp
 *
 *  Created on: Sep 13, 2013
 *      Author: wladt
 */

#include "../include/cwt/time.hpp"
#include "../include/cwt/safeformat.hpp"

CWT_NS_BEGIN


bool Time::setTime (int hour, int min, int sec, int millis)
{
    if (isValid (hour, min, sec, millis)) {
        m_millis = (hour * SecondsPerHour + min * SecondsPerMinute + sec) * 1000 + millis;
        return true;
    }

    m_millis = NullTime;
    return false;
}


Time Time::addMillis (int millis) const
{
    Time r;
    if (isValid()) {
        if (millis >= 0) {
        	r.m_millis = (m_millis + millis) % MillisPerDay;
        } else {
            int ndays = (MillisPerDay - millis) / MillisPerDay;
            r.m_millis = (m_millis + millis + ndays * MillisPerDay) % MillisPerDay;
        }
    }
    return r;
}


/**
 *
 *
 * @return
 */
String Time::toString () const
{
	return _F("%2d:%2d:%2d") % hour() % minute() % second();
}

CWT_NS_END



