/**
 * @file   uri.c
 * @author wladt
 * @date   Oct 12, 2012 11:18:36 AM
 *
 * @brief URI testing
 */
#include <cwt/test.h>
#include <cwt/utils.h>
#include <cwt/str.h>
#include <cwt/fsm.h>

static CwtFsm __fsm;

#include "../src/core/uri-rfc3986.h"

/* ALPHA / DIGIT / "-" / "." / "_" / "~" */
static void test_unreserved_fsm(void)
{
	size_t i, n;
	const CWT_CHAR *s = _T("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~");

	n = __strNS->strLen(s);
	__fsm.trans_tab = unreserved_fsm;

	for( i = 0; i < n; i++ )
		CWT_TEST_FAIL(cwtFsmNS()->exec(&__fsm, 0, &s[i], 1) >= 0);

	CWT_TEST_NOK(cwtFsmNS()->exec(&__fsm, 0, _T("?"), 1) >= 0);
	CWT_TEST_NOK(cwtFsmNS()->exec(&__fsm, 0, _T("+"), 1) >= 0);
	CWT_TEST_NOK(cwtFsmNS()->exec(&__fsm, 0, _T("/"), 1) >= 0);
}

/* "%" HEXDIG HEXDIG */
static void test_pct_encoded_fsm(void)
{
	size_t i, n;
	const CWT_CHAR *s[] = { _T("%FF"), _T("%00"), _T("%9F"), _T("%AB") };

	__fsm.trans_tab = pct_encoded_fsm;
	n = sizeof(s)/sizeof(s[0]);

	for( i = 0; i < n ; i++ )
		CWT_TEST_FAIL(cwtFsmNS()->exec(&__fsm, 0, s[i], __strNS->strLen(s[i])) >= 0);

	CWT_TEST_NOK(cwtFsmNS()->exec(&__fsm, 0, _T("A9"), 2) >= 0);
	CWT_TEST_NOK(cwtFsmNS()->exec(&__fsm, 0, _T("%%A9"), 4) >= 0);
}

int main(int argc, char *argv[])
{
	CwtUtilsNS *utilsNS = cwtUtilsNS();
	CwtUri uri;
	const CWT_CHAR *uri_string = _T("https://192.168.1.1");

	__strNS = cwtStrNS(); /* declared in ../src/core/uri-rfc3986.h */
	FSM_INIT(__fsm, CWT_CHAR, NULL, NULL, cwtBelongCwtChar, cwtExactCwtChar);

	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(2);

	test_unreserved_fsm();
	test_pct_encoded_fsm();

	utilsNS->initURI(&uri);

/*
	CWT_TEST_OK(utilsNS->parseURI(uri_string, &uri));
	CWT_TEST_OK(__strNS->strEq(_T("https"), uri.scheme));
	CWT_TEST_OK(__strNS->strEq(_T("192.168.1.1"), uri.host));
*/

	utilsNS->destroyURI(&uri);

	CWT_END_TESTS;
}
