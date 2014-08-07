/*
 * safeformat.cpp
 *
 *  Created on: Jul 29, 2013
 *      Author: wladt
 */

#include <cwt/test.hpp>
#include <pfs/string.hpp>
#include <pfs/safeformat.hpp>

template <typename T>
bool test_arg(const char * format, T v)
{
	char buf[64];
	sprintf(buf, format, v);
	printf("sprintf: \"%s\" == \"%s\" : SafeFormat\n", buf, pfs::string(pfs::safeformat(format) % v).c_str());
	return pfs::safeformat(format) % v == pfs::string(buf);
}


int main(int argc, char *argv[])
{
	PFS_CHECK_SIZEOF_TYPES;
	PFS_UNUSED2(argc, argv);
	BEGIN_TESTS(38);

	// When 0 is printed with an explicit precision 0, the output is empty.
	TEST_OK(pfs::safeformat("%.0d") % 0 != pfs::string("0"));
	TEST_OK(test_arg<int>("%.0d", 0));

	TEST_OK(test_arg<int>("% d", -245));
	TEST_OK(test_arg<int>("% d",  245));
	TEST_OK(test_arg<int>("%+d", -245));
	TEST_OK(test_arg<int>("%+d",  245));

	TEST_OK(test_arg<int>("%.6d" , 245));
	TEST_OK(test_arg<int>("%06d" , 245));
	TEST_OK(test_arg<int>("%6d"  , 245));
	TEST_OK(test_arg<int>("%6.6d", 245));
	TEST_OK(test_arg<int>("%7.6d", 245));
	TEST_OK(test_arg<int>("%6.5d", 245));
	TEST_OK(test_arg<int>("%7.6o", 245));
	TEST_OK(test_arg<int>("%7.6X", 245));

	TEST_OK(test_arg<int>("%.6d" , 2147483647));
	TEST_OK(test_arg<int>("%06d" , 2147483647));
	TEST_OK(test_arg<int>("%6d"  , 2147483647));
	TEST_OK(test_arg<int>("%6.6d", 2147483647));
	TEST_OK(test_arg<int>("%7.6d", 2147483647));
	TEST_OK(test_arg<int>("%6.5d", 2147483647));

	TEST_OK(test_arg<float>("%.6f"   , 245.9876542f));
	TEST_OK(test_arg<float>("%06f"   , 245.9876542f));
	TEST_OK(test_arg<float>("%6f"    , 245.9876542f));
	TEST_OK(test_arg<float>("%6.6f"  , 245.9876542f));
	TEST_OK(test_arg<float>("%7.6f"  , 245.9876542f));
	TEST_OK(test_arg<float>("%6.5f"  , 245.9876542f));
	TEST_OK(test_arg<float>("%-20f"  , 245.9876542f));
	TEST_OK(test_arg<float>("%-20.5f", 245.98f     ));
	TEST_OK(test_arg<float>("%.6f"   , 245.98f     ));
	TEST_OK(test_arg<float>("%06f"   , 245.98f     ));
	TEST_OK(test_arg<float>("%6f"    , 245.98f     ));
	TEST_OK(test_arg<float>("%6.6f"  , 245.98f     ));
	TEST_OK(test_arg<float>("%7.6f"  , 245.98f     ));
	TEST_OK(test_arg<float>("%6.5f"  , 245.98f     ));

	TEST_OK(test_arg<char>("%05c" , 'A')); // gcc warning: '0' flag used with ‘%c’ gnu_printf format
	TEST_OK(test_arg<char>("%-5c" , 'A'));
	TEST_OK(test_arg<char>("%+5c" , 'A')); // gcc warning: '+' flag used with ‘%c’ gnu_printf format

	TEST_OK(test_arg<int>("%d\n", 245));

	int i;
	pfs::string s = pfs::safeformat("Hello, %s:\t iptr = %p! 0x%X 0x%x") % "World" % &i % 255 % 255;
	printf("%s\n", s.c_str());

	END_TESTS;
}




