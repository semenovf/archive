/*
 * time.cpp
 *
 *  Created on: Sep 13, 2013
 *      Author: wladt
 */

#include <pfs/test/test.hpp>
#include <pfs/time.hpp>

typedef std::string string_type;
typedef std::stringstream stringstream_type;
typedef pfs::time time_type;

void test_base (void)
{
	ADD_TESTS(8);

	pfs::time n(14, 0, 0);                 // n == 14:00:00
	pfs::time t;

	t = n.add_seconds(70);                // t == 14:01:10
	TEST_OK(t.hour() == 14 && t.minute() == 1 && t.second() == 10)

	t = n.add_seconds(-70);               // t == 13:58:50
	TEST_OK(t.hour() == 13 && t.minute() == 58 && t.second() == 50)

	t = n.add_seconds(10 * 60 * 60 + 5);  // t == 00:00:05
	TEST_OK(t.hour() == 0 && t.minute() == 0 && t.second() == 5)

	t = n.add_seconds(-15 * 60 * 60);     // t == 23:00:00
	TEST_OK(t.hour() == 23 && t.minute() == 0 && t.second() == 0)

	pfs::time t1(11, 59, 12, 414);
	TEST_OK(t1.hour() == 11);
	TEST_OK(t1.minute() == 59);
	TEST_OK(t1.second() == 12);
	TEST_OK(t1.millis() == 414);
}

void test_stringify ()
{
	ADD_TESTS(6);

	string_type s;
	TEST_OK(pfs::to_string(s, time_type(14, 0, 0), string_type("%H:%M:%S")) == string_type("14:00:00"));
	TEST_OK(pfs::to_string(s, time_type(9, 54, 3), string_type("%H:%M:%S")) == string_type("09:54:03"));
	TEST_OK(pfs::to_string(s, time_type(9, 54, 3), string_type("[%%%H::%M::%S%%]")) == string_type("[%09::54::03%]"));
	TEST_OK(pfs::to_string(s, time_type(14, 0, 0)) == string_type("14:00:00"));
	TEST_OK(pfs::to_string(s, time_type(9, 54, 3)) == string_type("09:54:03"));
	TEST_OK(*pfs::to_string(time_type(11, 59, 12, 414), string_type("%J")) == string_type("11:59:12.414"));
}

int main(int argc, char *argv[])
{
    PFS_UNUSED2(argc, argv);
	BEGIN_TESTS(0);

	test_base();
	test_stringify();

    END_TESTS;
}




