/*
 * ini.c
 *
 *  Created on: 23.07.2012
 *      Author: wladt
 */


#include <stdio.h>
#include <cwt/types.h>
#include <cwt/test.h>
#include <cwt/logger.h>
#include <cwt/str.h>
#include <cwt/txtcodec.h>
#include <cwt/csv/csv.h>
#include <cwt/io/channel.h>
#include <cwt/io/bufdev.h>


static CwtChannelNS   *__chNS  = NULL;
static CwtStrNS       *__strNS = NULL;
static CwtCsvNS       *__csvNS = NULL;
static CwtTextCodecNS *__codecNS = NULL;


static void __csvOnError(CwtCsvHandler *csv, const CWT_CHAR *errstr)
{
	printf_error(_T("%s at line %lu"), errstr, cwtCsvNS()->line(csv));
}

static BOOL __csvOnRow(CwtCsvHandler *h, const CWT_CHAR* argv[], size_t argc)
{
	CWT_TEST_FAIL(argc == 10);
	CWT_TEST_OK(__strNS->streq(_T("Two"), argv[1]));
	CWT_TEST_OK(__strNS->streq(_T("Three"), argv[2]));

	if( __strNS->streq(_T("Fox"), argv[0]) ) {
		printf_debug(_T("%s"), argv[7]);
		CWT_TEST_OK(__strNS->streq(_T("Eight;The quick brown; fox jumps over ;the lazy dog"), argv[7]));
	}

	if( __strNS->streq(_T("Quo"), argv[0]) ) {
		printf_debug(_T("%s"), argv[7]);
		CWT_TEST_OK(__strNS->streq(_T("Eight\nEight"), argv[7]));
	}


	return TRUE;
}



static void test_00(void)
{
	CWT_CHAR *__rows[] = {
		  { _T("One;Two;Three;Four;Five;Six;Seven;Eight;Nine;Ten\n") }
		, { _T("One;Two;Three;Four;;;Seven;Eight;Nine;Ten\n") }
		, { _T("Fox;Two;Three;Four;;;Seven;\"Eight;The quick brown; fox jumps over ;the lazy dog\";Nine;Ten\n") }
		, { _T("Quo;Two;Three;Four;;;Seven;\"Eight\nEight\";Nine;Ten\n") }
	};

	size_t i;
	CwtCsvHandler *csv;
	CwtChannel    *pchan0;
	CwtChannel    *pchan1;

	CWT_ASSERT(pchan0 = __chNS->create(cwtBufferDeviceOpen()));
	CWT_ASSERT(pchan1 = __chNS->create(cwtBufferDeviceOpenPeer(__chNS->device(pchan0))));
	CWT_ASSERT(csv = __csvNS->createWithArgs(_T(";"), 32));
	__csvNS->setOnRow(csv, __csvOnRow);
	__csvNS->setOnError(csv, __csvOnError);

	for( i = 0; i < sizeof(__rows)/sizeof(__rows[0]); i++ ) {
		char *str = __codecNS->toUtf8(__rows[i], __strNS->strlen(__rows[i]));
		__chNS->write(pchan0, str, strlen(str));
		CWT_FREE(str);
	}

	__csvNS->parse(csv, pchan1);

	__csvNS->free(csv);
	__chNS->free(pchan0);
	__chNS->free(pchan1);
}


static void test_01(void)
{
#	define NCOLUMNS 5

	CWT_CHAR *__rows[] = {
		  { _T("One;Two;Three;Four;Five\n") }
		, { _T("One;Two;Three;Four;Five\n") }
		, { _T("One;Two;Three;Four;Five\n") }
		, { _T("One;Two;Three;Four;Five") }
	};


	size_t i;
	size_t nrows;
	CwtCsvHandler *csv;
	CwtChannel    *pchan0;
	CwtChannel    *pchan1;


	CWT_ASSERT(pchan0 = __chNS->create(cwtBufferDeviceOpen()));
	CWT_ASSERT(pchan1 = __chNS->create(cwtBufferDeviceOpenPeer(__chNS->device(pchan0))));
	CWT_ASSERT(csv = __csvNS->createWithArgs(_T(";"), 32));

	__csvNS->setOnError(csv, __csvOnError);

	for( i = 0; i < sizeof(__rows)/sizeof(__rows[0]); i++ ) {
		char *str = __codecNS->toUtf8(__rows[i], __strNS->strlen(__rows[i]));
		__chNS->write(pchan0, str, strlen(str));
		CWT_FREE(str);
	}

	__csvNS->begin(csv, pchan1);
	nrows = 0;
	while( __csvNS->next(csv) ) {
		CWT_CHAR *argv[NCOLUMNS];

		CWT_TEST_FAIL(__csvNS->columnsCount(csv) == NCOLUMNS);
		CWT_TEST_FAIL(__csvNS->row(csv, argv, NCOLUMNS) == NCOLUMNS);

		CWT_TEST_OK( __strNS->streq(argv[0], _T("One")));
		CWT_TEST_OK( __strNS->streq(argv[1], _T("Two")));
		CWT_TEST_OK( __strNS->streq(argv[2], _T("Three")));
		CWT_TEST_OK( __strNS->streq(argv[3], _T("Four")));
		CWT_TEST_OK( __strNS->streq(argv[4], _T("Five")));
		nrows++;
	}

	CWT_TEST_OK(nrows == sizeof(__rows)/sizeof(__rows[0]));

	__csvNS->free(csv);
	__chNS->free(pchan0);
	__chNS->free(pchan1);
}

static void test_02(void)
{
#	define NCOLUMNS 5

	CWT_CHAR *__rows[] = {
		  { _T("\"One\"    ;Two;  \"Three\";Four;Five\n") }
		, { _T("One;Two;Three;Four;Five\n") }
		, { _T("One;\"Two\";Three;Four;Five\n") }
		, { _T("One;Two;\"Three\";  Four  ;\"Five\"") }
	};


	size_t i;
	size_t nrows;
	CwtCsvHandler *csv;
	CwtChannel    *pchan0;
	CwtChannel    *pchan1;


	CWT_ASSERT(pchan0 = __chNS->create(cwtBufferDeviceOpen()));
	CWT_ASSERT(pchan1 = __chNS->create(cwtBufferDeviceOpenPeer(__chNS->device(pchan0))));
	CWT_ASSERT(csv = __csvNS->createWithArgs(_T(";"), 32));

	__csvNS->setOnError(csv, __csvOnError);

	for( i = 0; i < sizeof(__rows)/sizeof(__rows[0]); i++ ) {
		char *str = __codecNS->toUtf8(__rows[i], __strNS->strlen(__rows[i]));
		__chNS->write(pchan0, str, strlen(str));
		CWT_FREE(str);
	}

	__csvNS->begin(csv, pchan1);
	nrows = 0;
	while( __csvNS->next(csv) ) {
		CWT_CHAR *argv[NCOLUMNS];

		CWT_TEST_FAIL(__csvNS->columnsCount(csv) == NCOLUMNS);
		CWT_TEST_FAIL(__csvNS->row(csv, argv, NCOLUMNS) == NCOLUMNS);

		CWT_TEST_OK( __strNS->streq(argv[0], _T("One")));
		CWT_TEST_OK( __strNS->streq(argv[1], _T("Two")));
		CWT_TEST_OK( __strNS->streq(argv[2], _T("Three")));
		CWT_TEST_OK( __strNS->streq(argv[3], _T("Four")));
		CWT_TEST_OK( __strNS->streq(argv[4], _T("Five")));
		nrows++;
	}

	CWT_TEST_OK(nrows == sizeof(__rows)/sizeof(__rows[0]));

	__csvNS->free(csv);
	__chNS->free(pchan0);
	__chNS->free(pchan1);
}

static void test_03(void)
{
#	define NCOLUMNS 5

	CWT_CHAR *__rows[] = {
		  { _T("One;Two;Three;Four;Five\n") }
		, { _T("11;22;33;44;55\n") }
		, { _T("11;22;33;44;55\n") }
		, { _T("11;22;33;44;55") }
	};

	size_t i;
	size_t nrows;
	CwtCsvHandler *csv;
	CwtChannel    *pchan0;
	CwtChannel    *pchan1;

	CWT_ASSERT(pchan0 = __chNS->create(cwtBufferDeviceOpen()));
	CWT_ASSERT(pchan1 = __chNS->create(cwtBufferDeviceOpenPeer(__chNS->device(pchan0))));
	CWT_ASSERT(csv = __csvNS->createWithArgs(_T(";"), 32));

	__csvNS->setOnError(csv, __csvOnError);

	for( i = 0; i < sizeof(__rows)/sizeof(__rows[0]); i++ ) {
		char *str = __codecNS->toUtf8(__rows[i], __strNS->strlen(__rows[i]));
		__chNS->write(pchan0, str, strlen(str));
		CWT_FREE(str);
	}

	__csvNS->begin(csv, pchan1);

	CWT_TEST_FAIL(__csvNS->header(csv) == NCOLUMNS);
	nrows = 1;

	while( __csvNS->next(csv) ) {
		CWT_TEST_OK(__csvNS->columnsCount(csv) == NCOLUMNS);

		CWT_TEST_OK(__strNS->streq(__csvNS->column(csv, _T("One")),  _T("11")));
		CWT_TEST_OK(__strNS->streq(__csvNS->column(csv, _T("Two")),  _T("22")));
		CWT_TEST_OK(__strNS->streq(__csvNS->column(csv, _T("Three")),_T("33")));
		CWT_TEST_OK(__strNS->streq(__csvNS->column(csv, _T("Four")), _T("44")));
		CWT_TEST_OK(__strNS->streq(__csvNS->column(csv, _T("Five")), _T("55")));
		nrows++;
	}

	CWT_TEST_OK(nrows == sizeof(__rows)/sizeof(__rows[0]));

	__csvNS->free(csv);
	__chNS->free(pchan0);
	__chNS->free(pchan1);

}

int main(int argc, char *argv[])
{
	__chNS  = cwtChannelNS();
	__strNS = cwtStrNS();
	__csvNS = cwtCsvNS();
	__codecNS = cwtTextCodecNS();

	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(92);

	test_00();
	test_01();
	test_02();
	test_03();

	CWT_END_TESTS;
}


