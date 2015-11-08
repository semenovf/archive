/*
 * date.hpp
 *
 *  Created on: Sep 12, 2013
 *      Author: wladt
 */

#ifndef __PFS_DATE_HPP__
#define __PFS_DATE_HPP__

#include <pfs.hpp>
#include <sstream>
#include <iomanip>

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
	date (int year, int month, int day) { set_date(year, month, day); }

	bool valid () const { return _jd >= MinJulianDay && _jd <= MaxJulianDay; }

	date add_days (int ndays) const;
	date add_months (int nmonths) const;
	date add_years (int nyears) const;

	int day_of_week   () const;
	int day_of_year   () const;
	int days_in_month () const;
	int days_in_year  () const;
	integral_t days_to   (const date & d) const;
	bool set_date    (int year, int month, int day);
	void split      (int * year, int * month, int * day);

	int year  () const;
	int month () const;
	int day   () const;

	integral_t julian_day() const { return _jd; }

	bool operator == ( const date & other ) const { return _jd == other._jd; }
	bool operator != ( const date & other ) const { return _jd != other._jd; }
	bool operator  < ( const date & other ) const { return _jd <  other._jd; }
	bool operator <= ( const date & other ) const { return _jd <= other._jd; }
	bool operator  > ( const date & other ) const { return _jd >  other._jd; }
	bool operator >= ( const date & other ) const { return _jd >= other._jd; }

	static integral_t julian_day (int year, int month, int day);
	static void   from_julian_day (integral_t julianDay, int * yearPtr, int * monthPtr, int * dayPtr);
	static date   from_julian_day (integral_t julianDay);
	static bool   is_leap_year (int year);
	static bool   valid (int year, int month, int day);

private:
	integral_t _jd; // Julian Day;
};

inline date date::from_julian_day (integral_t julianDay)
{
	date d;
	if (julianDay >= MinJulianDay && julianDay <= MaxJulianDay)
		d._jd = julianDay;
	return d;
}

inline date date::add_days (int ndays) const
{
	return valid() ? from_julian_day(_jd + ndays) : date();
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
inline bool date::is_leap_year(int year)
{
    if (year < 1) // There is no year 0 in Gregorian calendar
        ++year;
    return year % 400 == 0 || (year % 4 == 0 && year % 100 != 0);
}

inline integral_t date::days_to (const date & d) const
{
	return valid() && d.valid()
			? d._jd - _jd
			: 0;
}

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
template <typename String>
String & lexical_cast (const date & d, const String & format, String & result)
{
	if (d.year() < 0 && d.year() > 9999)
		return result;

//	std::stringstream ss;
	std::basic_stringstream<typename String::value_type> ss;

	typename String::const_iterator p = format.begin();
	typename String::const_iterator end = format.end();

	bool need_spec = false; // true if conversion specifier character expected

	while (p < end) {
		if (*p == '%') {
			if (need_spec) {
				ss << '%';
				need_spec = false;
			} else {
				need_spec = true;
			}
		} else {
			if (!need_spec) {
				ss << *p;
			} else {
				switch (*p) {
				case 'n':
					ss << std::endl;
					break;
				case 't':
					ss << '\t';
					break;
				case 'C':
					ss << std::setw(2) << std::setfill('0') << d.year()/100;
					break;
				case 'd':
					ss << std::setw(2) << std::setfill('0') << d.day();
					break;
				case 'e':
					ss << std::setw(2) << std::setfill(' ') << d.day();
					break;
				case 'F':
					ss << std::setw(4) << std::setfill('0') << d.year()
					   << '-'
					   << std::setw(2) << std::setfill('0') << d.month()
					   << '-'
					   << std::setw(2) << std::setfill('0') << d.day();
					break;
				case 'j':
					ss << std::setw(3) << std::setfill('0') << d.day_of_year();
					break;
				case 'm':
					ss << std::setw(2) << std::setfill('0') << d.month();
					break;
				case 'u':
					ss << d.day_of_week();
					break;
				case 'y':
					ss << std::setw(2) << std::setfill('0') << d.year() % 100;
					break;
				case 'Y':
					ss << std::setw(4) << std::setfill('0') << d.year();
					break;

				default:
					ss << *p;
					break;
				}

				need_spec = false;
			}
		}
		++p;
	}

	result = ss.str();
	return result;
}

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
template <typename String>
String & lexical_cast (const date & d, String & result)
{
	return lexical_cast(d, String("%F"), result); // equivalent to %H:%M:%S
}

/*
inline uint_t hash_func(const date & key, uint_t seed)
{
	return hash_func (key.julianDay(), seed);
}
*/

} // pfs

#endif /* __PFS_DATE_HPP__ */
