/*
 * tests/endian.cpp
 *
 *  Created on: May 08, 2015
 *      Author: wladt
 */

#include <pfs/test/test.hpp>
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
		T i0 = pfs::endian::bswap(i);
		T i1 = pfs::endian::bswap(i0);
		if (i != i1) {
			cout << "Failed with value: " << i << endl;
			ok = false;
			break;
		}
	}

	// And now check for max value
	if (ok) {
		T i0 = pfs::endian::bswap(max);
		T i1 = pfs::endian::bswap(i0);
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

	for (T i = min, j = 1 ; ok && j < pfs::max_value<int16_t>(); i = min / j++) {
		T i0 = pfs::endian::bswap(i);
		T i1 = pfs::endian::bswap(i0);
		if (i != i1) {
			cout << "min: Failed on step " << j - 1 << " with value: " << i << endl;
			ok = false;
			break;
		}
	}

	for (T i = max, j = 1 ; ok && j < pfs::max_value<int16_t>(); i = max / j++) {
		T i0 = pfs::endian::bswap(i);
		T i1 = pfs::endian::bswap(i0);
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
    PFS_UNUSED2(argc, argv);
	BEGIN_TESTS(7);

	test_swap_integral<char>(pfs::min_value<char>(), pfs::max_value<char>());
	test_swap_integral<int8_t>(pfs::min_value<int8_t>(), pfs::max_value<int8_t>());
	test_swap_integral<uint8_t>(pfs::min_value<uint8_t>(), pfs::max_value<uint8_t>());
	test_swap_integral<int16_t>(pfs::min_value<int16_t>(), pfs::max_value<int16_t>());
	test_swap_integral<uint16_t>(pfs::min_value<uint16_t>(), pfs::max_value<uint16_t>());
	test_swap_big_integral<int32_t>(pfs::min_value<int32_t>(), pfs::max_value<int32_t>());
	test_swap_big_integral<uint32_t>(pfs::min_value<uint32_t>(), pfs::max_value<uint32_t>());

#ifdef PFS_HAVE_INT64
	ADD_TESTS(2);
	test_swap_big_integral<int64_t>(pfs::min_value<int64_t>(), pfs::max_value<int64_t>());
	test_swap_big_integral<uint64_t>(pfs::min_value<uint64_t>(), pfs::max_value<uint64_t>());
#endif

	return END_TESTS;
}
