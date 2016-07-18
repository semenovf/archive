/**
 * @file   test.cpp
 * @author
 * @date
 *
 * @brief testing ...
 */

#include <iostream>
#include <cstring>

#include <pfs/test/test.hpp>
#include <pfs/byte_string.hpp>
#include <pfs/binary_stream.hpp>

using std::cout;
using std::endl;

struct output_byte_string_device
{
    pfs::byte_string d;
    
    ssize_t write (char const * data, size_t n)
    {
        d.append(data, n);
    }
};

struct input_string_device
{
    pfs::byte_string d;
    pfs::byte_string::const_iterator it;
    pfs::byte_string::const_iterator it_end;
    
    ssize_t read (char * chars, size_t n)
    {
        if (std::distance(it, it_end) >= n) {
            memcpy(chars, it.base(), n);
            std::advance(it, n);
            return ssize_t(n);
        }
        
        return -1;
    }
};

typedef pfs::binary_stream<output_byte_string_device> binary_stream;

void test_pack ()
{
	ADD_TESTS(2);
    
    char const le_sample[] = 
            "\xBC"
            "\xFF\x7F"
            "\x00\x80"
            "\xFF\xFF\xFF\x7F"
            "\x00\x00\x00\x80"
            "\x78\x56\x34\x12"
#if PFS_HAVE_INT64
            "\xFF\xFF\xFF\x7F\x00\x00\x00\x00"
            "\x00\x00\x00\x80\x00\x00\x00\x00"
            "\x78\x56\x34\x12\x00\x00\x00\x00"
            "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x7F"
            "\x00\x00\x00\x00\x00\x00\x00\x80"
            "\xDE\xCD\xBC\xAB\x78\x56\x34\x12"
#endif    
    ;
    
    char const be_sample[] = 
            "\xBC"
            "\x7F\xFF"
            "\x80\x00"
            "\x7F\xFF\xFF\xFF"
            "\x80\x00\x00\x00"
            "\x12\x34\x56\x78"
#ifdef PFS_HAVE_LONGLONG
            "\x00\x00\x00\x00\x7F\xFF\xFF\xFF"
            "\x00\x00\x00\x00\x80\x00\x00\x00"
            "\x00\x00\x00\x00\x12\x34\x56\x78"
            "\x7F\xFF\xFF\xFF\xFF\xFF\xFF\xFF"
            "\x80\x00\x00\x00\x00\x00\x00\x00"
            "\x12\x34\x56\x78\xAB\xBC\xCD\xDE"
#endif
    ;
    
    binary_stream le_bs(output_byte_string_device(), pfs::endian::little_endian);
    binary_stream be_bs(output_byte_string_device(), pfs::endian::big_endian);
    
    le_bs << '\xBC' 
            << int16_t(0x7FFF) 
            << int16_t(0x8000)
            << int32_t(0x7FFFFFFFL) 
            << int32_t(0x80000000L) 
            << int32_t(0x12345678L);

    be_bs << '\xBC' 
            << int16_t(0x7FFF) 
            << int16_t(0x8000)
            << int32_t(0x7FFFFFFFL) 
            << int32_t(0x80000000L) 
            << int32_t(0x12345678L);

#if PFS_HAVE_INT64
	le_bs << int64_t(0x7FFFFFFFLL) 
            << int64_t(0x80000000LL) 
            << int64_t(0x12345678LL) 
            << int64_t(0x7FFFFFFFFFFFFFFFLL) 
            << int64_t(0x8000000000000000LL) 
            << int64_t(0x12345678ABBCCDDELL);

	be_bs << int64_t(0x7FFFFFFFLL)
            << int64_t(0x80000000LL) 
            << int64_t(0x12345678LL)
            << int64_t(0x7FFFFFFFFFFFFFFFLL)
            << int64_t(0x8000000000000000LL)
            << int64_t(0x12345678ABBCCDDELL);
#endif

    cout << to_string(pfs::byte_string(be_sample, sizeof(be_sample) - 1), 16
        , true // zero_padded
        , true // uppercase
        , _u8("\\x")) << endl;

    cout << to_string(be_bs.device().d, 16
        , true // zero_padded
        , true // uppercase
        , _u8("\\x")) << endl;
   
    TEST_OK(pfs::byte_string(le_sample, sizeof(le_sample) - 1) == le_bs.device().d); // - 1 for ignoring string terminator symbol
    TEST_OK(pfs::byte_string(be_sample, sizeof(be_sample) - 1) == be_bs.device().d);
}

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

#if __COMMENT__

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
#endif

int main(int argc, char *argv[])
{
	PFS_UNUSED(argc);
	PFS_UNUSED(argv);

	BEGIN_TESTS(0);

  	test_pack();
//	test_unpack();
//    test_real_pack_unpack();

	return END_TESTS;
}

