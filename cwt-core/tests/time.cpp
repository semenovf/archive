/*
 * time.cpp
 *
 *  Created on: Sep 13, 2013
 *      Author: wladt
 */

#include <cwt/test.h>
#include <cwt/time.hpp>
#include <iostream>

using namespace cwt;

void test_base (void)
{
	 Time n(14, 0, 0);                 // n == 14:00:00
	 Time t;

	 t = n.addSecs(70);                // t == 14:01:10
	 CWT_TEST_OK(t.hour() == 14 && t.minute() == 1 && t.second() == 10)

	 t = n.addSecs(-70);               // t == 13:58:50
	 CWT_TEST_OK(t.hour() == 13 && t.minute() == 58 && t.second() == 50)

	 t = n.addSecs(10 * 60 * 60 + 5);  // t == 00:00:05
	 CWT_TEST_OK(t.hour() == 0 && t.minute() == 0 && t.second() == 5)

	 t = n.addSecs(-15 * 60 * 60);     // t == 23:00:00
	 CWT_TEST_OK(t.hour() == 23 && t.minute() == 0 && t.second() == 0)
}

int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
	CWT_BEGIN_TESTS(4);

	test_base();

    CWT_END_TESTS;
}




