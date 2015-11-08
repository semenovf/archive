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

	datetime (const datetime & other) : _date(other._date), _time(other._time) {}
	datetime & operator = (const datetime & other);

	datetime add_days   (int ndays) const;
	datetime add_months (int nmonths) const;
	datetime add_years  (int nyears) const;
	datetime add_millis (integral_t millis) const;
	datetime add_seconds   (integral_t secs) const;

	integral_t days_to   (const datetime & other) const;
	integral_t seconds_to   (const datetime & other) const;
	integral_t millis_to (const datetime & other) const;
	integral_t millis_since_epoch () const;

	date get_date () const { return _date; }
	time get_time () const { return _time; }
	void set_date (const date & d);
	void set_time (const time & t) { _time = t; }
	void set_millis_since_epoch (integral_t millis);

	bool valid () const { return _date.valid() && _time.valid(); }

	bool operator == (const datetime & other) const;
	bool operator != (const datetime & other) const { return ! (*this == other); }
	bool operator  < (const datetime & other) const;
	bool operator <= (const datetime & other) const { return ! (other < *this); }
	bool operator  > (const datetime & other) const { return other < *this; }
	bool operator >= (const datetime & other) const { return ! (*this < other); }

	static datetime from_millis_since_epoch (integral_t millis);
};

inline datetime & datetime::operator = (const datetime & other)
{
	_date = other._date;
	_time = other._time;
	return *this;
}

inline datetime datetime::add_days (int ndays) const
{
    return datetime(_date.add_days(ndays), _time);
}

inline datetime datetime::add_months(int nmonths) const
{
    return datetime(_date.add_months(nmonths), _time);
}

inline datetime datetime::add_years(int nyears) const
{
    return datetime(_date.add_years(nyears), _time);
}

inline datetime datetime::add_seconds(integral_t secs) const
{
    return add_millis(secs * 1000);
}

inline integral_t datetime::days_to(const datetime & other) const
{
    return _date.days_to(other._date);
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
template <typename String>
String & lexical_cast (const datetime & dt, String & result)
{
	String r;
	lexical_cast<String>(dt.get_date(), result);
	result.push_back('T');
	lexical_cast<String>(dt.get_time(), r);
	result.append(r);
	return result;
}

/**
 * @brief Returns integer representation of date & time in format YYYYMMDDhhmmss
 *
 * @return integer representation of date & time in format YYYYMMDDhhmmss
 */
integral_t lexical_cast (const datetime & dt, integral_t & result);

} // pfs

#endif /* __PFS_DATETIME_HPP__ */
