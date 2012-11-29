/**
 * @file   uri.c
 * @author wladt
 * @date   Oct 12, 2012 11:18:36 AM
 *
 * @brief URI testing
 */

#include <cwt/fsm_test.h>
#include <cwt/logger.h>
#include <cwt/stdio.h>
#include <cwt/uri.h>

#include "../src/core/uri-rfc3986.h"

static struct _FsmTestEntry __fsmTestEntries[] = {

		  { VHEADER(authority_fsm)
			, { _T("192.168.1.1"), _T("user@192.168.1.1"), VNULL }
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
			, { _T("!"), _T("$"), _T("&"), _T("'"), VNULL }
			, { INULL }}
		, { VHEADER(sub_delims_fsm)
			, { _T("("), _T(")"), _T("*"), _T("+"), VNULL }
			, { INULL }}
		, { VHEADER(sub_delims_fsm)
			, { _T(","), _T(";"), _T("="), VNULL }
			, {   {-1, _T(":") }
				, {-1, _T("~") }
				, {-1, _T("\\") }
				, INULL }}


		/* unreserved / pct-encoded / sub-delims / ":" / "@" */
		, { VHEADER(pchar_fsm)
			, { _T("A"), _T(":"), _T("&"), _T("%DE"), VNULL }
			, {   {-1, _T("%AR") }
				, {-1, _T("[") }
				, INULL }}

		/* 1*pchar */
		, { VHEADER(segment_nz_fsm)
			, { _T("ABCDE"), _T("@"), _T("@:"), _T("%DE"), VNULL }
			, { { 3, _T("%BE%AR") }
				, {-1, _T("}") }
				, {-1, _T("%AR") }
				, INULL}}

		/* "/" [ segment-nz *( "/" segment ) ] */
		, { VHEADER(path_absolute_fsm)
			, { _T("/"), _T("/ABCDE"), _T("/name@domain.com/%DE%AD%BE%EF"), _T("/name@domain.com/"), VNULL }
			, {   { 6, _T("/name@{}/") }
				, {-1, _T("name@{}/") }
				, INULL }}

		/* 1*( unreserved / pct-encoded / sub-delims / "@" )
		   		; non-zero-length segment without any colon ":" */
		, { VHEADER(segment_nz_nc_fsm)
			, { _T("@"), _T("@@"), _T("%DE%AD"), _T("$@;"), VNULL }
			, { { -1, _T("{}") }
				, INULL }}

		/* segment-nz-nc *( "/" segment ) */
		, { VHEADER(path_noscheme_fsm)
			, { _T("%DE%AD"), _T("name@domain"), _T("name@domain/%DE%AD%BE%EF"), VNULL }
			, { {12, _T("name@domain/{}") }
				, INULL }}

		/* 1*4HEXDIG */
		, { VHEADER(h16_fsm)
			, { _T("A"), _T("AB"), _T("ABC"), _T("ABCD"), VNULL }
			, {   {-1, _T("WBCD") }
				, { 1, _T("AWCD") }
				, { 2, _T("ABWD") }
				, { 3, _T("ABCW") }
				, INULL }}


		/* dec-octet "." dec-octet "." dec-octet "." dec-octet */
		, { VHEADER(ipv4address_fsm)
			, { _T("192.168.0.1"), _T("255.255.255.0"), _T("127.0.0.1") }
			, {   { -1, _T("W92.168.0.1") }
				, { -1, _T("192.168.0") }
				, { -1, _T("192.168") }
				, { -1, _T("192") }
				, INULL } }

		/* [ h16 ] "::" 4( h16 ":" ) ls32 */
		, { VHEADER(ipv6address_fsm_3)
			, { _T("::1B:2C:3D:4E:192.168.1.1"), _T("AB::1B:2C:3D:4E:192.168.1.1"), VNULL }
			, { { -1, _T("AR::1B:2C:3D:4E:192.168.1.1") }, INULL } }

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

		/*  *( "/" segment ) */
		, { VHEADER(path_abempty_fsm)
			, { _T("/"), _T("/segment"), _T("/path/to"), _T("/path/to/"), VNULL }
				, { INULL }}

        /* relative-part = "//" authority path-abempty
		              / path-absolute
		              / path-noscheme
		              / path-empty
		*/
		, { VHEADER(relative_part_fsm)
			, { _T("//user@host"), _T("//user@host/"), VNULL }
				, { INULL }}

		/* *( pchar / "/" / "?" ) */
		, { VHEADER(query_fsm)
			, { _T("/?"), _T("/"), _T("?query%20string"), VNULL }
				, { INULL }}

		/* relative-part [ "?" query ] [ "#" fragment ] */
		, { VHEADER(relative_ref_fsm)
			, { _T("//user@host/?query%20string"), _T("#fragment%20string")
				, _T("//user@host/?query%20string#fragment%20string")
				, VNULL }
				, { INULL }}

		/* ALPHA *( ALPHA / DIGIT / "+" / "-" / "." ) */
		, { VHEADER(scheme_fsm)
			, { _T("a"), _T("aA"), _T("A2"), _T("A+")
				, VNULL }
				, {   {-1, _T("2a") }
					, {-1, _T("+A") }
					, {-1, _T(".A") }
					, {-1, _T("-a") }
				    , INULL }}

		/* scheme ":" hier-part [ "?" query ] [ "#" fragment ] */
		, { VHEADER(uri_fsm)
		, { _T("http://user@host/?query%20string"), _T("http://user@host/#fragment%20string")
			, _T("ftp://user@host/path/to?query%20string#fragment%20string")
			, VNULL }
			, { INULL }}


		/* URI / relative-ref */
		, { VHEADER(uri_reference_fsm)
			, { _T("http://user@host/?query%20string"), _T("http://user@host#fragment%20string")
				, _T("ftp://user@host/?query%20string#fragment%20string")
				, VNULL }
				, { INULL }}
};


int main(int argc, char *argv[])
{
	CwtUriNS *uriNS = cwt_uri_ns();
	CwtStrNS *strNS = cwt_str_ns();
	CwtUri uri;
	static CwtFsm fsm;
	int i;
	CWT_CHAR uri_string[512];
	int nentries = sizeof(__fsmTestEntries)/sizeof(__fsmTestEntries[0]);

	const CWT_CHAR *scheme = _T("https");
	const CWT_CHAR *userinfo = _T("user");
	const CWT_CHAR *host = _T("192.168.1.1");
	BOOL host_is_ip = TRUE;
	UINT16 port = 25;
	const CWT_CHAR *path  = _T("/path/to");
	const CWT_CHAR *query = _T("query%20string");
	const CWT_CHAR *fragment = _T("fragment%20string");
	CWT_CHAR *uri_result;

	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	cwt_stdio_ns()->sprintf(uri_string
		, _T("%s://%s@%s:%u%s?%s#%s")
		, scheme
		, userinfo
		, host
		, port
		, path
		, query
		, fragment);

	FSM_INIT(fsm, CWT_CHAR, NULL, NULL, cwt_fsm_belong_cwtchar, cwt_fsm_exact_cwtchar, cwt_fsm_range_cwtchar);

	CWT_BEGIN_TESTS(180);

	for( i = 0; i < nentries; i++ )
		fsm_test_entries(&fsm, &__fsmTestEntries[i]);

	uriNS->init(&uri);
	CWT_TEST_FAIL(uriNS->parse(uri_string, &uri) == (ssize_t)strNS->strLen(uri_string));
	CWT_TEST_FAIL(uri.scheme);
	CWT_TEST_FAIL(uri.userinfo);
	CWT_TEST_FAIL(uri.host);
	CWT_TEST_FAIL(uri.path);
	CWT_TEST_FAIL(uri.query);
	CWT_TEST_FAIL(uri.fragment);
	CWT_TEST_OK(strNS->strEq(uri.scheme, scheme));
	CWT_TEST_OK(strNS->strEq(uri.userinfo, userinfo));
	CWT_TEST_OK(strNS->strEq(uri.host, host));
	CWT_TEST_OK(uri.host_is_ip == host_is_ip);
	CWT_TEST_OK(uri.port == port);
	CWT_TEST_OK(strNS->strEq(uri.path, path));
	CWT_TEST_OK(strNS->strEq(uri.query, query));
	CWT_TEST_OK(strNS->strEq(uri.fragment, fragment));

	uri_result = uriNS->compose(&uri);
	CWT_TEST_OK(strNS->strEq(uri_string, uri_result));
	CWT_FREE(uri_result);
	uriNS->destroy(&uri);

	uriNS->init(&uri);
	CWT_TEST_FAIL(uriNS->parse(_T("file:/tmp/text.txt"), &uri) == 18);
	CWT_TEST_FAIL(uri.scheme);
	CWT_TEST_FAIL(uri.path);
	CWT_TEST_OK(strNS->strEq(uri.scheme, _T("file")));
	CWT_TEST_OK(strNS->strEq(uri.path, _T("/tmp/text.txt")));
	uriNS->destroy(&uri);

	uriNS->init(&uri);
	CWT_TEST_FAIL(uriNS->parse(_T("scheme:relative/path?query#fragment"), &uri) == 35);
	CWT_TEST_FAIL(uri.scheme);
	CWT_TEST_OK(strNS->strEq(uri.scheme, _T("scheme")));
	CWT_TEST_OK(strNS->isEmpty(uri.userinfo));
	CWT_TEST_OK(strNS->isEmpty(uri.host));
	CWT_TEST_OK(uri.port == 0);
	CWT_TEST_OK(strNS->strEq(uri.path, _T("relative/path")));
	CWT_TEST_OK(strNS->strEq(uri.query, _T("query")));
	CWT_TEST_OK(strNS->strEq(uri.fragment, _T("fragment")));

	uriNS->destroy(&uri);


	CWT_END_TESTS;
}
