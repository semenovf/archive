/*
 * string.cpp
 *
 *  Created on: Mar 22, 2013
 *      Author: wladt
 */

#include <pfs/test/test.hpp>
#include <pfs/stringlist.hpp>
#include <pfs/regexp.hpp>
#include <iostream>

void test_simple_match()
{
    ADD_TESTS(16);
	pfs::regexp rx("\\d\\d \\w+");

	TEST_OK(rx.match("abc123 def"));
	TEST_OK(rx.match("12 abc 45 def"));

	std::cout << "Captured: '" << rx.captured() << '\'' << std::endl;

	TEST_OK(rx.captured() == "12 abc");
	TEST_OK(rx.matchNext());
	TEST_OK(rx.captured() == "45 def");
	TEST_OK(!rx.matchNext());

	rx.setPattern("^(\\d\\d)/(\\d\\d)/(\\d\\d\\d\\d)$");
	TEST_FAIL(rx.match("08/12/1985"));
	TEST_FAIL(rx.capturedCount() == 4);
	TEST_OK(rx.captured(0) == "08/12/1985");
	TEST_OK(rx.captured(1) == "08");
	TEST_OK(rx.captured(2) == "12");
	TEST_OK(rx.captured(3) == "1985");

	pfs::stringlist groups = rx.groups();

	TEST_FAIL(groups.size() == 3);
	TEST_OK(groups[0] == "08");
	TEST_OK(groups[1] == "12");
	TEST_OK(groups[2] == "1985");
}

int main(int argc, char *argv[])
{
    PFS_UNUSED2(argc, argv);
	BEGIN_TESTS(0);

	test_simple_match();

	return END_TESTS;
}
