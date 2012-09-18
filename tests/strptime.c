/*
 * strlist.c
 *
 *  Created on: 19.07.2012
 *      Author: wladt
 */


#include <stdio.h>
#include <cwt/test.h>
#include <cwt/str.h>

static CwtStrNS *__strNS = NULL;

void test_00(void)
{
	/*CWT_CHAR *ptr;*/
	struct tm tm;

	CWT_TEST_OK(__strNS->strPtime(_T("2012"), _T("%Y"), &tm));
	CWT_TEST_OK(tm.tm_year + 1900 == 2012);

	CWT_TEST_OK(__strNS->strPtime(_T("08.2012"), _T("%m.%Y"), &tm));
	CWT_TEST_OK(tm.tm_year + 1900 == 2012);
	CWT_TEST_OK(tm.tm_mon + 1 == 8);

	CWT_TEST_OK(__strNS->strPtime(_T("10.08.2012"), _T("%d.%m.%Y"), &tm));
	CWT_TEST_OK(tm.tm_year + 1900 == 2012);
	CWT_TEST_OK(tm.tm_mon + 1 == 8);
	CWT_TEST_OK(tm.tm_mday == 10);

	CWT_TEST_OK(__strNS->strPtime(_T("16:35:59 10.08.2012"), _T("%H:%M:%S %d.%m.%Y"), &tm));
	CWT_TEST_OK(tm.tm_year + 1900 == 2012);
	CWT_TEST_OK(tm.tm_mon + 1 == 8);
	CWT_TEST_OK(tm.tm_mday == 10);
	CWT_TEST_OK(tm.tm_hour == 16);
	CWT_TEST_OK(tm.tm_min  == 35);
	CWT_TEST_OK(tm.tm_sec  == 59);
}

int main(int argc, char *argv[])
{
	__strNS = cwtStrNS();

	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(16);

	test_00();

	CWT_END_TESTS;
}


