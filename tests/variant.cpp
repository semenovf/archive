#include <pfs/test/test.hpp>
#include <pfs/variant.hpp>
#include <pfs/string.hpp>
#include <pfs/byte_string.hpp>
#include <pfs/vector.hpp>
#include <iostream>

using std::cout;
using std::endl;

int main (int argc, char *argv[])
{
	PFS_UNUSED2(argc, argv);

	BEGIN_TESTS(24);

	typedef pfs::variant<real_t, integral_t, pfs::byte_string, pfs::utf8_string, pfs::ucchar> variant_t;

	variant_t v1(int(123));
	variant_t v2(3.14159f);
	variant_t v3('c');
	variant_t v4(pfs::utf8_string("STRING"));
	variant_t v5(pfs::byte_string("BYTE_STRING"));
	variant_t v6(v5);

	TEST_OK(v1.is<int>());
	TEST_OK(v2.is<float>());
	TEST_OK(v3.is<char>());
	TEST_OK(v4.is<pfs::utf8_string>());
	TEST_OK(v5.is<pfs::byte_string>());
	TEST_OK(v6.is<pfs::byte_string>());

	TEST_OK(v1.get<int>() == 123);
	TEST_OK(v2.get<float>() == 3.14159f);
	TEST_OK(v3.get<char>() == 'c');
	TEST_OK(strcmp(v4.get<pfs::utf8_string>().c_str(), "STRING") == 0);
	TEST_OK(strcmp(v5.get<pfs::byte_string>().c_str(), "BYTE_STRING") == 0);
	TEST_OK(strcmp(v6.get<pfs::byte_string>().c_str(), "BYTE_STRING") == 0);

	variant_t v7;

	v7 = v1;
	TEST_FAIL(v7.is<int>());
	TEST_OK(v7.get<int>() == 123);

	v7 = v2;
	TEST_FAIL(v7.is<float>());
	TEST_OK(v7.get<float>() == 3.14159f);

	v7 = v3;
	TEST_FAIL(v7.is<char>());
	TEST_OK(v7.get<char>() == 'c');

	v7 = v4;
	TEST_FAIL(v7.is<pfs::utf8_string>());
	TEST_OK(strcmp(v7.get<pfs::utf8_string>().c_str(), "STRING") == 0);

	v7 = v5;
	TEST_FAIL(v7.is<pfs::byte_string>());
	TEST_OK(strcmp(v7.get<pfs::byte_string>().c_str(), "BYTE_STRING") == 0);

	v7 = v6;
	TEST_FAIL(v7.is<pfs::byte_string>());
	TEST_OK(strcmp(v7.get<pfs::byte_string>().c_str(), "BYTE_STRING") == 0);


	// Try vector of variants
	// MSVC at compile time output
	// error C2719: '_Val': formal parameter with __declspec(align('8')) won't be aligned
	// FIXME
//	pfs::vector<variant_t> vec;
//	vec << v1 << v2 << v3 << v4 << v5 << v6 << v7;

	ADD_TESTS(6);
	TEST_OK(sizeof(pfs::aligned_storage<10, 1>::type) == 10);
	TEST_OK(sizeof(pfs::aligned_storage<10, 2>::type) == 10);
	TEST_OK(sizeof(pfs::aligned_storage<9, 2>::type) == 10);
	TEST_OK(sizeof(pfs::aligned_storage<8, 2>::type) == 8);
	TEST_OK(sizeof(pfs::aligned_storage<10, 128>::type) == 128);
	TEST_OK(sizeof(pfs::aligned_storage<20, 8>::type) == 24);
	END_TESTS;
}
