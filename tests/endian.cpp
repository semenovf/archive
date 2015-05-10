/*
 * tests/endian.cpp
 *
 *  Created on: May 08, 2015
 *      Author: wladt
 */

#include <pfs/test.hpp>
#include <pfs/endian.hpp>
#include <pfs/typeinfo.hpp>
#include <iostream>
#include <sstream>

using std::cout;
using std::endl;

template <typename T>
void test_swap_integral (T min, T max)
{
	bool ok = true;

	for (T i = min; i < max; ++i) { // i < max: to avoid from infinite loop
		T i0 = pfs::endian::swap(i);
		T i1 = pfs::endian::swap(i0);
		if (i != i1) {
			cout << "Failed with value: " << i << endl;
			ok = false;
			break;
		}
	}

	// And now check for max value
	if (ok) {
		T i0 = pfs::endian::swap(max);
		T i1 = pfs::endian::swap(i0);
		if (max != i1) {
			cout << "Failed with value: " << max << endl;
			ok = false;
		}
	}

	std::stringstream s;
	s << "Check swap of '" << pfs::type_name<T>() << "'";

	TEST_OK2(ok, s.str().c_str());
}

template <typename T>
void test_swap_big_integral (T min, T max)
{
	bool ok = true;

	for (T i = min, j = 1 ; ok && j < PFS_INT16_MAX; i = min / j++) {
		T i0 = pfs::endian::swap(i);
		T i1 = pfs::endian::swap(i0);
		if (i != i1) {
			cout << "min: Failed on step " << j - 1 << " with value: " << i << endl;
			ok = false;
			break;
		}
	}

	for (T i = max, j = 1 ; ok && j < PFS_INT16_MAX; i = max / j++) {
		T i0 = pfs::endian::swap(i);
		T i1 = pfs::endian::swap(i0);
		if (i != i1) {
			cout << "max: Failed on step " << j - 1 << " with value: " << i << endl;
			ok = false;
			break;
		}
	}

	std::stringstream s;
	s << "Check swap of '" << pfs::type_name<T>() << "'";

	TEST_OK2(ok, s.str().c_str());
}


int main (int argc, char * argv[])
{
    PFS_CHECK_SIZEOF_TYPES;
    PFS_UNUSED2(argc, argv);
	BEGIN_TESTS(7);

	test_swap_integral<char>(PFS_CHAR_MIN, PFS_CHAR_MAX);
	test_swap_integral<int8_t>(PFS_INT8_MIN, PFS_INT8_MAX);
	test_swap_integral<uint8_t>(PFS_UINT8_MIN, PFS_UINT8_MAX);
	test_swap_integral<int16_t>(PFS_INT16_MIN, PFS_INT16_MAX);
	test_swap_integral<uint16_t>(PFS_UINT16_MIN, PFS_UINT16_MAX);
	test_swap_big_integral<int32_t>(PFS_INT32_MIN, PFS_INT32_MAX);
	test_swap_big_integral<uint32_t>(PFS_UINT32_MIN, PFS_UINT32_MAX);

#ifdef PFS_HAVE_INT64
	ADD_TESTS(2);
	test_swap_big_integral<int64_t>(PFS_INT64_MIN, PFS_INT64_MAX);
	test_swap_big_integral<uint64_t>(PFS_UINT64_MIN, PFS_UINT64_MAX);
#endif

	END_TESTS;
}
