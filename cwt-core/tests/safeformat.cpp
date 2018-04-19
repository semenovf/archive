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


template <typename T>
bool test_arg(const char * format, T v)
{
	char buf[64];
	sprintf(buf, format, v);
	printf("sprintf: \"%s\" == \"%s\" : SafeFormat\n", buf, String(SafeFormat(format) % v).c_str());
	return SafeFormat(format) % v == String(buf);
}


int main(int argc, char *argv[])
{
	CWT_CHECK_SIZEOF_TYPES;

	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(38);

	// When 0 is printed with an explicit precision 0, the output is empty.
	CWT_TEST_NOK(SafeFormat("%.0d") % 0 == String("0"));
	CWT_TEST_OK(test_arg<int>("%.0d", 0));

	CWT_TEST_OK(test_arg<int>("% d", -245));
	CWT_TEST_OK(test_arg<int>("% d",  245));
	CWT_TEST_OK(test_arg<int>("%+d", -245));
	CWT_TEST_OK(test_arg<int>("%+d",  245));

	CWT_TEST_OK(test_arg<int>("%.6d" , 245));
	CWT_TEST_OK(test_arg<int>("%06d" , 245));
	CWT_TEST_OK(test_arg<int>("%6d"  , 245));
	CWT_TEST_OK(test_arg<int>("%6.6d", 245));
	CWT_TEST_OK(test_arg<int>("%7.6d", 245));
	CWT_TEST_OK(test_arg<int>("%6.5d", 245));
	CWT_TEST_OK(test_arg<int>("%7.6o", 245));
	CWT_TEST_OK(test_arg<int>("%7.6X", 245));

	CWT_TEST_OK(test_arg<int>("%.6d" , 2147483647));
	CWT_TEST_OK(test_arg<int>("%06d" , 2147483647));
	CWT_TEST_OK(test_arg<int>("%6d"  , 2147483647));
	CWT_TEST_OK(test_arg<int>("%6.6d", 2147483647));
	CWT_TEST_OK(test_arg<int>("%7.6d", 2147483647));
	CWT_TEST_OK(test_arg<int>("%6.5d", 2147483647));

	CWT_TEST_OK(test_arg<float>("%.6f"   , 245.9876542f));
	CWT_TEST_OK(test_arg<float>("%06f"   , 245.9876542f));
	CWT_TEST_OK(test_arg<float>("%6f"    , 245.9876542f));
	CWT_TEST_OK(test_arg<float>("%6.6f"  , 245.9876542f));
	CWT_TEST_OK(test_arg<float>("%7.6f"  , 245.9876542f));
	CWT_TEST_OK(test_arg<float>("%6.5f"  , 245.9876542f));
	CWT_TEST_OK(test_arg<float>("%-20f"  , 245.9876542f));
	CWT_TEST_OK(test_arg<float>("%-20.5f", 245.98f     ));
	CWT_TEST_OK(test_arg<float>("%.6f"   , 245.98f     ));
	CWT_TEST_OK(test_arg<float>("%06f"   , 245.98f     ));
	CWT_TEST_OK(test_arg<float>("%6f"    , 245.98f     ));
	CWT_TEST_OK(test_arg<float>("%6.6f"  , 245.98f     ));
	CWT_TEST_OK(test_arg<float>("%7.6f"  , 245.98f     ));
	CWT_TEST_OK(test_arg<float>("%6.5f"  , 245.98f     ));

	CWT_TEST_OK(test_arg<char>("%05c" , 'A')); // gcc warning: '0' flag used with ‘%c’ gnu_printf format
	CWT_TEST_OK(test_arg<char>("%-5c" , 'A'));
	CWT_TEST_OK(test_arg<char>("%+5c" , 'A')); // gcc warning: '+' flag used with ‘%c’ gnu_printf format

	CWT_TEST_OK(test_arg<int>("%d\n", 245));

	int i;
	String s = SafeFormat("Hello, %s:\t iptr = %p! 0x%X 0x%x") % "World" % &i % 255 % 255;
	printf("%s\n", s.c_str());

	CWT_END_TESTS;
}




