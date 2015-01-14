/*
 * test.cpp
 *
 *  Created on: Oct 10, 2014
 *      Author: wladt
 */

#include <cwt/test.hpp>
#include <pfs/bits/strtolong.hpp>
#include <cstring>
//#include <cstdio>
//#include <cstdlib>

//#include <iostream>
//#include <clocale>
//#include <cerrno>
//#include <cmath> // for isnan and ininf

bool compare_with_number (const char * s, long_t num, int radix = 10)
{
	const char * endptr;
	const char * begin = s;
	const char * end = s + std::strlen(s);

	return pfs::strtolong<char,const char *>(begin, end, radix, endptr) == num
			&& endptr == end;
}

int main(int argc, char *argv[])
{
    PFS_CHECK_SIZEOF_TYPES;
    PFS_UNUSED2(argc, argv);
    int ntests = 1;
	BEGIN_TESTS(ntests);

//	TEST_OK(compare_with_number("0", 0));
//	TEST_OK(compare_with_number("+0", 0));
//	TEST_OK(compare_with_number("-0", 0));
//
//	TEST_OK(compare_with_number("1", 1));
//	TEST_OK(compare_with_number("+1", 1));
//	TEST_OK(compare_with_number("-1", -1));

	TEST_OK(compare_with_number("123", 123));
	TEST_OK(compare_with_number("+123", 123));
	TEST_OK(compare_with_number("-123", -123));

    END_TESTS;
}
