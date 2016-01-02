/**
 * @file   test.cpp
 * @author
 * @date
 *
 * @brief testing ...
 */

#include <pfs/test/test.hpp>
#include <pfs/types.hpp>
#include <pfs/limits.hpp>
#include <iostream>

using std::cout;
using std::endl;

int main (int /*argc*/, char **/*argv*/)
{
	BEGIN_TESTS(32);

	int8_t i8;
	TEST_OK(pfs::min_value<int8_t>() == -128);
	TEST_OK(pfs::min_value<int8_t>() == pfs::min_value(i8));
	TEST_OK(pfs::max_value<int8_t>() == 127);
	TEST_OK(pfs::max_value<int8_t>() == pfs::max_value(i8));

	uint8_t ui8;
	TEST_OK(pfs::min_value<uint8_t>() == 0);
	TEST_OK(pfs::min_value<uint8_t>() == pfs::min_value(ui8));
	TEST_OK(pfs::max_value<uint8_t>() == 255);
	TEST_OK(pfs::max_value<uint8_t>() == pfs::max_value(ui8));

	int16_t i16;
	TEST_OK(pfs::min_value<int16_t>() == -32768);
	TEST_OK(pfs::min_value<int16_t>() == pfs::min_value(i16));
	TEST_OK(pfs::max_value<int16_t>() == 32767);
	TEST_OK(pfs::max_value<int16_t>() == pfs::max_value(i16));

	uint16_t ui16;
	TEST_OK(pfs::min_value<uint16_t>() == 0);
	TEST_OK(pfs::min_value<uint16_t>() == pfs::min_value(ui16));
	TEST_OK(pfs::max_value<uint16_t>() == 65535);
	TEST_OK(pfs::max_value<uint16_t>() == pfs::max_value(ui16));

	int32_t i32;
	TEST_OK(pfs::min_value<int32_t>() == -2147483647-1);
	TEST_OK(pfs::min_value<int32_t>() == pfs::min_value(i32));
	TEST_OK(pfs::max_value<int32_t>() == 2147483647);
	TEST_OK(pfs::max_value<int32_t>() == pfs::max_value(i32));

	uint32_t ui32;
	TEST_OK(pfs::min_value<uint32_t>() == 0);
	TEST_OK(pfs::min_value<uint32_t>() == pfs::min_value(ui32));
	TEST_OK(pfs::max_value<uint32_t>() == 4294967295UL);
	TEST_OK(pfs::max_value<uint32_t>() == pfs::max_value(ui32));

#ifdef PFS_HAVE_INT64
	ADD_TESTS(8);

	int64_t i64;
	TEST_OK(pfs::min_value<int64_t>() == -9223372036854775807LL-1);
	TEST_OK(pfs::min_value<int64_t>() == pfs::min_value(i64));
	TEST_OK(pfs::max_value<int64_t>() == 9223372036854775807LL);
	TEST_OK(pfs::max_value<int64_t>() == pfs::max_value(i64));

	uint64_t ui64;
	TEST_OK(pfs::min_value<uint64_t>() == 0);
	TEST_OK(pfs::min_value<uint64_t>() == pfs::min_value(ui64));
	TEST_OK(pfs::max_value<uint64_t>() == 18446744073709551615ULL);
	TEST_OK(pfs::max_value<uint64_t>() == pfs::max_value(ui64));
#endif

	float f;
	TEST_OK(pfs::min_value<float>() == 1.175494351e-38F);
	TEST_OK(pfs::min_value<float>() == pfs::min_value(f));
	TEST_OK(pfs::max_value<float>() == 3.402823466e+38F);
	TEST_OK(pfs::max_value<float>() == pfs::max_value(f));

	double d;
	TEST_OK(pfs::min_value<double>() == 2.22507385850720138309e-308);
	TEST_OK(pfs::min_value<double>() == pfs::min_value(d));
	TEST_OK(pfs::max_value<double>() == 1.79769313486231570815e+308);
	TEST_OK(pfs::max_value<double>() == pfs::max_value(d));

#ifdef PFS_HAVE_LONG_DOUBLE
	ADD_TESTS(4);

	long double dd;
	TEST_OK(pfs::min_value<long double>() == 3.36210314311209350626e-4932L);
	TEST_OK(pfs::min_value<long double>() == pfs::min_value(dd));
	TEST_OK(pfs::max_value<long double>() == 1.18973149535723176502e+4932L);
	TEST_OK(pfs::max_value<long double>() == pfs::max_value(dd));
#endif

	END_TESTS;
}

