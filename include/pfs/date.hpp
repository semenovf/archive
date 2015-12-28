/*
 * date.hpp
 *
 *  Created on: Sep 12, 2013
 *      Author: wladt
 */

#ifndef __PFS_DATE_HPP__
#define __PFS_DATE_HPP__

#include <pfs/string.hpp>

namespace pfs {

class DLL_API date
{
	intmax_t _jd; // Julian Day;

public:
	static const intmax_t NullJulianDay  = PFS_INTEGRAL_MIN;
	static const intmax_t MinJulianDay   = intmax_t(-784366681008); // Date::julianDay(PFS_INT_MIN, 1, 1)
	static const intmax_t MaxJulianDay   = intmax_t(784354017364);  // Date::julianDay(PFS_INT_MAX, 12, 31)
    static const intmax_t EpochJulianDay = intmax_t(2440588);       // Date::julianDay(1970, 1, 1)

public:
	date ()
		: _jd (NullJulianDay)
	{}

	date (int year, int month, int day)
	{
		set_date(year, month, day);
	}

	bool valid () const
	{
		return _jd >= MinJulianDay && _jd <= MaxJulianDay;
	}

	date add_days (int ndays) const
	{
		return valid() ? from_julian_day(_jd + ndays) : date();
	}

	/**
	 *
	 * @param nmonths
	 * @return
	 */
	date add_months (int nmonths) const;

	/**
	 *
	 * @param nyears
	 * @return
	 */
	date add_years (int nyears) const;

	/**
	 *
	 * @return
	 */
	int day_of_week () const;

	/**
	 *
	 * @return
	 */
	int day_of_year () const;

	/**
	 *
	 * @return
	 */
	int days_in_month () const;

	/**
	 *
	 * @return
	 */
	int days_in_year () const;

	/**
	 *
	 * @param d
	 * @return
	 */
	intmax_t days_to (const date & d) const
	{
		return valid() && d.valid()
				? d._jd - _jd
				: 0;
	}

	/**
	 *
	 * @param year
	 * @param month
	 * @param day
	 * @return
	 */
	bool set_date (int year, int month, int day);

	/**
	 *
	 * @param year
	 * @param month
	 * @param day
	 */
	void split (int * year, int * month, int * day);

	/**
	 *
	 * @return
	 */
	int year () const;

	/**
	 *
	 * @return
	 */
	int month () const;

	/**
	 *
	 * @return
	 */
	int day () const;

	/**
	 *
	 * @return
	 */
	intmax_t julian_day() const
	{
		return _jd;
	}

	/**
	 *
	 * @param other
	 * @return
	 */
	bool operator == ( const date & other ) const
	{
		return _jd == other._jd;
	}

	bool operator != ( const date & other ) const
	{
		return _jd != other._jd;
	}

	bool operator  < ( const date & other ) const
	{
		return _jd <  other._jd;
	}

	bool operator <= ( const date & other ) const
	{
		return _jd <= other._jd;
	}

	bool operator > (const date & other) const
	{
		return _jd >  other._jd;
	}

	bool operator >= (const date & other) const
	{
		return _jd >= other._jd;
	}

	/**
	 *
	 * @param year
	 * @param month
	 * @param day
	 * @return
	 */
	static intmax_t julian_day (int year, int month, int day);

	/**
	 *
	 * @param julianDay
	 * @param yearPtr
	 * @param monthPtr
	 * @param dayPtr
	 */
	static void from_julian_day (intmax_t julianDay, int * yearPtr, int * monthPtr, int * dayPtr);

	/**
	 *
	 * @param julianDay
	 * @return
	 */
	static date from_julian_day (intmax_t julianDay)
	{
		date d;
		if (julianDay >= MinJulianDay && julianDay <= MaxJulianDay)
			d._jd = julianDay;
		return d;
	}

	/*
	 * Algorithm (http://en.wikipedia.org/wiki/Leap_year):
	 *
	 * if year is divisible by 400 then
	 * 		is_leap_year
	 * else if year is divisible by 100 then
	 * 		not_leap_year
	 * else if year is divisible by 4 then
	 * 		is_leap_year
	 * else
	 * 		not_leap_year
	 */
	static bool is_leap_year (int year)
	{
	    if (year < 1) // There is no year 0 in Gregorian calendar
	        ++year;
	    return year % 400 == 0 || (year % 4 == 0 && year % 100 != 0);
	}

	/**
	 *
	 * @param year
	 * @param month
	 * @param day
	 * @return
	 */
	static bool valid (int year, int month, int day);
};


/**
 *
 * @param format
 * @return
 *
 * %n     A newline character.
 * %t     A tab character.
 * %C     The century number (year/100) as a 2-digit integer.
 * %d     The day of the month as a decimal number (range 01 to 31).
 * %e     Like %d, the day of the month as a decimal number, but a leading zero is replaced by a space.
 * %F     Equivalent to %Y-%m-%d (the ISO 8601 date format).
 * %j     The day of the year as a decimal number (range 001 to 366).
 * %m     The month as a decimal number (range 01 to 12).
 * %u     The day of the week as a decimal, range 1 to 7, Monday being 1.
 * %y     The year as a decimal number without a century (range 00 to 99).
 * %Y     The year as a decimal number including the century.
 *
 * TODO need to support
 * %U     The week number of the current year as a decimal number, range 00 to 53,
 *        starting with the first Sunday as the first day of week 01.
 * %V     The ISO 8601 week number (see NOTES) of the current year as a decimal number,
 *        range 01 to 53, where week 1 is the first week that has at least 4 days in the new year.
 *        See also  %U  and %W.
 */
string to_string (const date & d, const string & format);

/**
 * @brief Converts date to string.
 *
 * @details The string format corresponds to the ISO 8601 specification,
 *          taking the form YYYY-MM-DD, where YYYY is the year,
 *          MM is the month of the year (between 01 and 12),
 *          and DD is the day of the month between 01 and 31.
 *
 * @return The date as string.
 */
inline string to_string (const date & d)
{
	return to_string(d, string("%F")); // equivalent to %H:%M:%S
}

} // pfs

#endif /* __PFS_DATE_HPP__ */
