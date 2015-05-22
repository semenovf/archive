/*
 * byte_string.cpp
 *
 *  Created on: May 16, 2013
 *      Author: wladt
 */

#include <pfs/test.hpp>
#include <cstring>
#include "pfs/byte_string.hpp"

#include <iostream>
using namespace std;

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

	nil.clear();
	TEST_OK(nil.isNull());

	serial.clear();
	TEST_OK(!serial.isNull());
	TEST_OK(serial.isEmpty());
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

if (sizeof(integral_t) == 4) {
	TEST_OK(pfs::byte_string("\xFF\xFF\xFF\x7F", 4) == pfs::byte_string::toBytes(integral_t(0x7FFFFFFFL), pfs::endian::LittleEndian));
	TEST_OK(pfs::byte_string("\x7F\xFF\xFF\xFF", 4) == pfs::byte_string::toBytes(integral_t(0x7FFFFFFFL), pfs::endian::BigEndian));
	TEST_OK(pfs::byte_string("\x00\x00\x00\x80", 4) == pfs::byte_string::toBytes(integral_t(0x80000000L), pfs::endian::LittleEndian));
	TEST_OK(pfs::byte_string("\x80\x00\x00\x00", 4) == pfs::byte_string::toBytes(integral_t(0x80000000L), pfs::endian::BigEndian));
	TEST_OK(pfs::byte_string("\x78\x56\x34\x12", 4) == pfs::byte_string::toBytes(integral_t(0x12345678L), pfs::endian::LittleEndian));
	TEST_OK(pfs::byte_string("\x12\x34\x56\x78", 4) == pfs::byte_string::toBytes(integral_t(0x12345678L), pfs::endian::BigEndian));
} else { // sizeof(long) == 8 - on x64
	TEST_OK(pfs::byte_string("\xFF\xFF\xFF\x7F\x00\x00\x00\x00", 8) == pfs::byte_string::toBytes(integral_t(0x7FFFFFFFL), pfs::endian::LittleEndian));
	TEST_OK(pfs::byte_string("\x00\x00\x00\x00\x7F\xFF\xFF\xFF", 8) == pfs::byte_string::toBytes(integral_t(0x7FFFFFFFL), pfs::endian::BigEndian));
	TEST_OK(pfs::byte_string("\x00\x00\x00\x80\x00\x00\x00\x00", 8) == pfs::byte_string::toBytes(integral_t(0x80000000L), pfs::endian::LittleEndian));
	TEST_OK(pfs::byte_string("\x00\x00\x00\x00\x80\x00\x00\x00", 8) == pfs::byte_string::toBytes(integral_t(0x80000000L), pfs::endian::BigEndian));
	TEST_OK(pfs::byte_string("\x78\x56\x34\x12\x00\x00\x00\x00", 8) == pfs::byte_string::toBytes(integral_t(0x12345678L), pfs::endian::LittleEndian));
	TEST_OK(pfs::byte_string("\x00\x00\x00\x00\x12\x34\x56\x78", 8) == pfs::byte_string::toBytes(integral_t(0x12345678L), pfs::endian::BigEndian));
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

void test_erase ()
{
	{
		pfs::byte_string sample("ABCDEFGHIJKLMabcdefghijklm");
		pfs::byte_string s;

		s = sample;
		TEST_OK(strcmp(s.erase(0, 0).c_str(), "ABCDEFGHIJKLMabcdefghijklm") == 0);

		s = sample;
		TEST_OK(strcmp(s.erase(0, 1).c_str(), "BCDEFGHIJKLMabcdefghijklm") == 0);

		s = sample;
		TEST_OK(strcmp(s.erase(0, 5).c_str(), "FGHIJKLMabcdefghijklm") == 0);

		s = sample;
		TEST_OK(strcmp(s.erase(13, 5).c_str(), "ABCDEFGHIJKLMfghijklm") == 0);

		s = sample;
		TEST_OK(strcmp(s.erase(100, 5).c_str(), "ABCDEFGHIJKLMabcdefghijklm") == 0);

		s = sample;
		TEST_OK(strcmp(s.erase(13, 100).c_str(), "ABCDEFGHIJKLM") == 0);

		s = sample;
		TEST_OK(strcmp(s.erase(0, 100).c_str(), "") == 0);
	}

	{
		pfs::byte_string sample("ABCDEFGHIJKLMabcdefghijklm");
		pfs::byte_string s;

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


void test_at ()
{
	pfs::byte_string s("ABCDEFGHIJKLMabcdefghijklm");
	bool ok = true;
	byte_t bytes[] = {
		  0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D
		, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D
	};

	TEST_OK(sizeof(bytes)/sizeof(bytes[0]) == s.length());
	for (size_t i = 0; i < s.length(); ++i) {
		if (s.valueAt(i) != bytes[i]) {
			ok = false;
			break;
		}
	}

	TEST_OK2(ok, "pfs::byte_string(\"ABCDEFGHIJKLMabcdefghijklm\") equals to array of bytes");
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

void test_append ()
{
	{
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

void test_prepend ()
{
	const char * sample = "Hello, World!";
	pfs::byte_string s;
	pfs::byte_string s1("Hello");
	pfs::byte_string s2(", ");
	pfs::byte_string s3("World!");

	s.prepend(s3).prepend(s2).prepend(s1);

	TEST_OK(s.size() == strlen(sample));
	TEST_OK(s.length() == s1.length() + s2.length() + s3.length());
	TEST_OK(strcmp(s.c_str(), sample) == 0);
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

void test_replace ()
{
	{
		pfs::byte_string s("Hallo Welt!");
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
		pfs::byte_string s("Hallo Welt!");
		pfs::byte_string s1("Hello, World!");

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

void test_substr ()
{
	pfs::byte_string s(("ABCDEFabcdef"));
	TEST_OK(s.substr(0,0).isEmpty());
	TEST_OK(s.substr(s.length(),100).isEmpty());
	TEST_OK(strcmp(s.substr(0,5).c_str(), "ABCDE") == 0);
	TEST_OK(strcmp(s.substr(6,6).c_str(), "abcdef") == 0);
	TEST_OK(strcmp(s.substr(6,s.length() + 1).c_str(), "abcdef") == 0);
}


void test_to_string ()
{
	TEST_OK(pfs::byte_string::toString(0) == "0");
	TEST_OK(pfs::byte_string::toString(127) == "127");
	TEST_OK(pfs::byte_string::toString(-128) == "-128");
	TEST_OK(pfs::byte_string::toString(255) == "255");
	TEST_OK(pfs::byte_string::toString(32767) == "32767");
	TEST_OK(pfs::byte_string::toString(-32768) == "-32768");
	TEST_OK(pfs::byte_string::toString(65535) == "65535");
	TEST_OK(pfs::byte_string::toString(8388607) == "8388607");
	TEST_OK(pfs::byte_string::toString(-8388608) == "-8388608");
	TEST_OK(pfs::byte_string::toString(16777215) == "16777215");
	TEST_OK(pfs::byte_string::toString(2147483647) == "2147483647");
	TEST_OK(pfs::byte_string::toString(PFS_INT32_MIN) == "-2147483648");
	TEST_OK(pfs::byte_string::toString(PFS_ULONG_LITERAL(4294967295)) == "4294967295");

#ifdef HAVE_INT64
	TEST_OK(pfs::byte_string::toString(PFS_INT64_MAX) == "9223372036854775807");
	TEST_OK(pfs::byte_string::toString(PFS_INT64_MIN) == "-9223372036854775808");
	TEST_OK(pfs::byte_string::toString(PFS_UINT64_MAX) == "18446744073709551615");
#endif

	// Note: single-precision floating-point numbers have a 24-bit mantissa, which is approximately 7.2 decimal digits.
	TEST_OK(pfs::byte_string::toString(0.0f, 'g') == "0");
	TEST_OK(pfs::byte_string::toString(0.0f, 'f', 6) == "0.000000");
	TEST_OK(pfs::byte_string::toString(0.0f, 'f', 0) == "0.000000");
	TEST_OK(pfs::byte_string::toString(0.0f, 'f', 1) == "0.0");
	TEST_OK(pfs::byte_string::toString(3.14159f, 'f', 5) == "3.14159");
	TEST_OK(pfs::byte_string::toString(1234567.875f, 'f', 3) == "1234567.875");

//	cout << "==" << utfstring::toString(0.0f, 'f', 1).c_str() << endl;
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
}

void test_convert_to_number ()
{
	bool ok;
	TEST_OK(pfs::byte_string("0").toUInt(& ok) == 0 && ok);
	TEST_OK(pfs::byte_string("+0").toUInt(& ok) == 0 && ok);
	TEST_OK(pfs::byte_string("-0").toUInt(& ok) == 0 && ok);

	TEST_OK(pfs::byte_string("1").toUInt(& ok) == 1 && ok);
	TEST_OK(pfs::byte_string("+1").toUInt(& ok) == 1 && ok);

	TEST_OK(pfs::byte_string("-1").toUInt(& ok) == 0 && !ok); // FIXME (result valid for linux64 and win64)
	//TEST_OK(pfs::byte_string("-1").toUInt(& ok) == PFS_UINT32_MAX && ok); // FIXME (result valid for win32)
	//cout << pfs::byte_string("-1").toUInt(& ok) << endl;

	TEST_OK(pfs::byte_string("123").toUInt(& ok) == 123 && ok);

#ifdef PFS_HAVE_INT64
#	ifdef PFS_HAVE_LONGLONG
	TEST_OK(pfs::byte_string("18446744073709551615").toULongLong(& ok) == PFS_ULONG_MAX && ok);
#	else
	TEST_OK(pfs::byte_string("18446744073709551615").toULong(& ok) == PFS_ULONG_MAX && ok);
#	endif
#else
	TEST_OK(pfs::byte_string("4294967295").toULong(& ok) == PFS_ULONG_MAX && ok);
#endif

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


#ifdef PFS_HAVE_INT64
#	ifdef PFS_HAVE_LONGLONG
	TEST_OK(pfs::byte_string("9223372036854775807").toLongLong(& ok) == PFS_LONG_MAX && ok);
	TEST_OK(pfs::byte_string("-9223372036854775808").toLongLong(& ok) == PFS_LONG_MIN && ok);
#	else
	TEST_OK(pfs::byte_string("9223372036854775807").toLong(& ok) == PFS_LONG_MAX && ok);
	TEST_OK(pfs::byte_string("-9223372036854775808").toLong(& ok) == PFS_LONG_MIN && ok);
#	endif
#else
	TEST_OK(pfs::byte_string("2147483647").toLong(& ok) == PFS_LONG_MAX && ok);
	TEST_OK(pfs::byte_string("-2147483648").toLong(& ok) == PFS_LONG_MIN && ok);
#endif

	TEST_OK(pfs::byte_string("9.").toReal(& ok) == 9.0 && ok);
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
#ifdef PFS_HAVE_REAL128
	ADD_TESTS(3);
	TEST_OK(pfs::byte_string("3.36210314311209350626e-4932").toReal(& ok) == PFS_REAL_LITERAL(3.36210314311209350626e-4932) && ok);
	TEST_OK(pfs::byte_string("1.18973149535723176502e+4932").toReal(& ok) == PFS_REAL_LITERAL(1.18973149535723176502e+4932) && ok);
	TEST_OK(pfs::byte_string("1.18973149535723176502126385303e+4932").toReal(& ok) == PFS_REAL_LITERAL(1.18973149535723176502126385303e+4932) && ok);
#endif
	TEST_OK(pfs::byte_string("18973149535723176502126385303").toReal(& ok) == PFS_REAL_LITERAL(18973149535723176502126385303.0) && ok);
	TEST_OK(pfs::byte_string("12345678901234567890123456789").toReal(& ok) == PFS_REAL_LITERAL(12345678901234567890123456789.0) && ok);
}

int main(int argc, char *argv[])
{
    PFS_UNUSED2(argc, argv);
    int ntests = 132;
#ifdef HAVE_LONGLONG
    ntests += 12;
#endif
	BEGIN_TESTS(ntests);

	test_constructors();
	test_at();
	test_erase();
	test_convert_to_bytes();
	test_insert();
	test_append();
	test_prepend();
	test_find();
	test_replace();
	test_substr();
	test_to_string();
	test_read_number();
	test_base64();
	test_cow();

	test_convert_to_number();

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
