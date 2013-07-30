/*
 * safeformat.cpp
 *
 *  Created on: Jul 29, 2013
 *      Author: wladt
 */

#include "../include/cwt/test.h"
#include "../include/cwt/string.hpp"
#include "../include/cwt/safeformat.hpp"
//#include <QString>

using namespace cwt;

int main(int argc, char *argv[])
{
	CWT_CHECK_SIZEOF_TYPES;

	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(1);

	int i;
	String s = SafeFormat("Hello, %s: iptr = %p!")("World")(&i);

/*
	CWT_TEST_OK(true == true);
	QString qs;
	qs.sprintf("Hello, %s: iptr = %p!", "World", &i);
	CWT_TEST_OK(true == true);

	char buf[256];
	CWT_TEST_OK(true == true);
	::sprintf(buf, "Hello, %s: iptr = %p!", "World", &i);
	CWT_TEST_OK(true == true);
*/

	printf("%s\n", s.c_str());
	printf("iptr = %p; sizeof(intptr_t) = %d, sizeof(void*) = %d\n", &i, sizeof(intptr_t), sizeof(void*));

	CWT_END_TESTS;
}




