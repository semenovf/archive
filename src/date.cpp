/*
 * date.cpp
 *
 *  Created on: Sep 12, 2013
 *      Author: wladt
 */

#include "../include/cwt/date.hpp"
#include "../include/cwt/math.hpp"
#include "../include/cwt/safeformat.hpp"

// Sources:
// 		http://en.wikipedia.org/wiki/Julian_day ,
// 		http://www.tondering.dk/claus/cal/julperiod.php ,
//      http://www.hermetic.ch/cal_stud/jdn.htm ,
// 		and source files from Qt library.
// Useful links:
//		Julian Day Calculator: http://www.csgnetwork.com/juliandaydate.html


CWT_NS_BEGIN

static const byte_t __daysInMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

static inline Date __valid_date(int y, int m, int d)
{
	// first day of the month
    Date r(y, m, 1);

    // set date according to the number of days in the specified month.
    r.setDate(y, m, CWT_MIN(d, r.daysInMonth()));
    return r;
}

/**
 * @brief Calculates Julian Day (JD) from Gregorian calendar date
 *
 * @param year  Year.
 * @param month Month.
 * @param day   Day.
 * @return JD value.
 */
long_t Date::julianDay (int year, int month, int day) // static
{
	if (year < 0) // there is no 0 year
		++year;

	int    a = Math::floorDiv(14 - month, 12);
	long_t y = long_t(year) + 4800 - a;
	int    m = month + 12 * a - 3;

	// Gregorian calendar: >= 15.10.1582
	if (year > 1582 || (year == 1582 && (month > 10 || (month == 10 && day >= 15)))) {
		return day + Math::floorDiv(153 * m + 2, 5) + 365 * y + Math::floorDiv(y, 4) - Math::floorDiv(y, 100) + Math::floorDiv(y, 400) - 32045;
	}
	// Julian calendar: <= 4.10.1582
	else if (year < 1582 || (year == 1582 && (month < 10 || (month == 10 && day <= 4)))) {
		return day + Math::floorDiv(153 * m + 2, 5) + 365 * y + Math::floorDiv(y, 4) - 32083;
	}

	return NullJulianDay;
}

/**
 * @brief Calculates Gregorian calendar date from Julian Day (JD)
 *
 * @param julianDay JD value.
 * @param yearPtr   Year result.
 * @param monthPtr  Month result.
 * @param dayPtr    Day result.
 *
 * @note see http://www.tondering.dk/claus/cal/julperiod.php.
 */
void Date::fromJulianDay (long_t julianDay, int * yearPtr, int * monthPtr, int * dayPtr) // static
{
	long_t b = 0;
	int c = 0;

	// Gregorian calendar
	if (julianDay >= 2299161) {
		long_t a = julianDay + 32044;
		b = Math::floorDiv(4 * a + 3, 146097);
		c = a - Math::floorDiv(146097 * b, 4);
	} else {
		b = 0;
		c = julianDay + 32082;
	}

    int    d = Math::floorDiv(4 * c + 3, 1461);
    int    e = c - Math::floorDiv(1461 * d, 4);
    int    m = Math::floorDiv(5 * e + 2, 153);

    int    day = e - Math::floorDiv(153 * m + 2, 5) + 1;
    int    month = m + 3 - 12 * Math::floorDiv(m, 10);
    int    year = 100 * b + d - 4800 + Math::floorDiv(m, 10);

    if (year <= 0)
        --year ;

    if (yearPtr)
        *yearPtr = year;
    if (monthPtr)
        *monthPtr = month;
    if (dayPtr)
        *dayPtr = day;
}


/**
 * @brief Checks if the specified date is valid
 *
 * @param year  Year value.
 * @param month Month value.
 * @param day   Day value.
 * @return @c true if the specified date is valid; otherwise returns @c false.
 */
bool Date::isValid (int year, int month, int day) // static
{
    return year == 0
        ? false
    	: (day > 0 && month > 0 && month <= 12) &&
           (day <= __daysInMonth[month] || (day == 29 && month == 2 && isLeapYear(year)));
}



// 31.01.2013 + 1 mon = 29.02.2013
// 31.01.2013 - 2 mon = 30.11.2012
Date Date::addMonths (int nmonths) const
{
	// Note: algorithm adopted from QDate::addMonths

    if (!isValid())
        return Date();

    if (!nmonths)
    	return *this;

    int y, m, d;
    fromJulianDay(m_jd, & y, & m, & d);

    int start_year = y;

    while (nmonths != 0) {
    	if (nmonths < 0 && nmonths + 12 <= 0) {
    		--y;
    		nmonths += 12;
    	} else if (nmonths < 0) {
    		m += nmonths;
    		nmonths = 0;

    		if (m <= 0) {
    			--y;
    			m += 12;
    		}
    	} else if (nmonths - 12 >= 0) {
    		++y;
    		nmonths -= 12;
    	} else if (m == 12) {
    		++y;
    		m = 0;
    	} else {
    		m += nmonths;
    		nmonths = 0;

    		if (m > 12) {
    			++y;
    			m -= 12;
    		}
    	}
    }

    // special cases: transition the year through 0
    if (start_year > 0 && y <= 0)      // decreasing months
    	--y;
    else if (start_year < 0 && y >= 0) // increasing months
    	++y;

    return __valid_date(y, m, d);
}

Date Date::addYears (int nyears) const
{
	// Note: algorithm adopted from QDate::addYears

    if (!isValid())
        return Date();

    int y, m, d;
    fromJulianDay(m_jd, & y, & m, & d);

    int start_year = y;
    y += nyears;

    // special cases: transition the year through 0
    if (start_year > 0 && y <= 0)      // decreasing months
    	--y;
    else if (start_year < 0 && y >= 0) // increasing months
    	++y;

    return __valid_date(y, m, d);
}


/**
 * @brief Returns the weekday (1 = Monday to 7 = Sunday) for this date.
 *
 * @return The weekday (1 = Monday to 7 = Sunday) for this date
 *         or 0 if the date is invalid.
 */
int Date::dayOfWeek() const
{
    return !isValid()
        ? 0
        : (m_jd >= 0)
          	  ? (m_jd % 7) + 1
    		  : ((m_jd + 1) % 7) + 7;
}

/**
 * @brief Returns the day of the year (1 to 365 or 366 on leap years) for
          this date.
 *
 * @return The day of the year (1 to 365 or 366 on leap years) for
    this date, or 0 if the date is invalid.
 */
int Date::dayOfYear() const
{
    return !isValid()
        ? 0
        : m_jd - julianDay(year(), 1, 1) + 1;
}

/**
 * @brief Calculates the number of days in the month (28 to 31) for this date.
 *
 * @return The number of days in the month (28 to 31) for this date
 *         or 0 if the date is invalid.
 */
int Date::daysInMonth() const
{
    if (! isValid())
        return 0;

    int y, m;

    fromJulianDay(m_jd, & y, & m, nullptr);

    return  (m == 2 && isLeapYear(y))
    		? 29
    		: __daysInMonth[m];
}

/**
 * @brief Calculates the number of days in the year (365 or 366) for this date.
 *
 * @return The number of days in the year (365 or 366) for this date
 *         or 0 if the date is invalid.
 */
int Date::daysInYear () const
{
    if (! isValid())
        return 0;

    int y;
    fromJulianDay(m_jd, & y, nullptr, nullptr);
    return isLeapYear(y) ? 366 : 365;
}

/**
 * @brief Extracts the date's year, month, and day.
 *
 * @param year
 * @param month
 * @param day
 */
void Date::split(int * year, int * month, int * day)
{
    if (isValid()) {
        fromJulianDay(m_jd, year, month, day);
    } else {
        if (year)
            *year = 0;
        if (month)
            *month = 0;
        if (day)
            *day = 0;
    }
}

bool Date::setDate (int year, int month, int day)
{
    if (isValid(year, month, day))
        m_jd = julianDay(year, month, day);
    else
        m_jd = NullJulianDay;

    return isValid();
}

int Date::year () const
{
    if (!isValid())
        return 0;

    int y;
    fromJulianDay(m_jd, & y, nullptr, nullptr);
    return y;
}

int Date::month () const
{
    if (!isValid())
        return 0;

    int m;
    fromJulianDay(m_jd, nullptr, & m, nullptr);
    return m;
}

int Date::day () const
{
    if (!isValid())
        return 0;

    int d;
    fromJulianDay(m_jd, nullptr, nullptr, & d);
    return d;
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
String Date::toString () const
{
	return (year() < 0 || year() > 9999)
			? String()
			: String(_F("%4d-%2d-%2d") % year() % month() % day());
}


CWT_NS_END
