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

#include <cwt/test.hpp>
#include <pfs/date.hpp>
#include <sstream>

void test_min_max (void)
{
	TEST_OK(pfs::date::MinJulianDay == pfs::date::julianDay(PFS_INT_MIN,  1,  1));
	TEST_OK(pfs::date::MaxJulianDay == pfs::date::julianDay(PFS_INT_MAX, 12, 31));
	TEST_OK(pfs::date::EpochJulianDay == pfs::date::julianDay(1970, 1, 1));
}

void test_jd (void)
{
	int y, m, d;

	TEST_OK(!pfs::date(0,  0,  0).isValid());
	TEST_OK(pfs::date::julianDay(    1,  1,  1) == 1721424);

	pfs::date::fromJulianDay(1721424, & y, & m, & d);
	TEST_OK2(y == 1 && m == 1 && d == 1, _Tr("JD(1721424) is 01.01.0001"));

	// Changes from Julian to Gregorian between October 4/15, 1582.
	TEST_OK(pfs::date::julianDay( 1582, 10,  4) == 2299160);
	TEST_OK(pfs::date::julianDay( 1582, 10,  5) == pfs::date::NullJulianDay); // invalid
	TEST_OK(pfs::date::julianDay( 1582, 10,  6) == pfs::date::NullJulianDay); // invalid
	TEST_OK(pfs::date::julianDay( 1582, 10,  7) == pfs::date::NullJulianDay); // invalid
	TEST_OK(pfs::date::julianDay( 1582, 10,  8) == pfs::date::NullJulianDay); // invalid
	TEST_OK(pfs::date::julianDay( 1582, 10,  9) == pfs::date::NullJulianDay); // invalid
	TEST_OK(pfs::date::julianDay( 1582, 10, 10) == pfs::date::NullJulianDay); // invalid
	TEST_OK(pfs::date::julianDay( 1582, 10, 11) == pfs::date::NullJulianDay); // invalid
	TEST_OK(pfs::date::julianDay( 1582, 10, 12) == pfs::date::NullJulianDay); // invalid
	TEST_OK(pfs::date::julianDay( 1582, 10, 13) == pfs::date::NullJulianDay); // invalid
	TEST_OK(pfs::date::julianDay( 1582, 10, 14) == pfs::date::NullJulianDay); // invalid
	TEST_OK(pfs::date::julianDay( 1582, 10, 15) == 2299161);

	TEST_OK(pfs::date::julianDay( 1582, 10,  3) == 2299159);
	TEST_OK(pfs::date::julianDay( 1582, 10, 16) == 2299162);

	TEST_OK(pfs::date::julianDay( 1600,  1,  1) == 2305448);

	TEST_OK(pfs::date::julianDay( 2013,  9, 13) == 2456549);
	TEST_OK(pfs::date::julianDay(-2013,  9, 13) ==  986431);

	TEST_OK(pfs::date(2013, 9, 13).daysTo(pfs::date(2013,  9, 13)) == 0);
	TEST_OK(pfs::date(2013, 9, 13).daysTo(pfs::date(2013,  9, 12)) == -1);
	TEST_OK(pfs::date(2013, 9, 13).daysTo(pfs::date(2013,  6,  5)) == -100);
	TEST_OK(pfs::date(2013, 9, 13).daysTo(pfs::date(2010, 12, 18)) == -1000);
	TEST_OK(pfs::date(2013, 9, 13).daysTo(pfs::date(1986,  4, 28)) == -10000);
	TEST_OK(pfs::date(2013, 9, 13).daysTo(pfs::date(1739, 11, 29)) == -100000);
	TEST_OK(pfs::date(2013, 9, 13).daysTo(pfs::date(-726, 10, 25)) == -1000000);
	TEST_OK(pfs::date(2013, 9, 13).daysTo(pfs::date(-25366, 2, 27)) == -10000000);

	pfs::date d1(2013, 9, 13);
	d1 = d1.addDays(-10000000);
	//pfs::date::fromJulianDay(d1.julianDay(), & y, & m, & d);
	//std::cout << "13.09.2013 - 10000000 days is " << d << "." << m << "." << y << std::endl;
	//long_t jd = pfs::date::julianDay(y, m, d);
	//TEST_OK(jd == d1.julianDay());

	pfs::date d2(-25366, 2, 27);
	TEST_OK2(d1.isValid(), _Tr("13.9.2013 - 10000000 is a valid date and it is 27.2.25366 BC"));
	TEST_OK2(d2.isValid(), _Tr("27.2.25366 BC is valid date"));
	TEST_OK2(!pfs::date(-25365, 2, 30).isValid(), _Tr("30.2.25365 BC is invalid date"));

	// 4.10.1582 + 1 day = 15.10.1582
	TEST_OK(pfs::date(1582, 10, 4).addDays(1) == pfs::date(1582, 10, 15));
	TEST_OK(pfs::date(1582, 10, 15).addDays(-1) == pfs::date(1582, 10, 4));
	//std::cout << d1.julianDay() << " == " << d2.julianDay()  << std::endl;
}

// From Qt's QDate class reference
void test_valid (void)
{
	 TEST_OK2(pfs::date::isValid(2002, 5, 17)   , _Tr("17.05.2002 is a valid date"));  // true
	 TEST_OK2(! pfs::date::isValid(2002, 2, 30) , _Tr("30.02.2002 is not a valid date: Feb 30 does not exist"));
	 TEST_OK2(pfs::date::isValid(2004, 2, 29)   , _Tr("29.02.2004 is a valid date: 2004 is a leap year"));
	 TEST_OK2(pfs::date::isValid(2000, 2, 29)   , _Tr("29.02.2004 is a valid date: 2000 is a leap year"));
	 TEST_OK2(! pfs::date::isValid(2006, 2, 29) , _Tr("29.02.2006 is not a valid date: 2006 is not a leap year"));
	 TEST_OK2(! pfs::date::isValid(2100, 2, 29) , _Tr("29.02.2100 is not a valid date: 2100 is not a leap year"));
	 TEST_OK2(pfs::date::isValid(1202, 6, 6)    , _Tr("06.06.1202 is a valid date: even though 1202 is pre-Gregorian"));

	 int year;
	 int month;
	 int day;
	 long_t jd = long_t(2456575);
	 pfs::date::fromJulianDay(jd, & year, & month, & day);

	 std::stringstream ss;
	 ss << "9.10.2013 == " << jd;
	 TEST_OK2(pfs::date::julianDay( 2013, 10,  9) == jd, ss.str().c_str());

	 ss.clear();
	 ss << jd << " (9.10.2013) == " << day << '.' << month << '.' << year;
	 TEST_OK2(jd == pfs::date::julianDay(year, month, day), ss.str().c_str());
}

void test_periods (void)
{
	 pfs::date d1(1995, 5, 17);  // May 17, 1995
	 pfs::date d2(1995, 5, 20);  // May 20, 1995
	 TEST_OK2(d1.daysTo(d2) == 3, _Tr("Days bitween 17.05.1995 and 20.05.1995 is 3"));
	 TEST_OK2(d2.daysTo(d1) == -3, _Tr("Days bitween 20.05.1995 and 17.05.1995 is -3"));
}

void test_stringify ()
{
	TEST_OK(pfs::date(2013, 11, 28).toString() == _u8("2013-11-28"));
	TEST_OK(pfs::date(1, 2, 3).toString() == _u8("0001-02-03"));
}

int main(int argc, char *argv[])
{
    PFS_CHECK_SIZEOF_TYPES;
    PFS_UNUSED2(argc, argv);
	BEGIN_TESTS(49);

	test_min_max();
	test_jd();
	test_valid();
	test_periods();
	test_stringify();

    END_TESTS;
}



