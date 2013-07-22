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

int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
    CWT_BEGIN_TESTS(14);

    test_basic();
    test_init();
    test_swap();

    CWT_END_TESTS;
}




