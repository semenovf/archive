/*
 * datetime.cpp
 *
 *  Created on: Sep 13, 2013
 *      Author: wladt
 */

#include "pfs/datetime.hpp"

namespace pfs {

void datetime::set_date (const date & d)
{
    _date = d;

    if (_date.valid() && !_time.valid())
        _time.set_time(0, 0, 0);
}


datetime datetime::add_millis (integral_t millis) const
{
	datetime r;

    integral_t dd = _date.julian_day();
    integral_t tt = time(0, 0, 0).millis_to(_time);
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
    r._date.from_julian_day(dd);
    r._time = time(0, 0, 0).add_millis(int(tt));

    return r;
}

integral_t datetime::seconds_to (const datetime & other) const
{
    return valid() && other.valid()
    		? _date.days_to(other._date) * time::SecondsPerDay + integral_t(_time.seconds_to(other._time))
    		: 0;
}

integral_t datetime::millis_to (const datetime & other) const
{

    return valid() && other.valid()
    		? _date.days_to(other._date) * time::MillisPerDay
    				+ integral_t(_time.millis_to(other._time))
    		: 0;
}

integral_t datetime::millis_since_epoch () const
{
    integral_t jd = _date.julian_day() - date::EpochJulianDay;
    return (jd * time::MillisPerDay) + time(0, 0, 0).millis_to(_time);
}


void datetime::set_millis_since_epoch (integral_t millis)
{
    integral_t days = millis / time::MillisPerDay;
    millis %= time::MillisPerDay;

    if (millis < 0) {
        --days;
        millis += time::MillisPerDay;
    }

    PFS_ASSERT(days >= PFS_INT_MIN && days <= PFS_INT_MAX);
    PFS_ASSERT(millis >= PFS_INT_MIN && millis <= PFS_INT_MAX);
    _date = date(1970, 1, 1).add_days(int(days));
    _time = time(0, 0, 0).add_millis(int(millis));
}

integral_t lexical_cast (const datetime & dt, integral_t & result)
{
	date d = dt.get_date();
	int Y = d.year();
	int M = d.month();
	int D = d.day();

	time t = dt.get_time();
	int	h = t.hour();
	int	m = t.minute();
	int	s = t.second();

	result = integral_t(10000000000) * Y + integral_t(100000000) * M + integral_t(1000000) * D
			+ integral_t(10000) * h + integral_t(100) * m + s;

	return result;
}


#if __OBSOLETE__
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

	if (!r.empty()) {
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
#endif


datetime datetime::from_millis_since_epoch (integral_t millis) // static
{
	datetime d;
	d.set_millis_since_epoch(millis);
	return d;
}

} // pfs


