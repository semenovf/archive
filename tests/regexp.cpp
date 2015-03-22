/*
 * string.cpp
 *
 *  Created on: Mar 22, 2013
 *      Author: wladt
 */

#include <cwt/test.hpp>
#include <pfs/regexp.hpp>
#include <iostream>

void test_simple_match()
{
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

	pfs::vector<pfs::string> groups = rx.groups();
	TEST_FAIL(groups.size() == 3);
	TEST_OK(groups[0] == _l1("08"));
	TEST_OK(groups[1] == _l1("12"));
	TEST_OK(groups[2] == _l1("1985"));
}

int main(int argc, char *argv[])
{
    PFS_CHECK_SIZEOF_TYPES;
    PFS_UNUSED2(argc, argv);
	BEGIN_TESTS(16);

	test_simple_match();

    END_TESTS;
}
