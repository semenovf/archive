/*
 * utf8string.cpp
 *
 *  Created on: Jul 21, 2013
 *      Author: wladt
 */

#include <cwt/test.h>
#include <cwt/utf8string.hpp>
#include <cstring>

using namespace cwt;

#define _u8(s)  Utf8String::fromUtf8(s)

void test_basic()
{
	static const char * latin1Chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
	Utf8String s;
	CWT_TEST_OK(s.isEmpty());

	Utf8String latin1String(latin1Chars);
	CWT_TEST_OK(strcmp(latin1Chars, latin1String.c_str()) == 0);

	Utf8String otherLatin1String;

	otherLatin1String.append(latin1String);
	CWT_TEST_OK(latin1String == otherLatin1String);

	CWT_TEST_OK(s != latin1String);
	s.append(latin1Chars);
	CWT_TEST_OK(s == latin1String);

	latin1String.clear();
	CWT_TEST_OK(latin1String.isEmpty());
}

void test_init()
{
	bool ok;
	static const char * cyrillicChars = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЬЫЪЭЮЯабвгдеёжзийклмнопрстуфхцчшщьыъэюя";
	Utf8String cyrillic (Utf8String::fromUtf8(cyrillicChars, &ok));

	CWT_TEST_OK2(ok, _Tr("Utf8String initialized from UTF-8 encoded cyrillic string"));
	CWT_TEST_OK(cyrillic.size() == strlen(cyrillicChars));
	CWT_TEST_OK2(cyrillic.size() == 66 * 2, _Tr("Cyrillic alphabet consists of 66 characters (multiple to 2 bytes per character in UTF-8)"));
	CWT_TEST_OK2(cyrillic.length() == 66, _Tr("Cyrillic alphabet consists of 66 characters"));
}

void test_swap()
{
	bool ok;
	static const char * latinChars    = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	static const char * cyrillicChars = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЬЫЪЭЮЯабвгдеёжзийклмнопрстуфхцчшщьыъэюя";
	Utf8String cyrillic (Utf8String::fromUtf8(cyrillicChars, &ok));
	Utf8String latin    (Utf8String::fromUtf8(latinChars, &ok));

	CWT_TEST_OK(strcmp(latin.c_str(), latinChars) == 0);
	CWT_TEST_OK(strcmp(cyrillic.c_str(), cyrillicChars) == 0);

	swap(cyrillic, latin);

	CWT_TEST_OK2(strcmp(latin.c_str(), cyrillicChars) == 0, _Tr("Latin string contains cyrillic characters after swap"));
	CWT_TEST_OK2(strcmp(cyrillic.c_str(), latinChars) == 0, _Tr("Cyrillic string contains latin characters after swap"));
}

void test_find()
{
	bool ok;
	Utf8String haystack (Utf8String::fromUtf8("АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЬЫЪЭЮЯабвгдеёжзийклмнопрстуфхцчшщьыъэюя", &ok));
	Utf8String needles[3];
	Utf8String::const_iterator its[3];

	needles[0] = Utf8String::fromUtf8("АБВГ", &ok);
	its[0] = haystack.begin();
	needles[1] = Utf8String::fromUtf8("а", &ok);
	its[1] = haystack.begin() + 33;
	needles[2] = Utf8String::fromUtf8("W", &ok);
	its[2] = haystack.end();

	for (size_t i = 0; i < sizeof(needles)/sizeof(needles[0]); ++i) {
		Utf8String::const_iterator it = haystack.find(needles[i], haystack.begin());

		CWT_TEST_OK(it == its[i]);
	}
}

template <typename _iterator>
void test_iterator()
{
	bool ok;
	Utf8String s (Utf8String::fromUtf8("GIJKLЁЖЗИЙЭЮЯgijklёжзийэюя", &ok));
	size_t n = 0;

	_iterator it = s.begin();
	while (it++ < s.end()) {
		++n;
	}

	CWT_TEST_OK(n == s.length());

	it = s.end();
	n = 0;
	while (it-- > s.begin()) {
		++n;
	}

	CWT_TEST_OK(n == s.length());
}

template <typename _reverse_iterator>
void test_reverse_iterator()
{
	CWT_TRACE("test_reverse_iterator() begin...");
	bool ok;
	Utf8String s (Utf8String::fromUtf8("GIJKLЁЖЗИЙЭЮЯgijklёжзийэюя", &ok));
	size_t n = 0;

	_reverse_iterator it = s.rbegin();
	while (it++ < s.rend()) {
		++n;
	}

	CWT_TEST_OK(n == s.length());

	it = s.rend();
	n = 0;
	while (it-- > s.rbegin()) {
		++n;
	}

	CWT_TEST_OK(n == s.length());
	CWT_TEST_OK(n == s.length(s.crbegin(), s.crend()));
	CWT_TRACE("test_reverse_iterator() completed");
}

void test_insert()
{
	bool ok;
	Utf8String s (Utf8String::fromUtf8("GIJKLЁЖЗИЙЭЮЯgijklёжзийэюя", &ok));

	CWT_TEST_OK(_u8("GIJKLЁЖЗИЙЭЮЯ").append(_u8("gijklёжзийэюя")) == s);
	CWT_TEST_OK(_u8("GIJKL").insert(_u8("ЁЖЗИЙЭЮЯgijklёжзийэюя"), 5) == s);
	CWT_TEST_OK(_u8("GIJKLЁ").insert(_u8("ЖЗИЙЭЮЯgijklёжзийэюя"), 6) == s);
	CWT_TEST_OK(_u8("GIJKLЁЖЗИgijklёжзийэюя").insert(_u8("ЙЭЮЯ"), 9) == s);
	CWT_TEST_OK(_u8("gijklёжзийэюя").prepend(_u8("GIJKLЁЖЗИЙЭЮЯ")) == s);
}


void test_substr()
{
	bool ok;
	Utf8String s (Utf8String::fromUtf8("GIJKLЁЖЗИЙЭЮЯgijklёжзийэюя", &ok));

	CWT_TEST_OK(s.substr(0) == s);
	CWT_TEST_OK(s.substr(1) == Utf8String::fromUtf8("IJKLЁЖЗИЙЭЮЯgijklёжзийэюя"));
	CWT_TEST_OK(s.substr(2) == Utf8String::fromUtf8("JKLЁЖЗИЙЭЮЯgijklёжзийэюя"));
	CWT_TEST_OK(s.substr(3) == Utf8String::fromUtf8("KLЁЖЗИЙЭЮЯgijklёжзийэюя"));
	CWT_TEST_OK(s.substr(13) == Utf8String::fromUtf8("gijklёжзийэюя"));
	CWT_TEST_OK(s.substr(12,1) == Utf8String::fromUtf8("Я"));

	CWT_TEST_OK(s.substr(0,0) == Utf8String());
	CWT_TEST_OK(s.substr(26) == Utf8String());
	CWT_TEST_OK(s.substr(126) == Utf8String());
}


void test_compare()
{
	CWT_TEST_OK(_u8("Б").compare(_u8("Б")) == 0);
	CWT_TEST_OK(_u8("Б").compare(_u8("А")) > 0);
	CWT_TEST_OK(_u8("Б").compare(_u8("В")) < 0);
	CWT_TEST_OK(_u8("АБВ").compare("АБВ") == 0);
}

void test_startWith()
{
	bool ok;
	Utf8String s (Utf8String::fromUtf8("ЁЖЗИЙЭЮЯGIJKLёжзийэюяgijkl", &ok));

	CWT_ASSERT(ok);

	CWT_TEST_OK(s.startsWith(s));
	CWT_TEST_OK(s.startsWith("Ё"));
	CWT_TEST_OK(s.startsWith("ЁЖ"));
	CWT_TEST_OK(s.startsWith("ЁЖЗИЙЭЮЯGIJKL"));
	CWT_TEST_OK(s.startsWith("ЁЖЗИЙЭЮЯGIJKLёжзийэюя"));
	CWT_TEST_OK(s.startsWith("ЁЖЗИЙЭЮЯGIJKLёжзийэюяgijkl"));
	CWT_TEST_NOK(s.startsWith("A"));
	CWT_TEST_NOK(s.startsWith("Жизнь"));
}

void test_endsWith()
{
	bool ok;
	Utf8String s (Utf8String::fromUtf8("ЁЖЗИЙЭЮЯGIJKLёжзийэюяgijkl", &ok));

	CWT_ASSERT(ok);

	CWT_TEST_OK(s.endsWith(s));
	CWT_TEST_OK(s.endsWith("l"));
	CWT_TEST_OK(s.endsWith("gijkl"));
	CWT_TEST_OK(s.endsWith("ёжзийэюяgijkl"));
	CWT_TEST_OK(s.endsWith("ЭЮЯGIJKLёжзийэюяgijkl"));
	CWT_TEST_OK(s.endsWith("ЁЖЗИЙЭЮЯGIJKLёжзийэюяgijkl"));
	CWT_TEST_NOK(s.endsWith("A"));
	CWT_TEST_NOK(s.endsWith("Жизнь"));
}

void test_number_conversions()
{
	bool ok;
	CWT_TEST_OK(_u8("123456").toUInt( &ok) == 123456 && ok);
	CWT_TEST_OK(_u8("-123456").toInt(&ok) == -123456 && ok);
	CWT_TEST_OK(_u8("/123456").toInt(&ok) == 0 && !ok);
	CWT_TEST_OK(_u8("123.456").toInt(&ok) == 0 && !ok);

	CWT_TEST_OK(_u8("123.456").toFloat(&ok) == 123.456f && ok);
	CWT_TEST_OK(_u8("123.456").toDouble(&ok) == double(123.456) && ok);
}



int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
    CWT_BEGIN_TESTS(67);

    test_basic();
    test_init();
    test_swap();
    test_find();
    test_iterator<Utf8String::iterator>();
    test_iterator<Utf8String::const_iterator>();
    test_reverse_iterator<Utf8String::reverse_iterator>();
    test_reverse_iterator<Utf8String::const_reverse_iterator>();
    test_insert();
    test_substr();
    test_compare();
    test_startWith();
    test_endsWith();
    test_number_conversions();

    CWT_END_TESTS;
}




