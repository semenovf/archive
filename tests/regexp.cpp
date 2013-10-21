/*
 * string.cpp
 *
 *  Created on: Mar 22, 2013
 *      Author: wladt
 */

#include <cwt/test.h>
#include <cwt/regexp.hpp>

using namespace cwt;

void test_exact_match()
{
	RegExp re("^([-+])?(\\d+)(px|pt|em|%)?$");
	RegExp::iterator it = re.begin("+230pt");
	RegExp::iterator itEnd = re.end();

	while (it != itEnd) {
		++it;
	}
/*
	re.capturedText(0);
	re.capturedText(1);
	re.capturedText(2);
*/
}

int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
	CWT_BEGIN_TESTS(1);

	test_exact_match();

    CWT_END_TESTS;
}
