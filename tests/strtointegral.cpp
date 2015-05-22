/*
 * test.cpp
 *
 *  Created on: Oct 10, 2014
 *      Author: wladt
 */

#include <pfs/test.hpp>
#include <pfs/bits/strtointegral.hpp>
#include <cstring>

bool compare_unsigned (const char * s, uintegral_t num, int radix = 10)
{
	const char * endptr;
	const char * begin = s;
	const char * end = s + std::strlen(s);

	return pfs::strtouintegral<char,const char *>(begin, end, radix,pfs::max_type<uintegral_t>(), & endptr) == num
			&& endptr == end && errno == 0;
}

bool compare_signed (const char * s, integral_t num, int radix = 10)
{
	const char * endptr;
	const char * begin = s;
	const char * end = s + std::strlen(s);

	return pfs::strtointegral<char,const char *>(begin, end, radix
			, pfs::min_type<integral_t>(), pfs::max_type<uintegral_t>(), & endptr) == num
			&& endptr == end && errno == 0;
}

int main(int argc, char *argv[])
{
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
	TEST_OK(compare_unsigned("18446744073709551615", PFS_UINTEGRAL_MAX));
#else
	TEST_OK(compare_unsigned("4294967295", PFS_UINTEGRAL_MAX));
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
	TEST_OK(compare_signed("9223372036854775807", PFS_INTEGRAL_MAX));
	TEST_OK(compare_signed("-9223372036854775808", PFS_INTEGRAL_MIN));
#else
	TEST_OK(compare_signed("2147483647", PFS_INTEGRAL_MAX));
	TEST_OK(compare_signed("-2147483648", PFS_INTEGRAL_MIN));
#endif

    END_TESTS;
}
