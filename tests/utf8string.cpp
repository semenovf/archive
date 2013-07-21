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
	CWT_TEST_OK(strcmp(latin1Chars, latin1String.data()) == 0);

	Utf8String otherLatin1String;

	otherLatin1String.append(latin1String);
	CWT_TEST_OK(latin1String == otherLatin1String);

	CWT_TEST_OK(s != latin1String);
	s.append(latin1Chars);
	CWT_TEST_OK(s == latin1String);

	latin1String.clear();
	CWT_TEST_OK(latin1String.isEmpty());
}

int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
    CWT_BEGIN_TESTS(1);

    test_basic();

    CWT_END_TESTS;
}




