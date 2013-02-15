/**
 * @file   uri.c
 * @author wladt
 * @date   Oct 12, 2012 11:18:36 AM
 *
 * @brief URI testing
 */

#include <cwt/uri.hpp>
#include <cstdio>

#include "../src/uri-rfc3986.hpp"
#include <cwt/fsm_test.hpp>

using namespace cwt;

static FsmTestEntry __fsmTestEntries[] = {

		  { VHEADER(authority_fsm)
			, { "192.168.1.1", "user@192.168.1.1", VNULL }
				, { INULL }}

		/* ALPHA / DIGIT / "-" / "." / "_" / "~" */
		, { VHEADER(unreserved_fsm)
			, { "Z", "z", "9", "~", VNULL }
			, {   {-1, "?" }
				, {-1, "+" }
				, {-1, "/" }
				, INULL }}

		/* "%" HEXDIG HEXDIG */
		, { VHEADER(pct_encoded_fsm)
			, { "%FF", "%00", "%9F", "%AB", VNULL }
			, {   {-1, "%AR" }
				, {-1, "}{" }
				, {-1, "%%A9" }
				, INULL }}

		/* "!" / "$" / "&" / "'" / "(" / ")"
		       / "*" / "+" / "," / ";" / "=" */
		, { VHEADER(sub_delims_fsm)
			, { "!", "$", "&", "'", VNULL }
			, { INULL }}
		, { VHEADER(sub_delims_fsm)
			, { "(", ")", "*", "+", VNULL }
			, { INULL }}
		, { VHEADER(sub_delims_fsm)
			, { ",", ";", "=", VNULL }
			, {   {-1, ":" }
				, {-1, "~" }
				, {-1, "\\" }
				, INULL }}


		/* unreserved / pct-encoded / sub-delims / ":" / "@" */
		, { VHEADER(pchar_fsm)
			, { "A", ":", "&", "%DE", VNULL }
			, {   {-1, "%AR" }
				, {-1, "[" }
				, INULL }}

		/* 1*pchar */
		, { VHEADER(segment_nz_fsm)
			, { "ABCDE", "@", "@:", "%DE", VNULL }
			, { { 3, "%BE%AR" }
				, {-1, "}" }
				, {-1, "%AR" }
				, INULL}}

		/* "/" [ segment-nz *( "/" segment ) ] */
		, { VHEADER(path_absolute_fsm)
			, { "/", "/ABCDE", "/name@domain.com/%DE%AD%BE%EF", "/name@domain.com/", VNULL }
			, {   { 6, "/name@{}/" }
				, {-1, "name@{}/" }
				, INULL }}

		/* 1*( unreserved / pct-encoded / sub-delims / "@" )
		   		; non-zero-length segment without any colon ":" */
		, { VHEADER(segment_nz_nc_fsm)
			, { "@", "@@", "%DE%AD", "$@;", VNULL }
			, { { -1, "{}" }
				, INULL }}

		/* segment-nz-nc *( "/" segment ) */
		, { VHEADER(path_noscheme_fsm)
			, { "%DE%AD", "name@domain", "name@domain/%DE%AD%BE%EF", VNULL }
			, { {12, "name@domain/{}" }
				, INULL }}

		/* 1*4HEXDIG */
		, { VHEADER(h16_fsm)
			, { "A", "AB", "ABC", "ABCD", VNULL }
			, {   {-1, "WBCD" }
				, { 1, "AWCD" }
				, { 2, "ABWD" }
				, { 3, "ABCW" }
				, INULL }}


		/* dec-octet "." dec-octet "." dec-octet "." dec-octet */
		, { VHEADER(ipv4address_fsm)
			, { "192.168.0.1", "255.255.255.0", "127.0.0.1" }
			, {   { -1, "W92.168.0.1" }
				, { -1, "192.168.0" }
				, { -1, "192.168" }
				, { -1, "192" }
				, INULL } }

		/* [ h16 ] "::" 4( h16 ":" ) ls32 */
		, { VHEADER(ipv6address_fsm_3)
			, { "::1B:2C:3D:4E:192.168.1.1", "AB::1B:2C:3D:4E:192.168.1.1", VNULL }
			, { { -1, "AR::1B:2C:3D:4E:192.168.1.1" }, INULL } }

		/* *1( h16 ":" ) h16 */
		, { VHEADER(ipv6address_fsm_4_1)
			, { "AB", "AB:CD", VNULL }
			, {   {-1, ":AB" }
			    , { 2, "AB:??" }
				, INULL }}

		/* [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32 */
		, { VHEADER(ipv6address_fsm_4)
			, {   "::1B:2C:3D:192.168.1.1"
				, "AB::01:23:45:192.168.1.1"
				, "AB:CD::1B:2C:3D:192.168.1.1", VNULL }
			, {   { -1, "AR::1B:2C:3D:4E:192.168.1.1" }
			 	, { -1, "AB:BC::1B:2C:192.168.1.1" }
				, INULL } }


		/* *2( h16 ":" ) h16 */
		, { VHEADER(ipv6address_fsm_5_1)
			, {"AB", "AB:CD", "AB:CD:EF", VNULL }
			, {   {-1, "$F" }
			    , { 2, "AB:??" }
				, INULL }}

		/* *3( h16 ":" ) h16 */
		, { VHEADER(ipv6address_fsm_6_1)
			, {"AB", "AB:CD", "AB:CD:EF", "AB:CD:EF:01", VNULL }
			, { {-1, "$F" }
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
			, {   "AB:CD:EF:01:23:45:192.168.1.1"
				, "::CD:EF:01:23:45:192.168.1.1"
				, "AB::CD:EF:01:23:192.168.1.1"
				, VNULL }
			, { INULL }
		}

		/* "v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" ) */
		, { VHEADER(ipvfuture_fsm)
			, {   "vAB.:"
				, "vAB.::::::::"
				, VNULL }
				, { {-1, "vA."}
				, INULL }}

		/* "[" ( IPv6address / IPvFuture  ) "]" */
		, { VHEADER(ip_literal_fsm)
			, {   "[::CD:EF:01:23:45:192.168.1.1]"
				, "[vAB.::::::::]"
				, VNULL }
				, { {-1, "[?]"}
				, INULL }}

		/* *( unreserved / pct-encoded / sub-delims ) */
		, { VHEADER(reg_name_fsm)
			, {   ""
				, "%AB"
				, "%AB%CD%EF"
				, "Hello_World."
				, VNULL }
				, { {0, "?AB"}
				, { 3, "%AB%"}
				, INULL }}

		/*  host = IP-literal / IPv4address / reg-name */
		, { VHEADER(host_fsm)
			, {   "[::CD:EF:01:23:45:192.168.1.1]"
				, "192.168.1.1"
				, "~domain.com"
				, VNULL }
				, { {0, "?AB"}
				, { 3, "%AB%"}
				, INULL }}

		/* ":" port */
		, { VHEADER(authority_fsm_2)
			, { ":2", ":25", ":"
				, VNULL }
				, { {1, ":AB"}
				, {-1, "AB"}
				, INULL }}

		/* [ userinfo "@" ] host [ ":" port ] */
		/* [ authority_fsm_1 ] host [ authority_fsm_2 ] */
		, { VHEADER(authority_fsm)
			, { "192.168.1.1", "192.168.1.1:", "192.168.1.1:25", "user@192.168.1.1"
				, VNULL }
				, { INULL }}

		/*  *( "/" segment ) */
		, { VHEADER(path_abempty_fsm)
			, { "/", "/segment", "/path/to", "/path/to/", VNULL }
				, { INULL }}

        /* relative-part = "//" authority path-abempty
		              / path-absolute
		              / path-noscheme
		              / path-empty
		*/
		, { VHEADER(relative_part_fsm)
			, { "//user@host", "//user@host/", VNULL }
				, { INULL }}

		/* *( pchar / "/" / "?" ) */
		, { VHEADER(query_fsm)
			, { "/?", "/", "?query%20string", VNULL }
				, { INULL }}

		/* relative-part [ "?" query ] [ "#" fragment ] */
		, { VHEADER(relative_ref_fsm)
			, { "//user@host/?query%20string", "#fragment%20string"
				, "//user@host/?query%20string#fragment%20string"
				, VNULL }
				, { INULL }}

		/* ALPHA *( ALPHA / DIGIT / "+" / "-" / "." ) */
		, { VHEADER(scheme_fsm)
			, { "a", "aA", "A2", "A+"
				, VNULL }
				, {   {-1, "2a" }
					, {-1, "+A" }
					, {-1, ".A" }
					, {-1, "-a" }
				    , INULL }}

		/* scheme ":" hier-part [ "?" query ] [ "#" fragment ] */
		, { VHEADER(uri_fsm)
		, { "http://user@host/?query%20string", "http://user@host/#fragment%20string"
			, "ftp://user@host/path/to?query%20string#fragment%20string"
			, VNULL }
			, { INULL }}


		/* URI / relative-ref */
		, { VHEADER(uri_reference_fsm)
			, { "http://user@host/?query%20string", "http://user@host#fragment%20string"
				, "ftp://user@host/?query%20string#fragment%20string"
				, VNULL }
				, { INULL }}
};


void test_uri_fsm()
{
	Uri uri;
	static Fsm<Char> fsm(NULL, NULL);
	int nentries = sizeof(__fsmTestEntries)/sizeof(__fsmTestEntries[0]);

	for( int i = 0; i < nentries; i++ )
		fsm_test_entries(fsm, &__fsmTestEntries[i]);
}


void test_uri_parse()
{
	char uri_string[512];
	const char *scheme = "https";
	const char *userinfo = "user";
	const char *host = "192.168.1.1";
	uint16_t port = 25;
	const char *path  = "/path/to";
	const char *query = "query%20string";
	const char *fragment = "fragment%20string";
	String uri_result;

	sprintf(uri_string
		, "%s://%s@%s:%u%s?%s#%s"
		, scheme
		, userinfo
		, host
		, port
		, path
		, query
		, fragment);

	Uri uri(String::fromUtf8(uri_string));

	CWT_TEST_FAIL(uri.isValid());
	CWT_TEST_FAIL(uri.scheme() == String::fromUtf8(scheme));
	CWT_TEST_FAIL(uri.userinfo() == String::fromUtf8(userinfo));
	CWT_TEST_FAIL(uri.host() == String::fromUtf8(host));
	CWT_TEST_FAIL(uri.path() == String::fromUtf8(path));
	CWT_TEST_FAIL(uri.query() == String::fromUtf8(query));
	CWT_TEST_FAIL(uri.fragment() == String::fromUtf8(fragment));
	CWT_TEST_OK(uri.hostIsIp() == true);
	CWT_TEST_OK(uri.port() == port);

	uri_result = uri.toString();
	printf("%s\n", uri_string);
	printf("%s\n", uri_result.toUtf8().data());
	CWT_TEST_OK(uri_result == String::fromUtf8(uri_string));

	CWT_TEST_FAIL(uri.parse(_U("file:/tmp/text.txt")));
	CWT_TEST_OK(uri.scheme() == _U("file"));
	CWT_TEST_OK(uri.path() == _U("/tmp/text.txt"));

	CWT_TEST_FAIL(uri.parse(_U("scheme:relative/path?query#fragment")));
	CWT_TEST_OK(uri.scheme() == _U("scheme"));
	CWT_TEST_OK(uri.userinfo().isEmpty());
	CWT_TEST_OK(uri.host().isEmpty());
	CWT_TEST_OK(uri.port() == 0);
	CWT_TEST_OK(uri.path() == _U("relative/path"));
	CWT_TEST_OK(uri.query() == _U("query"));
	CWT_TEST_OK(uri.fragment() == _U("fragment"));

	CWT_TEST_OK(uri.parse(_U("http://ru.indeed.com/%D0%A2%D0%B5%D1%81%D1%82%D0%B8%D1%80%D0%BE%D0%B2%D1%89%D0%B8%D0%BA-%D0%92%D0%B5%D0%B1-jobs")));
	CWT_TEST_OK(uri.parse(_U("https://webcache.googleusercontent.com/search?client=ubuntu&channel=fs&q=cache:Aaap4fYxHwAJ:http://toriava.ru/category/11/47/b1067/b2047/%2BCrystalMedia+%D0%B5%D0%BA%D0%B0%D1%82%D0%B5%D1%80%D0%B8%D0%BD%D0%B1%D1%83%D1%80%D0%B3&oe=utf-8&redir_esc=&hl=ru&ct=clnk")));
}

int main(int argc, char *argv[])
{
	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(173);

	test_uri_fsm();
	test_uri_parse();

	CWT_END_TESTS;
}
