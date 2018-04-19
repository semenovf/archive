/*
 * string.cpp
 *
 *  Created on: Mar 22, 2013
 *      Author: wladt
 */

#include <cwt/test.h>
#include <cwt/regexp.hpp>
#include <iostream>

using namespace cwt;

void test_simple_match()
{
	RegExp rx("\\d\\d \\w+");
	CWT_TEST_OK(rx.match("abc123 def"));
	CWT_TEST_OK(rx.match("12 abc 45 def"));
	CWT_TEST_OK(rx.captured() == "12 abc");
	CWT_TEST_OK(rx.matchNext());
	CWT_TEST_OK(rx.captured() == "45 def");
	CWT_TEST_NOK(rx.matchNext());

	rx.setPattern("^(\\d\\d)/(\\d\\d)/(\\d\\d\\d\\d)$");
	CWT_TEST_FAIL(rx.match("08/12/1985"));
	CWT_TEST_FAIL(rx.capturedCount() == 4);
	CWT_TEST_OK(rx.captured(0) == "08/12/1985");
	CWT_TEST_OK(rx.captured(1) == "08");
	CWT_TEST_OK(rx.captured(2) == "12");
	CWT_TEST_OK(rx.captured(3) == "1985");

	StringList groups = rx.groups();
	CWT_TEST_FAIL(groups.size() == 3);
	CWT_TEST_OK(groups[0] == "08");
	CWT_TEST_OK(groups[1] == "12");
	CWT_TEST_OK(groups[2] == "1985");
}

int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
	CWT_BEGIN_TESTS(16);

	test_simple_match();

    CWT_END_TESTS;
}
