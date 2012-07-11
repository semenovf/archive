/*
 * strutils.c
 *
 *  Created on: 29.06.2012
 *      Author: user
 */


#include <stdio.h>
#include <cwt/types.h>
#include <cwt/test.h>
#include <cwt/str.h>

int main(int argc, char *argv[])
{
	CwtStrNS *strNS = cwtStrNS();

	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(12);

	CWT_TEST_OK(CWT_LONGLONG_MIN   == strNS->toLONGLONG(_T("-9223372036854775808"), 0, NULL));
	CWT_TEST_NOK(CWT_LONGLONG_MIN  == strNS->toLONGLONG(_T("-9223372036854775809"), 0, NULL));
	CWT_TEST_OK(CWT_LONGLONG_MAX   == strNS->toLONGLONG(_T("9223372036854775807"), 0, NULL));
	CWT_TEST_NOK(CWT_LONGLONG_MAX  == strNS->toLONGLONG(_T("9223372036854775808"), 0, NULL));
	CWT_TEST_OK(CWT_ULONGLONG_MAX  == strNS->toULONGLONG(_T("18446744073709551615"), 0, NULL));
	CWT_TEST_NOK(CWT_ULONGLONG_MAX == strNS->toULONGLONG(_T("18446744073709551616"), 0, NULL));
	CWT_TEST_OK(CWT_LONG_MIN       == strNS->toLONG(_T("-2147483648"), 0, NULL));
	CWT_TEST_NOK(CWT_LONG_MIN      == strNS->toLONG(_T("-2147483649"), 0, NULL));
	CWT_TEST_OK(CWT_LONG_MAX       == strNS->toLONG(_T("2147483647"), 0, NULL));
	CWT_TEST_NOK(CWT_LONG_MAX      == strNS->toLONG(_T("2147483648"), 0, NULL));
	CWT_TEST_OK(CWT_ULONG_MAX      == strNS->toULONG(_T("4294967295"), 0, NULL));
	CWT_TEST_NOK(CWT_ULONG_MAX     == strNS->toULONG(_T("4294967296"), 0, NULL));


	CWT_END_TESTS;
}


