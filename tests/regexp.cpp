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
	RegExp re("\\d\\d \\w+");
	RegExpMatch match = re.match("abc123 def");
	CWT_TEST_OK(match.hasMatch());

	re.setPattern("\\d\\d \\w+");
	match = re.match("abc123 def");
	CWT_TEST_FAIL(match.hasMatch());
	CWT_TEST_OK(match.captured(0) == "23 def");

	re.setPattern("\\d\\d \\w+");
	match = re.match("12 abc 45 def");
	CWT_TEST_FAIL(match.hasMatch());
	CWT_TEST_OK(match.captured(0) == "12 abc");
	CWT_TEST_OK(match.captured(1) == "45 def");


	/*
	RegExp rx("^([-+])?(\\d+)(px|pt|em|%)?$");
	RegExpMatch match = rx.match("+230pt");
	RegExpMatch::iterator it = match.begin();
	RegExpMatch::iterator itEnd = match.end();

	while (it != itEnd) {
		std::cout << *it << std::endl;
		++it;
	}
*/
}

int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
	CWT_BEGIN_TESTS(1);

	test_simple_match();

    CWT_END_TESTS;
}
