/*
 * utf8string_constptr.cpp
 *
 *  Created on: Nov 15, 2013
 *  Author: wladt
 */

#include <cwt/test.hpp>
#include <cstring>
#include <iostream>
#include <pfs/ucchar.hpp>
#include <pfs/mbcs_string.hpp>

using std::cout;
using std::endl;

// FIXME Replace strcmp with compare_with and compare_with_utf8

template <typename _CodeUnitT>
int compare_with (const _CodeUnitT * ptr1, const _CodeUnitT * ptr2);

template <typename _CodeUnitT>
int compare_with_utf8 (const _CodeUnitT * ptr1, const char * ptr2);

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


template <typename _CodeUnitT>
void test_size_length ()
{
	typedef pfs::mbcs_string_impl<_CodeUnitT> utfstring_impl;

	//-----------------------0---------10--------20----
	const char * latin1   = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	const char * cyrillic = "АБВГДЕЁЖЫИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";

	TEST_OK(utfstring_impl::size(latin1, 0) == 0);
	TEST_OK(utfstring_impl::size(latin1, 1) == 1);
	TEST_OK(utfstring_impl::size(latin1 + strlen(latin1), 0) == 0);
	TEST_OK(utfstring_impl::size(latin1, 26) == strlen(latin1));

	TEST_OK(utfstring_impl::size(cyrillic, 1) == 2);
	TEST_OK(utfstring_impl::size(cyrillic, 33) == 66);

	TEST_OK(utfstring_impl::length(latin1, latin1 + strlen(latin1)) == 26);
	TEST_OK(utfstring_impl::length(cyrillic, cyrillic + strlen(cyrillic)) == 33);
}

template <typename _CodeUnitT>
void test_constructors ()
{
	typedef pfs::mbcs_string<_CodeUnitT> utfstring;

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

template <typename _CodeUnitT>
void test_insert_latin1 ()
{
	typedef pfs::mbcs_string<_CodeUnitT> utfstring;

	utfstring s;
	utfstring s1("Hello");
	utfstring s2("+++World!+++");
	utfstring s3(", ");

	s.insert(0, s1, 0, s1.length());
	TEST_OK(s.size() == s1.size());
	TEST_OK(s.length() == s1.length());
	TEST_OK(strcmp(s.c_str(), s1.c_str()) == 0);
	TEST_OK(strcmp(s.c_str(), "Hello") == 0);

	s.insert(5, s2, 3, 6);
	TEST_OK(strcmp(s.c_str(), "HelloWorld!") == 0);

	s.insert(5, s3, 0, s3.length());
	TEST_OK(strcmp(s.c_str(), "Hello, World!") == 0);

//	std::cout << "'" << s.c_str() << '"' << std::endl;
}

template <typename _CodeUnitT>
void test_insert_cyrillic ()
{
	typedef pfs::mbcs_string<_CodeUnitT> utfstring;

	{
		utfstring s;
		utfstring s1(utfstring::fromUtf8("Привет"));
		utfstring s2(utfstring::fromUtf8("+++Мир!+++"));
		utfstring s3(", ");

		s.insert(s.cbegin(), s1.cbegin(), s1.cend());
		TEST_OK(s.size() == s1.size());
		TEST_OK(s.length() == s1.length());
		TEST_OK(strcmp(s.c_str(), s1.c_str()) == 0);
		TEST_OK(strcmp(s.c_str(), "Привет") == 0);

		typename utfstring::const_iterator first = s2.cbegin() + 3;
		typename utfstring::const_iterator last = s2.cbegin() + 7;

		s.insert(s.cbegin() + s.length(), first, last);
		TEST_OK(strcmp(s.c_str(), "ПриветМир!") == 0);

		s.insert(s.cbegin() + s1.length(), s3.cbegin(), s3.cend());
		TEST_OK(strcmp(s.c_str(), "Привет, Мир!") == 0);
	}

	{
		utfstring s;
		utfstring s1(utfstring::fromUtf8("Привет"));
		utfstring s2(utfstring::fromUtf8("+++Мир!+++"));
		utfstring s3(", ");

		s.insert(0, s1, 0, s1.length());
		TEST_OK(s.size() == s1.size());
		//std::cout  << s.size() << "=" << s1.size() << std::endl;
		TEST_OK(s.length() == s1.length());
		TEST_OK(strcmp(s.c_str(), s1.c_str()) == 0);
		TEST_OK(strcmp(s.c_str(), "Привет") == 0);

		s.insert(6, s2, 3, 4);
		TEST_OK(strcmp(s.c_str(), "ПриветМир!") == 0);

		s.insert(6, s3, 0, s3.length());
		TEST_OK(strcmp(s.c_str(), "Привет, Мир!") == 0);
	}
}

template <typename _CodeUnitT>
void test_append_cyrillic ()
{
	typedef pfs::mbcs_string<_CodeUnitT> utfstring;

	{
		const char * sample = "Привет, Мир!";
		utfstring s;
		utfstring s1(utfstring::fromUtf8("Привет"));
		utfstring s2(", ");
		utfstring s3(utfstring::fromUtf8("Мир!"));

		s.append(s1).append(s2).append(s3);

		TEST_OK(s.size() == strlen(sample));
		TEST_OK(s.length() == s1.length() + s3.length() + 2);
		TEST_OK(strcmp(s.c_str(), sample) == 0);
	}

	{
		utfstring s;
		s += utfstring::fromUtf8("Привет");
		s += pfs::ucchar(',');
		s += ' ';
		s += "World!";

		TEST_OK(strcmp(s.c_str(), "Привет, World!") == 0);
		TEST_OK(s.size() == 20);
		TEST_OK(s.length() == 14);
	}
}

template <typename _CodeUnitT>
void test_ptr ()
{
	typedef pfs::mbcs_string<_CodeUnitT> utfstring;

	utfstring s(utfstring::fromUtf8("GIJKLЁЖЗИЙЭЮЯgijklёжзийэюя"));
	typename utfstring::pointer ptr(s, 0);
	typename utfstring::pointer begin(s, 0);
	typename utfstring::pointer end(s, s.size());

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


template <typename _CodeUnitT>
void test_at ()
{
	typedef pfs::mbcs_string<_CodeUnitT> utfstring;

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

template <typename _CodeUnitT>
void test_erase ()
{
	typedef pfs::mbcs_string<_CodeUnitT> utfstring;

	{
		utfstring sample(utfstring::fromUtf8("GIJKLЁЖЗИЙЭЮЯgijklёжзийэюя"));
		utfstring s;

		s = sample;
		TEST_OK(strcmp(s.erase(0, 0).c_str(), "GIJKLЁЖЗИЙЭЮЯgijklёжзийэюя") == 0);

		s = sample;
		TEST_OK(strcmp(s.erase(0, 1).c_str(), "IJKLЁЖЗИЙЭЮЯgijklёжзийэюя") == 0);

		s = sample;
		TEST_OK(strcmp(s.erase(0, 5).c_str(), "ЁЖЗИЙЭЮЯgijklёжзийэюя") == 0);

		s = sample;
		TEST_OK(strcmp(s.erase(13, 5).c_str(), "GIJKLЁЖЗИЙЭЮЯёжзийэюя") == 0);

		s = sample;
		TEST_OK(strcmp(s.erase(100, 5).c_str(), "GIJKLЁЖЗИЙЭЮЯgijklёжзийэюя") == 0);

		s = sample;
		TEST_OK(strcmp(s.erase(13, 100).c_str(), "GIJKLЁЖЗИЙЭЮЯ") == 0);

		s = sample;
		TEST_OK(strcmp(s.erase(0, 100).c_str(), "") == 0);
	}

	{
		utfstring sample(utfstring::fromUtf8("GIJKLЁЖЗИЙЭЮЯgijklёжзийэюя"));
		utfstring s;

		s = sample;
		s.erase(s.cbegin(), s.cbegin());
		TEST_OK(strcmp(s.c_str(), "GIJKLЁЖЗИЙЭЮЯgijklёжзийэюя") == 0);

		s = sample;
		s.erase(s.cbegin(), s.cbegin() + 1);
		TEST_OK(strcmp(s.c_str(), "IJKLЁЖЗИЙЭЮЯgijklёжзийэюя") == 0);

		s = sample;
		s.erase(s.cbegin(), s.cbegin() + 5);
		TEST_OK(strcmp(s.c_str(), "ЁЖЗИЙЭЮЯgijklёжзийэюя") == 0);

		s = sample;
		s.erase(s.cbegin() + 13, s.cbegin() + 18);
		TEST_OK(strcmp(s.c_str(), "GIJKLЁЖЗИЙЭЮЯёжзийэюя") == 0);

		s = sample;
		s.erase(s.cbegin(), s.cend());
		TEST_OK(strcmp(s.c_str(), "") == 0);
	}
}

template <typename _CodeUnitT>
void test_substr ()
{
	typedef pfs::mbcs_string<_CodeUnitT> utfstring;

	utfstring s(utfstring::fromUtf8("GIJKLЁЖЗИЙЭЮЯgijklёжзийэюя"));
	TEST_OK(s.substr(0,0).isEmpty());
	TEST_OK(s.substr(s.length(),100).isEmpty());
	TEST_OK(strcmp(s.substr(0,5).c_str(), "GIJKL") == 0);
	TEST_OK(strcmp(s.substr(5,8).c_str(), "ЁЖЗИЙЭЮЯ") == 0);
	TEST_OK(strcmp(s.substr(5,s.length() + 1).c_str(), "ЁЖЗИЙЭЮЯgijklёжзийэюя") == 0);
}

template <typename _CodeUnitT>
void test_pop_back ()
{
	typedef pfs::mbcs_string<_CodeUnitT> utfstring;

	utfstring s(utfstring::fromUtf8("GIJЁЖЗgijёжз"));
	s.pop_back();
	TEST_OK(strcmp(s.c_str(), "GIJЁЖЗgijёж") == 0);

	s.pop_back();
	TEST_OK(strcmp(s.c_str(), "GIJЁЖЗgijё") == 0);
	//std::cout << '"' << s.c_str() << '"' << std::endl;

	s.pop_back();
	TEST_OK(strcmp(s.c_str(), "GIJЁЖЗgij") == 0);

	s.pop_back();
	TEST_OK(strcmp(s.c_str(), "GIJЁЖЗgi") == 0);

	s.pop_back();
	TEST_OK(strcmp(s.c_str(), "GIJЁЖЗg") == 0);

	s.pop_back();
	TEST_OK(strcmp(s.c_str(), "GIJЁЖЗ") == 0);

	s.pop_back();
	TEST_OK(strcmp(s.c_str(), "GIJЁЖ") == 0);

	s.pop_back();
	TEST_OK(strcmp(s.c_str(), "GIJЁ") == 0);

	s.pop_back();
	TEST_OK(strcmp(s.c_str(), "GIJ") == 0);

	s.pop_back();
	TEST_OK(strcmp(s.c_str(), "GI") == 0);

	s.pop_back();
	TEST_OK(strcmp(s.c_str(), "G") == 0);

	s.pop_back();
	TEST_OK(strcmp(s.c_str(), "") == 0);

	TEST_OK(s.isEmpty());
}

template <typename _CodeUnitT>
void test_compare ()
{
	typedef pfs::mbcs_string<_CodeUnitT> utfstring;

	const char * cyr = "АБВГДЕЁЖЫИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
	utfstring s(utfstring::fromUtf8("АБВГДЕЁЖЫИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"));
	utfstring s1(utfstring::fromUtf8("АБВГДЕЁЖЫИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"));

	TEST_OK(utfstring().compare(utfstring()) == 0);
	TEST_OK(s.compare(cyr) == 0);
	TEST_OK(s.compare(s1) == 0);
}


template <typename _CodeUnitT>
void test_replace ()
{
	{
		typedef pfs::mbcs_string<_CodeUnitT> utfstring;
		utfstring s(utfstring::fromUtf8("Привет, Мир!"));

		s.replace(0, 6, utfstring::fromUtf8("Hello, World!"), 0, 5);
		TEST_OK(s.size() == 14);
		TEST_OK(s.length() == 11);
		TEST_OK(compare_with_utf8(s.c_str(), "Hello, Мир!") == 0);

		s.replace(7, 3, utfstring::fromUtf8("Hello, World!"), 7, 5);
		TEST_OK(s.size() == 13);
		TEST_OK(s.length() == 13);
		TEST_OK(compare_with_utf8(s.c_str(), "Hello, World!") == 0);

		// replace full string with empty string - get empty string
		s.replace(0, s.length(), utfstring(), 0, 0);
		TEST_OK(compare_with_utf8(s.c_str(), "") == 0);
	}

	{
		typedef pfs::mbcs_string<_CodeUnitT> utfstring;
		utfstring s(utfstring::fromUtf8("Привет, Мир!"));
		utfstring s1(utfstring::fromUtf8("Hello, World!"));

		s.replace(s.cbegin(), s.cbegin() + 6, s1.cbegin(), s1.cbegin() + 5);
		TEST_OK(s.size() == 14);
		TEST_OK(s.length() == 11);
		TEST_OK(compare_with_utf8(s.c_str(), "Hello, Мир!") == 0);

		s.replace(s.cbegin() + 7, s.cbegin() + 10, s.cbegin() + 7, s.cbegin() + 12);
		TEST_OK(s.size() == 13);
		TEST_OK(s.length() == 13);
		TEST_OK(compare_with_utf8(s.c_str(), "Hello, World!") == 0);

		// replace full string with empty string - get empty string
		utfstring nil;
		s.replace(s.cbegin(), s.cend(), nil.cbegin(), nil.cend());
		TEST_OK(compare_with_utf8(s.c_str(), "") == 0);
	}
}

template <typename _CodeUnitT>
void test_suite ()
{
	test_size_length<_CodeUnitT>();
	test_constructors<_CodeUnitT>();
	test_insert_latin1<_CodeUnitT>();
	test_insert_cyrillic<_CodeUnitT>();
	test_append_cyrillic<_CodeUnitT>();
	test_ptr<_CodeUnitT>();
	test_at<_CodeUnitT>();
	test_erase<_CodeUnitT>();
	test_substr<_CodeUnitT>();
	test_pop_back<_CodeUnitT>();
	test_compare<_CodeUnitT>();
	test_replace<_CodeUnitT>();
}

int main(int argc, char *argv[])
{
    PFS_CHECK_SIZEOF_TYPES;
    PFS_UNUSED2(argc, argv);
	BEGIN_TESTS(130);

	test_suite<char>();

    END_TESTS;
}
