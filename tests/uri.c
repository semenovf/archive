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
#include <cwt/logger.h>

static CwtFsm __fsm;

#include "../src/core/uri-rfc3986.h"

/* ALPHA / DIGIT / "-" / "." / "_" / "~" */
static void test_unreserved_fsm(void)
{
	size_t i, n;
	const CWT_CHAR *s = _T("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~");

	n = __strNS->strLen(s);
	__fsm.trans_tab = unreserved_fsm;

	cwtLoggerNS()->trace(_T("Test 'unreserved_fsm'..."));

	for( i = 0; i < n; i++ )
		CWT_TEST_FAIL(cwtFsmNS()->exec(&__fsm, 0, &s[i], 1) >= 0);

	CWT_TEST_FAIL(! (cwtFsmNS()->exec(&__fsm, 0, _T("?"), 1) >= 0) );
	CWT_TEST_FAIL(! (cwtFsmNS()->exec(&__fsm, 0, _T("+"), 1) >= 0) );
	CWT_TEST_FAIL(! (cwtFsmNS()->exec(&__fsm, 0, _T("/"), 1) >= 0) );
}


/* "%" HEXDIG HEXDIG */
static void test_pct_encoded_fsm(void)
{
	size_t i, n;
	const CWT_CHAR *s[] = { _T("%FF"), _T("%00"), _T("%9F"), _T("%AB") };

	cwtLoggerNS()->trace(_T("Test 'pct_encoded_fsm'..."));

	__fsm.trans_tab = pct_encoded_fsm;
	n = sizeof(s)/sizeof(s[0]);

	for( i = 0; i < n ; i++ )
		CWT_TEST_FAIL(cwtFsmNS()->exec(&__fsm, 0, s[i], __strNS->strLen(s[i])) >= 0);

	CWT_TEST_FAIL(! (cwtFsmNS()->exec(&__fsm, 0, _T("%AR"), 1) >= 0) );
	CWT_TEST_FAIL(! (cwtFsmNS()->exec(&__fsm, 0, _T("%AR"), 2) >= 0) );
	CWT_TEST_FAIL(! (cwtFsmNS()->exec(&__fsm, 0, _T("%AR"), 3) >= 0) );
	CWT_TEST_FAIL(! (cwtFsmNS()->exec(&__fsm, 0, _T("}{"), 2) >= 0) );
	CWT_TEST_FAIL(! (cwtFsmNS()->exec(&__fsm, 0, _T("%%A9"), 4) >= 0) );
}

/* "!" / "$" / "&" / "'" / "(" / ")"
       / "*" / "+" / "," / ";" / "="
*/
static void test_sub_delims_fsm(void)
{
	size_t i, n;
	const CWT_CHAR *s = _T("!$&'()*+,;=");

	cwtLoggerNS()->trace(_T("Test 'sub_delims_fsm'..."));

	n = __strNS->strLen(s);
	__fsm.trans_tab = sub_delims_fsm;

	for( i = 0; i < n; i++ )
		CWT_TEST_FAIL(cwtFsmNS()->exec(&__fsm, 0, &s[i], 1) >= 0);

	CWT_TEST_FAIL(! (cwtFsmNS()->exec(&__fsm, 0, _T(":"), 1) >= 0) );
	CWT_TEST_FAIL(! (cwtFsmNS()->exec(&__fsm, 0, _T("~"), 1) >= 0) );
	CWT_TEST_FAIL(! (cwtFsmNS()->exec(&__fsm, 0, _T("\\"), 1) >= 0) );
}

static void test_pchar_fsm(void)
{
	size_t i, n;
	const CWT_CHAR *s[] = { _T("ABC"), _T(":"), _T("@"), _T("@:"), _T("%DE") };

	cwtLoggerNS()->trace(_T("Test 'pchar_fsm'..."));

	n = sizeof(s)/sizeof(s[0]);
	__fsm.trans_tab = pchar_fsm;

	for( i = 0; i < n; i++ )
		CWT_TEST_FAIL(cwtFsmNS()->exec(&__fsm, 0, s[i], __strNS->strLen(s[i])) >= 0);

	CWT_TEST_FAIL(! (cwtFsmNS()->exec(&__fsm, 0, _T("%AR"), 1) >= 0) );
	CWT_TEST_FAIL(! (cwtFsmNS()->exec(&__fsm, 0, _T("%AR"), 2) >= 0) );
	CWT_TEST_FAIL(! (cwtFsmNS()->exec(&__fsm, 0, _T("%AR"), 3) >= 0) );
}

/* 1*pchar */
static void test_segment_nz_fsm(void)
{
	size_t i, n;
	const CWT_CHAR *s[] = { _T("ABCDE"), _T(":"), _T("@"), _T("@:"), _T("%DE") };

	cwtLoggerNS()->trace(_T("Test 'segment_nz_fsm'..."));

	n = sizeof(s)/sizeof(s[0]);
	__fsm.trans_tab = segment_nz_fsm;

	for( i = 0; i < n; i++ )
		CWT_TEST_FAIL(cwtFsmNS()->exec(&__fsm, 0, s[i], __strNS->strLen(s[i])) >= 0);

	CWT_TEST_FAIL(! (cwtFsmNS()->exec(&__fsm, 0, _T("}"), 1) >= 0) );
	CWT_TEST_FAIL(! (cwtFsmNS()->exec(&__fsm, 0, _T("%AR"), 1) >= 0) );
	CWT_TEST_FAIL(! (cwtFsmNS()->exec(&__fsm, 0, _T("%AR"), 2) >= 0) );
	CWT_TEST_FAIL(! (cwtFsmNS()->exec(&__fsm, 0, _T("%AR"), 3) >= 0) );
}


int main(int argc, char *argv[])
{
	CwtUtilsNS *utilsNS = cwtUtilsNS();
	CwtUri uri;
	/*const CWT_CHAR *uri_string = _T("https://192.168.1.1");*/

	__strNS = cwtStrNS(); /* declared in ../src/core/uri-rfc3986.h */
	FSM_INIT(__fsm, CWT_CHAR, NULL, NULL, cwtBelongCwtChar, cwtExactCwtChar);

	CWT_UNUSED(argc);
	CWT_UNUSED(argv);
	CWT_UNUSED(uri_reference_fsm);

	CWT_BEGIN_TESTS(8);

	test_unreserved_fsm();
	test_pct_encoded_fsm();

	test_sub_delims_fsm();
	test_pchar_fsm();
	test_segment_nz_fsm();

	utilsNS->initURI(&uri);

/*
	CWT_TEST_OK(utilsNS->parseURI(uri_string, &uri));
	CWT_TEST_OK(__strNS->strEq(_T("https"), uri.scheme));
	CWT_TEST_OK(__strNS->strEq(_T("192.168.1.1"), uri.host));
*/

	utilsNS->destroyURI(&uri);

	CWT_END_TESTS;
}
