/*
 * tests/real_cast.cpp
 *
 *  Created on: May 08, 2015
 *      Author: wladt
 */

#include <pfs/test.hpp>
#include <pfs/typeinfo.hpp>
#include <iostream>

using std::cout;
using std::endl;

template <typename Fp, typename T>
void test_compare_fp_cast (T min, T max)
{
	cout << "\ntest_compare_fp_cast<"
			<< pfs::type_name<Fp>()
			<< ", "
			<< pfs::type_name<T>()
			<< ">()\n{\n";

	real_t rmin = static_cast<Fp>(pfs::min_type<T>());
	real_t rmax = static_cast<Fp>(pfs::max_type<T>());

	TEST_OK(pfs::min_type<T>() == min);
	TEST_OK(pfs::max_type<T>() == max);

	TEST_OK(static_cast<Fp>(min) == rmin);
	TEST_OK(static_cast<Fp>(max) == rmax);

	TEST_OK(rmin == static_cast<Fp>(pfs::min_type<T>()));
	TEST_OK(rmax == static_cast<Fp>(pfs::max_type<T>()));
	TEST_OK(static_cast<Fp>(pfs::min_type<T>()) == rmin);
	TEST_OK(static_cast<Fp>(pfs::max_type<T>()) == rmax);

	TEST_OK(static_cast<Fp>(pfs::min_type<T>()) == static_cast<Fp>(pfs::min_type<T>()));
	TEST_OK(static_cast<Fp>(pfs::max_type<T>()) == static_cast<Fp>(pfs::max_type<T>()));
	TEST_OK(static_cast<Fp>(min) == static_cast<Fp>(min));
	TEST_OK(static_cast<Fp>(max) == static_cast<Fp>(max));

	cout << "} // test_compare_fp_cast<"
			<< pfs::type_name<Fp>()
			<< ", "
			<< pfs::type_name<T>()
			<< ">\n";
}

template <typename Fp>
void test_compare_fp_cast ()
{
	test_compare_fp_cast<Fp, char>(PFS_CHAR_MIN, PFS_CHAR_MAX);
	test_compare_fp_cast<Fp, signed char>(PFS_SCHAR_MIN, PFS_SCHAR_MAX);
	test_compare_fp_cast<Fp, unsigned char>(0, PFS_UCHAR_MAX);
	test_compare_fp_cast<Fp, short>(PFS_SHORT_MIN, PFS_SHORT_MAX);
	test_compare_fp_cast<Fp, unsigned short>(0, PFS_USHORT_MAX);
	test_compare_fp_cast<Fp, int>(PFS_INT_MIN, PFS_INT_MAX);
	test_compare_fp_cast<Fp, unsigned int>(0, PFS_UINT_MAX);
	test_compare_fp_cast<Fp, long>(PFS_LONG_MIN, PFS_LONG_MAX);
	test_compare_fp_cast<Fp, unsigned long>(0, PFS_ULONG_MAX);

#ifdef PFS_HAVE_LONGLONG
	ADD_TESTS(24);
	// FIXME Below expression executed with failure under MSVC while comparing with
	// casting max value of long long to any of floating type (float, double, long double)
	test_compare_fp_cast<Fp, long long>(PFS_LLONG_MIN, PFS_LLONG_MAX);

	test_compare_fp_cast<Fp, unsigned long long>(0, PFS_ULLONG_MAX);
#endif

	test_compare_fp_cast<Fp, float>(PFS_FLOAT_MIN, PFS_FLOAT_MAX);
	test_compare_fp_cast<Fp, double>(PFS_DOUBLE_MIN, PFS_DOUBLE_MAX);

#ifdef PFS_HAVE_LONG_DOUBLE
	ADD_TESTS(12);
	test_compare_fp_cast<Fp, long double>(PFS_LONG_DOUBLE_MIN, PFS_LONG_DOUBLE_MAX);
#endif
}


int main (int argc, char * argv[])
{
    PFS_CHECK_SIZEOF_TYPES;
    PFS_UNUSED2(argc, argv);
	BEGIN_TESTS(264);

	test_compare_fp_cast<float>();
	test_compare_fp_cast<double>();

#ifdef PFS_HAVE_LONG_DOUBLE
	ADD_TESTS(132);
	test_compare_fp_cast<real_t>();
#endif

	END_TESTS;
}
