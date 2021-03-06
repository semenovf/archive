/*
 * ini.c
 *
 *  Created on: 23.07.2012
 *      Author: wladt
 */


#include <stdio.h>
#include <string.h>
#include <cwt/types.h>
#include <cwt/test.h>
#include <cwt/ini.h>
#include <cwt/logger.h>
#include <cwt/str.h>
#include <cwt/txtcodec.h>
#include <cwt/io/channel.h>
#include <cwt/io/bufdev.h>


static CWT_CHAR *directives[] = {
	  _T("directive1 arg11 arg12 arg13 arg14 arg15\n")
	, _T("directive2 arg21 arg22 arg23 arg24\n")
	, _T("directive3 arg31 arg32 arg33\n")
	, _T("directive4 arg41 arg42\n")
	, _T("directive5 arg51\n")
	, _T("directive6\n")
};

static void __iniOnError(CwtIniContext *h, const CWT_CHAR *errstr)
{
	cwt_logger_ns()->error(_T("%s at line %lu"), errstr, cwt_ini_ns()->line(h));
}

static BOOL __onIniDirective1(CwtIniContext *h, const CWT_CHAR **argv, size_t argc)
{
	CWT_UNUSED(h);
	CWT_TEST_OK(argc == 6);
	CWT_TEST_OK(cwt_str_ns()->strEq(_T("directive1"), argv[0]));
	return TRUE;
}

static BOOL __onIniDirective2(CwtIniContext *h, const CWT_CHAR* argv[], size_t argc)
{
	CWT_UNUSED(h);
	CWT_TEST_OK(argc == 5);
	CWT_TEST_OK(cwt_str_ns()->strEq(_T("directive2"), argv[0]));
	return TRUE;
}

static BOOL __onIniDirective3(CwtIniContext *h, const CWT_CHAR* argv[], size_t argc)
{
	CWT_UNUSED(h);
	CWT_TEST_OK(argc == 4);
	CWT_TEST_OK(cwt_str_ns()->strEq(_T("directive3"), argv[0]));
	return TRUE;
}

static BOOL __onIniDirective4(CwtIniContext *h, const CWT_CHAR* argv[], size_t argc)
{
	CWT_UNUSED(h);
	CWT_TEST_OK(argc == 3);
	CWT_TEST_OK(cwt_str_ns()->strEq(_T("directive4"), argv[0]));
	return TRUE;
}

static BOOL __onIniDirective5(CwtIniContext *h, const CWT_CHAR* argv[], size_t argc)
{
	CWT_UNUSED(h);
	CWT_TEST_OK(argc == 2);
	CWT_TEST_OK(cwt_str_ns()->strEq(_T("directive5"), argv[0]));
	return TRUE;
}

static BOOL __onIniDirective6(CwtIniContext *h, const CWT_CHAR* argv[], size_t argc)
{
	CWT_UNUSED(h);
	CWT_TEST_OK(argc == 1);
	CWT_TEST_OK(cwt_str_ns()->strEq(_T("directive6"), argv[0]));
	return TRUE;
}


int main(int argc, char *argv[])
{
	size_t i;
	CwtChannelNS *chNS = cwt_channel_ns();
	CwtStrNS     *strNS = cwt_str_ns();
	CwtIniNS     *iniNS = cwt_ini_ns();
	CwtTextCodecNS *codecNS = cwt_textcodec_ns();
	CwtIniContext *ini;
	CwtChannel   *pchan0;
	CwtChannel   *pchan1;

	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(12);

	CWT_ASSERT(pchan0 = chNS->open(cwtBufferDeviceOpen()));
	CWT_ASSERT(pchan1 = chNS->open(cwtBufferDeviceOpenPeer(chNS->device(pchan0))));
	CWT_ASSERT(ini = iniNS->create());
	ini->onError = __iniOnError;
	iniNS->addDirective(ini, _T("directive1"), __onIniDirective1);
	iniNS->addDirective(ini, _T("directive2"), __onIniDirective2);
	iniNS->addDirective(ini, _T("directive3"), __onIniDirective3);
	iniNS->addDirective(ini, _T("directive4"), __onIniDirective4);
	iniNS->addDirective(ini, _T("directive5"), __onIniDirective5);
	iniNS->addDirective(ini, _T("directive6"), __onIniDirective6);

	for( i = 0; i < sizeof(directives)/sizeof(directives[0]); i++ ) {
		char *str = codecNS->toUtf8(directives[i], strNS->strLen(directives[i]));
		chNS->write(pchan0, (BYTE*)str, strlen(str));
		CWT_FREE(str);
	}

	iniNS->parse(ini, pchan1);

	iniNS->free(ini);
	chNS->close(pchan0);
	chNS->close(pchan1);

	CWT_END_TESTS;
}


