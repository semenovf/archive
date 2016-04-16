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

datetime datetime::add_millis (intmax_t millis) const
{
	datetime r;

    intmax_t dd = _date.julian_day();
    intmax_t tt = time(0, 0, 0).millis_to(_time);
    int sign = 1;

    if (millis < 0) {
    	millis = -millis;
        sign = -1;
    }

    if (millis >= intmax_t(time::MillisPerDay)) {
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

    PFS_ASSERT(tt >= min_value<int>() && tt <= max_value<int>());
    r._date.from_julian_day(dd);
    r._time = time(0, 0, 0).add_millis(int(tt));

    return r;
}

void datetime::set_millis_since_epoch (intmax_t millis)
{
    intmax_t days = millis / time::MillisPerDay;
    millis %= time::MillisPerDay;

    if (millis < 0) {
        --days;
        millis += time::MillisPerDay;
    }

    PFS_ASSERT(days >= min_value<int>() && days <= max_value<int>());
    PFS_ASSERT(millis >= min_value<int>() && millis <= max_value<int>());
    _date = date(1970, 1, 1).add_days(int(days));
    _time = time(0, 0, 0).add_millis(int(millis));
}

intmax_t lexical_cast (const datetime & dt)
{
	date d = dt.get_date();
	int Y = d.year();
	int M = d.month();
	int D = d.day();

	time t = dt.get_time();
	int	h = t.hour();
	int	m = t.minute();
	int	s = t.second();

	intmax_t r = intmax_t(10000000000) * Y + intmax_t(100000000) * M + intmax_t(1000000) * D
			+ intmax_t(10000) * h + intmax_t(100) * m + s;

	return r;
}

string to_string (datetime const & dt)
{
	string r = to_string(dt.get_date());
	r.push_back('T');
	r.append(to_string(dt.get_time()));
	return r;
}

string to_string (datetime const & dt, string const & format)
{
    string r = to_string(dt.get_date(), format);
    r = to_string(dt.get_time(), r);
    return r;
}
        

} // pfs


