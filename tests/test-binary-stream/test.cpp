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
        return pfs::integral_cast_check<ssize_t>(n);
    }
};

struct input_byte_string_device
{
    pfs::byte_string d;
    pfs::byte_string::const_iterator it;
    pfs::byte_string::const_iterator it_end;

    input_byte_string_device (input_byte_string_device const & other)
        : d(other.d)
    {
        it = d.cbegin();
        it_end = d.cend();
    }
    
    input_byte_string_device (pfs::byte_string const & data)
        : d(data)
    {
        it = d.cbegin();
        it_end = d.cend();
    }
    
    ssize_t read (char * chars, size_t n)
    {
        if (std::distance(it, it_end) >= n) {
            memcpy(chars, it.base(), n);
            std::advance(it, n);
            return pfs::integral_cast_check<ssize_t>(n);
        }
        
        return -1;
    }
};

typedef pfs::binary_stream<output_byte_string_device> obinary_stream;
typedef pfs::binary_stream<input_byte_string_device> ibinary_stream;

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
#ifdef PFS_HAVE_INT64
            "\x00\x00\x00\x00\x7F\xFF\xFF\xFF"
            "\x00\x00\x00\x00\x80\x00\x00\x00"
            "\x00\x00\x00\x00\x12\x34\x56\x78"
            "\x7F\xFF\xFF\xFF\xFF\xFF\xFF\xFF"
            "\x80\x00\x00\x00\x00\x00\x00\x00"
            "\x12\x34\x56\x78\xAB\xBC\xCD\xDE"
#endif
    ;
    
    obinary_stream le_bs(output_byte_string_device(), pfs::endian::little_endian);
    obinary_stream be_bs(output_byte_string_device(), pfs::endian::big_endian);
    
    le_bs   << '\xBC' 
            << int16_t(0x7FFF) 
            << int16_t(0x8000)
            << int32_t(0x7FFFFFFFL) 
            << int32_t(0x80000000L) 
            << int32_t(0x12345678L)
#if PFS_HAVE_INT64
	        << int64_t(0x7FFFFFFFLL) 
            << int64_t(0x80000000LL) 
            << int64_t(0x12345678LL) 
            << int64_t(0x7FFFFFFFFFFFFFFFLL) 
            << int64_t(0x8000000000000000LL) 
            << int64_t(0x12345678ABBCCDDELL)
#endif
    ;
    
    be_bs   << '\xBC' 
            << int16_t(0x7FFF) 
            << int16_t(0x8000)
            << int32_t(0x7FFFFFFFL) 
            << int32_t(0x80000000L) 
            << int32_t(0x12345678L)
#if PFS_HAVE_INT64
	        << int64_t(0x7FFFFFFFLL)
            << int64_t(0x80000000LL) 
            << int64_t(0x12345678LL)
            << int64_t(0x7FFFFFFFFFFFFFFFLL)
            << int64_t(0x8000000000000000LL)
            << int64_t(0x12345678ABBCCDDELL)
#endif
    ;

//    cout << "===" << to_string(pfs::byte_string(be_sample, sizeof(be_sample) - 1), 16
//        , true // zero_padded
//        , true // uppercase
//        , _u8("\\x")) << endl;
//
//    cout << "===" << to_string(be_bs.device().d, 16
//        , true // zero_padded
//        , true // uppercase
//        , _u8("\\x")) << endl;
   
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

    ibinary_stream le_bs(input_byte_string_device(pfs::byte_string(str_le, sizeof(str_le))), pfs::endian::little_endian);
    ibinary_stream be_bs(input_byte_string_device(pfs::byte_string(str_be, sizeof(str_be))), pfs::endian::big_endian);

    char a1;
    short a2, a3;
    unsigned short int a4, a5; 
    int a6, a7, a8;

    char b1;
    short b2, b3;
    unsigned short int b4, b5; 
    int b6, b7, b8;
    
    le_bs >> a1 >> a2 >> a3 >> a4 >> a5 >> a6 >> a7 >> a8;
    be_bs >> b1 >> b2 >> b3 >> b4 >> b5 >> b6 >> b7 >> b8;

	TEST_OK(a1 == '\xBC');
    TEST_OK(a2 == 0x7FFF);
    TEST_OK(a3 == static_cast<short int>(0x8000));
    TEST_OK(a4 == 0x7FFF);
    TEST_OK(a5 == 0x8000);
    TEST_OK(a6 == 0x7FFFFFFF);
    TEST_OK(a7 == static_cast<int>(0x80000000));
    TEST_OK(a8 == 0x12345678);
    
	TEST_OK(b1 == '\xBC');
	TEST_OK(b2 == 0x7FFF);
	TEST_OK(b3 == static_cast<short int>(0x8000));
	TEST_OK(b4 == 0x7FFF);
	TEST_OK(b5 == 0x8000);
	TEST_OK(b6 == 0x7FFFFFFF);
	TEST_OK(b7 == static_cast<int>(0x80000000));
	TEST_OK(b8 == 0x12345678);

	//TEST_OK(it == bytes.cend());
	// TODO Add remaining tests (see test_convert_to_bytes)
}

void test_real_pack_unpack ()
{
    {    
        double double_nums[8] = {
                  -3490.6677f
                , 0.0f
                , 1.0f
                , -1.0f
                , DBL_MIN * 2
                , DBL_MAX / 2
                , DBL_MIN
                , DBL_MAX 
        };

        int ntests = sizeof(double_nums)/sizeof(double_nums[0]);
        ADD_TESTS(ntests * 2);

        obinary_stream ole_bs(output_byte_string_device(), pfs::endian::little_endian);
        obinary_stream obe_bs(output_byte_string_device(), pfs::endian::big_endian);

        for (int i = 0; i < ntests; ++i) {
            ole_bs << double_nums[i];
            obe_bs << double_nums[i];
        }

        ibinary_stream ile_bs(input_byte_string_device(ole_bs.device().d), pfs::endian::little_endian);
        ibinary_stream ibe_bs(input_byte_string_device(obe_bs.device().d), pfs::endian::big_endian);

        for (int i = 0; i < ntests; ++i) {
            double a, b;

            ile_bs >> a;
            ibe_bs >> b;

            TEST_OK(a == double_nums[i]);
            TEST_OK(b == double_nums[i]);
        }
    }
    
    {
        float float_nums[] = { 
                  0.0
                , 1.0
                , -1.0
                , 10
                , -3.6677
                , 3.1875
                , -3.1875
                , FLT_MIN * 2
                , FLT_MAX / 2
                , FLT_MIN
                , FLT_MAX 
        };
        
        int ntests = sizeof(float_nums)/sizeof(float_nums[0]);
        ADD_TESTS(ntests * 2);

        obinary_stream ole_bs(output_byte_string_device(), pfs::endian::little_endian);
        obinary_stream obe_bs(output_byte_string_device(), pfs::endian::big_endian);

        for (int i = 0; i < ntests; ++i) {
            ole_bs << float_nums[i];
            obe_bs << float_nums[i];
        }

        ibinary_stream ile_bs(input_byte_string_device(ole_bs.device().d), pfs::endian::little_endian);
        ibinary_stream ibe_bs(input_byte_string_device(obe_bs.device().d), pfs::endian::big_endian);

        for (int i = 0; i < ntests; ++i) {
            float a, b;

            ile_bs >> a;
            ibe_bs >> b;

            TEST_OK(a == float_nums[i]);
            TEST_OK(b == float_nums[i]);
        }
    }
}


void test_byte_string ()
{
    ADD_TESTS(2);
    
    pfs::byte_string sample("1234567890", 10);
    
    obinary_stream ole_bs(output_byte_string_device(), pfs::endian::little_endian);
    obinary_stream obe_bs(output_byte_string_device(), pfs::endian::big_endian);
    
    ole_bs << sample;
    obe_bs << sample;
    
    ibinary_stream ile_bs(input_byte_string_device(ole_bs.device().d), pfs::endian::little_endian);
    ibinary_stream ibe_bs(input_byte_string_device(obe_bs.device().d), pfs::endian::big_endian);
    
    pfs::byte_string a;
    pfs::byte_string b;

    ile_bs >> a;
    ibe_bs >> b;

//    cout << "===" << to_string(sample, 16
//        , true // zero_padded
//        , true // uppercase
//        , _u8("\\x")) << endl;
//
//    cout << "===" << to_string(a, 16
//        , true // zero_padded
//        , true // uppercase
//        , _u8("\\x")) << endl;

    
    TEST_OK(a == sample);
    TEST_OK(b == sample);
}

int main(int argc, char *argv[])
{
	PFS_UNUSED(argc);
	PFS_UNUSED(argv);

	BEGIN_TESTS(0);

  	test_pack();
	test_unpack();
    test_real_pack_unpack();
    test_byte_string();

	return END_TESTS;
}
