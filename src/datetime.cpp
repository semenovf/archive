/*
 * datetime.cpp
 *
 *  Created on: Sep 13, 2013
 *      Author: wladt
 */

#include "pfs/datetime.hpp"

namespace pfs {

void datetime::setDate (const date & d)
{
    _date = d;

    if (_date.isValid() && !_time.isValid())
        _time.setTime(0, 0, 0);
}


datetime datetime::addMillis (integral_t millis) const
{
	datetime r;

    integral_t dd = _date.julianDay();
    integral_t tt = time(0, 0, 0).millisTo(_time);
    int sign = 1;

    if (millis < 0) {
    	millis = -millis;
        sign = -1;
    }

    if (millis >= integral_t(time::MillisPerDay)) {
        dd += sign * (millis / time::MillisPerDay);
        millis %= time::MillisPerDay;
    }

    tt += sign * millis;

    if (tt < 0) {
        tt = time::MillisPerDay - tt - 1;
        dd -= tt / time::MillisPerDay;
        tt = tt % time::MillisPerDay;
        tt = time::MillisPerDay - tt - 1;
    } else if (tt >= time::MillisPerDay) {
        dd += tt / time::MillisPerDay;
        tt = tt % time::MillisPerDay;
    }

    PFS_ASSERT(tt >= PFS_INT_MIN && tt <= PFS_INT_MAX);
    r._date.fromJulianDay(dd);
    r._time = time(0, 0, 0).addMillis(int(tt));

    return r;
}

integral_t datetime::secsTo (const datetime & other) const
{
    return isValid() && other.isValid()
    		? _date.daysTo(other._date) * time::SecondsPerDay + integral_t(_time.secsTo(other._time))
    		: 0;
}

integral_t datetime::millisTo (const datetime & other) const
{

    return isValid() && other.isValid()
    		? _date.daysTo(other._date) * time::MillisPerDay
    				+ integral_t(_time.millisTo(other._time))
    		: 0;
}

integral_t datetime::millisSinceEpoch () const
{
    integral_t jd = _date.julianDay() - date::EpochJulianDay;
    return (jd * time::MillisPerDay) + time(0, 0, 0).millisTo(_time);
}


void datetime::setMillisSinceEpoch (integral_t millis)
{
    integral_t days = millis / time::MillisPerDay;
    millis %= time::MillisPerDay;

    if (millis < 0) {
        --days;
        millis += time::MillisPerDay;
    }

    PFS_ASSERT(days >= PFS_INT_MIN && days <= PFS_INT_MAX);
    PFS_ASSERT(millis >= PFS_INT_MIN && millis <= PFS_INT_MAX);
    _date = date(1970, 1, 1).addDays(int(days));
    _time = time(0, 0, 0).addMillis(int(millis));
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
string datetime::toString () const
{
	string r = _date.toString();

	if (!r.isEmpty()) {
		r.append(1, ucchar('T'));
		r.append(_time.toString());
	}
	return r;
}

/**
 * @brief Returns integer representation of date & time in format YYYYMMDDhhmmss
 *
 * @return integer representation of date & time in format YYYYMMDDhhmmss
 */
integral_t datetime::toInteger () const
{
	int Y = _date.year();
	int M = _date.month();
	int D = _date.day();

	int	h = _time.hour();
	int	m = _time.minute();
	int	s = _time.second();

	integral_t r = integral_t(10000000000) * Y + integral_t(100000000) * M + integral_t(1000000) * D
			+ integral_t(10000) * h + integral_t(100) * m + s;

	return r;
}


datetime datetime::fromMillisSinceEpoch (integral_t millis) // static
{
	datetime d;
	d.setMillisSinceEpoch(millis);
	return d;
}

} // pfs


