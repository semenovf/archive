/*
 * Copyright (c) 2012, 2013 Fedor Semenov
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose with or without fee is hereby granted, provided
 * that the above copyright notice and this permission notice appear
 * in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * uri-rfc3986.h
 *
 *  Created on: 12.10.2012
 *      Author: Fedor Semenov
 */


#ifndef __PFS_URI_RFC3986_HPP__
#define __PFS_URI_RFC3986_HPP__

#include <pfs/fsm.hpp>

namespace pfs {

struct UriParseContext {
	uri_data   *uridata;
};


#ifdef __PFS_TEST__
#	define __TRACE_ACT __trace
#	define __TRACE_ARGS(arg) (reinterpret_cast<void*>(const_cast<char*>(arg)))
	static bool __trace(const string::const_iterator &, const string::const_iterator &, void *, void * action_args)
	{
		CWT_TRACE(reinterpret_cast<const char *>(action_args));
		return true;
	}
#else
#	define __TRACE_ACT nullptr
#	define __TRACE_ARGS(arg) nullptr
#endif

static bool set_scheme    (string::const_iterator begin, string::const_iterator end, void * context, void * action_args);
static bool set_query     (string::const_iterator begin, string::const_iterator end, void * context, void * action_args);
static bool set_fragment  (string::const_iterator begin, string::const_iterator end, void * context, void * action_args);
static bool set_path      (string::const_iterator begin, string::const_iterator end, void * context, void * action_args);
static bool set_userinfo  (string::const_iterator begin, string::const_iterator end, void * context, void * action_args);
static bool unset_userinfo(string::const_iterator begin, string::const_iterator end, void * context, void * action_args);
static bool set_host      (string::const_iterator begin, string::const_iterator end, void * context, void * action_args);
static bool set_port      (string::const_iterator begin, string::const_iterator end, void * context, void * action_args);
static bool set_host_is_ip(string::const_iterator begin, string::const_iterator end, void * context, void * action_args);
/*static void __check_host_is_ip(const void *data, size_t len, void *context, void *action_args);*/

/*
   RFC 3986: Uniform Resource Identifier (URI)
  -----------------------------------------------------------------------
   URI           = scheme ":" hier-part [ "?" query ] [ "#" fragment ]

   hier-part     = "//" authority path-abempty
                 / path-absolute
                 / path-rootless
                 / path-empty

   URI-reference = URI / relative-ref

   absolute-URI  = scheme ":" hier-part [ "?" query ]

   relative-ref  = relative-part [ "?" query ] [ "#" fragment ]

   relative-part = "//" authority path-abempty
                 / path-absolute
                 / path-noscheme
                 / path-empty

   scheme        = ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )

   authority     = [ userinfo "@" ] host [ ":" port ]
   userinfo      = *( unreserved / pct-encoded / sub-delims / ":" )
   host          = IP-literal / IPv4address / reg-name
   port          = *DIGIT

   IP-literal    = "[" ( IPv6address / IPvFuture  ) "]"

   IPvFuture     = "v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" )

   IPv6address   =                            6( h16 ":" ) ls32
                 /                       "::" 5( h16 ":" ) ls32
                 / [               h16 ] "::" 4( h16 ":" ) ls32
                 / [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32
                 / [ *2( h16 ":" ) h16 ] "::" 2( h16 ":" ) ls32
                 / [ *3( h16 ":" ) h16 ] "::"    h16 ":"   ls32
                 / [ *4( h16 ":" ) h16 ] "::"              ls32
                 / [ *5( h16 ":" ) h16 ] "::"              h16
                 / [ *6( h16 ":" ) h16 ] "::"

   h16           = 1*4HEXDIG
   ls32          = ( h16 ":" h16 ) / IPv4address
   IPv4address   = dec-octet "." dec-octet "." dec-octet "." dec-octet

   dec-octet     = DIGIT                 ; 0-9
                 / %x31-39 DIGIT         ; 10-99
                 / "1" 2DIGIT            ; 100-199
                 / "2" %x30-34 DIGIT     ; 200-249
                 / "25" %x30-35          ; 250-255

   reg-name      = *( unreserved / pct-encoded / sub-delims )

   path          = path-abempty    ; begins with "/" or is empty
                 / path-absolute   ; begins with "/" but not "//"
                 / path-noscheme   ; begins with a non-colon segment
                 / path-rootless   ; begins with a segment
                 / path-empty      ; zero characters

   slash-segment = "/" segment

   path-abempty  = *slash-segment
   path-absolute = "/" [ segment-nz *slash-segment ]
   path-noscheme = segment-nz-nc *slash-segment
   path-rootless = segment-nz *slash-segment
   path-empty    = 0<pchar>

   segment       = *pchar
   segment-nz    = 1*pchar
   segment-nz-nc = 1*( unreserved / pct-encoded / sub-delims / "@" )
                 ; non-zero-length segment without any colon ":"

   pchar         = unreserved / pct-encoded / sub-delims / ":" / "@"

   query         = *( pchar / "/" / "?" )

   fragment      = *( pchar / "/" / "?" )

   pct-encoded   = "%" HEXDIG HEXDIG

   unreserved    = ALPHA / DIGIT / "-" / "." / "_" / "~"
   reserved      = gen-delims / sub-delimsstatic void __set_port(const void *data, size_t len, void *context, void *action_args);
   gen-delims    = ":" / "/" / "?" / "#" / "[" / "]" / "@"
   sub-delims    = "!" / "$" / "&" / "'" / "(" / ")"
                 / "*" / "+" / "," / ";" / "="
  */

static string _URI_ALPHA("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
static string _URI_DIGIT("0123456789");
static string _URI_HEXDIGIT("0123456789ABCDEFabcdef"); /* DIGIT / "A" / "B" / "C" / "D" / "E" / "F" */

static fsm::transition<string> digit_fsm[] = {
    {-1,-1, FSM_MATCH_CHAR(_URI_DIGIT), FSM_ACCEPT, nullptr, nullptr }
};

static fsm::transition<string> hexdig_fsm[] = {
    {-1,-1, FSM_MATCH_CHAR(_URI_HEXDIGIT), FSM_ACCEPT, nullptr, nullptr }
};

/* ALPHA / DIGIT / "-" / "." / "_" / "~" */
static fsm::transition<string> unreserved_fsm[] = {
      {-1, 1, FSM_MATCH_CHAR(_URI_ALPHA) , FSM_ACCEPT, nullptr, nullptr }
    , {-1, 2, FSM_MATCH_CHAR(_URI_DIGIT) , FSM_ACCEPT, nullptr, nullptr }
    , {-1,-1, FSM_MATCH_CHAR(_u8("-._~")), FSM_ACCEPT, nullptr, nullptr }
};

/* "%" HEXDIG HEXDIG */
static fsm::transition<string> pct_encoded_fsm[] = {
      { 1,-1, FSM_MATCH_STR(_u8("%"))      , FSM_NORMAL, nullptr, nullptr }
    , { 2,-1, FSM_MATCH_CHAR(_URI_HEXDIGIT), FSM_NORMAL, nullptr, nullptr }
    , {-1,-1, FSM_MATCH_CHAR(_URI_HEXDIGIT), FSM_ACCEPT, nullptr, nullptr }
};

/* "!" / "$" / "&" / "'" / "(" / ")"
       / "*" / "+" / "," / ";" / "="
*/
static fsm::transition<string> sub_delims_fsm[] = {
    {-1,-1, FSM_MATCH_CHAR(_u8("!$&'()*+,;=")), FSM_ACCEPT, nullptr, nullptr }
};

/* unreserved / pct-encoded / sub-delims / ":" / "@" */
static fsm::transition<string> pchar_fsm[] = {
      {-1, 1, FSM_MATCH_FSM(unreserved_fsm) , FSM_ACCEPT, nullptr, nullptr }
    , {-1, 2, FSM_MATCH_FSM(pct_encoded_fsm), FSM_ACCEPT, nullptr, nullptr }
    , {-1, 3, FSM_MATCH_FSM(sub_delims_fsm) , FSM_ACCEPT, nullptr, nullptr }
    , {-1,-1, FSM_MATCH_CHAR(_u8(":@"))     , FSM_ACCEPT, nullptr, nullptr }
};


/* 1*pchar */
static fsm::transition<string> segment_nz_fsm[] = {
	  { 1,-1, FSM_MATCH_FSM(pchar_fsm),     FSM_ACCEPT, nullptr, nullptr }
	, { 1,-1, FSM_MATCH_FSM(pchar_fsm),     FSM_ACCEPT, nullptr, nullptr }
};

/* *pchar */
static fsm::transition<string> segment_fsm[] = {
      { 0, 1, FSM_MATCH_FSM(pchar_fsm), FSM_ACCEPT, nullptr, nullptr }
    , {-1,-1, FSM_MATCH_NOTHING,        FSM_ACCEPT, nullptr, nullptr }
};

/* "/" segment */
static fsm::transition<string> slash_segment_fsm[] = {
      { 1,-1, FSM_MATCH_STR(_u8("/"))   , FSM_NORMAL, nullptr, nullptr }
    , {-1,-1, FSM_MATCH_FSM(segment_fsm), FSM_ACCEPT, nullptr, nullptr }
};

/* segment-nz *slash_segment */
static fsm::transition<string> segment_nz_slash_fsm[] = {
	  { 1,-1, FSM_MATCH_FSM(segment_nz_fsm)              , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_RPT_FSM(slash_segment_fsm, 0, -1), FSM_ACCEPT, nullptr, nullptr }
};

/* "/" [ segment-nz *slash_segment ] */
static fsm::transition<string> path_absolute_fsm[] = {
	  { 1,-1, FSM_MATCH_STR(_u8("/"))                 , FSM_NORMAL, nullptr, nullptr }
    , {-1,-1, FSM_MATCH_OPT_FSM(segment_nz_slash_fsm) , FSM_ACCEPT, nullptr, nullptr }
};

/* 1*( unreserved / pct-encoded / sub-delims / "@" )
   		; non-zero-length segment without any colon ":"
*/
static fsm::transition<string> segment_nc_fsm[] = {
      {-1, 1, FSM_MATCH_FSM(unreserved_fsm) , FSM_ACCEPT, nullptr, nullptr }
    , {-1, 2, FSM_MATCH_FSM(pct_encoded_fsm), FSM_ACCEPT, nullptr, nullptr }
    , {-1, 3, FSM_MATCH_FSM(sub_delims_fsm) , FSM_ACCEPT, nullptr, nullptr }
    , {-1,-1, FSM_MATCH_CHAR(_u8("@"))      , FSM_ACCEPT, nullptr, nullptr }
};

static fsm::transition<string> segment_nz_nc_fsm[] = {
    {-1,-1, FSM_MATCH_RPT_FSM(segment_nc_fsm, 1, -1),  FSM_ACCEPT, nullptr, nullptr }
};

/* segment-nz-nc *slash-segment */
static fsm::transition<string> path_noscheme_fsm[] = {
      { 1,-1, FSM_MATCH_FSM(segment_nz_nc_fsm), FSM_ACCEPT, nullptr, nullptr }
    , {-1,-1, FSM_MATCH_RPT_FSM(slash_segment_fsm, 0, -1), FSM_ACCEPT, nullptr, nullptr }
};

/* 0<pchar> */
static fsm::transition<string> path_empty_fsm[] = {
    {-1,-1, FSM_MATCH_NOTHING, FSM_ACCEPT, nullptr, nullptr }
};

/* 1*4HEXDIG */
static fsm::transition<string> h16_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(_URI_HEXDIGIT), FSM_ACCEPT, nullptr, nullptr }
	, { 2,-1, FSM_MATCH_CHAR(_URI_HEXDIGIT), FSM_ACCEPT, nullptr, nullptr }
	, { 3,-1, FSM_MATCH_CHAR(_URI_HEXDIGIT), FSM_ACCEPT, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_CHAR(_URI_HEXDIGIT), FSM_ACCEPT, nullptr, nullptr }
};

/* "25" %x30-35        ; 250-255 */
static fsm::transition<string> dec_octet_fsm_4[] = {
	  { 1,-1, FSM_MATCH_STR(_u8("25"))      , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_CHAR(_u8("012345")) , FSM_ACCEPT, nullptr, nullptr }
};

/* "2" %x30-34 DIGIT   ; 200-249 */
static fsm::transition<string> dec_octet_fsm_3[] = {
	  { 1,-1, FSM_MATCH_STR(_u8("2"))        , FSM_NORMAL, nullptr, nullptr }
	, { 2,-1, FSM_MATCH_CHAR(_u8("01234"))   , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_CHAR(_URI_DIGIT), FSM_ACCEPT, nullptr, nullptr }
};

/* "1" 2DIGIT  ; 100-199 */
static fsm::transition<string> dec_octet_fsm_2[] = {
	  { 1,-1, FSM_MATCH_STR(_u8("1"))   , FSM_NORMAL, nullptr, nullptr }
	, { 2,-1, FSM_MATCH_CHAR(_URI_DIGIT), FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_CHAR(_URI_DIGIT), FSM_ACCEPT, nullptr, nullptr }
};

/* %x31-39 DIGIT       ; 10-99*/
static fsm::transition<string> dec_octet_fsm_1[] = {
      { 1,-1, FSM_MATCH_CHAR(_u8("123456789")), FSM_NORMAL, nullptr, nullptr }
    , {-1,-1, FSM_MATCH_CHAR(_URI_DIGIT) , FSM_ACCEPT, nullptr, nullptr }
};

/*
  DIGIT                 ; 0-9
  / %x31-39 DIGIT       ; 10-99
  / "1" 2DIGIT          ; 100-199
  / "2" %x30-34 DIGIT   ; 200-249
  / "25" %x30-35        ; 250-255
*/
static fsm::transition<string> dec_octet_fsm[] = {
      {-1, 1, FSM_MATCH_FSM(dec_octet_fsm_4), FSM_ACCEPT, nullptr, nullptr } /* 250 - 255 */
    , {-1, 2, FSM_MATCH_FSM(dec_octet_fsm_3), FSM_ACCEPT, nullptr, nullptr } /* 200 - 249 */
    , {-1, 3, FSM_MATCH_FSM(dec_octet_fsm_2), FSM_ACCEPT, nullptr, nullptr } /* 100 - 199 */
    , {-1, 4, FSM_MATCH_FSM(dec_octet_fsm_1), FSM_ACCEPT, nullptr, nullptr } /* 10 - 99 */
    , {-1,-1, FSM_MATCH_CHAR(_URI_DIGIT)        , FSM_ACCEPT, nullptr, nullptr } /* 0 - 9 */
};

/* dec-octet "." dec-octet "." dec-octet "." dec-octet */
static fsm::transition<string> ipv4address_fsm[] = {
	  { 1,-1, FSM_MATCH_FSM(dec_octet_fsm), FSM_NORMAL, nullptr, nullptr }
	, { 2,-1, FSM_MATCH_STR(_u8("."))     , FSM_NORMAL, nullptr, nullptr }
	, { 3,-1, FSM_MATCH_FSM(dec_octet_fsm), FSM_NORMAL, nullptr, nullptr }
	, { 4,-1, FSM_MATCH_STR(_u8("."))     , FSM_NORMAL, nullptr, nullptr }
	, { 5,-1, FSM_MATCH_FSM(dec_octet_fsm), FSM_NORMAL, nullptr, nullptr }
	, { 6,-1, FSM_MATCH_STR(_u8("."))     , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(dec_octet_fsm), FSM_ACCEPT, nullptr, nullptr }
};

/* ( h16 ":" h16 ) / IPv4address */
static fsm::transition<string> ls32_fsm[] = {
	  {-1, 1, FSM_MATCH_FSM(ipv4address_fsm), FSM_ACCEPT, nullptr, nullptr }
	, { 2,-1, FSM_MATCH_FSM(h16_fsm)        , FSM_NORMAL, nullptr, nullptr }
	, { 3,-1, FSM_MATCH_STR(_u8(":"))       , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(h16_fsm)        , FSM_ACCEPT, nullptr, nullptr }
};

/* h16 ":" */
static fsm::transition<string> h16_semi_fsm[] = {
	  { 1,-1, FSM_MATCH_FSM(h16_fsm)  , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_STR(_u8(":")) , FSM_ACCEPT, nullptr, nullptr }
};

/* *1( h16 ":" ) h16 */
static fsm::transition<string> ipv6address_fsm_4_1[] = {
	  { 1,-1, FSM_MATCH_RPT_FSM(h16_semi_fsm, 0, 1), FSM_NORMAL, nullptr, nullptr }
	, {-1, 2, FSM_MATCH_FSM(h16_fsm)           , FSM_ACCEPT, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(h16_fsm)           , FSM_ACCEPT, nullptr, nullptr }
};

/* *2( h16 ":" ) h16 */
static fsm::transition<string> ipv6address_fsm_5_1[] = {
	  { 1, 2, FSM_MATCH_RPT_FSM(h16_semi_fsm, 0, 2), FSM_NORMAL, nullptr, nullptr }
	, {-1, 2, FSM_MATCH_FSM(h16_fsm)           , FSM_ACCEPT, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(h16_fsm)           , FSM_ACCEPT, nullptr, nullptr }
};

/* *3( h16 ":" ) h16 */
static fsm::transition<string> ipv6address_fsm_6_1[] = {
	  { 1, 2, FSM_MATCH_RPT_FSM(h16_semi_fsm, 0, 3), FSM_NORMAL, nullptr, nullptr }
	, {-1, 2, FSM_MATCH_FSM(h16_fsm)           , FSM_ACCEPT, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(h16_fsm)           , FSM_ACCEPT, nullptr, nullptr }
};

/* *4( h16 ":" ) h16 */
static fsm::transition<string> ipv6address_fsm_7_1[] = {
	  { 1, 2, FSM_MATCH_RPT_FSM(h16_semi_fsm, 0, 4), FSM_NORMAL, nullptr, nullptr }
	, {-1, 2, FSM_MATCH_FSM(h16_fsm)           , FSM_ACCEPT, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(h16_fsm)           , FSM_ACCEPT, nullptr, nullptr }
};

/* *5( h16 ":" ) h16 */
static fsm::transition<string> ipv6address_fsm_8_1[] = {
	  { 1, 2, FSM_MATCH_RPT_FSM(h16_semi_fsm, 0, 5), FSM_NORMAL, nullptr, nullptr }
	, {-1, 2, FSM_MATCH_FSM(h16_fsm)               , FSM_ACCEPT, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(h16_fsm)               , FSM_ACCEPT, nullptr, nullptr }
};

/* *6( h16 ":" ) h16 */
static fsm::transition<string> ipv6address_fsm_9_1[] = {
	  { 1, 2, FSM_MATCH_RPT_FSM(h16_semi_fsm, 0, 6), FSM_NORMAL, nullptr, nullptr }
	, {-1, 2, FSM_MATCH_FSM(h16_fsm)               , FSM_ACCEPT, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(h16_fsm)               , FSM_ACCEPT, nullptr, nullptr }
};

/* 6( h16 ":" ) ls32 */
static fsm::transition<string> ipv6address_fsm_1[] = {
      { 1,-1, FSM_MATCH_RPT_FSM(h16_semi_fsm, 6, 6), FSM_NORMAL, nullptr, nullptr }
    , {-1,-1, FSM_MATCH_FSM(ls32_fsm)              , FSM_ACCEPT, nullptr, nullptr }
};

/* "::" 5( h16 ":" ) ls32 */
static fsm::transition<string> ipv6address_fsm_2[] = {
	  { 1,-1, FSM_MATCH_STR(_u8("::"))             , FSM_NORMAL, nullptr, nullptr }
	, { 2,-1, FSM_MATCH_RPT_FSM(h16_semi_fsm, 5, 5), FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(ls32_fsm)              , FSM_ACCEPT, nullptr, nullptr }
};

/* [ h16 ] "::" 4( h16 ":" ) ls32 */
static fsm::transition<string> ipv6address_fsm_3[] = {
      { 1, 1, FSM_MATCH_FSM(h16_fsm)               , FSM_NORMAL, nullptr, nullptr }
    , { 2,-1, FSM_MATCH_STR(_u8("::"))             , FSM_NORMAL, nullptr, nullptr }
    , { 3,-1, FSM_MATCH_RPT_FSM(h16_semi_fsm, 4, 4), FSM_NORMAL, nullptr, nullptr }
    , {-1,-1, FSM_MATCH_FSM(ls32_fsm)              , FSM_ACCEPT, nullptr, nullptr }
};

/* [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32 */
static fsm::transition<string> ipv6address_fsm_4[] = {
      { 1, 1, FSM_MATCH_FSM(ipv6address_fsm_4_1), FSM_NORMAL, nullptr, nullptr }

    , { 2,-1, FSM_MATCH_STR(_u8("::"))    , FSM_NORMAL, nullptr, nullptr }
    , { 3,-1, FSM_MATCH_FSM(h16_semi_fsm) , FSM_NORMAL, nullptr, nullptr }
    , { 4,-1, FSM_MATCH_FSM(h16_semi_fsm) , FSM_NORMAL, nullptr, nullptr }
    , { 5,-1, FSM_MATCH_FSM(h16_semi_fsm) , FSM_NORMAL, nullptr, nullptr }
    , {-1,-1, FSM_MATCH_FSM(ls32_fsm)     , FSM_ACCEPT, nullptr, nullptr }
};

/* [ *2( h16 ":" ) h16 ] "::" 2( h16 ":" ) ls32 */
static fsm::transition<string> ipv6address_fsm_5[] = {
	  { 1, 2, FSM_MATCH_FSM(ipv6address_fsm_5_1), FSM_NORMAL, nullptr, nullptr }
	, { 2, 2, FSM_MATCH_FSM(ipv6address_fsm_5_1), FSM_NORMAL, nullptr, nullptr }

    , { 3,-1, FSM_MATCH_STR(_u8("::"))    , FSM_NORMAL, nullptr, nullptr }
	, { 4,-1, FSM_MATCH_FSM(h16_semi_fsm) , FSM_NORMAL, nullptr, nullptr }
	, { 5,-1, FSM_MATCH_FSM(h16_semi_fsm) , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(ls32_fsm)     , FSM_ACCEPT, nullptr, nullptr }
};

/* [ *3( h16 ":" ) h16 ] "::" h16 ":"   ls32 */
static fsm::transition<string> ipv6address_fsm_6[] = {
	  { 1, 3, FSM_MATCH_FSM(ipv6address_fsm_6_1), FSM_NORMAL, nullptr, nullptr }
	, { 2, 3, FSM_MATCH_FSM(ipv6address_fsm_6_1), FSM_NORMAL, nullptr, nullptr }
	, { 3, 3, FSM_MATCH_FSM(ipv6address_fsm_6_1), FSM_NORMAL, nullptr, nullptr }

	, { 4,-1, FSM_MATCH_STR(_u8("::"))    , FSM_NORMAL, nullptr, nullptr }
	, { 5,-1, FSM_MATCH_FSM(h16_semi_fsm) , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(ls32_fsm)     , FSM_ACCEPT, nullptr, nullptr }
};


/* [ *4( h16 ":" ) h16 ] "::" ls32 */
static fsm::transition<string> ipv6address_fsm_7[] = {
	  { 1, 4, FSM_MATCH_FSM(ipv6address_fsm_7_1), FSM_NORMAL, nullptr, nullptr }
	, { 2, 4, FSM_MATCH_FSM(ipv6address_fsm_7_1), FSM_NORMAL, nullptr, nullptr }
	, { 3, 4, FSM_MATCH_FSM(ipv6address_fsm_7_1), FSM_NORMAL, nullptr, nullptr }
	, { 4, 4, FSM_MATCH_FSM(ipv6address_fsm_7_1), FSM_NORMAL, nullptr, nullptr }

	, { 5,-1, FSM_MATCH_STR(_u8("::")), FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(ls32_fsm) , FSM_ACCEPT, nullptr, nullptr }
};

/* [ *5( h16 ":" ) h16 ] "::"              h16 */
static fsm::transition<string> ipv6address_fsm_8[] = {
	  { 1, 5, FSM_MATCH_FSM(ipv6address_fsm_8_1), FSM_NORMAL, nullptr, nullptr }
	, { 2, 5, FSM_MATCH_FSM(ipv6address_fsm_8_1), FSM_NORMAL, nullptr, nullptr }
	, { 3, 5, FSM_MATCH_FSM(ipv6address_fsm_8_1), FSM_NORMAL, nullptr, nullptr }
	, { 4, 5, FSM_MATCH_FSM(ipv6address_fsm_8_1), FSM_NORMAL, nullptr, nullptr }
	, { 5, 5, FSM_MATCH_FSM(ipv6address_fsm_8_1), FSM_NORMAL, nullptr, nullptr }

	, { 6,-1, FSM_MATCH_STR(_u8("::")) , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(h16_fsm)   , FSM_ACCEPT, nullptr, nullptr }
};


/* [ *6( h16 ":" ) h16 ] "::" */
static fsm::transition<string> ipv6address_fsm_9[] = {
	  { 1, 6, FSM_MATCH_FSM(ipv6address_fsm_9_1), FSM_NORMAL, nullptr, nullptr }
	, { 2, 6, FSM_MATCH_FSM(ipv6address_fsm_9_1), FSM_NORMAL, nullptr, nullptr }
	, { 3, 6, FSM_MATCH_FSM(ipv6address_fsm_9_1), FSM_NORMAL, nullptr, nullptr }
	, { 4, 6, FSM_MATCH_FSM(ipv6address_fsm_9_1), FSM_NORMAL, nullptr, nullptr }
	, { 5, 6, FSM_MATCH_FSM(ipv6address_fsm_9_1), FSM_NORMAL, nullptr, nullptr }
	, { 6, 6, FSM_MATCH_FSM(ipv6address_fsm_9_1), FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_STR(_u8("::"))          , FSM_ACCEPT, nullptr, nullptr }
};


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
static fsm::transition<string> ipv6address_fsm[] = {
	  {-1, 1, FSM_MATCH_FSM(ipv6address_fsm_1), FSM_ACCEPT, nullptr, nullptr }
	, {-1, 2, FSM_MATCH_FSM(ipv6address_fsm_2), FSM_ACCEPT, nullptr, nullptr }
	, {-1, 3, FSM_MATCH_FSM(ipv6address_fsm_3), FSM_ACCEPT, nullptr, nullptr }
	, {-1, 4, FSM_MATCH_FSM(ipv6address_fsm_4), FSM_ACCEPT, nullptr, nullptr }
	, {-1, 5, FSM_MATCH_FSM(ipv6address_fsm_5), FSM_ACCEPT, nullptr, nullptr }
	, {-1, 6, FSM_MATCH_FSM(ipv6address_fsm_6), FSM_ACCEPT, nullptr, nullptr }
	, {-1, 7, FSM_MATCH_FSM(ipv6address_fsm_7), FSM_ACCEPT, nullptr, nullptr }
	, {-1, 8, FSM_MATCH_FSM(ipv6address_fsm_8), FSM_ACCEPT, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(ipv6address_fsm_9), FSM_ACCEPT, nullptr, nullptr }
};

/* "v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" ) */
static fsm::transition<string> ipvfuture_tail_fsm[] = {
	  {-1, 1, FSM_MATCH_FSM(unreserved_fsm), FSM_ACCEPT, nullptr, nullptr }
	, {-1, 2, FSM_MATCH_FSM(sub_delims_fsm), FSM_ACCEPT, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_STR(_u8(":"))      , FSM_ACCEPT, nullptr, nullptr }
};

static fsm::transition<string> ipvfuture_fsm[] = {
      { 1,-1, FSM_MATCH_STR(_u8("v"))                     , FSM_NORMAL, nullptr, nullptr }
	, { 2,-1, FSM_MATCH_RPT_FSM(hexdig_fsm, 1, -1)        , FSM_NORMAL, nullptr, nullptr }
    , { 3,-1, FSM_MATCH_STR(_u8("."))                     , FSM_NORMAL, nullptr, nullptr }
    , {-1,-1, FSM_MATCH_RPT_FSM(ipvfuture_tail_fsm, 1, -1), FSM_ACCEPT, nullptr, nullptr }
};

/* "[" ( IPv6address / IPvFuture  ) "]" */
static fsm::transition<string> ip_literal_entry_fsm[] = {
	  {-1, 1, FSM_MATCH_FSM(ipv6address_fsm), FSM_ACCEPT, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(ipvfuture_fsm)  , FSM_ACCEPT, nullptr, nullptr }
};
static fsm::transition<string> ip_literal_fsm[] = {
	  { 1,-1, FSM_MATCH_STR(_u8("["))            , FSM_NORMAL, nullptr, nullptr }
	, { 2,-1, FSM_MATCH_FSM(ip_literal_entry_fsm), FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_STR(_u8("]"))            , FSM_ACCEPT, nullptr, nullptr }
};

/* *( unreserved / pct-encoded / sub-delims ) */
static fsm::transition<string> reg_name_entry_fsm[] = {
      {-1, 1, FSM_MATCH_FSM(unreserved_fsm),  FSM_ACCEPT, nullptr, nullptr }
    , {-1, 2, FSM_MATCH_FSM(pct_encoded_fsm), FSM_ACCEPT, nullptr, nullptr }
    , {-1,-1, FSM_MATCH_FSM(sub_delims_fsm),  FSM_ACCEPT, nullptr, nullptr }
};
static fsm::transition<string> reg_name_fsm[] = {
      {-1,-1, FSM_MATCH_RPT_FSM(reg_name_entry_fsm, 0, -1), FSM_ACCEPT, nullptr, nullptr }
};

/*  host = IP-literal / IPv4address / reg-name */
static fsm::transition<string> host_fsm[] = {
	  {-1, 1, FSM_MATCH_FSM(ip_literal_fsm), FSM_ACCEPT, set_host_is_ip, nullptr}
	, {-1, 2, FSM_MATCH_FSM(ipv4address_fsm), FSM_ACCEPT, set_host_is_ip, nullptr}

	/* OBSOLETE NOTE: this poses a parsing problem - all IPv4 addresses are valid
	reg_name as well. Fix this by doing the reg_name_fsm now,
	then on match, do a function to check if its an IPv4
	address */
	, {-1,-1, FSM_MATCH_FSM(reg_name_fsm),   FSM_ACCEPT, nullptr, nullptr }
};

/* *DIGIT */
static fsm::transition<string> port_fsm[] = {
      {-1,-1, FSM_MATCH_RPT_FSM(digit_fsm, 0, -1), FSM_ACCEPT, nullptr, nullptr }
};

/* ":" port */
static fsm::transition<string> authority_fsm_2[] = {
	  { 1,-1, FSM_MATCH_STR(_u8(":")), FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(port_fsm), FSM_ACCEPT, set_port, nullptr}
};

/* *( unreserved / pct-encoded / sub-delims / ":" ) */
static fsm::transition<string> userinfo_fsm[] = {
	  { 0, 1, FSM_MATCH_FSM(unreserved_fsm) , FSM_ACCEPT, __TRACE_ACT, __TRACE_ARGS("userinfo_fsm: FSM_MATCH_FSM(unreserved_fsm)") }
	, { 0, 2, FSM_MATCH_FSM(pct_encoded_fsm), FSM_ACCEPT, __TRACE_ACT, __TRACE_ARGS("userinfo_fsm: FSM_MATCH_FSM(pct_encoded_fsm)") }
	, { 0, 3, FSM_MATCH_FSM(sub_delims_fsm) , FSM_ACCEPT, __TRACE_ACT, __TRACE_ARGS("userinfo_fsm: FSM_MATCH_FSM(sub_delims_fsm)") }
	, { 0, 4, FSM_MATCH_STR(_u8(":"))       , FSM_ACCEPT, __TRACE_ACT, __TRACE_ARGS("userinfo_fsm: FSM_MATCH_STR(:)") }
	, {-1,-1, FSM_MATCH_NOTHING             , FSM_ACCEPT, __TRACE_ACT, __TRACE_ARGS("userinfo_fsm: FSM_MATCH_NOTHING") }
};

/* userinfo "@" */
static fsm::transition<string> authority_fsm_1[] = {
	  { 1,-1, FSM_MATCH_FSM(userinfo_fsm), FSM_NORMAL, set_userinfo, nullptr }
	, {-1,-1, FSM_MATCH_STR(_u8("@"))    , FSM_ACCEPT, nullptr, nullptr }
};

/* [ userinfo "@" ] host [ ":" port ] */
/* [ authority_fsm_1 ] host [ authority_fsm_2 ] */
static fsm::transition<string> authority_fsm[] = {
	  { 1, 3, FSM_MATCH_OPT_FSM(authority_fsm_1) , FSM_NORMAL, __TRACE_ACT, __TRACE_ARGS("FSM_MATCH_OPT_FSM(authority_fsm_1)") }
	, { 2, 3, FSM_MATCH_FSM(host_fsm)            , FSM_NORMAL, set_host, nullptr }
	, {-1, 3, FSM_MATCH_OPT_FSM(authority_fsm_2) , FSM_ACCEPT, __TRACE_ACT, __TRACE_ARGS("FSM_MATCH_OPT_FSM(authority_fsm_2)") }
	, {-1,-1, FSM_MATCH_NOTHING                  , FSM_REJECT, unset_userinfo, nullptr }
};

/*  *( "/" segment ) */
static fsm::transition<string> path_abempty_fsm[] = {
      {-1,-1, FSM_MATCH_RPT_FSM(slash_segment_fsm, 0, -1), FSM_ACCEPT, nullptr, nullptr }
};

/*
relative-part = "//" authority path-abempty
              / path-absolute
              / path-noscheme`
              / path-empty
*/
static fsm::transition<string> relative_part_fsm[] = {
	  { 1, 3, FSM_MATCH_STR(_u8("//"))       , FSM_NORMAL, nullptr, nullptr }
	, { 2, 3, FSM_MATCH_FSM(authority_fsm)   , FSM_NORMAL, set_path, nullptr }
	, {-1, 3, FSM_MATCH_FSM(path_abempty_fsm), FSM_ACCEPT, set_path, nullptr }

	, {-1, 4, FSM_MATCH_FSM(path_absolute_fsm), FSM_ACCEPT, set_path, nullptr }
	, {-1, 5, FSM_MATCH_FSM(path_noscheme_fsm), FSM_ACCEPT, set_path, nullptr }
	, {-1,-1, FSM_MATCH_FSM(path_empty_fsm)   , FSM_ACCEPT, set_path, nullptr }
};

/* *( pchar / "/" / "?" ) */
static fsm::transition<string> query_chars_fsm[] = {
	  {-1, 1, FSM_MATCH_FSM(pchar_fsm)  , FSM_ACCEPT, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_CHAR(_u8("/?")) , FSM_ACCEPT, nullptr, nullptr }
};
static fsm::transition<string> query_fsm[] = {
    {-1,-1, FSM_MATCH_RPT_FSM(query_chars_fsm, 0, -1), FSM_ACCEPT, nullptr, nullptr }
};

/* "?" query */
static fsm::transition<string> relative_ref_fsm_1[] = {
	  { 1,-1, FSM_MATCH_STR(_u8("?")) , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(query_fsm), FSM_ACCEPT, set_query, nullptr }
};

/* *( pchar / "/" / "?" ) */
static fsm::transition<string> fragment_fsm[] = {
	{-1,-1, FSM_MATCH_RPT_FSM(query_chars_fsm, 0, -1), FSM_ACCEPT, nullptr, nullptr }
};

/* "#" fragment */
static fsm::transition<string> relative_ref_fsm_2[] = {
	  { 1,-1, FSM_MATCH_STR(_u8("#"))    , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(fragment_fsm), FSM_ACCEPT, set_fragment, nullptr}
};

/* relative-part [ "?" query ] [ "#" fragment ] */
static fsm::transition<string> relative_ref_fsm[] = {
      { 1,-1, FSM_MATCH_FSM(relative_part_fsm)       , FSM_NORMAL, nullptr, nullptr }
    , { 2,-1, FSM_MATCH_RPT_FSM(relative_ref_fsm_1, 0, 1), FSM_NORMAL, nullptr, nullptr }
    , {-1,-1, FSM_MATCH_RPT_FSM(relative_ref_fsm_2, 0, 1), FSM_ACCEPT, nullptr, nullptr }
};

/* ALPHA *( ALPHA / DIGIT / "+" / "-" / "." ) */
static fsm::transition<string> scheme_chars_fsm[] = {
	  {-1, 1, FSM_MATCH_CHAR(_URI_ALPHA) , FSM_ACCEPT, nullptr, nullptr }
	, {-1, 2, FSM_MATCH_CHAR(_URI_DIGIT) , FSM_ACCEPT, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_CHAR(_u8("+-.")) , FSM_ACCEPT, nullptr, nullptr }
};
static fsm::transition<string> scheme_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(_URI_ALPHA), FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_RPT_FSM(scheme_chars_fsm, 0, -1), FSM_ACCEPT, nullptr, nullptr }
};

/* segment-nz *( "/" segment ) */
static fsm::transition<string> path_rootless_fsm[] = {
	  { 1,-1, FSM_MATCH_FSM(segment_nz_fsm)              , FSM_ACCEPT, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_RPT_FSM(slash_segment_fsm, 0, -1), FSM_ACCEPT, nullptr, nullptr }
};

/*
    "//" authority path-abempty
	 	 / path-absolute
	 	 / path-rootless
	 	 / path-empty
*/
static fsm::transition<string> hier_part_fsm[] = {
	  { 1, 3, FSM_MATCH_STR(_u8("//"))       , FSM_NORMAL, nullptr, nullptr }
	, { 2, 3, FSM_MATCH_FSM(authority_fsm)   , FSM_NORMAL, set_path, nullptr }
	, {-1, 3, FSM_MATCH_FSM(path_abempty_fsm), FSM_ACCEPT, set_path, nullptr }

	, {-1, 4, FSM_MATCH_FSM(path_absolute_fsm), FSM_ACCEPT, set_path, nullptr }
	, {-1, 5, FSM_MATCH_FSM(path_rootless_fsm), FSM_ACCEPT, set_path, nullptr }
	, {-1,-1, FSM_MATCH_FSM(path_empty_fsm),    FSM_ACCEPT, set_path, nullptr }
};


/* "?" query */
static fsm::transition<string> uri_fsm_1[] = {
	  { 1,-1, FSM_MATCH_STR(_u8("?")) , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(query_fsm), FSM_ACCEPT, set_query, nullptr }
};

/* "#" fragment */
static fsm::transition<string> uri_fsm_2[] = {
	  { 1,-1, FSM_MATCH_STR(_u8("#"))    , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(fragment_fsm), FSM_ACCEPT, set_fragment, nullptr }
};


/* scheme ":" hier-part [ "?" query ] [ "#" fragment ] */
static fsm::transition<string> uri_fsm[] = {
	  { 1,-1, FSM_MATCH_FSM(scheme_fsm)         , FSM_NORMAL, set_scheme, nullptr }
    , { 2,-1, FSM_MATCH_STR(_u8(":"))           , FSM_NORMAL, nullptr, nullptr }
	, { 3,-1, FSM_MATCH_FSM(hier_part_fsm)      , FSM_NORMAL, nullptr, nullptr }
	, { 4,-1, FSM_MATCH_RPT_FSM(uri_fsm_1, 0, 1), FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_RPT_FSM(uri_fsm_2, 0, 1), FSM_ACCEPT, nullptr, nullptr }
};


/* URI / relative-ref */
static fsm::transition<string> uri_reference_fsm[] = {
	  {-1, 1, FSM_MATCH_FSM(uri_fsm)         , FSM_ACCEPT, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(relative_ref_fsm), FSM_ACCEPT, nullptr, nullptr }
};


static bool set_port (string::const_iterator begin
		, string::const_iterator end
		, void * context
		, void *)
{
	if (context) {
		UriParseContext *ctx = reinterpret_cast<UriParseContext*>(context);
		string digits(begin, end);

		bool ok;
		ctx->uridata->port = lexical_cast<uint16_t>(digits, & ok);

		if (!ok) {
			return false;
		}
	}
	return true;
}

static bool set_host_is_ip (string::const_iterator
		, string::const_iterator
		, void * context
		, void *)
{
	if (context) {
		UriParseContext *ctx = reinterpret_cast<UriParseContext*>(context);
		ctx->uridata->hostIsIp = true;
	}
	return true;
}

static bool unset_userinfo (string::const_iterator
		, string::const_iterator
		, void * context
		, void *)
{
	if (context) {
		UriParseContext *ctx = reinterpret_cast<UriParseContext*>(context);
		ctx->uridata->userinfo.clear();
	}
	return true;
}

static bool set_scheme (string::const_iterator begin
		, string::const_iterator end
		, void * context
		, void *)
{
	if (context) {
		reinterpret_cast<UriParseContext*>(context)->uridata->scheme = string(begin, end);
	}
	return true;
}

static bool set_userinfo (string::const_iterator begin
		, string::const_iterator end
		, void * context
		, void *)
{
	if (context) {
		reinterpret_cast<UriParseContext*>(context)->uridata->userinfo = string(begin, end);
	}
	return true;
}

static bool set_host (string::const_iterator begin
		, string::const_iterator end
		, void * context
		, void *)
{
	if (context) {
		reinterpret_cast<UriParseContext*>(context)->uridata->host = string(begin, end);
	}
	return true;
}

static bool set_path (string::const_iterator begin
		, string::const_iterator end
		, void * context
		, void *)
{
	if (context) {
		reinterpret_cast<UriParseContext*>(context)->uridata->path = string(begin, end);
	}
	return true;
}

static bool set_query (string::const_iterator begin
		, string::const_iterator end
		, void * context
		, void *)
{
	if (context) {
		reinterpret_cast<UriParseContext*>(context)->uridata->query = string(begin, end);
	}
	return true;
}

static bool set_fragment (string::const_iterator begin
		, string::const_iterator end
		, void * context
		, void *)
{
	if (context) {
		reinterpret_cast<UriParseContext*>(context)->uridata->fragment = string(begin, end);
	}
	return true;
}

} // pfs

#endif /* __PFS_URI_RFC3986_HPP__ */
