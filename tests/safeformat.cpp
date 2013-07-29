/*
 * safeformat.cpp
 *
 *  Created on: Jul 29, 2013
 *      Author: wladt
 */

#include "../include/cwt/test.h"
#include "../include/cwt/string.hpp"
#include "../include/cwt/safeformat.hpp"

using namespace cwt;

int main(int argc, char *argv[])
{
	CWT_CHECK_SIZEOF_TYPES;

	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(1);

	String s = SafeFormat<String>("Hello, %s")("World");
	printf("%s\n", s.c_str());

	CWT_END_TESTS;
}




