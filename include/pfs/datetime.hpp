/*
 * datetime.hpp
 *
 *  Created on: Sep 13, 2013
 *      Author: wladt
 */

#ifndef __PFS_DATETIME_HPP__
#define __PFS_DATETIME_HPP__

#include <pfs/date.hpp>
#include <pfs/time.hpp>
#include <pfs/string.hpp>

namespace pfs {

class DLL_API datetime
{
	date _date;
	time _time;

public:
	datetime () : _date(), _time() {}
	datetime (const date & d) : _date(d), _time() {}
	datetime (const date & d, const time & t) : _date(d), _time(t) {}

	datetime(const datetime & other) : _date(other._date), _time(other._time) {}
	datetime & operator = (const datetime & other);

	datetime addDays   (int ndays) const;
	datetime addMonths (int nmonths) const;
	datetime addYears  (int nyears) const;
	datetime addMillis (integral_t millis) const;
	datetime addSecs   (integral_t secs) const;

	integral_t daysTo   (const datetime & other) const;
	integral_t secsTo   (const datetime & other) const;
	integral_t millisTo (const datetime & other) const;
	integral_t millisSinceEpoch () const;

	date getDate () const { return _date; }
	time getTime () const { return _time; }
	void setDate (const date & d);
	void setTime (const time & t) { _time = t; }
	void setMillisSinceEpoch (integral_t millis);

	bool isValid () const { return _date.isValid() && _time.isValid(); }
	string toString () const;
	integral_t toInteger () const;

	bool operator == (const datetime & other) const;
	bool operator != (const datetime & other) const { return ! (*this == other); }
	bool operator  < (const datetime & other) const;
	bool operator <= (const datetime & other) const { return ! (other < *this); }
	bool operator  > (const datetime & other) const { return other < *this; }
	bool operator >= (const datetime & other) const { return ! (*this < other); }

	static datetime fromMillisSinceEpoch (integral_t millis);
};

inline datetime & datetime::operator = (const datetime & other)
{
	_date = other._date;
	_time = other._time;
	return *this;
}

inline datetime datetime::addDays (int ndays) const
{
    return datetime(_date.addDays(ndays), _time);
}

inline datetime datetime::addMonths(int nmonths) const
{
    return datetime(_date.addMonths(nmonths), _time);
}

inline datetime datetime::addYears(int nyears) const
{
    return datetime(_date.addYears(nyears), _time);
}

inline datetime datetime::addSecs(integral_t secs) const
{
    return addMillis(secs * 1000);
}

inline integral_t datetime::daysTo(const datetime & other) const
{
    return _date.daysTo(other._date);
}

inline bool datetime::operator == (const datetime & other) const
{
	return _time == other._time
			&& _date == other._date;
}

inline bool datetime::operator < (const datetime & other) const
{
	return _date != other._date
			? _date < other._date
			: _time < other._time;
}

/*
inline uint_t hash_func(const datetime & key, uint_t seed)
{
	return hash_func(key.millisSinceEpoch(), seed);
}
*/

} // pfs

#endif /* __PFS_DATETIME_HPP__ */
