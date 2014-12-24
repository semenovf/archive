/*
 * byte_string.cpp
 *
 *  Created on: May 16, 2013
 *      Author: wladt
 */

#include <cwt/test.hpp>
#include <cstring>
//#include <iostream>
//#include "pfs/endian.hpp"
#include "pfs/byte_string.hpp"

void test_constructors ()
{
	pfs::byte_string nil;
	pfs::byte_string serial(10, 'W');

	TEST_OK(nil.isNull());
	TEST_OK(nil.isEmpty());
	TEST_OK(serial.size() == 10);
	TEST_OK(strcmp("WWWWWWWWWW", serial.c_str()) == 0);

	const char * loremipsum  = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit";

	TEST_OK(strcmp(loremipsum, pfs::byte_string(loremipsum).c_str()) == 0);
	TEST_OK(strcmp(loremipsum, pfs::byte_string(loremipsum, strlen(loremipsum)).c_str()) == 0);
//	TEST_OK(strcmp(loremipsum, pfs::byte_string(std::string(loremipsum)).c_str()) == 0);
}


void test_convert_to_bytes ()
{
	TEST_OK(pfs::byte_string("\xBC", 1) == pfs::byte_string::toBytes('\xBC', pfs::endian::LittleEndian));
	TEST_OK(pfs::byte_string("\xBC", 1) == pfs::byte_string::toBytes('\xBC', pfs::endian::BigEndian));
	TEST_OK(pfs::byte_string("\xFF\x7F", 2) == pfs::byte_string::toBytes(short(0x7FFF), pfs::endian::LittleEndian));
	TEST_OK(pfs::byte_string("\x7F\xFF", 2) == pfs::byte_string::toBytes(short(0x7FFF), pfs::endian::BigEndian));
	TEST_OK(pfs::byte_string("\x00\x80", 2) == pfs::byte_string::toBytes(short(0x8000), pfs::endian::LittleEndian));
	TEST_OK(pfs::byte_string("\x80\x00", 2) == pfs::byte_string::toBytes(short(0x8000), pfs::endian::BigEndian));

	TEST_OK(pfs::byte_string("\xFF\xFF\xFF\x7F", 4) == pfs::byte_string::toBytes(int(0x7FFFFFFF), pfs::endian::LittleEndian));
	TEST_OK(pfs::byte_string("\x7F\xFF\xFF\xFF", 4) == pfs::byte_string::toBytes(int(0x7FFFFFFF), pfs::endian::BigEndian));
	TEST_OK(pfs::byte_string("\x00\x00\x00\x80", 4) == pfs::byte_string::toBytes(int(0x80000000), pfs::endian::LittleEndian));
	TEST_OK(pfs::byte_string("\x80\x00\x00\x00", 4) == pfs::byte_string::toBytes(int(0x80000000), pfs::endian::BigEndian));
	TEST_OK(pfs::byte_string("\x78\x56\x34\x12", 4) == pfs::byte_string::toBytes(int(0x12345678), pfs::endian::LittleEndian));
	TEST_OK(pfs::byte_string("\x12\x34\x56\x78", 4) == pfs::byte_string::toBytes(int(0x12345678), pfs::endian::BigEndian));

if (sizeof(long_t) == 4) {
	TEST_OK(pfs::byte_string("\xFF\xFF\xFF\x7F", 4) == pfs::byte_string::toBytes(long_t(0x7FFFFFFFL), pfs::endian::LittleEndian));
	TEST_OK(pfs::byte_string("\x7F\xFF\xFF\xFF", 4) == pfs::byte_string::toBytes(long_t(0x7FFFFFFFL), pfs::endian::BigEndian));
	TEST_OK(pfs::byte_string("\x00\x00\x00\x80", 4) == pfs::byte_string::toBytes(long_t(0x80000000L), pfs::endian::LittleEndian));
	TEST_OK(pfs::byte_string("\x80\x00\x00\x00", 4) == pfs::byte_string::toBytes(long_t(0x80000000L), pfs::endian::BigEndian));
	TEST_OK(pfs::byte_string("\x78\x56\x34\x12", 4) == pfs::byte_string::toBytes(long_t(0x12345678L), pfs::endian::LittleEndian));
	TEST_OK(pfs::byte_string("\x12\x34\x56\x78", 4) == pfs::byte_string::toBytes(long_t(0x12345678L), pfs::endian::BigEndian));
} else { // sizeof(long) == 8 - on x64
	TEST_OK(pfs::byte_string("\xFF\xFF\xFF\x7F\x00\x00\x00\x00", 8) == pfs::byte_string::toBytes(long_t(0x7FFFFFFFL), pfs::endian::LittleEndian));
	TEST_OK(pfs::byte_string("\x00\x00\x00\x00\x7F\xFF\xFF\xFF", 8) == pfs::byte_string::toBytes(long_t(0x7FFFFFFFL), pfs::endian::BigEndian));
	TEST_OK(pfs::byte_string("\x00\x00\x00\x80\x00\x00\x00\x00", 8) == pfs::byte_string::toBytes(long_t(0x80000000L), pfs::endian::LittleEndian));
	TEST_OK(pfs::byte_string("\x00\x00\x00\x00\x80\x00\x00\x00", 8) == pfs::byte_string::toBytes(long_t(0x80000000L), pfs::endian::BigEndian));
	TEST_OK(pfs::byte_string("\x78\x56\x34\x12\x00\x00\x00\x00", 8) == pfs::byte_string::toBytes(long_t(0x12345678L), pfs::endian::LittleEndian));
	TEST_OK(pfs::byte_string("\x00\x00\x00\x00\x12\x34\x56\x78", 8) == pfs::byte_string::toBytes(long_t(0x12345678L), pfs::endian::BigEndian));
}

#ifdef HAVE_LONGLONG
	TEST_OK(pfs::byte_string("\xFF\xFF\xFF\x7F\x00\x00\x00\x00", 8) == pfs::byte_string::toBytes(0x7FFFFFFFLL, pfs::endian::LittleEndian));
	TEST_OK(pfs::byte_string("\x00\x00\x00\x00\x7F\xFF\xFF\xFF", 8) == pfs::byte_string::toBytes(0x7FFFFFFFLL, pfs::endian::BigEndian));
	TEST_OK(pfs::byte_string("\x00\x00\x00\x80\x00\x00\x00\x00", 8) == pfs::byte_string::toBytes(0x80000000LL, pfs::endian::LittleEndian));
	TEST_OK(pfs::byte_string("\x00\x00\x00\x00\x80\x00\x00\x00", 8) == pfs::byte_string::toBytes(0x80000000LL, pfs::endian::BigEndian));
	TEST_OK(pfs::byte_string("\x78\x56\x34\x12\x00\x00\x00\x00", 8) == pfs::byte_string::toBytes(0x12345678LL, pfs::endian::LittleEndian));
	TEST_OK(pfs::byte_string("\x00\x00\x00\x00\x12\x34\x56\x78", 8) == pfs::byte_string::toBytes(0x12345678LL, pfs::endian::BigEndian));
	TEST_OK(pfs::byte_string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x7F", 8) == pfs::byte_string::toBytes(0x7FFFFFFFFFFFFFFFLL, pfs::endian::LittleEndian));
	TEST_OK(pfs::byte_string("\x7F\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8) == pfs::byte_string::toBytes(0x7FFFFFFFFFFFFFFFLL, pfs::endian::BigEndian));
	TEST_OK(pfs::byte_string("\x00\x00\x00\x00\x00\x00\x00\x80", 8) == pfs::byte_string::toBytes(0x8000000000000000LL, pfs::endian::LittleEndian));
	TEST_OK(pfs::byte_string("\x80\x00\x00\x00\x00\x00\x00\x00", 8) == pfs::byte_string::toBytes(0x8000000000000000LL, pfs::endian::BigEndian));
	TEST_OK(pfs::byte_string("\xDE\xCD\xBC\xAB\x78\x56\x34\x12", 8) == pfs::byte_string::toBytes(0x12345678ABBCCDDELL, pfs::endian::LittleEndian));
	TEST_OK(pfs::byte_string("\x12\x34\x56\x78\xAB\xBC\xCD\xDE", 8) == pfs::byte_string::toBytes(0x12345678ABBCCDDELL, pfs::endian::BigEndian));
#endif
}

#ifdef __COMMENT__

void test_append ()
{
	pfs::byte_string ba("One");
	ba.append("Two");

	TEST_OK(ba.length() == 6);
	TEST_OK(strlen(ba.constData()) == ba.size());
	TEST_OK(ba == "OneTwo");
}

void test_read_number ()
{
	char  ch;
	short short_int;
	unsigned short ushort_int;
	int int_int;

	TEST_OK(pfs::byte_string("\xBC", 1).readNumber(ch, 0, pfs::endian::LittleEndian) == sizeof(ch) && ch == '\xBC');
	TEST_OK(pfs::byte_string("\xBC", 1).readNumber(ch, 0, pfs::endian::BigEndian) == sizeof(ch) && ch == '\xBC');

	TEST_OK(pfs::byte_string("\xFF\x7F", 2).readNumber(short_int, 0, pfs::endian::LittleEndian)  == sizeof(short_int) && short_int == (short)0x7FFF);
	TEST_OK(pfs::byte_string("\x7F\xFF", 2).readNumber(short_int, 0, pfs::endian::BigEndian)     == sizeof(short_int) && short_int == (short)0x7FFF);
	TEST_OK(pfs::byte_string("\x00\x80", 2).readNumber(short_int, 0, pfs::endian::LittleEndian)  == sizeof(short_int) && short_int == (short)0x8000);
	TEST_OK(pfs::byte_string("\x80\x00", 2).readNumber(short_int, 0, pfs::endian::BigEndian)     == sizeof(short_int) && short_int == (short)0x8000);

	TEST_OK(pfs::byte_string("\xFF\x7F", 2).readNumber(ushort_int, 0, pfs::endian::LittleEndian) == sizeof(ushort_int) && ushort_int == 0x7FFF);
	TEST_OK(pfs::byte_string("\x7F\xFF", 2).readNumber(ushort_int, 0, pfs::endian::BigEndian)    == sizeof(ushort_int) && ushort_int == 0x7FFF);
	TEST_OK(pfs::byte_string("\x00\x80", 2).readNumber(ushort_int, 0, pfs::endian::LittleEndian) == sizeof(ushort_int) && ushort_int == 0x8000);
	TEST_OK(pfs::byte_string("\x80\x00", 2).readNumber(ushort_int, 0, pfs::endian::BigEndian)    == sizeof(ushort_int) && ushort_int == 0x8000);

	TEST_OK(pfs::byte_string("\xFF\xFF\xFF\x7F", 4).readNumber(int_int, 0, pfs::endian::LittleEndian) == sizeof(int_int) && int_int == (int)0x7FFFFFFF);
	TEST_OK(pfs::byte_string("\x7F\xFF\xFF\xFF", 4).readNumber(int_int, 0, pfs::endian::BigEndian)    == sizeof(int_int) && int_int == (int)0x7FFFFFFF);
	TEST_OK(pfs::byte_string("\x00\x00\x00\x80", 4).readNumber(int_int, 0, pfs::endian::LittleEndian) == sizeof(int_int) && int_int == (int)0x80000000);
	TEST_OK(pfs::byte_string("\x80\x00\x00\x00", 4).readNumber(int_int, 0, pfs::endian::BigEndian)    == sizeof(int_int) && int_int == (int)0x80000000);
	TEST_OK(pfs::byte_string("\x78\x56\x34\x12", 4).readNumber(int_int, 0, pfs::endian::LittleEndian) == sizeof(int_int) && int_int == (int)0x12345678);
	TEST_OK(pfs::byte_string("\x12\x34\x56\x78", 4).readNumber(int_int, 0, pfs::endian::BigEndian)    == sizeof(int_int) && int_int == (int)0x12345678);


	// TODO Add remaining tests (see test_convert_to_bytes)
}


void test_convert_number()
{
	pfs::byte_string num("1234");
	bool ok;

	pfs::byte_string ba_double;
	ba_double.setNumber(double(13.14159));
	printf("13.14159 == %s\n", ba_double.constData());
	printf("13.14159 == %g\n", double(13.14159f));
	printf("13.1415912345678 == %e\n", 13.1415912345678f);

	TEST_OK(num.toInt(&ok) == 1234 && ok);

	pfs::byte_string bnull;

	TEST_OK(bnull.toLong() == long_t(0));
}

void test_base64 ()
{
	pfs::byte_string text("Qt is great!");
	TEST_OK(text.toBase64() == "UXQgaXMgZ3JlYXQh");

	pfs::byte_string based64("UXQgaXMgZ3JlYXQh");
	TEST_OK(text == pfs::byte_string::fromBase64(based64));
}

void test_cow ()
{
	pfs::byte_string s1("Hello?");
	pfs::byte_string::reference c = s1[5]; // Non-const detachment does nothing here
	pfs::byte_string s2(s1);  // Lazy-copy, shared state

	TEST_OK(s1[5] == s2[5]);

	c = '!';

	TEST_OK(s1[5] != s2[5]); // COW OK
	TEST_OK(s1[5] == '!');   // COW OK
	TEST_OK(s2[5] == '?');   // COW OK

	TEST_OK(s1 == "Hello!");
	TEST_OK(s2 == "Hello?");
//	std::cout << "s1: " << s1.c_str() << std::endl;
//	std::cout << "s2: " << s2.c_str() << std::endl;
}
#endif

int main(int argc, char *argv[])
{
    PFS_CHECK_SIZEOF_TYPES;
    PFS_UNUSED2(argc, argv);
    int ntests = 54;
#ifdef HAVE_LONGLONG
    ntests += 12;
#endif
	BEGIN_TESTS(ntests);

	test_constructors();
//	test_append();
	test_convert_to_bytes();
//	test_read_number();
//	test_convert_number();
//	test_base64();
//	test_cow();

    END_TESTS;
}

