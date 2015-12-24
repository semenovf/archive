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
	/**
	 *
	 */
	datetime ()
		: _date()
		, _time()
	{}

	/**
	 *
	 * @param d
	 */
	datetime (const date & d)
		: _date(d)
		, _time()
	{}

	/**
	 *
	 * @param d
	 * @param t
	 */
	datetime (const date & d, const time & t)
		: _date(d)
		, _time(t)
	{}

	/**
	 *
	 * @param ndays
	 * @return
	 */
	datetime add_days (int ndays) const
	{
	    return datetime(_date.add_days(ndays), _time);
	}

	/**
	 *
	 * @param nmonths
	 * @return
	 */
	datetime add_months (int nmonths) const
	{
	    return datetime(_date.add_months(nmonths), _time);
	}

	/**
	 *
	 * @param nyears
	 * @return
	 */
	datetime add_years (int nyears) const
	{
	    return datetime(_date.add_years(nyears), _time);
	}

	/**
	 *
	 * @param millis
	 * @return
	 */
	datetime add_millis (intmax_t millis) const;

	/**
	 *
	 * @param secs
	 * @return
	 */
	datetime add_seconds (intmax_t secs) const
	{
	    return add_millis(secs * 1000);
	}

	/**
	 *
	 * @param other
	 * @return
	 */
	intmax_t days_to (const datetime & other) const
	{
	    return _date.days_to(other._date);
	}

	/**
	 *
	 * @param other
	 * @return
	 */
	intmax_t seconds_to (const datetime & other) const
	{
	    return valid() && other.valid()
	    		? _date.days_to(other._date) * time::SecondsPerDay + integral_t(_time.seconds_to(other._time))
	    		: 0;
	}

	/**
	 *
	 * @param other
	 * @return
	 */
	intmax_t millis_to (const datetime & other) const
	{

	    return valid() && other.valid()
	    		? _date.days_to(other._date) * time::MillisPerDay
	    				+ integral_t(_time.millis_to(other._time))
	    		: 0;
	}

	/**
	 *
	 * @return
	 */
	intmax_t millis_since_epoch () const
	{
	    intmax_t jd = _date.julian_day() - date::EpochJulianDay;
	    return (jd * time::MillisPerDay) + time(0, 0, 0).millis_to(_time);
	}

	/**
	 *
	 * @return
	 */
	date get_date () const
	{
		return _date;
	}

	/**
	 *
	 * @return
	 */
	time get_time () const
	{
		return _time;
	}

	/**
	 *
	 * @param d
	 */
	void set_date (const date & d);

	/**
	 *
	 * @param t
	 */
	void set_time (const time & t)
	{
		_time = t;
	}

	/**
	 *
	 * @param millis
	 */
	void set_millis_since_epoch (intmax_t millis);

	/**
	 *
	 * @return
	 */
	bool valid () const
	{
		return _date.valid() && _time.valid();
	}

	bool operator == (const datetime & other) const
	{
		return _time == other._time
				&& _date == other._date;
	}

	bool operator != (const datetime & other) const
	{
		return ! (*this == other);
	}

	bool operator < (const datetime & other) const
	{
		return _date != other._date
				? _date < other._date
				: _time < other._time;
	}

	bool operator <= (const datetime & other) const
	{
		return ! (other < *this);
	}

	bool operator  > (const datetime & other) const
	{
		return other < *this;
	}

	bool operator >= (const datetime & other) const
	{
		return ! (*this < other);
	}

	/**
	 *
	 * @param millis
	 * @return
	 */
	static datetime from_millis_since_epoch (intmax_t millis)
	{
		datetime d;
		d.set_millis_since_epoch(millis);
		return d;
	}
};

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
string to_string (const datetime & dt)
{
	string r = to_string(dt.get_date());
	r.push_back('T');
	r.append(to_string(dt.get_time()));
	return r;
}

/**
 * @brief Returns integer representation of date & time in format YYYYMMDDhhmmss
 *
 * @return integer representation of date & time in format YYYYMMDDhhmmss
 */
intmax_t lexical_cast (const datetime & dt);

} // pfs

#endif /* __PFS_DATETIME_HPP__ */
