/*
 * datetime.cpp
 *
 *  Created on: Sep 13, 2013
 *      Author: wladt
 */

#include "../include/cwt/datetime.hpp"
#include "../include/cwt/safeformat.hpp"

CWT_NS_BEGIN

void DateTime::setDate (const Date & date)
{
    m_d.detach();
    m_d->date = date;

    if (date.isValid() && !m_d->time.isValid())
        m_d->time.setTime(0, 0, 0);
}

void DateTime::setTime (const Time & time)
{
    m_d.detach();
    m_d->time = time;
}


DateTime DateTime::addMillis (long_t millis) const
{
	DateTime r;

    long_t dd = m_d->date.julianDay();
    int tt = Time(0, 0, 0).millisTo(m_d->time);
    int sign = 1;

    if (millis < 0) {
    	millis = -millis;
        sign = -1;
    }

    if (millis >= long_t(Time::MillisPerDay)) {
        dd += sign * (millis / Time::MillisPerDay);
        millis %= Time::MillisPerDay;
    }

    tt += sign * millis;

    if (tt < 0) {
        tt = Time::MillisPerDay - tt - 1;
        dd -= tt / Time::MillisPerDay;
        tt = tt % Time::MillisPerDay;
        tt = Time::MillisPerDay - tt - 1;
    } else if (tt >= Time::MillisPerDay) {
        dd += tt / Time::MillisPerDay;
        tt = tt % Time::MillisPerDay;
    }

    r.m_d->date.fromJulianDay(dd);
    r.m_d->time = Time(0, 0, 0).addMillis(tt);

    return r;
}

long_t DateTime::secsTo (const DateTime & other) const
{
    return isValid() && other.isValid()
    		? m_d->date.daysTo(other.m_d->date) * Time::SecondsPerDay + long_t(m_d->time.secsTo(other.m_d->time))
    		: 0;
}

long_t DateTime::millisTo (const DateTime & other) const
{

    return isValid() && other.isValid()
    		? m_d->date.daysTo(other.m_d->date) * Time::MillisPerDay
    				+ long_t(m_d->time.millisTo(other.m_d->time))
    		: 0;
}

long_t DateTime::millisSinceEpoch () const
{
    long_t jd = m_d->date.julianDay() - Date::EpochJulianDay;
    return (jd * Time::MillisPerDay) + Time(0, 0, 0).millisTo(m_d->time);
}


void DateTime::setMillisSinceEpoch (long_t millis)
{
    m_d.detach();

    long_t days = millis / Time::MillisPerDay;
    millis %= Time::MillisPerDay;

    if (millis < 0) {
        --days;
        millis += Time::MillisPerDay;
    }

    m_d->date = Date(1970, 1, 1).addDays(days);
    m_d->time = Time(0, 0, 0).addMillis(millis);
}

/**
 * @brief Converts date and time to string.
 *
 * @details The string format corresponds to the ISO 8601 specification,
 *          taking the form YYYY-MM-DDTHH:mm:ss, where YYYY is the year,
 *          MM is the month of the year (between 01 and 12),
 *          and DD is the day of the month between 01 and 31.
 *
 * @return The date and time as string.
 */
String DateTime::toString () const
{
	String r = m_d->date.toString();

	if (!r.isEmpty()) {
		r.append(1, UChar('T'));
		r.append(m_d->time.toString());
	}
	return r;
}

DateTime DateTime::fromMillisSinceEpoch (long_t millis) // static
{
	DateTime d;
	d.setMillisSinceEpoch(millis);
	return d;
}

CWT_NS_END


