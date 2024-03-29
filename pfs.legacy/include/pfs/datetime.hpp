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
	    		? _date.days_to(other._date) * time::SecondsPerDay + intmax_t(_time.seconds_to(other._time))
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
	    				+ intmax_t(_time.millis_to(other._time))
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

class DLL_API timezone
{
    string   _tzname;
    long int _offset;
    
public:
    timezone () 
        : _tzname("XXXX")
        , _offset(0) 
    {}
        
    timezone (string const & tzname, long int offset)
        : _tzname(tzname)
        , _offset(offset) 
    {}

    timezone (timezone const & other)
        : _tzname(other._tzname)
        , _offset(other._offset)
    {}

    timezone & operator = (timezone const & other)
    {
        _tzname = other._tzname;
        _offset = other._offset;
        return *this;
    }
    
    string const & tzname () const 
    {
        return _tzname;
    }
    
    long int offset () const
    {
        return _offset;
    }
    
    string offset_to_string () const
    {
        return offset_to_string(_offset);
    }
    
public:
    /*
     * @brief Converts UTC offset to string. 
     * @param off UTC offset value.
     * 
     * @return String in format '+hhmm' or '-hhmm' (that is, the hour and minute offset from UTC).
     */
    static string offset_to_string (long int off);
    
};

/**
 * @brief Converts date and time to string according to @a format.
 * 
 * @param dt Date and time value.
 * @param format Format to convert @a dt.
 * @return String representation of @a dt.
 */
string to_string (datetime const & dt, timezone const & tz, string const & format);

inline string to_string (datetime const & dt, string const & format)
{
    return to_string(dt, timezone(), format);
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
string to_string (datetime const & dt);

DLL_API datetime current_datetime ();

DLL_API string timezone_name ();
DLL_API long int offset_utc ();

inline timezone current_timezone ()
{
    return timezone(timezone_name(), offset_utc());
}

} // pfs

#endif /* __PFS_DATETIME_HPP__ */
