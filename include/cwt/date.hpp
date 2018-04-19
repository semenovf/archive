/*
 * date.hpp
 *
 *  Created on: Sep 12, 2013
 *      Author: wladt
 */

#ifndef __CWT_DATE_HPP__
#define __CWT_DATE_HPP__

#include <cwt/hash.hpp>
#include <cwt/string.hpp>

CWT_NS_BEGIN

class DLL_API Date
{
public:
	static const long_t NullJulianDay  = CWT_LONG_MIN;
	static const long_t MinJulianDay   = long_t(-784366681008); // Date::julianDay(CWT_INT_MIN, 1, 1)
	static const long_t MaxJulianDay   = long_t(784354017364);  // Date::julianDay(CWT_INT_MAX, 12, 31)
    static const long_t EpochJulianDay = long_t(2440588);       // Date::julianDay(1970, 1, 1)

public:
	Date() : m_jd (NullJulianDay) {}
	Date(int year, int month, int day) { setDate(year, month, day); }

	bool isValid() const { return m_jd >= MinJulianDay && m_jd <= MaxJulianDay; }

	Date addDays (int ndays) const;
	Date addMonths (int nmonths) const;
	Date addYears (int nyears) const;

	int dayOfWeek() const;
	int dayOfYear() const;
	int daysInMonth () const;
	int daysInYear () const;
	long_t daysTo (const Date & d) const;
	bool setDate (int year, int month, int day);
	void split (int * year, int * month, int * day);

	int year () const;
	int month () const;
	int day () const;

	long_t julianDay() const { return m_jd; }
	String toString () const;

	bool operator == ( const Date & other ) const { return m_jd == other.m_jd; }
	bool operator != ( const Date & other ) const { return m_jd != other.m_jd; }
	bool operator  < ( const Date & other ) const { return m_jd <  other.m_jd; }
	bool operator <= ( const Date & other ) const { return m_jd <= other.m_jd; }
	bool operator  > ( const Date & other ) const { return m_jd >  other.m_jd; }
	bool operator >= ( const Date & other ) const { return m_jd >= other.m_jd; }

	static long_t julianDay (int year, int month, int day);
	static void   fromJulianDay (long_t julianDay, int * yearPtr, int * monthPtr, int * dayPtr);
	static Date   fromJulianDay (long_t julianDay);
	static bool   isLeapYear (int year);
	static bool   isValid (int year, int month, int day);
	static Date	  currentDate ();

private:
	long_t m_jd; // Julian Day;
};

inline Date Date::fromJulianDay (long_t julianDay)
{
	Date d;
	if (julianDay >= MinJulianDay && julianDay <= MaxJulianDay)
		d.m_jd = julianDay;
	return d;
}

inline Date Date::addDays (int ndays) const
{
	return isValid() ? fromJulianDay(m_jd + ndays) : Date();
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
inline bool Date::isLeapYear(int year)
{
    if (year < 1) // There is no year 0 in Gregorian calendar
        ++year;
    return year % 400 == 0 || (year % 4 == 0 && year % 100 != 0);
}

inline long_t Date::daysTo (const Date & d) const
{
	return isValid() && d.isValid()
			? d.m_jd - m_jd
			: 0;
}

inline uint_t hash_func(const Date & key, uint_t seed)
{
	return hash_func (key.julianDay(), seed);
}

CWT_NS_END

#endif /* __CWT_DATE_HPP__ */
