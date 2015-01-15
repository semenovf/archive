/*
 * test.cpp
 *
 *  Created on: Oct 10, 2014
 *      Author: wladt
 */

#include <cwt/test.hpp>
#include <pfs/bits/strtolong.hpp>
#include <cstring>

bool compare_unsigned (const char * s, ulong_t num, int radix = 10)
{
	const char * endptr;
	const char * begin = s;
	const char * end = s + std::strlen(s);

	return pfs::strtoulong<char,const char *>(begin, end, radix, & endptr) == num
			&& endptr == end && errno == 0;
}

bool compare_signed (const char * s, long_t num, int radix = 10)
{
	const char * endptr;
	const char * begin = s;
	const char * end = s + std::strlen(s);

	return pfs::strtolong<char,const char *>(begin, end, radix, & endptr) == num
			&& endptr == end && errno == 0;
}

int main(int argc, char *argv[])
{
    PFS_CHECK_SIZEOF_TYPES;
    PFS_UNUSED2(argc, argv);
    int ntests = 21;

    BEGIN_TESTS(ntests);

	TEST_OK(compare_unsigned("0", 0));
	TEST_OK(compare_unsigned("+0", 0));
	TEST_OK(compare_unsigned("-0", 0));

	TEST_OK(compare_unsigned("1", 1));
	TEST_OK(compare_unsigned("+1", 1));
	TEST_OK(compare_unsigned("-1", -1));

	TEST_OK(compare_unsigned("123", 123));
	TEST_OK(compare_unsigned("+123", 123));
	TEST_OK(compare_unsigned("-123", -123));

#ifdef PFS_HAVE_INT64
	TEST_OK(compare_unsigned("18446744073709551615", PFS_ULONG_MAX));
#else
	TEST_OK(compare_unsigned("4294967295", PFS_ULONG_MAX));
#endif


	TEST_OK(compare_signed("0", 0));
	TEST_OK(compare_signed("+0", 0));
	TEST_OK(compare_signed("-0", 0));

	TEST_OK(compare_signed("1", 1));
	TEST_OK(compare_signed("+1", 1));
	TEST_OK(compare_signed("-1", -1));

	TEST_OK(compare_signed("123", 123));
	TEST_OK(compare_signed("+123", 123));
	TEST_OK(compare_signed("-123", -123));

#ifdef PFS_HAVE_INT64
	TEST_OK(compare_signed("9223372036854775807", PFS_LONG_MAX));
	TEST_OK(compare_signed("-9223372036854775808", PFS_LONG_MIN));
#else
	TEST_OK(compare_signed("2147483647", PFS_LONG_MAX));
	TEST_OK(compare_signed("-2147483648", PFS_LONG_MIN));
#endif

    END_TESTS;
}
