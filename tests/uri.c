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

#define VHEADER(fsm) _T(#fsm), fsm
#define VNULL   NULL
#define VFILL_1 NULL
#define VFILL_2 NULL, NULL
#define VFILL_3 NULL, NULL, NULL
#define VFILL_4 NULL, NULL, NULL, NULL
#define INULL   {0, NULL}
#define IFILL_1 {0, NULL}
#define IFILL_2 {0, NULL}, {0, NULL}
#define IFILL_3 {0, NULL}, {0, NULL}, {0, NULL}
#define IFILL_4 {0, NULL}, {0, NULL}, {0, NULL}, {0, NULL}
#define IFILL_5 {0, NULL}, {0, NULL}, {0, NULL}, {0, NULL}, {0, NULL}

struct _FsmInvalidEntry {
	ssize_t ret;
	const CWT_CHAR * const invalid_str;
};

static struct _FsmTestEntry {
	const CWT_CHAR *name;
	CwtFsmTransition *trans_tab;
	const CWT_CHAR *const valid_str[5];
	struct _FsmInvalidEntry invalid_entries[5];
} __fsmTestEntries[] = {

		{ VHEADER(authority_fsm)
			, { _T("user@192.168.1.1")
				, VNULL }
				, { INULL }}


		/* ALPHA / DIGIT / "-" / "." / "_" / "~" */
		, { VHEADER(unreserved_fsm)
			, { _T("Z"), _T("z"), _T("9"), _T("~"), VNULL }
			, {   {-1, _T("?") }
				, {-1, _T("+") }
				, {-1, _T("/") }
				, INULL }}

		/* "%" HEXDIG HEXDIG */
		, { VHEADER(pct_encoded_fsm)
			, { _T("%FF"), _T("%00"), _T("%9F"), _T("%AB"), VNULL }
			, {   {-1, _T("%AR") }
				, {-1, _T("}{") }
				, {-1, _T("%%A9") }
				, INULL }}

		/* "!" / "$" / "&" / "'" / "(" / ")"
		       / "*" / "+" / "," / ";" / "=" */
		, { VHEADER(sub_delims_fsm)
			, { _T("!"), _T("$"), _T("&"), _T("'"), VFILL_1 }
			, { IFILL_5 }}
		, { VHEADER(sub_delims_fsm)
			, { _T("("), _T(")"), _T("*"), _T("+"), VFILL_1 }
			, { IFILL_5 }}
		, { VHEADER(sub_delims_fsm)
			, { _T(","), _T(";"), _T("="), VFILL_2 }
			, {   {-1, _T(":") }
				, {-1, _T("~") }
				, {-1, _T("\\") }
				, IFILL_2 }}


		/* unreserved / pct-encoded / sub-delims / ":" / "@" */
		, { VHEADER(pchar_fsm)
			, { _T("A"), _T(":"), _T("&"), _T("%DE"), VFILL_1 }
			, {   {-1, _T("%AR") }
				, {-1, _T("[") }
				, IFILL_3 }}

		/* 1*pchar */
		, { VHEADER(segment_nz_fsm)
			, { _T("ABCDE"), _T("@"), _T("@:"), _T("%DE"), VFILL_1 }
			, { { 3, _T("%BE%AR") }
				, {-1, _T("}") }
				, {-1, _T("%AR") }
				, IFILL_2}}

		/* "/" [ segment-nz *( "/" segment ) ] */
		, { VHEADER(path_absolute_fsm)
			, { _T("/"), _T("/ABCDE"), _T("/name@domain.com/%DE%AD%BE%EF"), _T("/name@domain.com/"), VFILL_1 }
			, {   { 6, _T("/name@{}/") }
				, {-1, _T("name@{}/") }
				, IFILL_3 }}

		/* 1*( unreserved / pct-encoded / sub-delims / "@" )
		   		; non-zero-length segment without any colon ":" */
		, { VHEADER(segment_nz_nc_fsm)
			, { _T("@"), _T("@@"), _T("%DE%AD"), _T("$@;"), VFILL_1 }
			, { { -1, _T("{}") }
				, IFILL_4 }}

		/* segment-nz-nc *( "/" segment ) */
		, { VHEADER(path_noscheme_fsm)
			, { _T("%DE%AD"), _T("name@domain"), _T("name@domain/%DE%AD%BE%EF"), VFILL_2 }
			, { {12, _T("name@domain/{}") }
				, IFILL_4 }}

		/* 1*4HEXDIG */
		, { VHEADER(h16_fsm)
			, { _T("A"), _T("AB"), _T("ABC"), _T("ABCD"), VFILL_1 }
			, {   {-1, _T("WBCD") }
				, { 1, _T("AWCD") }
				, { 2, _T("ABWD") }
				, { 3, _T("ABCW") }
				, IFILL_1 }}


		/* dec-octet "." dec-octet "." dec-octet "." dec-octet */
		, { VHEADER(ipv4address_fsm)
			, { _T("192.168.0.1"), _T("255.255.255.0"), _T("127.0.0.1") }
			, {   { -1, _T("W92.168.0.1") }
				, { -1, _T("192.168.0") }
				, { -1, _T("192.168") }
				, { -1, _T("192") }
				, IFILL_1 } }

		/* [ h16 ] "::" 4( h16 ":" ) ls32 */
		, { VHEADER(ipv6address_fsm_3)
			, { _T("::1B:2C:3D:4E:192.168.1.1"), _T("AB::1B:2C:3D:4E:192.168.1.1"), VFILL_3 }
			, { { -1, _T("AR::1B:2C:3D:4E:192.168.1.1") }, IFILL_4 } }

		/* *1( h16 ":" ) h16 */
		, { VHEADER(ipv6address_fsm_4_1)
			, { _T("AB"), _T("AB:CD"), VNULL }
			, {   {-1, _T(":AB") }
			    , { 2, _T("AB:??") }
				, INULL }}

		/* [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32 */
		, { VHEADER(ipv6address_fsm_4)
			, {   _T("::1B:2C:3D:192.168.1.1")
				, _T("AB::01:23:45:192.168.1.1")
				, _T("AB:CD::1B:2C:3D:192.168.1.1"), VNULL }
			, {   { -1, _T("AR::1B:2C:3D:4E:192.168.1.1") }
			 	, { -1, _T("AB:BC::1B:2C:192.168.1.1") }
				, INULL } }


		/* *2( h16 ":" ) h16 */
		, { VHEADER(ipv6address_fsm_5_1)
			, {_T("AB"), _T("AB:CD"), _T("AB:CD:EF"), VNULL }
			, {   {-1, _T("$F") }
			    , { 2, _T("AB:??") }
				, INULL }}

		/* *3( h16 ":" ) h16 */
		, { VHEADER(ipv6address_fsm_6_1)
			, {_T("AB"), _T("AB:CD"), _T("AB:CD:EF"), _T("AB:CD:EF:01"), VNULL }
			, { {-1, _T("$F") }
				, INULL }}

		/*
									     6( h16 ":" ) ls32
			/                       "::" 5( h16 ":" ) ls32
			/ [               h16 ] "::" 4( h16 ":" ) ls32
			/ [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32
			/ [ *2( h16 ":" ) h16 ] "::" 2( h16 ":" ) ls32
			/ [ *3( h16 ":" ) h16 ] "::"    h16 ":"   ls32
			/ [ *4( h16 ":" ) h16 ] "::"              ls32
			/ [ *5( h16 ":" ) h16 ] "::"              h16
			/ [ *6( h16 ":" ) h16 ] "::"
		*/
		, { VHEADER(ipv6address_fsm)
			, {   _T("AB:CD:EF:01:23:45:192.168.1.1")
				, _T("::CD:EF:01:23:45:192.168.1.1")
				, _T("AB::CD:EF:01:23:192.168.1.1")
				, VNULL }
			, { INULL }
		}

		/* "v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" ) */
		, { VHEADER(ipvfuture_fsm)
			, {   _T("vAB.:")
				, _T("vAB.::::::::")
				, VNULL }
				, { {-1, _T("vA.")}
				, INULL }}

		/* "[" ( IPv6address / IPvFuture  ) "]" */
		, { VHEADER(ip_literal_fsm)
			, {   _T("[::CD:EF:01:23:45:192.168.1.1]")
				, _T("[vAB.::::::::]")
				, VNULL }
				, { {-1, _T("[?]")}
				, INULL }}

		/* *( unreserved / pct-encoded / sub-delims ) */
		, { VHEADER(reg_name_fsm)
			, {   _T("")
				, _T("%AB")
				, _T("%AB%CD%EF")
				, _T("Hello_World.")
				, VNULL }
				, { {0, _T("?AB")}
				, { 3, _T("%AB%")}
				, INULL }}

		/*  host = IP-literal / IPv4address / reg-name */
		, { VHEADER(host_fsm)
			, {   _T("[::CD:EF:01:23:45:192.168.1.1]")
				, _T("192.168.1.1")
				, _T("~domain.com")
				, VNULL }
				, { {0, _T("?AB")}
				, { 3, _T("%AB%")}
				, INULL }}

		/* ":" port */
		, { VHEADER(authority_fsm_2)
			, { _T(":2"), _T(":25"), _T(":")
				, VNULL }
				, { {1, _T(":AB")}
				, {-1, _T("AB")}
				, INULL }}

		/* [ userinfo "@" ] host [ ":" port ] */
		/* [ authority_fsm_1 ] host [ authority_fsm_2 ] */
		, { VHEADER(authority_fsm)
			, { _T("192.168.1.1"), _T("192.168.1.1:"), _T("192.168.1.1:25"), _T("user@192.168.1.1")
				, VNULL }
				, { INULL }}
};

static void test_fsm_valid_entries(int index)
{
	const CWT_CHAR *fsmname = __fsmTestEntries[index].name;
	const CWT_CHAR * const *valid_str  = __fsmTestEntries[index].valid_str;
	struct _FsmInvalidEntry *invalid_enries = __fsmTestEntries[index].invalid_entries;

	cwtLoggerNS()->trace(_T("Test '%s'..."), fsmname);
	__fsm.trans_tab = __fsmTestEntries[index].trans_tab;

	while( *valid_str != NULL ) {
		CWT_TEST_FAIL(cwtFsmNS()->exec(&__fsm, 0, *valid_str, __strNS->strLen(*valid_str)) == (ssize_t)__strNS->strLen(*valid_str));
		valid_str++;
	}

	while( invalid_enries->invalid_str != NULL ) {
		CWT_TEST_FAIL(cwtFsmNS()->exec(&__fsm, 0
				, invalid_enries->invalid_str
				, __strNS->strLen(invalid_enries->invalid_str)) == invalid_enries->ret);
		invalid_enries++;
	}

}


int main(int argc, char *argv[])
{
/*
	CwtUriNS *uriNS = cwtUriNS();
*/
	CwtUri uri;

	int i;
	int nentries = sizeof(__fsmTestEntries)/sizeof(__fsmTestEntries[0]);
	__strNS = cwtStrNS();
	/*const CWT_CHAR *uri_string = _T("https://192.168.1.1");*/

	CWT_UNUSED(argc);
	CWT_UNUSED(argv);
	fsm_common_unused();

	FSM_INIT(__fsm, CWT_CHAR, NULL, &uri, cwtBelongCwtChar, cwtExactCwtChar);

	CWT_BEGIN_TESTS(95);

	for( i = 0; i < nentries; i++ )
		test_fsm_valid_entries(i);
/*
	uriNS->init(&uri);
	uriNS->destroy(&uri);
*/

	CWT_END_TESTS;
}
