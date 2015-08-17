/*
 * date.cpp
 *
 *  Created on: Sep 12, 2013
 *      Author: wladt
 */

#include "pfs/date.hpp"
#include "pfs/math.hpp"
#include <cstring>
#include <sstream>
#include <iomanip>

// Sources:
// 		http://en.wikipedia.org/wiki/Julian_day ,
// 		http://www.tondering.dk/claus/cal/julperiod.php ,
//      http://www.hermetic.ch/cal_stud/jdn.htm ,
// 		and source files from Qt library.
// Useful links:
//		Julian Day Calculator: http://www.csgnetwork.com/juliandaydate.html


namespace pfs {

static const byte_t __daysInMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

static inline date __valid_date(int y, int m, int d)
{
	// first day of the month
    date r(y, m, 1);

    // set date according to the number of days in the specified month.
    r.setDate(y, m, pfs_min(d, r.daysInMonth()));
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
integral_t date::julianDay (int year, int month, int day) // static
{
	if (year < 0) // there is no 0 year
		++year;

	int    a = math::floorDiv(14 - month, 12);
	integral_t y = integral_t(year) + 4800 - a;
	int    m = month + 12 * a - 3;

	// Gregorian calendar: >= 15.10.1582
	if (year > 1582 || (year == 1582 && (month > 10 || (month == 10 && day >= 15)))) {
		return day + math::floorDiv(153 * m + 2, 5) + 365 * y + math::floorDiv(y, 4) - math::floorDiv(y, 100) + math::floorDiv(y, 400) - 32045;
	}
	// Julian calendar: <= 4.10.1582
	else if (year < 1582 || (year == 1582 && (month < 10 || (month == 10 && day <= 4)))) {
		return day + math::floorDiv(153 * m + 2, 5) + 365 * y + math::floorDiv(y, 4) - 32083;
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
void date::fromJulianDay (integral_t julianDay, int * yearPtr, int * monthPtr, int * dayPtr) // static
{
	integral_t b = 0;
	integral_t c = 0;

	// Gregorian calendar
	if (julianDay >= 2299161) {
		integral_t a = julianDay + 32044;
		b = math::floorDiv(4 * a + 3, 146097);
		c = a - math::floorDiv(146097 * b, 4);
	} else {
		b = 0;
		c = julianDay + 32082;
	}

    integral_t    d = math::floorDiv(4 * c + 3, 1461);
    integral_t    e = c - math::floorDiv(1461 * d, 4);
    integral_t    m = math::floorDiv(5 * e + 2, 153);

    integral_t    day = e - math::floorDiv(153 * m + 2, 5) + 1;
    integral_t    month = m + 3 - 12 * math::floorDiv(m, 10);
    integral_t    year = 100 * b + d - 4800 + math::floorDiv(m, 10);

    if (year <= 0)
        --year ;

    if (yearPtr)
        *yearPtr = int(year);
    if (monthPtr)
        *monthPtr = int(month);
    if (dayPtr)
        *dayPtr = int(day);
}


/**
 * @brief Checks if the specified date is valid
 *
 * @param year  Year value.
 * @param month Month value.
 * @param day   Day value.
 * @return @c true if the specified date is valid; otherwise returns @c false.
 */
bool date::isValid (int year, int month, int day) // static
{
    return year == 0
        ? false
    	: (day > 0 && month > 0 && month <= 12) &&
           (day <= __daysInMonth[month] || (day == 29 && month == 2 && isLeapYear(year)));
}



// 31.01.2013 + 1 mon = 29.02.2013
// 31.01.2013 - 2 mon = 30.11.2012
date date::addMonths (int nmonths) const
{
	// Note: algorithm adopted from QDate::addMonths

    if (!isValid())
        return date();

    if (!nmonths)
    	return *this;

    int y, m, d;
    fromJulianDay(_jd, & y, & m, & d);

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

date date::addYears (int nyears) const
{
	// Note: algorithm adopted from QDate::addYears

    if (!isValid())
        return date();

    int y, m, d;
    fromJulianDay(_jd, & y, & m, & d);

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
int date::dayOfWeek() const
{
    return !isValid()
        ? 0
        : (_jd >= 0)
          	  ? (_jd % 7) + 1
    		  : ((_jd + 1) % 7) + 7;
}

/**
 * @brief Returns the day of the year (1 to 365 or 366 on leap years) for
          this date.
 *
 * @return The day of the year (1 to 365 or 366 on leap years) for
    this date, or 0 if the date is invalid.
 */
int date::dayOfYear() const
{
    return !isValid()
        ? 0
        : int(_jd - julianDay(year(), 1, 1) + 1);
}

/**
 * @brief Calculates the number of days in the month (28 to 31) for this date.
 *
 * @return The number of days in the month (28 to 31) for this date
 *         or 0 if the date is invalid.
 */
int date::daysInMonth() const
{
    if (! isValid())
        return 0;

    int y, m;

    fromJulianDay(_jd, & y, & m, nullptr);

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
int date::daysInYear () const
{
    if (! isValid())
        return 0;

    int y;
    fromJulianDay(_jd, & y, nullptr, nullptr);
    return isLeapYear(y) ? 366 : 365;
}

/**
 * @brief Extracts the date's year, month, and day.
 *
 * @param year
 * @param month
 * @param day
 */
void date::split(int * year, int * month, int * day)
{
    if (isValid()) {
        fromJulianDay(_jd, year, month, day);
    } else {
        if (year)
            *year = 0;
        if (month)
            *month = 0;
        if (day)
            *day = 0;
    }
}

bool date::setDate (int year, int month, int day)
{
    if (isValid(year, month, day))
        _jd = julianDay(year, month, day);
    else
        _jd = NullJulianDay;

    return isValid();
}

int date::year () const
{
    if (!isValid())
        return 0;

    int y;
    fromJulianDay(_jd, & y, nullptr, nullptr);
    return y;
}

int date::month () const
{
    if (!isValid())
        return 0;

    int m;
    fromJulianDay(_jd, nullptr, & m, nullptr);
    return m;
}

int date::day () const
{
    if (!isValid())
        return 0;

    int d;
    fromJulianDay(_jd, nullptr, nullptr, & d);
    return d;
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
string date::toString (const char * format) const
{
	if (year() < 0 && year() > 9999)
		return string();

	std::stringstream ss;
	const char * p = format;
	const char * end = format + strlen(format);
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
					ss << std::setw(2) << std::setfill('0') << year()/100;
					break;
				case 'd':
					ss << std::setw(2) << std::setfill('0') << day();
					break;
				case 'e':
					ss << std::setw(2) << std::setfill(' ') << day();
					break;
				case 'F':
					ss << std::setw(4) << std::setfill('0') << year()
					   << '-'
					   << std::setw(2) << std::setfill('0') << month()
					   << '-'
					   << std::setw(2) << std::setfill('0') << day();
					break;
				case 'j':
					ss << std::setw(3) << std::setfill('0') << dayOfYear();
					break;
				case 'm':
					ss << std::setw(2) << std::setfill('0') << month();
					break;
				case 'u':
					ss << dayOfWeek();
					break;
				case 'y':
					ss << std::setw(2) << std::setfill('0') << year() % 100;
					break;
				case 'Y':
					ss << std::setw(4) << std::setfill('0') << year();
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
	return string::fromLatin1(ss.str().c_str());
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
string date::toString () const
{
	return toString("%F"); // equivalent to %H:%M:%S
}

} // pfs
