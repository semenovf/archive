/**
 * @file   test.cpp
 * @author
 * @date
 *
 * @brief testing ...
 */

#include <pfs/test/test.hpp>
#include <pfs/byte_string.hpp>
#include <pfs/binary_stream.hpp>
#include <iostream>

using std::cout;
using std::endl;

// TODO OBSOLETE, see binary_stream
void test_pack ()
{
	using pfs::endian;

	ADD_TESTS(12);
	TEST_OK(pfs::byte_string("\xBC", 1) == pfs::pack('\xBC', endian::little_endian));
	TEST_OK(pfs::byte_string("\xBC", 1) == pfs::pack('\xBC', endian::big_endian));
	TEST_OK(pfs::byte_string("\xFF\x7F", 2) == pfs::pack(short(0x7FFF), endian::little_endian));
	TEST_OK(pfs::byte_string("\x7F\xFF", 2) == pfs::pack(short(0x7FFF), endian::big_endian));
	TEST_OK(pfs::byte_string("\x00\x80", 2) == pfs::pack(short(0x8000), endian::little_endian));
	TEST_OK(pfs::byte_string("\x80\x00", 2) == pfs::pack(short(0x8000), endian::big_endian));

	TEST_OK(pfs::byte_string("\xFF\xFF\xFF\x7F", 4) == pfs::pack(int(0x7FFFFFFF), endian::little_endian));
	TEST_OK(pfs::byte_string("\x7F\xFF\xFF\xFF", 4) == pfs::pack(int(0x7FFFFFFF), endian::big_endian));
	TEST_OK(pfs::byte_string("\x00\x00\x00\x80", 4) == pfs::pack(int(0x80000000), endian::little_endian));
	TEST_OK(pfs::byte_string("\x80\x00\x00\x00", 4) == pfs::pack(int(0x80000000), endian::big_endian));
	TEST_OK(pfs::byte_string("\x78\x56\x34\x12", 4) == pfs::pack(int(0x12345678), endian::little_endian));
	TEST_OK(pfs::byte_string("\x12\x34\x56\x78", 4) == pfs::pack(int(0x12345678), endian::big_endian));

if (sizeof(intmax_t) == 4) {
	ADD_TESTS(6);
	TEST_OK(pfs::byte_string("\xFF\xFF\xFF\x7F", 4) == pfs::pack(intmax_t(0x7FFFFFFFL), pfs::endian::little_endian));
	TEST_OK(pfs::byte_string("\x7F\xFF\xFF\xFF", 4) == pfs::pack(intmax_t(0x7FFFFFFFL), pfs::endian::big_endian));
	TEST_OK(pfs::byte_string("\x00\x00\x00\x80", 4) == pfs::pack(intmax_t(0x80000000L), pfs::endian::little_endian));
	TEST_OK(pfs::byte_string("\x80\x00\x00\x00", 4) == pfs::pack(intmax_t(0x80000000L), pfs::endian::big_endian));
	TEST_OK(pfs::byte_string("\x78\x56\x34\x12", 4) == pfs::pack(intmax_t(0x12345678L), pfs::endian::little_endian));
	TEST_OK(pfs::byte_string("\x12\x34\x56\x78", 4) == pfs::pack(intmax_t(0x12345678L), pfs::endian::big_endian));
} else { // sizeof(long) == 8 - on x64
	ADD_TESTS(6);
	TEST_OK(pfs::byte_string("\xFF\xFF\xFF\x7F\x00\x00\x00\x00", 8) == pfs::pack(intmax_t(0x7FFFFFFFL), pfs::endian::little_endian));
	TEST_OK(pfs::byte_string("\x00\x00\x00\x00\x7F\xFF\xFF\xFF", 8) == pfs::pack(intmax_t(0x7FFFFFFFL), pfs::endian::big_endian));
	TEST_OK(pfs::byte_string("\x00\x00\x00\x80\x00\x00\x00\x00", 8) == pfs::pack(intmax_t(0x80000000L), pfs::endian::little_endian));
	TEST_OK(pfs::byte_string("\x00\x00\x00\x00\x80\x00\x00\x00", 8) == pfs::pack(intmax_t(0x80000000L), pfs::endian::big_endian));
	TEST_OK(pfs::byte_string("\x78\x56\x34\x12\x00\x00\x00\x00", 8) == pfs::pack(intmax_t(0x12345678L), pfs::endian::little_endian));
	TEST_OK(pfs::byte_string("\x00\x00\x00\x00\x12\x34\x56\x78", 8) == pfs::pack(intmax_t(0x12345678L), pfs::endian::big_endian));
}

#ifdef PFS_HAVE_LONGLONG
	ADD_TESTS(12);
	TEST_OK(pfs::byte_string("\xFF\xFF\xFF\x7F\x00\x00\x00\x00", 8) == pfs::pack(0x7FFFFFFFLL, pfs::endian::little_endian));
	TEST_OK(pfs::byte_string("\x00\x00\x00\x00\x7F\xFF\xFF\xFF", 8) == pfs::pack(0x7FFFFFFFLL, pfs::endian::big_endian));
	TEST_OK(pfs::byte_string("\x00\x00\x00\x80\x00\x00\x00\x00", 8) == pfs::pack(0x80000000LL, pfs::endian::little_endian));
	TEST_OK(pfs::byte_string("\x00\x00\x00\x00\x80\x00\x00\x00", 8) == pfs::pack(0x80000000LL, pfs::endian::big_endian));
	TEST_OK(pfs::byte_string("\x78\x56\x34\x12\x00\x00\x00\x00", 8) == pfs::pack(0x12345678LL, pfs::endian::little_endian));
	TEST_OK(pfs::byte_string("\x00\x00\x00\x00\x12\x34\x56\x78", 8) == pfs::pack(0x12345678LL, pfs::endian::big_endian));
	TEST_OK(pfs::byte_string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x7F", 8) == pfs::pack(0x7FFFFFFFFFFFFFFFLL, pfs::endian::little_endian));
	TEST_OK(pfs::byte_string("\x7F\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8) == pfs::pack(0x7FFFFFFFFFFFFFFFLL, pfs::endian::big_endian));
	TEST_OK(pfs::byte_string("\x00\x00\x00\x00\x00\x00\x00\x80", 8) == pfs::pack(0x8000000000000000LL, pfs::endian::little_endian));
	TEST_OK(pfs::byte_string("\x80\x00\x00\x00\x00\x00\x00\x00", 8) == pfs::pack(0x8000000000000000LL, pfs::endian::big_endian));
	TEST_OK(pfs::byte_string("\xDE\xCD\xBC\xAB\x78\x56\x34\x12", 8) == pfs::pack(0x12345678ABBCCDDELL, pfs::endian::little_endian));
	TEST_OK(pfs::byte_string("\x12\x34\x56\x78\xAB\xBC\xCD\xDE", 8) == pfs::pack(0x12345678ABBCCDDELL, pfs::endian::big_endian));
#endif
}

// TODO OBSOLETE, see binary_stream
void test_unpack ()
{
	ADD_TESTS(16);

	const char str_le [] = {'\xBC'
		, '\xFF', '\x7F'
        , '\x00', '\x80'
		, '\xFF', '\x7F'
        , '\x00', '\x80'
		, '\xFF', '\xFF', '\xFF', '\x7F'
		, '\x00', '\x00', '\x00', '\x80'
		, '\x78', '\x56', '\x34', '\x12'
	};

    	const char str_be [] = {'\xBC'
        , '\x7F', '\xFF'
        , '\x80', '\x00'
        , '\x7F', '\xFF'
        , '\x80', '\x00'
        , '\x7F', '\xFF', '\xFF', '\xFF'
        , '\x80', '\x00', '\x00', '\x00'
        , '\x12', '\x34', '\x56', '\x78'
	};

	byte_string bytes_le(str_le, sizeof(str_le)/sizeof(str_le[0]));
    byte_string bytes_be(str_be, sizeof(str_be)/sizeof(str_be[0]));

    pfs::unpack_context ctx_le(bytes_le.begin(), bytes_le.end(), pfs::endian::little_endian);
    pfs::unpack_context ctx_be(bytes_be.begin(), bytes_be.end(), pfs::endian::big_endian);

	TEST_OK(pfs::unpack<char>(ctx_le) == '\xBC');
    TEST_OK(pfs::unpack<short int>(ctx_le) == 0x7FFF);
    TEST_OK(pfs::unpack<short int>(ctx_le) == static_cast<short int>(0x8000));
    TEST_OK(pfs::unpack<unsigned short int>(ctx_le) == 0x7FFF);
    TEST_OK(pfs::unpack<unsigned short int>(ctx_le) == 0x8000);
    TEST_OK(pfs::unpack<int>(ctx_le) == 0x7FFFFFFF);
    TEST_OK(pfs::unpack<int>(ctx_le) == static_cast<int>(0x80000000));
    TEST_OK(pfs::unpack<int>(ctx_le) == 0x12345678);
    
	TEST_OK(pfs::unpack<char>(ctx_be)      == '\xBC');
	TEST_OK(pfs::unpack<short int>(ctx_be) == 0x7FFF);
	TEST_OK(pfs::unpack<short int>(ctx_be) == static_cast<short int>(0x8000));
	TEST_OK(pfs::unpack<unsigned short int>(ctx_be) == 0x7FFF);
	TEST_OK(pfs::unpack<unsigned short int>(ctx_be) == 0x8000);
	TEST_OK(pfs::unpack<int>(ctx_be) == 0x7FFFFFFF);
	TEST_OK(pfs::unpack<int>(ctx_be) == static_cast<int>(0x80000000));
	TEST_OK(pfs::unpack<int>(ctx_be) == 0x12345678);

	//TEST_OK(it == bytes.cend());
	// TODO Add remaining tests (see test_convert_to_bytes)
}

// TODO OBSOLETE, see binary_stream
void test_real_pack_unpack ()
{
    ADD_TESTS(8 * 4);
    
    double double_nums[8] = {-3490.6677f
            , 0.0f
            , 1.0f
            , -1.0f
            , DBL_MIN * 2
            , DBL_MAX / 2
            , DBL_MIN
            , DBL_MAX 
    };
    
    for (int i = 0; i < 8; ++i) {
        pfs::pack_context packed_le(pfs::endian::little_endian);
        pfs::pack_context packed_be(pfs::endian::big_endian);
        
        pfs::pack(packed_le, double_nums[i]);
        pfs::pack(packed_be, double_nums[i]);

        double d1;
        double d2;
        pfs::unpack_context ctx_le(packed_le.buffer.begin(), packed_le.buffer.end(), pfs::endian::little_endian);
        pfs::unpack_context ctx_be(packed_be.buffer.begin(), packed_be.buffer.end(), pfs::endian::big_endian);
        
        TEST_OK(pfs::unpack(ctx_le, d1));
        TEST_OK(pfs::unpack(ctx_be, d2));
        
        TEST_OK(d1 == double_nums[i]);
        TEST_OK(d2 == double_nums[i]);
    }
    
    ADD_TESTS(7 * 4);
    
    float float_nums[7] = { 0.0, 1.0, -1.0, 10, -3.6677, 3.1875, -3.1875 };
    
    for (int i = 0; i < 7; ++i) {
        pfs::pack_context packed_le(pfs::endian::little_endian);
        pfs::pack_context packed_be(pfs::endian::big_endian);
        
        pfs::pack(packed_le, float_nums[i]);
        pfs::pack(packed_be, float_nums[i]);

        float f1;
        float f2;
        pfs::unpack_context ctx_le(packed_le.buffer.begin(), packed_le.buffer.end(), pfs::endian::little_endian);
        pfs::unpack_context ctx_be(packed_be.buffer.begin(), packed_be.buffer.end(), pfs::endian::big_endian);
        
        TEST_OK(pfs::unpack(ctx_le, f1));
        TEST_OK(pfs::unpack(ctx_be, f2));
        
        TEST_OK(f1 == float_nums[i]);
        TEST_OK(f2 == float_nums[i]);
    }
}


int main(int argc, char *argv[])
{
	PFS_UNUSED(argc);
	PFS_UNUSED(argv);

	BEGIN_TESTS(0);

  	test_pack();
	test_unpack();
    test_real_pack_unpack();

	return END_TESTS;
}

