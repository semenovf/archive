/*
 * date.cpp
 *
 *  Created on: Sep 12, 2013
 *      Author: wladt
 */

/*
 * string.cpp
 *
 *  Created on: Mar 22, 2013
 *      Author: wladt
 */

#include <cwt/test.h>
#include <cwt/date.hpp>
#include <iostream>

using namespace cwt;

void test_min_max (void)
{
	CWT_TEST_OK(Date::MinJulianDay == Date::julianDay(CWT_INT_MIN,  1,  1));
	CWT_TEST_OK(Date::MaxJulianDay == Date::julianDay(CWT_INT_MAX, 12, 31));
	CWT_TEST_OK(Date::EpochJulianDay == Date::julianDay(1970, 1, 1));
}

void test_jd (void)
{
	int y, m, d;

	CWT_TEST_NOK(Date(0,  0,  0).isValid());
	CWT_TEST_OK(Date::julianDay(    1,  1,  1) == 1721424);

	Date::fromJulianDay(1721424, & y, & m, & d);
	CWT_TEST_OK2(y == 1 && m == 1 && d == 1, _Tr("JD(1721424) is 01.01.0001"));

	// Changes from Julian to Gregorian between October 4/15, 1582.
	CWT_TEST_OK(Date::julianDay( 1582, 10,  4) == 2299160);
	CWT_TEST_OK(Date::julianDay( 1582, 10,  5) == Date::NullJulianDay); // invalid
	CWT_TEST_OK(Date::julianDay( 1582, 10,  6) == Date::NullJulianDay); // invalid
	CWT_TEST_OK(Date::julianDay( 1582, 10,  7) == Date::NullJulianDay); // invalid
	CWT_TEST_OK(Date::julianDay( 1582, 10,  8) == Date::NullJulianDay); // invalid
	CWT_TEST_OK(Date::julianDay( 1582, 10,  9) == Date::NullJulianDay); // invalid
	CWT_TEST_OK(Date::julianDay( 1582, 10, 10) == Date::NullJulianDay); // invalid
	CWT_TEST_OK(Date::julianDay( 1582, 10, 11) == Date::NullJulianDay); // invalid
	CWT_TEST_OK(Date::julianDay( 1582, 10, 12) == Date::NullJulianDay); // invalid
	CWT_TEST_OK(Date::julianDay( 1582, 10, 13) == Date::NullJulianDay); // invalid
	CWT_TEST_OK(Date::julianDay( 1582, 10, 14) == Date::NullJulianDay); // invalid
	CWT_TEST_OK(Date::julianDay( 1582, 10, 15) == 2299161);

	CWT_TEST_OK(Date::julianDay( 1582, 10,  3) == 2299159);
	CWT_TEST_OK(Date::julianDay( 1582, 10, 16) == 2299162);

	CWT_TEST_OK(Date::julianDay( 1600,  1,  1) == 2305448);

	CWT_TEST_OK(Date::julianDay( 2013,  9, 13) == 2456549);
	CWT_TEST_OK(Date::julianDay(-2013,  9, 13) ==  986431);

	CWT_TEST_OK(Date(2013, 9, 13).daysTo(Date(2013,  9, 13)) == 0);
	CWT_TEST_OK(Date(2013, 9, 13).daysTo(Date(2013,  9, 12)) == -1);
	CWT_TEST_OK(Date(2013, 9, 13).daysTo(Date(2013,  6,  5)) == -100);
	CWT_TEST_OK(Date(2013, 9, 13).daysTo(Date(2010, 12, 18)) == -1000);
	CWT_TEST_OK(Date(2013, 9, 13).daysTo(Date(1986,  4, 28)) == -10000);
	CWT_TEST_OK(Date(2013, 9, 13).daysTo(Date(1739, 11, 29)) == -100000);
	CWT_TEST_OK(Date(2013, 9, 13).daysTo(Date(-726, 10, 25)) == -1000000);
	CWT_TEST_OK(Date(2013, 9, 13).daysTo(Date(-25366, 2, 27)) == -10000000);

	Date d1(2013, 9, 13);
	d1 = d1.addDays(-10000000);
	//Date::fromJulianDay(d1.julianDay(), & y, & m, & d);
	//std::cout << "13.09.2013 - 10000000 days is " << d << "." << m << "." << y << std::endl;
	//long_t jd = Date::julianDay(y, m, d);
	//CWT_TEST_OK(jd == d1.julianDay());

	Date d2(-25366, 2, 27);
	CWT_TEST_OK2(d1.isValid(), _Tr("13.9.2013 - 10000000 is a valid date and it is 27.2.25366 BC"));
	CWT_TEST_OK2(d2.isValid(), _Tr("27.2.25366 BC is valid date"));
	CWT_TEST_OK2(!Date(-25365, 2, 30).isValid(), _Tr("30.2.25365 BC is invalid date"));

	// 4.10.1582 + 1 day = 15.10.1582
	CWT_TEST_OK(Date(1582, 10, 4).addDays(1) == Date(1582, 10, 15));
	CWT_TEST_OK(Date(1582, 10, 15).addDays(-1) == Date(1582, 10, 4));
	//std::cout << d1.julianDay() << " == " << d2.julianDay()  << std::endl;
}

// From Qt's QDate class reference
void test_valid (void)
{
	 CWT_TEST_OK2(Date::isValid(2002, 5, 17)   , _Tr("17.05.2002 is a valid date"));  // true
	 CWT_TEST_OK2(! Date::isValid(2002, 2, 30) , _Tr("30.02.2002 is not a valid date: Feb 30 does not exist"));
	 CWT_TEST_OK2(Date::isValid(2004, 2, 29)   , _Tr("29.02.2004 is a valid date: 2004 is a leap year"));
	 CWT_TEST_OK2(Date::isValid(2000, 2, 29)   , _Tr("29.02.2004 is a valid date: 2000 is a leap year"));
	 CWT_TEST_OK2(! Date::isValid(2006, 2, 29) , _Tr("29.02.2006 is not a valid date: 2006 is not a leap year"));
	 CWT_TEST_OK2(! Date::isValid(2100, 2, 29) , _Tr("29.02.2100 is not a valid date: 2100 is not a leap year"));
	 CWT_TEST_OK2(Date::isValid(1202, 6, 6)    , _Tr("06.06.1202 is a valid date: even though 1202 is pre-Gregorian"));

	 int year;
	 int month;
	 int day;
	 long_t jd = long_t(2456575);
	 Date::fromJulianDay(jd, & year, & month, & day);
	 CWT_TEST_OK(jd == Date::julianDay(year, month, day));
}

void test_periods (void)
{
	 Date d1(1995, 5, 17);  // May 17, 1995
	 Date d2(1995, 5, 20);  // May 20, 1995
	 CWT_TEST_OK2(d1.daysTo(d2) == 3, _Tr("Days bitween 17.05.1995 and 20.05.1995 is 3"));
	 CWT_TEST_OK2(d2.daysTo(d1) == -3, _Tr("Days bitween 20.05.1995 and 17.05.1995 is -3"));
}

int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
	CWT_BEGIN_TESTS(45);

	test_min_max();
	test_jd();
	test_valid();
	test_periods();

    CWT_END_TESTS;
}



