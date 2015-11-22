/*
 * byte_string.cpp
 *
 *  Created on: May 16, 2013
 *      Author: wladt
 */

#include <cstring>
#include <pfs/test/test.hpp>
#include "pfs/byte_string.hpp"
#include "pfs/cast/lexical_cast.hpp"

#include <iostream>
using std::cout;
using std::endl;
using pfs::byte_string;

void test_constructors ()
{
	ADD_TESTS(8);

	pfs::byte_string empty;
	pfs::byte_string serial0(5, 'W');
	pfs::byte_string serial1("ABCDF");

	TEST_OK(empty.empty());
	TEST_OK(serial0.size() == 5);
	TEST_OK(serial1.size() == 5);
	TEST_OK(strcmp("WWWWW", serial0.c_str()) == 0);

	const char * loremipsum  = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit";

	TEST_OK(strcmp(loremipsum, pfs::byte_string(loremipsum).c_str()) == 0);
	TEST_OK(strcmp(loremipsum, pfs::byte_string(loremipsum, strlen(loremipsum)).c_str()) == 0);
	TEST_OK(strcmp("Lorem", pfs::byte_string(loremipsum, 5).c_str()) == 0);

	serial0.clear();
	TEST_OK(serial0.empty());
}

void test_at ()
{
	ADD_TESTS(2);
	byte_string s("ABCDEFGHIJKLMabcdefghijklm");
	bool ok = true;

	byte_t bytes[] = {
		  0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D
		, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D
	};

	TEST_OK(sizeof(bytes)/sizeof(bytes[0]) == s.length());
	for (size_t i = 0; i < s.length(); ++i) {
		if (s.at(i) != bytes[i]) {
			ok = false;
			break;
		}
	}

	TEST_OK2(ok, "pfs::byte_string(\"ABCDEFGHIJKLMabcdefghijklm\") equals to array of bytes");
}

void test_pack ()
{
	pfs::byte_string result;

	ADD_TESTS(12);
	TEST_OK(pfs::byte_string("\xBC", 1) == pfs::pack('\xBC', pfs::endian::LittleEndian, result));
	TEST_OK(pfs::byte_string("\xBC", 1) == pfs::pack('\xBC', pfs::endian::BigEndian, result));
	TEST_OK(pfs::byte_string("\xFF\x7F", 2) == pfs::pack(short(0x7FFF), pfs::endian::LittleEndian, result));
	TEST_OK(pfs::byte_string("\x7F\xFF", 2) == pfs::pack(short(0x7FFF), pfs::endian::BigEndian, result));
	TEST_OK(pfs::byte_string("\x00\x80", 2) == pfs::pack(short(0x8000), pfs::endian::LittleEndian, result));
	TEST_OK(pfs::byte_string("\x80\x00", 2) == pfs::pack(short(0x8000), pfs::endian::BigEndian, result));

	TEST_OK(pfs::byte_string("\xFF\xFF\xFF\x7F", 4) == pfs::pack(int(0x7FFFFFFF), pfs::endian::LittleEndian, result));
	TEST_OK(pfs::byte_string("\x7F\xFF\xFF\xFF", 4) == pfs::pack(int(0x7FFFFFFF), pfs::endian::BigEndian, result));
	TEST_OK(pfs::byte_string("\x00\x00\x00\x80", 4) == pfs::pack(int(0x80000000), pfs::endian::LittleEndian, result));
	TEST_OK(pfs::byte_string("\x80\x00\x00\x00", 4) == pfs::pack(int(0x80000000), pfs::endian::BigEndian, result));
	TEST_OK(pfs::byte_string("\x78\x56\x34\x12", 4) == pfs::pack(int(0x12345678), pfs::endian::LittleEndian, result));
	TEST_OK(pfs::byte_string("\x12\x34\x56\x78", 4) == pfs::pack(int(0x12345678), pfs::endian::BigEndian, result));

if (sizeof(integral_t) == 4) {
	ADD_TESTS(6);
	TEST_OK(pfs::byte_string("\xFF\xFF\xFF\x7F", 4) == pfs::pack(integral_t(0x7FFFFFFFL), pfs::endian::LittleEndian, result));
	TEST_OK(pfs::byte_string("\x7F\xFF\xFF\xFF", 4) == pfs::pack(integral_t(0x7FFFFFFFL), pfs::endian::BigEndian, result));
	TEST_OK(pfs::byte_string("\x00\x00\x00\x80", 4) == pfs::pack(integral_t(0x80000000L), pfs::endian::LittleEndian, result));
	TEST_OK(pfs::byte_string("\x80\x00\x00\x00", 4) == pfs::pack(integral_t(0x80000000L), pfs::endian::BigEndian, result));
	TEST_OK(pfs::byte_string("\x78\x56\x34\x12", 4) == pfs::pack(integral_t(0x12345678L), pfs::endian::LittleEndian, result));
	TEST_OK(pfs::byte_string("\x12\x34\x56\x78", 4) == pfs::pack(integral_t(0x12345678L), pfs::endian::BigEndian, result));
} else { // sizeof(long) == 8 - on x64
	ADD_TESTS(6);
	TEST_OK(pfs::byte_string("\xFF\xFF\xFF\x7F\x00\x00\x00\x00", 8) == pfs::pack(integral_t(0x7FFFFFFFL), pfs::endian::LittleEndian, result));
	TEST_OK(pfs::byte_string("\x00\x00\x00\x00\x7F\xFF\xFF\xFF", 8) == pfs::pack(integral_t(0x7FFFFFFFL), pfs::endian::BigEndian, result));
	TEST_OK(pfs::byte_string("\x00\x00\x00\x80\x00\x00\x00\x00", 8) == pfs::pack(integral_t(0x80000000L), pfs::endian::LittleEndian, result));
	TEST_OK(pfs::byte_string("\x00\x00\x00\x00\x80\x00\x00\x00", 8) == pfs::pack(integral_t(0x80000000L), pfs::endian::BigEndian, result));
	TEST_OK(pfs::byte_string("\x78\x56\x34\x12\x00\x00\x00\x00", 8) == pfs::pack(integral_t(0x12345678L), pfs::endian::LittleEndian, result));
	TEST_OK(pfs::byte_string("\x00\x00\x00\x00\x12\x34\x56\x78", 8) == pfs::pack(integral_t(0x12345678L), pfs::endian::BigEndian, result));
}

#ifdef PFS_HAVE_LONGLONG
	ADD_TESTS(12);
	TEST_OK(pfs::byte_string("\xFF\xFF\xFF\x7F\x00\x00\x00\x00", 8) == pfs::pack(0x7FFFFFFFLL, pfs::endian::LittleEndian, result));
	TEST_OK(pfs::byte_string("\x00\x00\x00\x00\x7F\xFF\xFF\xFF", 8) == pfs::pack(0x7FFFFFFFLL, pfs::endian::BigEndian, result));
	TEST_OK(pfs::byte_string("\x00\x00\x00\x80\x00\x00\x00\x00", 8) == pfs::pack(0x80000000LL, pfs::endian::LittleEndian, result));
	TEST_OK(pfs::byte_string("\x00\x00\x00\x00\x80\x00\x00\x00", 8) == pfs::pack(0x80000000LL, pfs::endian::BigEndian, result));
	TEST_OK(pfs::byte_string("\x78\x56\x34\x12\x00\x00\x00\x00", 8) == pfs::pack(0x12345678LL, pfs::endian::LittleEndian, result));
	TEST_OK(pfs::byte_string("\x00\x00\x00\x00\x12\x34\x56\x78", 8) == pfs::pack(0x12345678LL, pfs::endian::BigEndian, result));
	TEST_OK(pfs::byte_string("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x7F", 8) == pfs::pack(0x7FFFFFFFFFFFFFFFLL, pfs::endian::LittleEndian, result));
	TEST_OK(pfs::byte_string("\x7F\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8) == pfs::pack(0x7FFFFFFFFFFFFFFFLL, pfs::endian::BigEndian, result));
	TEST_OK(pfs::byte_string("\x00\x00\x00\x00\x00\x00\x00\x80", 8) == pfs::pack(0x8000000000000000LL, pfs::endian::LittleEndian, result));
	TEST_OK(pfs::byte_string("\x80\x00\x00\x00\x00\x00\x00\x00", 8) == pfs::pack(0x8000000000000000LL, pfs::endian::BigEndian, result));
	TEST_OK(pfs::byte_string("\xDE\xCD\xBC\xAB\x78\x56\x34\x12", 8) == pfs::pack(0x12345678ABBCCDDELL, pfs::endian::LittleEndian, result));
	TEST_OK(pfs::byte_string("\x12\x34\x56\x78\xAB\xBC\xCD\xDE", 8) == pfs::pack(0x12345678ABBCCDDELL, pfs::endian::BigEndian, result));
#endif
}

void test_unpack ()
{
	char  ch;
	short short_int;
	unsigned short ushort_int;
	int int_int;

	ADD_TESTS(16);

	TEST_OK(pfs::unpack(byte_string("\xBC", 1), 0, pfs::endian::LittleEndian, ch) == sizeof(ch) && ch == '\xBC');
	TEST_OK(pfs::unpack(byte_string("\xBC", 1), 0, pfs::endian::BigEndian, ch) == sizeof(ch) && ch == '\xBC');

	TEST_OK(pfs::unpack(byte_string("\xFF\x7F", 2), 0, pfs::endian::LittleEndian, short_int)  == sizeof(short_int) && short_int == (short)0x7FFF);
	TEST_OK(pfs::unpack(byte_string("\x7F\xFF", 2), 0, pfs::endian::BigEndian, short_int)     == sizeof(short_int) && short_int == (short)0x7FFF);
	TEST_OK(pfs::unpack(byte_string("\x00\x80", 2), 0, pfs::endian::LittleEndian, short_int)  == sizeof(short_int) && short_int == (short)0x8000);
	TEST_OK(pfs::unpack(byte_string("\x80\x00", 2), 0, pfs::endian::BigEndian, short_int)     == sizeof(short_int) && short_int == (short)0x8000);

	TEST_OK(pfs::unpack(byte_string("\xFF\x7F", 2), 0, pfs::endian::LittleEndian, ushort_int) == sizeof(ushort_int) && ushort_int == 0x7FFF);
	TEST_OK(pfs::unpack(byte_string("\x7F\xFF", 2), 0, pfs::endian::BigEndian, ushort_int)    == sizeof(ushort_int) && ushort_int == 0x7FFF);
	TEST_OK(pfs::unpack(byte_string("\x00\x80", 2), 0, pfs::endian::LittleEndian, ushort_int) == sizeof(ushort_int) && ushort_int == 0x8000);
	TEST_OK(pfs::unpack(byte_string("\x80\x00", 2), 0, pfs::endian::BigEndian, ushort_int)    == sizeof(ushort_int) && ushort_int == 0x8000);

	TEST_OK(pfs::unpack(byte_string("\xFF\xFF\xFF\x7F", 4), 0, pfs::endian::LittleEndian, int_int) == sizeof(int_int) && int_int == (int)0x7FFFFFFF);
	TEST_OK(pfs::unpack(byte_string("\x7F\xFF\xFF\xFF", 4), 0, pfs::endian::BigEndian, int_int)    == sizeof(int_int) && int_int == (int)0x7FFFFFFF);
	TEST_OK(pfs::unpack(byte_string("\x00\x00\x00\x80", 4), 0, pfs::endian::LittleEndian, int_int) == sizeof(int_int) && int_int == (int)0x80000000);
	TEST_OK(pfs::unpack(byte_string("\x80\x00\x00\x00", 4), 0, pfs::endian::BigEndian, int_int)    == sizeof(int_int) && int_int == (int)0x80000000);
	TEST_OK(pfs::unpack(byte_string("\x78\x56\x34\x12", 4), 0, pfs::endian::LittleEndian, int_int) == sizeof(int_int) && int_int == (int)0x12345678);
	TEST_OK(pfs::unpack(byte_string("\x12\x34\x56\x78", 4), 0, pfs::endian::BigEndian, int_int)    == sizeof(int_int) && int_int == (int)0x12345678);

	// TODO Add remaining tests (see test_convert_to_bytes)
}

void test_append ()
{
	{
		ADD_TESTS(3);
		const char * sample = "Hello, World!";
		pfs::byte_string s;
		pfs::byte_string s1("Hello");
		pfs::byte_string s2(", ");
		pfs::byte_string s3("World!");

		s.append(s1).append(s2).append(s3);

		TEST_OK(s.size() == strlen(sample));
		TEST_OK(s.length() == s1.length() + s2.length() + s3.length());
		TEST_OK(strcmp(s.c_str(), sample) == 0);
	}

	{
		ADD_TESTS(3);
		pfs::byte_string s;
		s += pfs::byte_string("Hello");
		s += ',';
		s += ' ';
		s += "World!";

		TEST_OK(strcmp(s.c_str(), "Hello, World!") == 0);
		TEST_OK(s.size() == 13);
		TEST_OK(s.length() == 13);
	}
}

void test_erase ()
{
	{
		ADD_TESTS(7);

		byte_string sample("ABCDEFGHIJKLMabcdefghijklm");
		byte_string s;

		s = sample;
		TEST_OK(strcmp(s.erase(0, 0).c_str(), "ABCDEFGHIJKLMabcdefghijklm") == 0);

		s = sample;
		TEST_OK(strcmp(s.erase(0, 1).c_str(), "BCDEFGHIJKLMabcdefghijklm") == 0);

		s = sample;
		TEST_OK(strcmp(s.erase(0, 5).c_str(), "FGHIJKLMabcdefghijklm") == 0);

		s = sample;
		TEST_OK(strcmp(s.erase(13, 5).c_str(), "ABCDEFGHIJKLMfghijklm") == 0);

		s = sample;
		TEST_OK(strcmp(s.erase(s.size(), 5).c_str(), "ABCDEFGHIJKLMabcdefghijklm") == 0);

		s = sample;
		TEST_OK(strcmp(s.erase(13, 100).c_str(), "ABCDEFGHIJKLM") == 0);

		s = sample;
		TEST_OK(strcmp(s.erase(0, 100).c_str(), "") == 0);
	}

	{
		ADD_TESTS(5);

		byte_string sample("ABCDEFGHIJKLMabcdefghijklm");
		byte_string s;

		s = sample;
		s.erase(s.cbegin(), s.cbegin());
		TEST_OK(strcmp(s.c_str(), "ABCDEFGHIJKLMabcdefghijklm") == 0);

		s = sample;
		s.erase(s.cbegin(), s.cbegin() + 1);
		TEST_OK(strcmp(s.c_str(), "BCDEFGHIJKLMabcdefghijklm") == 0);

		s = sample;
		s.erase(s.cbegin(), s.cbegin() + 5);
		TEST_OK(strcmp(s.c_str(), "FGHIJKLMabcdefghijklm") == 0);

		s = sample;
		s.erase(s.cbegin() + 13, s.cbegin() + 18);
		TEST_OK(strcmp(s.c_str(), "ABCDEFGHIJKLMfghijklm") == 0);

		s = sample;
		s.erase(s.cbegin(), s.cend());
		TEST_OK(strcmp(s.c_str(), "") == 0);
	}
}

#if __COMMENT__

void test_replace ()
{
	{
		byte_string s("Hallo Welt!");
		const char * s1 = "Hello, World!";

		s.replace(0, 5, s1, 6);
		TEST_OK(s.length() == 12);
		TEST_OK(strcmp(s.c_str(), "Hello, Welt!") == 0);

		s.replace(7, 4, s1 + 7, 5);
		TEST_OK(s.length() == 13);
		TEST_OK(strcmp(s.c_str(), "Hello, World!") == 0);

		// replace full string with empty string - get empty string
		s.replace(0, s.length(), "", 0);
		TEST_OK(strcmp(s.c_str(), "") == 0);
	}

	{
		byte_string s("Hallo Welt!");
		byte_string s1("Hello, World!");

		s.replace(s.cbegin(), s.cbegin() + 5, s1.cbegin(), s1.cbegin() + 6);
		TEST_OK(s.length() == 12);
		TEST_OK(strcmp(s.c_str(), "Hello, Welt!") == 0);

		s.replace(s.cbegin() + 7, s.cbegin() + 11, s1.cbegin() + 7, s1.cbegin() + 12);
		TEST_OK(s.length() == 13);
		TEST_OK(strcmp(s.c_str(), "Hello, World!") == 0);

		// replace full string with empty string - get empty string
		pfs::byte_string nil;
		s.replace(s.cbegin(), s.cend(), nil.cbegin(), nil.cend());
		TEST_OK(strcmp(s.c_str(), "") == 0);
	}
}


void test_insert ()
{
	pfs::byte_string s;
	pfs::byte_string s1("Hello");
	pfs::byte_string s2("+++World!+++");
	pfs::byte_string s3(", ");

	s.insert(0, s1, 0, s1.length());
	TEST_OK(s.size() == s1.size());
	TEST_OK(s.length() == s1.length());
	TEST_OK(strcmp(s.c_str(), s1.c_str()) == 0);
	TEST_OK(strcmp(s.c_str(), "Hello") == 0);

	s.insert(5, s2, 3, 6);
	TEST_OK(strcmp(s.c_str(), "HelloWorld!") == 0);

	s.insert(5, s3, 0, s3.length());
	TEST_OK(strcmp(s.c_str(), "Hello, World!") == 0);
}

void test_find ()
{
	pfs::byte_string s("Hello, World!");

	TEST_OK(s.find("Hello") == s.begin());
	TEST_OK(s.find("World") != s.begin());
	TEST_OK(s.find("World") == s.begin() + 7);
	TEST_OK(s.find("Hallo") == s.end());

	TEST_OK(s.contains(pfs::byte_string("World")));
	TEST_OK(s.contains("World"));
	TEST_OK(s.contains(byte_t('W')));
	TEST_OK(s.contains('W'));

//	TEST_OK(s.startsWith (const byte_string & str) const { return find(str) == cbegin(); }
//	TEST_OK(s.startsWith (const char * s) const          { return find(s)   == cbegin(); }
//	TEST_OK(s.startsWith (byte_t v) const                { return find(v)   == cbegin(); }
//	TEST_OK(s.startsWith (char v) const                  { return find(v)   == cbegin(); }
//
//	TEST_OK(s.endsWith   (const byte_string & str) const { return find(str) == cend() - str.length(); }
//	TEST_OK(s.endsWith   (const char * s) const          { return find(s)   == cend() - std::strlen(s); }
//	TEST_OK(s.endsWith   (byte_t v) const                { return find(v)   == cend() - 1; }
//	TEST_OK(s.endsWith   (char v) const                  { return find(v)   == cend() - 1; }
}



#endif // __COMMENT__

void test_lexical_cast ()
{
	ADD_TESTS(13);
	byte_string result;
	TEST_OK(pfs::lexical_cast(0, result) == "0");
	TEST_OK(pfs::lexical_cast(127, result) == "127");
	TEST_OK(pfs::lexical_cast(-128, result) == "-128");
	TEST_OK(pfs::lexical_cast(255, result) == "255");
	TEST_OK(pfs::lexical_cast(32767, result) == "32767");
	TEST_OK(pfs::lexical_cast(-32768, result) == "-32768");
	TEST_OK(pfs::lexical_cast(65535, result) == "65535");
	TEST_OK(pfs::lexical_cast(8388607, result) == "8388607");
	TEST_OK(pfs::lexical_cast(-8388608, result) == "-8388608");
	TEST_OK(pfs::lexical_cast(16777215, result) == "16777215");
	TEST_OK(pfs::lexical_cast(2147483647, result) == "2147483647");
	TEST_OK(pfs::lexical_cast(PFS_INT32_MIN, result) == "-2147483648");
	TEST_OK(pfs::lexical_cast(PFS_ULONG_LITERAL(4294967295), result) == "4294967295");

#ifdef PFS_HAVE_INT64
	ADD_TESTS(3);
	TEST_OK(pfs::lexical_cast(PFS_INT64_MAX, result)  == "9223372036854775807");
	TEST_OK(pfs::lexical_cast(PFS_INT64_MIN, result)  == "-9223372036854775808");
	TEST_OK(pfs::lexical_cast(PFS_UINT64_MAX, result) == "18446744073709551615");
#endif

	ADD_TESTS(6);
	// Note: single-precision floating-point numbers have a 24-bit mantissa, which is approximately 7.2 decimal digits.
	TEST_OK(pfs::lexical_cast(0.0f, 'g', result) == "0");
	TEST_OK(pfs::lexical_cast(0.0f, 'f', 6, result) == "0.000000");
	TEST_OK(pfs::lexical_cast(0.0f, 'f', 0, result) == "0.000000");
	TEST_OK(pfs::lexical_cast(0.0f, 'f', 1, result) == "0.0");
	TEST_OK(pfs::lexical_cast(3.14159f, 'f', 5, result) == "3.14159");
	TEST_OK(pfs::lexical_cast(1234567.875f, 'f', 3, result) == "1234567.875");

	unsigned int u;

	ADD_TESTS(7);

	TEST_OK(pfs::lexical_cast(byte_string("0"), u)   && u == 0);
	TEST_OK(pfs::lexical_cast(byte_string("+0"), u)  && u == 0);
	TEST_OK(pfs::lexical_cast(byte_string("-0"), u)  && u == 0);
	TEST_OK(pfs::lexical_cast(byte_string("1"), u)   && u == 1);
	TEST_OK(pfs::lexical_cast(byte_string("+1"), u)  && u == 1);
	TEST_OK(pfs::lexical_cast(byte_string("-1"), u)  && u == PFS_UINT32_MAX);
	TEST_OK(pfs::lexical_cast(byte_string("123"), u) && u == 123);

#ifdef PFS_HAVE_LONGLONG
	unsigned long long ull;
	if (sizeof(long long) == 8) {
		ADD_TESTS(1);
		TEST_OK(pfs::lexical_cast(byte_string("18446744073709551615"), ull) && ull == PFS_ULLONG_MAX);
	}
#endif

	ADD_TESTS(1);
	unsigned long ul;

	if (sizeof(long) == 8) {
		TEST_OK(pfs::lexical_cast(byte_string("18446744073709551615"), ul) && ul == PFS_ULLONG_MAX);
	} else {
		TEST_OK(pfs::lexical_cast(byte_string("4294967295"), ul) && ul == PFS_ULONG_MAX);
	}

//	TEST_OK(compare_signed("0", 0));
//	TEST_OK(compare_signed("+0", 0));
//	TEST_OK(compare_signed("-0", 0));
//
//	TEST_OK(compare_signed("1", 1));
//	TEST_OK(compare_signed("+1", 1));
//	TEST_OK(compare_signed("-1", -1));
//
//	TEST_OK(compare_signed("123", 123));
//	TEST_OK(compare_signed("+123", 123));
//	TEST_OK(compare_signed("-123", -123));
//

#ifdef PFS_HAVE_LONGLONG
	if (sizeof(long long) == 8) {
		ADD_TESTS(3);
		long long ll;

		TEST_OK(pfs::lexical_cast(byte_string("9223372036854775807"), ll) && ll == PFS_LLONG_MAX);
		TEST_OK(pfs::lexical_cast(byte_string("-9223372036854775808"), ll) && ll == PFS_LLONG_MIN);
		TEST_OK(pfs::lexical_cast(byte_string("-9223372036854775807"), ll) && ll == (PFS_LLONG_MIN + 1));
	}
#endif

	ADD_TESTS(2);
	long l;

	if (sizeof(long) == 8) {
		TEST_OK(pfs::lexical_cast(byte_string("9223372036854775807"), l) && l == PFS_LONG_MAX);
		TEST_OK(pfs::lexical_cast(byte_string("-9223372036854775808"), l) && l == PFS_LONG_MIN);
	} else {
		TEST_OK(pfs::lexical_cast(byte_string("2147483647"), l) && l == PFS_LONG_MAX);
		TEST_OK(pfs::lexical_cast(byte_string("-2147483648"), l) && l == PFS_LONG_MIN);
	}

#if __COMMENT__
	ADD_TESTS(1);
	real_t r;
	TEST_OK(pfs::lexical_cast(byte_string("9."), r) && r == 9.0f);
	TEST_OK(pfs::byte_string("9").toReal(& ok) == 9.0 && ok);
	TEST_OK(pfs::byte_string("9.0").toReal(& ok) == 9.0 && ok);
	TEST_OK(pfs::byte_string("9.0000").toReal(& ok) == 9.0 && ok);
	TEST_OK(pfs::byte_string("9.00001").toReal(& ok) == PFS_REAL_LITERAL(9.00001) && ok);
	TEST_OK(pfs::byte_string("009").toReal(& ok) == 9.0 && ok);
	TEST_OK(pfs::byte_string("0.09e02").toReal(& ok) == 9.0 && ok);
	TEST_OK(pfs::byte_string("0.9999999999999999999999999999999999").toReal(& ok) == PFS_REAL_LITERAL(0.9999999999999999999999999999999999) && ok);
	TEST_OK(pfs::byte_string("2.22e-308").toReal(& ok) == PFS_REAL_LITERAL(2.22e-308) && ok);
	TEST_OK(pfs::byte_string("1.34").toReal(& ok) == PFS_REAL_LITERAL(1.34) && ok);
	TEST_OK(pfs::byte_string("12.34").toReal(& ok) == PFS_REAL_LITERAL(12.34) && ok);
	TEST_OK(pfs::byte_string("123.456").toReal(& ok) == PFS_REAL_LITERAL(123.456) && ok);

	TEST_OK(pfs::byte_string("2.22507385850720138309e-308").toReal(& ok) == PFS_REAL_LITERAL(2.22507385850720138309e-308) && ok);
	TEST_OK(pfs::byte_string("1.79769313486231570815e+308").toReal(& ok) == PFS_REAL_LITERAL(1.79769313486231570815e+308) && ok);
//#ifdef PFS_HAVE_REAL128
//	ADD_TESTS(3);
//	TEST_OK(pfs::byte_string("3.36210314311209350626e-4932").toReal(& ok) == PFS_REAL_LITERAL(3.36210314311209350626e-4932) && ok);
//	TEST_OK(pfs::byte_string("1.18973149535723176502e+4932").toReal(& ok) == PFS_REAL_LITERAL(1.18973149535723176502e+4932) && ok);
//	TEST_OK(pfs::byte_string("1.18973149535723176502126385303e+4932").toReal(& ok) == PFS_REAL_LITERAL(1.18973149535723176502126385303e+4932) && ok);
//#endif
	cout.precision(50);
	TEST_OK(pfs::byte_string("18973149535723176502126385303").toReal(& ok) == PFS_REAL_LITERAL(18973149535723176502126385303.0) && ok);
	TEST_OK(pfs::approx_eq(pfs::byte_string("12345678901234567890123456789").toReal(& ok)
					, PFS_REAL_LITERAL(12345678901234567890123456789.0)
					, 10.0) && ok);
	cout << pfs::byte_string("12345678901234567890123456789").toReal(& ok) << endl;
	cout << PFS_REAL_LITERAL(12345678901234567890123456789.0) << endl;
	real_t rr = pfs::byte_string("12345678901234567890123456789").toReal(& ok) - PFS_REAL_LITERAL(12345678901234567890123456789.0);
	cout << (pfs::byte_string("12345678901234567890123456789").toReal(& ok) - PFS_REAL_LITERAL(12345678901234567890123456789.0)) << endl;

	cout << rr << endl;
#endif
}

void test_base64 ()
{
	ADD_TESTS(2);
	pfs::byte_string result;
	pfs::byte_string text("Qt is great!");
	TEST_OK(pfs::base64_encode(text, result) == "UXQgaXMgZ3JlYXQh");

	pfs::byte_string based64("UXQgaXMgZ3JlYXQh");
	TEST_OK(text == pfs::base64_decode(based64, result));
}

void test_substr ()
{
	ADD_TESTS(5);

	byte_string s(("ABCDEFabcdef"));

	TEST_OK(s.substr(0,0).empty());
	TEST_OK(s.substr(s.length(),0).empty());
	TEST_OK(strcmp(s.substr(0,5).c_str(), "ABCDE") == 0);
	TEST_OK(strcmp(s.substr(6,6).c_str(), "abcdef") == 0);
	TEST_OK(strcmp(s.substr(6,s.length() + 1).c_str(), "abcdef") == 0);
}

int main(int argc, char *argv[])
{
    PFS_UNUSED2(argc, argv);
	BEGIN_TESTS(0);

	test_constructors();
	test_at();
	test_erase();
	test_pack();
	test_unpack();
	test_append();
//	test_replace();
//	test_insert();
//	test_find();
	test_lexical_cast();
	test_base64();
	test_substr();

    END_TESTS;
}

// TODO Add this tests
/*
void
test(char* str)
{
double d1, d2;
char *e1, *e2;
int x1, x2;

printf("CASE: %s\n", str);

errno = 0;
e1 = NULL;
d1 = vim_strtod(str, &e1);
x1 = errno;

errno = 0;
e2 = NULL;
d2 = strtod(str, &e2);
x2 = errno;

if (d1 != d2 || e1 != e2 || x1 != x2) {
printf(" ERR: %s, %s\n", str, strerror(errno));
printf(" E1 %f, %g, %s, %d\n", d1, d1, e1 ? e1 : "", x1);
printf(" E2 %f, %g, %s, %d\n", d2, d2, e2 ? e2 : "", x2);
if (d1 != d2) puts("different value");
if (e1 != e2) puts("different end position");
if (x1 != x2) puts("different errno");
} else {
printf(" SUCCESS [%f][%s]: %s\n", d1, e1 ? e1 : "", strerror(errno));
}
printf("\n");
}

int
main()
{
test(".1");
test(" .");
test(" 1.2e3");
test(" +1.2e3");
test("1.2e3");
test("+1.2e3");
test("+1.e3");
test("-1.2e3");
test("-1.2e3.5");
test("-1.2e");
test("--1.2e3.5");
test("--1-.2e3.5");
test("-a");
test("a");
test(".1e");
test(".1e3");
test(".1e-3");
test(".1e-");
test(" .e-");
test(" .e");
test(" e");
test(" e0");
test(" ee");
test(" -e");
test(" .9");
test(" ..9");
test("009");
test("0.09e02");
// http://thread.gmane.org/gmane.editors.vim.devel/19268/
test("0.9999999999999999999999999999999999");
test("2.2250738585072010e-308"); // BUG
// PHP (slashdot.jp): http://opensource.slashdot.jp/story/11/01/08/0527259/PHP%E3%81%AE%E6%B5%AE%E5%8B%95%E5%B0%8F%E6%95%B0%E7%82%B9%E5%87%A6%E7%90%86%E3%81%AB%E7%84%A1%E9%99%90%E3%83%AB%E3%83%BC%E3%83%97%E3%81%AE%E3%83%90%E3%82%B0
test("2.2250738585072011e-308");
// Gauche: http://blog.practical-scheme.net/gauche/20110203-bitten-by-floating-point-numbers-again
test("2.2250738585072012e-308");
test("2.2250738585072013e-308"); // Hmm.
test("2.2250738585072014e-308"); // Hmm.
}
 ***/
