/**
 * @file   uri.c
 * @author wladt
 * @date   Oct 12, 2012 11:18:36 AM
 *
 * @brief URI testing
 */
#include <cwt/test.h>
#include <cwt/uri.h>
#include <cwt/str.h>
#include <cwt/fsm.h>
#include <cwt/logger.h>

static CwtFsm __fsm;

static CwtStrNS *__strNS = NULL; /* declare before header file 'uri-rfc3986.h' will be included */

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

	CWT_TEST_FAIL(cwtFsmNS()->exec(&__fsm, 0, _T("?"), 1) < 0 );
	CWT_TEST_FAIL(cwtFsmNS()->exec(&__fsm, 0, _T("+"), 1) < 0 );
	CWT_TEST_FAIL(cwtFsmNS()->exec(&__fsm, 0, _T("/"), 1) < 0 );
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

	CWT_TEST_FAIL( cwtFsmNS()->exec(&__fsm, 0, _T("%AR"), 1) < 0 );
	CWT_TEST_FAIL( cwtFsmNS()->exec(&__fsm, 0, _T("%AR"), 2) < 0 );
	CWT_TEST_FAIL( cwtFsmNS()->exec(&__fsm, 0, _T("%AR"), 3) < 0 );
	CWT_TEST_FAIL( cwtFsmNS()->exec(&__fsm, 0, _T("}{"), 2)  < 0 );
	CWT_TEST_FAIL( cwtFsmNS()->exec(&__fsm, 0, _T("%%A9"), 4) < 0 );
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

	CWT_TEST_FAIL( cwtFsmNS()->exec(&__fsm, 0, _T(":"), 1) < 0 );
	CWT_TEST_FAIL( cwtFsmNS()->exec(&__fsm, 0, _T("~"), 1) < 0 );
	CWT_TEST_FAIL( cwtFsmNS()->exec(&__fsm, 0, _T("\\"), 1) < 0 );
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

	CWT_TEST_FAIL( cwtFsmNS()->exec(&__fsm, 0, _T("%AR"), 1) < 0 );
	CWT_TEST_FAIL( cwtFsmNS()->exec(&__fsm, 0, _T("%AR"), 2) < 0 );
	CWT_TEST_FAIL( cwtFsmNS()->exec(&__fsm, 0, _T("%AR"), 3) < 0 );
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

	CWT_TEST_FAIL( cwtFsmNS()->exec(&__fsm, 0, _T("%BE%AR"), 6) == 3 );
	CWT_TEST_FAIL( cwtFsmNS()->exec(&__fsm, 0, _T("}"), 1)   < 0 );
	CWT_TEST_FAIL( cwtFsmNS()->exec(&__fsm, 0, _T("%AR"), 1) < 0 );
	CWT_TEST_FAIL( cwtFsmNS()->exec(&__fsm, 0, _T("%AR"), 2) < 0 );
	CWT_TEST_FAIL( cwtFsmNS()->exec(&__fsm, 0, _T("%AR"), 3) < 0 );
}

int main(int argc, char *argv[])
{
/*
	CwtUriNS *uriNS = cwtUriNS();
	CwtUri uri;
*/

	__strNS = cwtStrNS();
	/*const CWT_CHAR *uri_string = _T("https://192.168.1.1");*/

	CWT_UNUSED(argc);
	CWT_UNUSED(argv);
	fsm_common_unused();

	FSM_INIT(__fsm, CWT_CHAR, NULL, NULL, cwtBelongCwtChar, cwtExactCwtChar);

	CWT_BEGIN_TESTS(110);

	test_unreserved_fsm();
	test_pct_encoded_fsm();
	test_sub_delims_fsm();
	test_pchar_fsm();
	test_segment_nz_fsm();

/*
	uriNS->init(&uri);
	uriNS->destroy(&uri);
*/

	CWT_END_TESTS;
}
