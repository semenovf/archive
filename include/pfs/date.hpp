/*
 * date.hpp
 *
 *  Created on: Sep 12, 2013
 *      Author: wladt
 */

#ifndef __PFS_DATE_HPP__
#define __PFS_DATE_HPP__

#include <pfs.hpp>
#include <pfs/string.hpp>

namespace pfs {

class DLL_API date
{
public:
	static const integral_t NullJulianDay  = PFS_INTEGRAL_MIN;
	static const integral_t MinJulianDay   = integral_t(-784366681008); // Date::julianDay(CWT_INT_MIN, 1, 1)
	static const integral_t MaxJulianDay   = integral_t(784354017364);  // Date::julianDay(CWT_INT_MAX, 12, 31)
    static const integral_t EpochJulianDay = integral_t(2440588);       // Date::julianDay(1970, 1, 1)

public:
	date () : _jd (NullJulianDay) {}
	date (int year, int month, int day) { setDate(year, month, day); }

	bool isValid () const { return _jd >= MinJulianDay && _jd <= MaxJulianDay; }

	date addDays (int ndays) const;
	date addMonths (int nmonths) const;
	date addYears (int nyears) const;

	int dayOfWeek   () const;
	int dayOfYear   () const;
	int daysInMonth () const;
	int daysInYear  () const;
	integral_t daysTo   (const date & d) const;
	bool setDate    (int year, int month, int day);
	void split      (int * year, int * month, int * day);

	int year  () const;
	int month () const;
	int day   () const;

	integral_t julianDay() const { return _jd; }

	string toString (const char * format) const;
	string toString () const;

	bool operator == ( const date & other ) const { return _jd == other._jd; }
	bool operator != ( const date & other ) const { return _jd != other._jd; }
	bool operator  < ( const date & other ) const { return _jd <  other._jd; }
	bool operator <= ( const date & other ) const { return _jd <= other._jd; }
	bool operator  > ( const date & other ) const { return _jd >  other._jd; }
	bool operator >= ( const date & other ) const { return _jd >= other._jd; }

	static integral_t julianDay (int year, int month, int day);
	static void   fromJulianDay (integral_t julianDay, int * yearPtr, int * monthPtr, int * dayPtr);
	static date   fromJulianDay (integral_t julianDay);
	static bool   isLeapYear (int year);
	static bool   isValid (int year, int month, int day);

private:
	integral_t _jd; // Julian Day;
};

inline date date::fromJulianDay (integral_t julianDay)
{
	date d;
	if (julianDay >= MinJulianDay && julianDay <= MaxJulianDay)
		d._jd = julianDay;
	return d;
}

inline date date::addDays (int ndays) const
{
	return isValid() ? fromJulianDay(_jd + ndays) : date();
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
inline bool date::isLeapYear(int year)
{
    if (year < 1) // There is no year 0 in Gregorian calendar
        ++year;
    return year % 400 == 0 || (year % 4 == 0 && year % 100 != 0);
}

inline integral_t date::daysTo (const date & d) const
{
	return isValid() && d.isValid()
			? d._jd - _jd
			: 0;
}

/*
inline uint_t hash_func(const date & key, uint_t seed)
{
	return hash_func (key.julianDay(), seed);
}
*/

} // pfs

#endif /* __PFS_DATE_HPP__ */
