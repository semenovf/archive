/*
 * mbcs_string.cpp
 *
 *  Created on: Nov 15, 2013
 *  Author: wladt
 */

#include <cstring>
#include <iostream>
#include <cstdio>
#include <cerrno>

#include <pfs/test/test.hpp>
#include <pfs/limits.hpp>
#include <pfs/string.hpp>
#include <pfs/byte_string.hpp>

using std::cout;
using std::endl;

void test_to_string ()
{
	ADD_TESTS(19);
	TEST_OK(pfs::to_string(0)       == pfs::string("0"));
	TEST_OK(pfs::to_string(127)     == pfs::string("127"));
	TEST_OK(pfs::to_string(-128)    == pfs::string("-128"));
	TEST_OK(pfs::to_string(255)     == pfs::string("255"));
	TEST_OK(pfs::to_string(32767)   == pfs::string("32767"));
	TEST_OK(pfs::to_string(-32768)  == pfs::string("-32768"));
	TEST_OK(pfs::to_string(65535)   == pfs::string("65535"));
	TEST_OK(pfs::to_string(8388607) == pfs::string("8388607"));
	TEST_OK(pfs::to_string(-8388608) == pfs::string("-8388608"));
	TEST_OK(pfs::to_string(16777215) == pfs::string("16777215"));
	TEST_OK(pfs::to_string(pfs::max_value<int32_t>()) == pfs::string("2147483647"));
	TEST_OK(pfs::to_string(pfs::min_value<int32_t>()) == pfs::string("-2147483648"));
	TEST_OK(pfs::to_string(pfs::max_value<uint32_t>()) == pfs::string("4294967295"));

#ifdef PFS_HAVE_LONG_LONG
	ADD_TESTS(3);
	TEST_OK(pfs::to_string(pfs::max_value<intmax_t>()) == pfs::string("9223372036854775807"));
	TEST_OK(pfs::to_string(pfs::min_value<intmax_t>()) == pfs::string("-9223372036854775808"));
	TEST_OK(pfs::to_string(pfs::max_value<uintmax_t>()) == pfs::string("18446744073709551615"));
#endif

	// Note: single-precision floating-point numbers have a 24-bit mantissa, which is approximately 7.2 decimal digits.
	TEST_OK(pfs::to_string(0.0f, 'g')        == pfs::string("0"));
	TEST_OK(pfs::to_string(0.0f, 'f', 6)     == pfs::string("0.000000"));
	TEST_OK(pfs::to_string(0.0f, 'f', 0)     == pfs::string("0.000000"));
	TEST_OK(pfs::to_string(0.0f, 'f', 1)     == pfs::string("0.0"));
	TEST_OK(pfs::to_string(3.14159f, 'f', 5) == pfs::string("3.14159"));
	TEST_OK(pfs::to_string(1234567.875f, 'f', 3) == pfs::string("1234567.875"));

//	cout << "==" << utfstring::toString(0.0f, 'f', 1).c_str() << endl;
}

void test_starts_ends_with ()
{
	ADD_TESTS(7);

	pfs::string s("[Привет, Мир!]");

	TEST_FAIL(s.starts_with(pfs::string(1, '[')));
	TEST_FAIL(s.ends_with(pfs::string(1, ']')));
	TEST_FAIL(s.starts_with(pfs::string("[")));
	TEST_FAIL(s.ends_with(pfs::string("]")));
	TEST_FAIL(s.starts_with(_u8("[Привет,")));
	TEST_FAIL(s.ends_with(_u8(" Мир!]")));
	TEST_FAIL(s.ends_with(_u8("!]")));
}

void test_insert ()
{
	ADD_TESTS(5);

	pfs::string s("[]");

	s.insert(1, _u8("Привет"));
	TEST_FAIL(s == _u8("[Привет]"));

	s.insert(7, 1, ',');
	TEST_FAIL(s == _u8("[Привет,]"));

	s.insert(8, _u8(" Мир!"));
	TEST_FAIL(s == _u8("[Привет, Мир!]"));

	s.insert(s.begin(), 2, '{');
	TEST_FAIL(s == _u8("{{[Привет, Мир!]"));

	s.insert(s.end(), 2, '}');
	TEST_FAIL(s == _u8("{{[Привет, Мир!]}}"));
}

void test_erase ()
{
	ADD_TESTS(5);

	pfs::string s("{{[Привет, Мир!]}}");

	s.erase(9, 6);

	TEST_FAIL(s == _u8("{{[Привет]}}"));

	pfs::string::iterator it = s.begin();
	std::advance(it, 10);

	s.erase(it, s.end());

	TEST_FAIL(s == _u8("{{[Привет]"));

	it = s.begin();
	std::advance(it, 2);

	s.erase(s.begin(), it);

	TEST_FAIL(s == _u8("[Привет]"));

	s.erase(s.begin(), s.end());

	TEST_FAIL(s.empty());

	s = _u8("{{[Привет, Мир!]}}");
	s.erase(0, pfs::max_value<size_t>());

	TEST_FAIL(s.empty());
}

void test_lexical_cast ()
{
    ADD_TESTS(5);
    
    TEST_OK(pfs::lexical_cast<bool>(pfs::string("true")) == true);
    TEST_OK(pfs::lexical_cast<bool>(pfs::string("yes")) == true);
    TEST_OK(pfs::lexical_cast<bool>(pfs::string("0")) == false);
    TEST_OK(pfs::lexical_cast<bool>(pfs::string("1")) == false);
    TEST_OK(pfs::lexical_cast<bool>(pfs::string("false")) == false);
}

#if __COMMENT__

const uint8_t cyrillic_alphabet_utf8[] = {
	      0xD0, 0x90, 0xD0, 0x91, 0xD0, 0x92, 0xD0, 0x93, 0xD0, 0x94, 0xD0, 0x95, 0xD0, 0x81, 0xD0, 0x96
		, 0xD0, 0x97, 0xD0, 0x98, 0xD0, 0x99, 0xD0, 0x9A, 0xD0, 0x9B, 0xD0, 0x9C, 0xD0, 0x9D, 0xD0, 0x9E
		, 0xD0, 0x9F, 0xD0, 0xA0, 0xD0, 0xA1, 0xD0, 0xA2, 0xD0, 0xA3, 0xD0, 0xA4, 0xD0, 0xA5, 0xD0, 0xA6
		, 0xD0, 0xA7, 0xD0, 0xA8, 0xD0, 0xA9, 0xD0, 0xAA, 0xD0, 0xAB, 0xD0, 0xAC, 0xD0, 0xAD, 0xD0, 0xAE
		, 0xD0, 0xAF, 0xD0, 0xB0, 0xD0, 0xB1, 0xD0, 0xB2, 0xD0, 0xB3, 0xD0, 0xB4, 0xD0, 0xB5, 0xD1, 0x91
		, 0xD0, 0xB6, 0xD0, 0xB7, 0xD0, 0xB8, 0xD0, 0xB9, 0xD0, 0xBA, 0xD0, 0xBB, 0xD0, 0xBC, 0xD0, 0xBD
		, 0xD0, 0xBE, 0xD0, 0xBF, 0xD1, 0x80, 0xD1, 0x81, 0xD1, 0x82, 0xD1, 0x83, 0xD1, 0x84, 0xD1, 0x85
		, 0xD1, 0x86, 0xD1, 0x87, 0xD1, 0x88, 0xD1, 0x89, 0xD1, 0x8A, 0xD1, 0x8B, 0xD1, 0x8C, 0xD1, 0x8D
		, 0xD1, 0x8E, 0xD1, 0x8F} ;

// FIXME Replace strcmp in test code with compare_with and compare_with_utf8
template <typename CodeUnitT>
int compare_with (const CodeUnitT * ptr1, const CodeUnitT * ptr2);

template <typename CodeUnitT>
int compare_with_utf8 (const CodeUnitT * ptr1, const char * ptr2);

template <>
inline int compare_with<char> (const char * ptr1, const char * ptr2)
{
	return strcmp(ptr1, ptr2);
}

template <>
inline int compare_with_utf8<char> (const char * ptr1, const char * ptr2)
{
	return strcmp(ptr1, ptr2);
}


//template <typename CodeUnitT> const CodeUnitT * latin_alphabet_caps ();
//template <typename CodeUnitT> const CodeUnitT * cyrillic_alphabet_caps ();
//template <> const char * latin_alphabet_caps<char> ()
//		{ return "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; }
//template <> const char * cyrillic_alphabet_caps<char> ()
//		{ return "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"; }
//template <> const uint16_t * latin_alphabet_caps<uint16_t> ()
//{
//	static uint16_t r[] = {10};
//	return r;
//}
//template <> const uint16_t * cyrillic_alphabet_caps<uint16_t> ()
//{
//	static uint16_t r[] = {10};
//	return r;
//}

template <typename CodeUnitT>
void test_size_length ()
{
//	typedef pfs::mbcs_string_impl<CodeUnitT> utfstring_impl;
//
//	//-----------------------0---------10--------20----
//	const CodeUnitT * latin1   = latin_alphabet_caps<CodeUnitT>();
//	const CodeUnitT * cyrillic = cyrillic_alphabet_caps<CodeUnitT>();
//
//	TEST_OK(utfstring_impl::size(latin1, 0) == 0);
//	TEST_OK(utfstring_impl::size(latin1, 1) == 1);
//	TEST_OK(utfstring_impl::size(latin1 + strlen(latin1), 0) == 0);
//	TEST_OK(utfstring_impl::size(latin1, 26) == strlen(latin1));

//	TEST_OK(utfstring_impl::size(cyrillic, 1) == 2);
//	TEST_OK(utfstring_impl::size(cyrillic, 33) == 66);

//	TEST_OK(utfstring_impl::length(latin1, latin1 + strlen(latin1)) == 26);
//	TEST_OK(utfstring_impl::length(cyrillic, cyrillic + strlen(cyrillic)) == 33);
}

template <typename CodeUnitT>
void test_constructors ()
{
	typedef pfs::mbcs_string<CodeUnitT> utfstring;

	utfstring nil;
//	pfs::bytearray serial(10, 'W');

	TEST_OK(nil.isNull());
	TEST_OK(nil.isEmpty());
//	TEST_OK(serial.size() == 10);
//	TEST_OK(strcmp("WWWWWWWWWW", serial.constData()) == 0);

//	const char * loremipsum  = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit";
//
//	TEST_OK(strcmp(loremipsum, pfs::bytearray(loremipsum).constData()) == 0);
//	TEST_OK(strcmp(loremipsum, pfs::bytearray(loremipsum, strlen(loremipsum)).constData()) == 0);
//	TEST_OK(strcmp(loremipsum, pfs::bytearray(std::string(loremipsum)).constData()) == 0);
}

template <typename CodeUnitT>
void test_insert_latin1 ()
{
	typedef pfs::mbcs_string<CodeUnitT> utfstring;

	utfstring s;
	utfstring s1("Hello");
	utfstring s2("+++World!+++");
	utfstring s3(", ");

	s.insert(0, s1, 0, s1.length());
	TEST_OK(s.size() == s1.size());
	TEST_OK(s.length() == s1.length());
//	TEST_OK(strcmp(s.c_str(), s1.c_str()) == 0);
//	TEST_OK(strcmp(s.c_str(), "Hello") == 0);
//
//	s.insert(5, s2, 3, 6);
//	TEST_OK(strcmp(s.c_str(), "HelloWorld!") == 0);
//
//	s.insert(5, s3, 0, s3.length());
//	TEST_OK(strcmp(s.c_str(), "Hello, World!") == 0);

//	std::cout << "'" << s.c_str() << '"' << std::endl;
}

template <typename CodeUnitT>
void test_insert_cyrillic ()
{
	typedef pfs::mbcs_string<CodeUnitT> utfstring;

	{
		utfstring s;
		utfstring s1(utfstring::fromUtf8("Привет"));
		utfstring s2(utfstring::fromUtf8("+++Мир!+++"));
		utfstring s3(", ");

		s.insert(s.cbegin(), s1.cbegin(), s1.cend());
		TEST_OK(s.size() == s1.size());
		TEST_OK(s.length() == s1.length());
//		TEST_OK(strcmp(s.c_str(), s1.c_str()) == 0);
//		TEST_OK(strcmp(s.c_str(), "Привет") == 0);

		typename utfstring::const_iterator first = s2.cbegin() + 3;
		typename utfstring::const_iterator last = s2.cbegin() + 7;

		s.insert(s.cbegin() + s.length(), first, last);
//		TEST_OK(strcmp(s.c_str(), "ПриветМир!") == 0);

		s.insert(s.cbegin() + s1.length(), s3.cbegin(), s3.cend());
//		TEST_OK(strcmp(s.c_str(), "Привет, Мир!") == 0);
	}

	{
		utfstring s;
		utfstring s1(utfstring::fromUtf8("Привет"));
		utfstring s2(utfstring::fromUtf8("+++Мир!+++"));
		utfstring s3(", ");

		s.insert(0, s1, 0, s1.length());
		TEST_OK(s.size() == s1.size());
		TEST_OK(s.length() == s1.length());
//		TEST_OK(strcmp(s.c_str(), s1.c_str()) == 0);
//		TEST_OK(strcmp(s.c_str(), "Привет") == 0);

		s.insert(6, s2, 3, 4);
//		TEST_OK(strcmp(s.c_str(), "ПриветМир!") == 0);

		s.insert(6, s3, 0, s3.length());
//		TEST_OK(strcmp(s.c_str(), "Привет, Мир!") == 0);
	}
}

template <typename CodeUnitT>
void test_append_cyrillic ()
{
	typedef pfs::mbcs_string<CodeUnitT> utfstring;

	{
		const char * sample = "Привет, Мир!";
		utfstring s;
		utfstring s1(utfstring::fromUtf8("Привет"));
		utfstring s2(", ");
		utfstring s3(utfstring::fromUtf8("Мир!"));

		s.append(s1).append(s2).append(s3);

		TEST_OK(s.size() == strlen(sample));
		TEST_OK(s.length() == s1.length() + s3.length() + 2);
//		TEST_OK(strcmp(s.c_str(), sample) == 0);
	}

	{
		utfstring s;
		s += utfstring::fromUtf8("Привет");
		s += pfs::ucchar(',');
		s += ' ';
		s += "World!";

//		TEST_OK(strcmp(s.c_str(), "Привет, World!") == 0);
		TEST_OK(s.size() == 20);
		TEST_OK(s.length() == 14);
	}
}

template <typename CodeUnitT>
void test_ptr ()
{
	typedef pfs::mbcs_string<CodeUnitT> utfstring;

	utfstring s(utfstring::fromUtf8("GIJKLЁЖЗИЙЭЮЯgijklёжзийэюя"));
	typename utfstring::pointer ptr(& s, 0);
	typename utfstring::pointer begin(& s, 0);
	typename utfstring::pointer end(& s, s.size());

	std::cout << "Iterate through string \"" << s << '"' << std::endl;

	TEST_OK(ptr == begin);

	TEST_OK(*ptr++ == pfs::ucchar('G'));
	TEST_OK(*ptr++ == pfs::ucchar('I'));
	TEST_OK(*ptr++ == pfs::ucchar('J'));
	TEST_OK(*ptr++ == pfs::ucchar('K'));
	TEST_OK(*ptr++ == pfs::ucchar('L'));
	TEST_OK2(*ptr++ == pfs::ucchar(0x401), "*ptr++ == 'Ё' (0x401, Cyrillic Capital Letter Io)");
	TEST_OK2(*ptr++ == pfs::ucchar(0x416), "*ptr++ == 'Ж' (0x416, Cyrillic Capital Letter Zhe)");
	TEST_OK2(*ptr++ == pfs::ucchar(0x417), "*ptr++ == 'З' (0x417, Cyrillic Capital Letter Ze)");
	TEST_OK2(*ptr++ == pfs::ucchar(0x418), "*ptr++ == 'И' (0x418, Cyrillic Capital Letter I)");
	TEST_OK2(*ptr++ == pfs::ucchar(0x419), "*ptr++ == 'Й  (0x419, Cyrillic Capital Letter Short I)");
	TEST_OK2(*ptr++ == pfs::ucchar(0x42D), "*ptr++ == 'Э' (0x42D, Cyrillic Capital Letter E)");
	TEST_OK2(*ptr++ == pfs::ucchar(0x42E), "*ptr++ == 'Ю' (0x42E, Cyrillic Capital Letter Yu)");
	TEST_OK2(*ptr++ == pfs::ucchar(0x42F), "*ptr++ == 'Я' (0x42F, Cyrillic Capital Letter Ya)");
	TEST_OK(*ptr++ == pfs::ucchar('g'));
	TEST_OK(*ptr++ == pfs::ucchar('i'));
	TEST_OK(*ptr++ == pfs::ucchar('j'));
	TEST_OK(*ptr++ == pfs::ucchar('k'));
	TEST_OK(*ptr++ == pfs::ucchar('l'));
	TEST_OK2(*ptr++ == pfs::ucchar(0x451), "*ptr++ == 'ё' (0x451, Cyrillic Small Letter Io)");
	TEST_OK2(*ptr++ == pfs::ucchar(0x436), "*ptr++ == 'ж' (0x436, Cyrillic Small Letter Zhe)");
	TEST_OK2(*ptr++ == pfs::ucchar(0x437), "*ptr++ == 'з' (0x437, Cyrillic Small Letter Ze)");
	TEST_OK2(*ptr++ == pfs::ucchar(0x438), "*ptr++ == 'и' (0x438, Cyrillic Small Letter I)");
	TEST_OK2(*ptr++ == pfs::ucchar(0x439), "*ptr++ == 'й' (0x439, Cyrillic Small Letter Short I)");
	TEST_OK2(*ptr++ == pfs::ucchar(0x44D), "*ptr++ == 'э' (0x44D, Cyrillic Small Letter E)");
	TEST_OK2(*ptr++ == pfs::ucchar(0x44E), "*ptr++ == 'ю' (0x44E, Cyrillic Small Letter Yu)");
	TEST_OK2(*ptr++ == pfs::ucchar(0x44F), "*ptr++ == 'я' (0x44F, Cyrillic Small Letter Ya)");

	TEST_OK(ptr == end);

	ptr = begin;
	TEST_OK(ptr == begin);

	TEST_OK(ptr[0] == pfs::ucchar('G'));
	TEST_OK(ptr[1] == pfs::ucchar('I'));
	TEST_OK(ptr[2] == pfs::ucchar('J'));
	TEST_OK(ptr[3] == pfs::ucchar('K'));
	TEST_OK(ptr[4] == pfs::ucchar('L'));
	TEST_OK2(ptr[5] == pfs::ucchar(0x401),  "ptr[5] == 'Ё' (0x401, Cyrillic Capital Letter Io)");
	TEST_OK2(ptr[6] == pfs::ucchar(0x416),  "ptr[6] == 'Ж' (0x416, Cyrillic Capital Letter Zhe)");
	TEST_OK2(ptr[7] == pfs::ucchar(0x417),  "ptr[7] == 'З' (0x417, Cyrillic Capital Letter Ze)");
	TEST_OK2(ptr[8] == pfs::ucchar(0x418),  "ptr[8] == 'И' (0x418, Cyrillic Capital Letter I)");
	TEST_OK2(ptr[9] == pfs::ucchar(0x419),  "ptr[9] == 'Й  (0x419, Cyrillic Capital Letter Short I)");
	TEST_OK2(ptr[10] == pfs::ucchar(0x42D), "ptr[10] == 'Э' (0x42D, Cyrillic Capital Letter E)");
	TEST_OK2(ptr[11] == pfs::ucchar(0x42E), "ptr[11] == 'Ю' (0x42E, Cyrillic Capital Letter Yu)");
	TEST_OK2(ptr[12] == pfs::ucchar(0x42F), "ptr[12] == 'Я' (0x42F, Cyrillic Capital Letter Ya)");
	TEST_OK(ptr[13] == pfs::ucchar('g'));
	TEST_OK(ptr[14] == pfs::ucchar('i'));
	TEST_OK(ptr[15] == pfs::ucchar('j'));
	TEST_OK(ptr[16] == pfs::ucchar('k'));
	TEST_OK(ptr[17] == pfs::ucchar('l'));
	TEST_OK2(ptr[18] == pfs::ucchar(0x451), "ptr[18] == 'ё' (0x451, Cyrillic Small Letter Io)");
	TEST_OK2(ptr[19] == pfs::ucchar(0x436), "ptr[19] == 'ж' (0x436, Cyrillic Small Letter Zhe)");
	TEST_OK2(ptr[20] == pfs::ucchar(0x437), "ptr[20] == 'з' (0x437, Cyrillic Small Letter Ze)");
	TEST_OK2(ptr[21] == pfs::ucchar(0x438), "ptr[21] == 'и' (0x438, Cyrillic Small Letter I)");
	TEST_OK2(ptr[22] == pfs::ucchar(0x439), "ptr[22] == 'й' (0x439, Cyrillic Small Letter Short I)");
	TEST_OK2(ptr[23] == pfs::ucchar(0x44D), "ptr[23] == 'э' (0x44D, Cyrillic Small Letter E)");
	TEST_OK2(ptr[24] == pfs::ucchar(0x44E), "ptr[24] == 'ю' (0x44E, Cyrillic Small Letter Yu)");
	TEST_OK2(ptr[25] == pfs::ucchar(0x44F), "ptr[25] == 'я' (0x44F, Cyrillic Small Letter Ya)");

	ptr = begin;
	TEST_OK(ptr == begin);
	TEST_OK(ptr >= begin);
	TEST_OK(ptr <= begin);

	ptr = end;
	TEST_OK(ptr == end);
	TEST_OK(ptr >= end);
	TEST_OK(ptr <= end);
}


template <typename CodeUnitT>
void test_at ()
{
	typedef pfs::mbcs_string<CodeUnitT> utfstring;

	utfstring s(utfstring::fromUtf8("GIJKLЁЖЗИЙЭЮЯgijklёжзийэюя"));
	bool ok = true;
	uint32_t chars[] = {
		  0x047, 0x049, 0x04A, 0x04B, 0x04C
		, 0x401, 0x416, 0x417, 0x418, 0x419, 0x42D, 0x42E, 0x42F
		, 0x067, 0x069, 0x06A, 0x06B, 0x06C
		, 0x451, 0x436, 0x437, 0x438, 0x439, 0x44D, 0x44E, 0x44F
	};

	TEST_OK(sizeof(chars)/sizeof(chars[0]) == s.length());
	for (size_t i = 0; i < s.length(); ++i) {
		if (s.valueAt(i) != pfs::ucchar(chars[i])) {
			ok = false;
			break;
		}
	}

	TEST_OK2(ok, "utfstring(utfstring::fromUtf8(\"GIJKLЁЖЗИЙЭЮЯgijklёжзийэюя\")) equals to array of unicode chars");
}

template <typename CodeUnitT>
void test_erase ()
{
	typedef pfs::mbcs_string<CodeUnitT> utfstring;

	{
		utfstring sample(utfstring::fromUtf8("GIJKLЁЖЗИЙЭЮЯgijklёжзийэюя"));
		utfstring s;

		s = sample;
//		TEST_OK(strcmp(s.erase(0, 0).c_str(), "GIJKLЁЖЗИЙЭЮЯgijklёжзийэюя") == 0);

		s = sample;
//		TEST_OK(strcmp(s.erase(0, 1).c_str(), "IJKLЁЖЗИЙЭЮЯgijklёжзийэюя") == 0);

		s = sample;
//		TEST_OK(strcmp(s.erase(0, 5).c_str(), "ЁЖЗИЙЭЮЯgijklёжзийэюя") == 0);

		s = sample;
//		TEST_OK(strcmp(s.erase(13, 5).c_str(), "GIJKLЁЖЗИЙЭЮЯёжзийэюя") == 0);

		s = sample;
//		TEST_OK(strcmp(s.erase(100, 5).c_str(), "GIJKLЁЖЗИЙЭЮЯgijklёжзийэюя") == 0);

		s = sample;
//		TEST_OK(strcmp(s.erase(13, 100).c_str(), "GIJKLЁЖЗИЙЭЮЯ") == 0);

		s = sample;
//		TEST_OK(strcmp(s.erase(0, 100).c_str(), "") == 0);
	}

	{
		utfstring sample(utfstring::fromUtf8("GIJKLЁЖЗИЙЭЮЯgijklёжзийэюя"));
		utfstring s;

		s = sample;
		s.erase(s.cbegin(), s.cbegin());
//		TEST_OK(strcmp(s.c_str(), "GIJKLЁЖЗИЙЭЮЯgijklёжзийэюя") == 0);

		s = sample;
		s.erase(s.cbegin(), s.cbegin() + 1);
//		TEST_OK(strcmp(s.c_str(), "IJKLЁЖЗИЙЭЮЯgijklёжзийэюя") == 0);

		s = sample;
		s.erase(s.cbegin(), s.cbegin() + 5);
//		TEST_OK(strcmp(s.c_str(), "ЁЖЗИЙЭЮЯgijklёжзийэюя") == 0);

		s = sample;
		s.erase(s.cbegin() + 13, s.cbegin() + 18);
//		TEST_OK(strcmp(s.c_str(), "GIJKLЁЖЗИЙЭЮЯёжзийэюя") == 0);

		s = sample;
		s.erase(s.cbegin(), s.cend());
//		TEST_OK(strcmp(s.c_str(), "") == 0);
	}
}

template <typename CodeUnitT>
void test_substr ()
{
	typedef pfs::mbcs_string<CodeUnitT> utfstring;

	utfstring s(utfstring::fromUtf8("GIJKLЁЖЗИЙЭЮЯgijklёжзийэюя"));
	TEST_OK(s.substr(0,0).isEmpty());
	TEST_OK(s.substr(s.length(),100).isEmpty());
//	TEST_OK(strcmp(s.substr(0,5).c_str(), "GIJKL") == 0);
//	TEST_OK(strcmp(s.substr(5,8).c_str(), "ЁЖЗИЙЭЮЯ") == 0);
//	TEST_OK(strcmp(s.substr(5,s.length() + 1).c_str(), "ЁЖЗИЙЭЮЯgijklёжзийэюя") == 0);

//	TEST_OK(strcmp(s.substr(s.cbegin() + 5, 8).c_str(), "ЁЖЗИЙЭЮЯ") == 0);
//	TEST_OK(strcmp(s.substr(s.cbegin() + 13, 100).c_str(), "gijklёжзийэюя") == 0);

//	TEST_OK(strcmp(s.substr(s.cbegin() + 5, s.cbegin() + 13).c_str(), "ЁЖЗИЙЭЮЯ") == 0);
//	TEST_OK(strcmp(s.substr(s.cbegin() + 13, s.cend()).c_str(), "gijklёжзийэюя") == 0);
}

template <typename CodeUnitT>
void test_pop_back ()
{
	typedef pfs::mbcs_string<CodeUnitT> utfstring;

	utfstring s(utfstring::fromUtf8("GIJЁЖЗgijёжз"));
	s.pop_back();
//	TEST_OK(strcmp(s.c_str(), "GIJЁЖЗgijёж") == 0);

	s.pop_back();
//	TEST_OK(strcmp(s.c_str(), "GIJЁЖЗgijё") == 0);
	//std::cout << '"' << s.c_str() << '"' << std::endl;

	s.pop_back();
//	TEST_OK(strcmp(s.c_str(), "GIJЁЖЗgij") == 0);

	s.pop_back();
//	TEST_OK(strcmp(s.c_str(), "GIJЁЖЗgi") == 0);

	s.pop_back();
//	TEST_OK(strcmp(s.c_str(), "GIJЁЖЗg") == 0);

	s.pop_back();
//	TEST_OK(strcmp(s.c_str(), "GIJЁЖЗ") == 0);

	s.pop_back();
//	TEST_OK(strcmp(s.c_str(), "GIJЁЖ") == 0);

	s.pop_back();
//	TEST_OK(strcmp(s.c_str(), "GIJЁ") == 0);

	s.pop_back();
//	TEST_OK(strcmp(s.c_str(), "GIJ") == 0);

	s.pop_back();
//	TEST_OK(strcmp(s.c_str(), "GI") == 0);

	s.pop_back();
//	TEST_OK(strcmp(s.c_str(), "G") == 0);

	s.pop_back();
//	TEST_OK(strcmp(s.c_str(), "") == 0);

	TEST_OK(s.isEmpty());
}

template <typename CodeUnitT>
void test_compare ()
{
	typedef pfs::mbcs_string<CodeUnitT> utfstring;

	const char * cyr = "АБВГДЕЁЖЫИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
	utfstring s(utfstring::fromUtf8("АБВГДЕЁЖЫИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"));
	utfstring s1(utfstring::fromUtf8("АБВГДЕЁЖЫИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"));

	TEST_OK(utfstring().compare(utfstring()) == 0);
	TEST_OK(s.compare(cyr) == 0);
	TEST_OK(s.compare(s1) == 0);
}


template <typename CodeUnitT>
void test_replace ()
{
	typedef pfs::mbcs_string<CodeUnitT> utfstring;

	{
		utfstring s(utfstring::fromUtf8("Привет, Мир!"));

		s.replace(0, 6, utfstring::fromUtf8("Hello, World!"), 0, 5);
		TEST_OK(s.size() == 14);
		TEST_OK(s.length() == 11);
//		TEST_OK(compare_with_utf8(s.c_str(), "Hello, Мир!") == 0);

		s.replace(7, 3, utfstring::fromUtf8("Hello, World!"), 7, 5);
		TEST_OK(s.size() == 13);
		TEST_OK(s.length() == 13);
//		TEST_OK(compare_with_utf8(s.c_str(), "Hello, World!") == 0);

		// replace full string with empty string - get empty string
		s.replace(0, s.length(), utfstring(), 0, 0);
//		TEST_OK(compare_with_utf8(s.c_str(), "") == 0);
	}

	{
		utfstring s(utfstring::fromUtf8("Привет, Мир!"));
		utfstring s1(utfstring::fromUtf8("Hello, World!"));

		s.replace(s.cbegin(), s.cbegin() + 6, s1.cbegin(), s1.cbegin() + 5);
		TEST_OK(s.size() == 14);
		TEST_OK(s.length() == 11);
//		TEST_OK(compare_with_utf8(s.c_str(), "Hello, Мир!") == 0);

		s.replace(s.cbegin() + 7, s.cbegin() + 10, s1.cbegin() + 7, s1.cbegin() + 12);
		TEST_OK(s.size() == 13);
		TEST_OK(s.length() == 13);
//		TEST_OK(compare_with_utf8(s.c_str(), "Hello, World!") == 0);

		// replace full string with empty string - get empty string
		utfstring nil;
		s.replace(s.cbegin(), s.cend(), nil.cbegin(), nil.cend());
//		TEST_OK(compare_with_utf8(s.c_str(), "") == 0);
	}

	{
		utfstring s(utfstring::fromUtf8("This/is/a/test/string"));
		utfstring s1(utfstring::fromUtf8("This--is--a--test--string"));

		s.replace(utfstring::fromUtf8("/"), utfstring::fromUtf8("--"));
		TEST_OK2(s == s1, "Replace character sequence by another");
	}
}

template <typename CodeUnitT>
void test_find ()
{
	typedef pfs::mbcs_string<CodeUnitT> utfstring;
	utfstring s(utfstring::fromUtf8("Привет, Мир!"));

	TEST_OK(s.find(utfstring::fromUtf8("Привет")) == s.begin());
	TEST_OK(s.find(utfstring::fromUtf8("Мир")) != s.begin());
	TEST_OK(s.find(utfstring::fromUtf8("Мир")) == s.begin() + 8);
	TEST_OK(s.find(utfstring::fromUtf8("Hello")) == s.end());
}

template <typename CodeUnitT>
void test_convert_to_number ()
{
    ADD_TESTS(39);
	typedef pfs::mbcs_string<CodeUnitT> utfstring;

	bool ok;
//	TEST_OK(utfstring("0").toUInt(& ok) == 0 && ok);
//	TEST_OK(utfstring("+0").toUInt(& ok) == 0 && ok);
//	TEST_OK(utfstring("-0").toUInt(& ok) == 0 && ok);
//	TEST_OK(utfstring("1").toUInt(& ok) == 1 && ok);
//	TEST_OK(utfstring("+1").toUInt(& ok) == 1 && ok);
//	TEST_OK(utfstring("-1").toUInt(& ok) == 0 && !ok);
//	TEST_OK(utfstring("123").toUInt(& ok) == 123 && ok);
//

	//
	// Check signed and unsigned char specific methods for boundary values
	//
//    TEST_OK(utfstring("255").toUnsignedChar(& ok)  == pfs::max_value<unsigned char>() && ok);
//    TEST_OK(utfstring("127").toUnsignedChar(& ok)  == static_cast<unsigned char>(pfs::max_value<signed char>()) && ok);
//    TEST_OK(utfstring("-128").toUnsignedChar(& ok) == static_cast<unsigned char>(pfs::min_value<signed char>()) && ok);
//    TEST_OK(utfstring("127").toSignedChar(& ok)   == pfs::max_value<signed char>() && ok);
    TEST_OK(utfstring("-128").toSignedChar(& ok)  == pfs::min_value<signed char>() && ok);

    TEST_OK(utfstring("256").toUnsignedChar(& ok)  == 0 && !ok && errno == ERANGE); // + 1 more overflow
    TEST_OK(utfstring("257").toUnsignedChar(& ok)  == 0 && !ok && errno == ERANGE); // + 2 more overflow
    TEST_OK(utfstring("258").toUnsignedChar(& ok)  == 0 && !ok && errno == ERANGE); // + 3 more overflow

    TEST_OK(utfstring("-129").toUnsignedChar(& ok)  == static_cast<unsigned char>(127) && ok); // - 1 more underflow
    TEST_OK(utfstring("-130").toUnsignedChar(& ok)  == static_cast<unsigned char>(126) && ok); // - 2 more underflow
    TEST_OK(utfstring("-131").toUnsignedChar(& ok)  == static_cast<unsigned char>(125) && ok); // - 3 more underflow

    TEST_OK(utfstring("128").toSignedChar(& ok)   == 0 && !ok && errno == ERANGE); // + 1 more overflow
    TEST_OK(utfstring("129").toSignedChar(& ok)   == 0 && !ok && errno == ERANGE); // + 2 more overflow
    TEST_OK(utfstring("130").toSignedChar(& ok)   == 0 && !ok && errno == ERANGE); // + 3 more overflow

    TEST_OK(utfstring("-129").toSignedChar(& ok)  == 0 && !ok && errno == ERANGE); // - 1 more underflow
    TEST_OK(utfstring("-130").toSignedChar(& ok)  == 0 && !ok && errno == ERANGE); // - 2 more underflow
    TEST_OK(utfstring("-131").toSignedChar(& ok)  == 0 && !ok && errno == ERANGE); // - 3 more underflow


//    if (sizeof(short) == 4) {
//        TEST_OK(utfstring("4294967295").toUnsignedShort(& ok)  == PFS_USHORT_MAX && ok);
//        TEST_OK(utfstring("2147483647").toUnsignedShort(& ok)  == PFS_SHORT_MAX && ok);
//        TEST_OK(utfstring("-2147483648").toUnsignedShort(& ok) == static_cast<unsigned short>(PFS_SHORT_MIN) && ok);
//        TEST_OK(utfstring("2147483647").toShort(& ok)   == PFS_SHORT_MAX && ok);
//        TEST_OK(utfstring("-2147483648").toShort(& ok)  == PFS_SHORT_MIN && ok);
//    } else {
//        TEST_OK(utfstring("65535").toUShort(& ok)  == PFS_USHORT_MAX && ok);
//        TEST_OK(utfstring("32767").toUShort(& ok)  == PFS_SHORT_MAX && ok);
//        TEST_OK(utfstring("-32768").toUShort(& ok) == static_cast<unsigned short>(PFS_SHORT_MIN) && ok);
//        TEST_OK(utfstring("32767").toShort(& ok)   == PFS_SHORT_MAX && ok);
//        TEST_OK(utfstring("-32768").toShort(& ok)  == PFS_SHORT_MIN && ok);
//        cout << utfstring("-32768").toShort(& ok) << endl;
//        TEST_OK(utfstring("65536").toUShort(& ok)  == 0 && !ok && errno == ERANGE);
//        TEST_OK(utfstring("32768").toShort(& ok)   == 0 && !ok && errno == ERANGE);
//        TEST_OK(utfstring("-32769").toShort(& ok)  == 0 && !ok && errno == ERANGE);
//    }

//    if (sizeof(int) == 8) {
//        TEST_OK(utfstring("18446744073709551615").toUInt(& ok) == PFS_UINT_MAX && ok);
//        TEST_OK(utfstring("9223372036854775807").toInt(& ok)   == PFS_INT_MAX && ok);
//        TEST_OK(utfstring("-9223372036854775808").toInt(& ok)  == PFS_INT_MIN && ok);
//    } else {
//        TEST_OK(utfstring("4294967295").toUInt(& ok) == PFS_UINT_MAX && ok);
//        TEST_OK(utfstring("2147483647").toInt(& ok)  == PFS_INT_MAX && ok);
//        TEST_OK(utfstring("-2147483648").toInt(& ok) == PFS_INT_MIN && ok);
//    }
//
//    if (sizeof(long) == 8) {
//        TEST_OK(utfstring("18446744073709551615").toULong(& ok) == PFS_ULONG_MAX && ok);
//        TEST_OK(utfstring("9223372036854775807").toLong(& ok)   == PFS_LONG_MAX && ok);
//        TEST_OK(utfstring("-9223372036854775808").toLong(& ok)  == PFS_LONG_MIN && ok);
//    } else {
//        TEST_OK(utfstring("4294967295").toULong(& ok) == PFS_ULONG_MAX && ok);
//        TEST_OK(utfstring("2147483647").toLong(& ok)  == PFS_LONG_MAX && ok);
//        TEST_OK(utfstring("-2147483648").toLong(& ok) == PFS_LONG_MIN && ok);
//    }
//
//
//#ifdef PFS_HAVE_LONGLONG
//    ADD_TESTS(3);
//	if (sizeof(long long) == 8) {
//	    TEST_OK(utfstring("18446744073709551615").toULongLong(& ok) == PFS_ULLONG_MAX && ok);
//	    TEST_OK(utfstring("9223372036854775807").toLongLong(& ok)   == PFS_LLONG_MAX && ok);
//	    TEST_OK(utfstring("-9223372036854775808").toLongLong(& ok)  == PFS_LLONG_MIN && ok);
//	} else {
//	    TEST_OK(utfstring("4294967295").toULongLong(& ok) == PFS_ULLONG_MAX && ok);
//	    TEST_OK(utfstring("2147483647").toLongLong(& ok) == PFS_LLONG_MAX && ok);
//	    TEST_OK(utfstring("-2147483648").toLongLong(& ok) == PFS_LLONG_MIN && ok);
//	}
//#endif
//
//    if (sizeof(intmax_t) == 8) {
//        TEST_OK(utfstring("18446744073709551615").toUIntegral(& ok) == PFS_UINTEGRAL_MAX && ok);
//        TEST_OK(utfstring("9223372036854775807").toIntegral(& ok) == PFS_INTEGRAL_MAX && ok);
//        TEST_OK(utfstring("-9223372036854775808").toIntegral(& ok) == PFS_INTEGRAL_MIN && ok);
//    } else {
//        TEST_OK(utfstring("4294967295").toUIntegral(& ok) == PFS_UINTEGRAL_MAX && ok);
//        TEST_OK(utfstring("2147483647").toIntegral(& ok) == PFS_INTEGRAL_MAX && ok);
//        TEST_OK(utfstring("-2147483648").toIntegral(& ok) == PFS_INTEGRAL_MIN && ok);
//    }
//
//
//	TEST_OK(utfstring("0").toInt(& ok) == 0 && ok);
////	TEST_OK(compare_signed("+0", 0));
////	TEST_OK(compare_signed("-0", 0));
////
////	TEST_OK(compare_signed("1", 1));
////	TEST_OK(compare_signed("+1", 1));
////	TEST_OK(compare_signed("-1", -1));
////
////	TEST_OK(compare_signed("123", 123));
////	TEST_OK(compare_signed("+123", 123));
////	TEST_OK(compare_signed("-123", -123));
//
//	TEST_OK(utfstring("9.").toReal(& ok) == 9.0 && ok);
//	TEST_OK(utfstring("9").toReal(& ok) == 9.0 && ok);
//	TEST_OK(utfstring("9.0").toReal(& ok) == 9.0 && ok);
//	TEST_OK(utfstring("9.0000").toReal(& ok) == 9.0 && ok);
//	TEST_OK(utfstring("9.00001").toReal(& ok) == PFS_REAL_LITERAL(9.00001) && ok);
//	TEST_OK(utfstring("009").toReal(& ok) == 9.0 && ok);
//	TEST_OK(utfstring("0.09e02").toReal(& ok) == 9.0 && ok);
//	TEST_OK(utfstring("0.9999999999999999999999999999999999").toReal(& ok) == PFS_REAL_LITERAL(0.9999999999999999999999999999999999) && ok);
//	TEST_OK(utfstring("2.22e-308").toReal(& ok) == PFS_REAL_LITERAL(2.22e-308) && ok);
//	TEST_OK(utfstring("1.34").toReal(& ok) == PFS_REAL_LITERAL(1.34) && ok);
//	TEST_OK(utfstring("12.34").toReal(& ok) == PFS_REAL_LITERAL(12.34) && ok);
//	TEST_OK(utfstring("123.456").toReal(& ok) == PFS_REAL_LITERAL(123.456) && ok);
//
//	TEST_OK(utfstring("2.22507385850720138309e-308").toReal(& ok) == PFS_REAL_LITERAL(2.22507385850720138309e-308) && ok);
//	TEST_OK(utfstring("1.79769313486231570815e+308").toReal(& ok) == PFS_REAL_LITERAL(1.79769313486231570815e+308) && ok);
//
//#if PFS_REAL_MAX_10_EXP == 4932
//	ADD_TESTS(3);
//	TEST_OK(utfstring("3.36210314311209350626e-4932").toReal(& ok) == PFS_REAL_LITERAL(3.36210314311209350626e-4932) && ok);
//	TEST_OK(utfstring("1.18973149535723176502e+4932").toReal(& ok) == PFS_REAL_LITERAL(1.18973149535723176502e+4932) && ok);
//	TEST_OK(utfstring("1.18973149535723176502126385303e+4932").toReal(& ok) == PFS_REAL_LITERAL(1.18973149535723176502126385303e+4932) && ok);
//#endif
//
//	TEST_OK(utfstring("18973149535723176502126385303").toReal(& ok) == PFS_REAL_LITERAL(18973149535723176502126385303.0) && ok);
//	TEST_OK(utfstring("12345678901234567890123456789").toReal(& ok) == PFS_REAL_LITERAL(12345678901234567890123456789.0) && ok);
}


template <typename CodeUnitT>
void test_split ()
{
//	typedef pfs::mbcs_string<CodeUnitT> utfstring;

//	utfstring nil; // nil
//	utfstring s1("This/is/a/test/string");
//	utfstring s2("This//is//a//test//string/");
//	utfstring s3("This is a test string");
//	utfstring sep1(1, '/');
//	bool keepEmpty = true;
//	bool noKeepEmpty = false;
//	pfs::ucchar noQuoteChar = pfs::ucchar::Null;

//	pfs::stringlist slist = nil.split(sep1, keepEmpty, noQuoteChar);
//	TEST_OK(slist.size() == 1);
//	TEST_OK(slist.valueAt(0).isNull()); // Inspect if valueAt(0) must be null or not
//	TEST_OK(slist.valueAt(0).isEmpty());
//
//	slist = s1.split(sep1, keepEmpty, noQuoteChar);
//	TEST_OK(slist.size() == 5);
//	TEST_OK(slist.valueAt(0) == "This");
//	TEST_OK(slist.valueAt(1) == "is");
//	TEST_OK(slist.valueAt(2) == "a");
//	TEST_OK(slist.valueAt(3) == "test");
//	TEST_OK(slist.valueAt(4) == "string");
//
//	slist = s2.split(sep1, keepEmpty, noQuoteChar);
//	TEST_OK(slist.size() == 10);
//	TEST_OK(slist.valueAt(0) == "This");
//	TEST_OK(slist.valueAt(1).isEmpty());
//	TEST_OK(slist.valueAt(2) == "is");
//	TEST_OK(slist.valueAt(3).isEmpty());
//	TEST_OK(slist.valueAt(4) == "a");
//	TEST_OK(slist.valueAt(5).isEmpty());
//	TEST_OK(slist.valueAt(6) == "test");
//	TEST_OK(slist.valueAt(7).isEmpty());
//	TEST_OK(slist.valueAt(8) == "string");
//	TEST_OK(slist.valueAt(9).isEmpty());
//
//	slist = s2.split(sep1, noKeepEmpty, noQuoteChar);
//	TEST_OK(slist.size() == 5);
//	TEST_OK(slist.valueAt(0) == "This");
//	TEST_OK(slist.valueAt(1) == "is");
//	TEST_OK(slist.valueAt(2) == "a");
//	TEST_OK(slist.valueAt(3) == "test");
//	TEST_OK(slist.valueAt(4) == "string");
//
//	slist = s3.split(sep1, keepEmpty, noQuoteChar);
//	TEST_OK(slist.size() == 1);
//	TEST_OK(slist.valueAt(0) == s3);
}

#endif

void test_pack_unpack ()
{
    ADD_TESTS(9);
    
    pfs::string orig("Привет, Мир!");
    pfs::string orig1("Привет, 1!");
    pfs::string orig2("Привет, 2!");
    pfs::string orig3("Привет, 3!");
    
    pfs::byte_string packed1 = pack(orig, pfs::endian::little_endian);
    pfs::byte_string packed2 = pack(orig, pfs::endian::big_endian);
    pfs::byte_string packed3 = pack(orig, pfs::endian::native_order());
    pfs::byte_string packed4 = pack(orig, pfs::endian::network_order());
    
    pfs::unpack_context ctx1(packed1.cbegin(), packed1.cend(), pfs::endian::little_endian);
    pfs::unpack_context ctx2(packed2.cbegin(), packed2.cend(), pfs::endian::big_endian);
    pfs::unpack_context ctx3(packed3.cbegin(), packed3.cend(), pfs::endian::native_order());
    pfs::unpack_context ctx4(packed4.cbegin(), packed4.cend(), pfs::endian::network_order());
    
    TEST_OK(pfs::unpack<pfs::string>(ctx1) == orig);
    TEST_OK(pfs::unpack<pfs::string>(ctx2) == orig);
    TEST_OK(pfs::unpack<pfs::string>(ctx3) == orig);
    TEST_OK(pfs::unpack<pfs::string>(ctx4) == orig);
    
    pfs::pack_context packed5(pfs::endian::network_order());
    pack(packed5, orig);
    pack(packed5, orig1);
    pack(packed5, orig2);
    pack(packed5, orig3);
    
    TEST_FAIL(packed5.buffer.size() == 4 * sizeof(pfs::string::size_type)
            + orig.size() 
            + orig1.size()
            + orig2.size()
            + orig3.size());
    
    pfs::unpack_context ctx5(packed5.buffer.begin(), packed5.buffer.end(), pfs::endian::network_order());
    
//    std::cout << pfs::unpack<pfs::string>(ctx5) << std::endl;
    
    TEST_OK(pfs::unpack<pfs::string>(ctx5) == orig);
    TEST_OK(pfs::unpack<pfs::string>(ctx5) == orig1);
    TEST_OK(pfs::unpack<pfs::string>(ctx5) == orig2);
    TEST_OK(pfs::unpack<pfs::string>(ctx5) == orig3);
}

//template <typename CodeUnitT>
void test_suite ()
{
//	test_size_length<CodeUnitT>();
//	test_constructors<CodeUnitT>();
//	test_insert_latin1<CodeUnitT>();
//	test_insert_cyrillic<CodeUnitT>();
//	test_append_cyrillic<CodeUnitT>();
//	test_ptr<CodeUnitT>();
//	test_at<CodeUnitT>();
//	test_erase<CodeUnitT>();
//	test_substr<CodeUnitT>();
//	test_pop_back<CodeUnitT>();
//	test_compare<CodeUnitT>();
//	test_replace<CodeUnitT>();
//	test_find<CodeUnitT>();
	test_starts_ends_with();
	test_to_string();
	test_insert();
	test_erase();
    test_lexical_cast();
    test_pack_unpack();

//	test_convert_to_number();
//	test_split<CodeUnitT>();
}

int main(int argc, char *argv[])
{
    PFS_UNUSED2(argc, argv);
	BEGIN_TESTS(0);

	cout << "*************************************************" << endl;
	cout << "*********** TEST UTF8-encoded strings ***********" << endl;
	cout << "*************************************************" << endl;
	test_suite();

//	cout << endl;
//	cout << "***************************************************" << endl;
//	cout <<  "*********** TEST UTF16-encoded strings ***********" << endl;
//	cout << "***************************************************" << endl;
//	test_suite<uint16_t>();

	return END_TESTS;
}
