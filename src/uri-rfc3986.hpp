/*
 * Copyright (c) 2012, Fedor Semenov
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


#ifndef __CWT_URI_RFC3986_HPP__
#define __CWT_URI_RFC3986_HPP__

#include <cwt/fsm.hpp>

CWT_NS_BEGIN

static bool set_scheme(const void *data, size_t len, void *context, void *action_args);
static bool set_query(const void *data, size_t len, void *context, void *action_args);
static bool set_fragment(const void *data, size_t len, void *context, void *action_args);
static bool set_path(const void *data, size_t len, void *context, void *action_args);
static bool set_userinfo(const void *data, size_t len, void *context, void *action_args);
static bool unset_userinfo(const void *data, size_t len, void *context, void *action_args);
static bool set_host(const void *data, size_t len, void *context, void *action_args);
static bool set_port(const void *data, size_t len, void *context, void *action_args);
static bool set_host_is_ip(const void *data, size_t len, void *context, void *action_args);
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

#define FSM_MATCH_CHAR(s) FsmMatch(new FsmMatchStringChar(s))
#define FSM_MATCH_STR(s)  FsmMatch(new FsmMatchStringStr(s))

static String _ALPHA(_U("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));
static String _DIGIT(_U("0123456789"));
static String _HEXDIGIT(_U("0123456789ABCDEFabcdef")); /* DIGIT / "A" / "B" / "C" / "D" / "E" / "F" */

static FsmTransition digit_fsm[] = {
    {-1,-1, FSM_MATCH_CHAR(_DIGIT), FSM_ACCEPT, NULL, NULL }
};

static FsmTransition hexdig_fsm[] = {
    {-1,-1, FSM_MATCH_CHAR(_HEXDIGIT), FSM_ACCEPT, NULL, NULL }
};

/* ALPHA / DIGIT / "-" / "." / "_" / "~" */
static FsmTransition unreserved_fsm[] = {
      {-1, 1, FSM_MATCH_CHAR(_ALPHA)     , FSM_ACCEPT, NULL, NULL }
    , {-1, 2, FSM_MATCH_CHAR(_DIGIT)     , FSM_ACCEPT, NULL, NULL }
    , {-1,-1, FSM_MATCH_CHAR(_U("-._~")) , FSM_ACCEPT, NULL, NULL }
};

/* "%" HEXDIG HEXDIG */
static FsmTransition pct_encoded_fsm[] = {
      { 1,-1, FSM_MATCH_STR(_U("%"))   , FSM_NORMAL, NULL, NULL }
    , { 2,-1, FSM_MATCH_CHAR(_HEXDIGIT), FSM_NORMAL, NULL, NULL }
    , {-1,-1, FSM_MATCH_CHAR(_HEXDIGIT), FSM_ACCEPT, NULL, NULL }
};

/* "!" / "$" / "&" / "'" / "(" / ")"
       / "*" / "+" / "," / ";" / "="
*/
static FsmTransition sub_delims_fsm[] = {
    {-1,-1, FSM_MATCH_CHAR(_U("!$&'()*+,;=")), FSM_ACCEPT, NULL, NULL }
};

/* unreserved / pct-encoded / sub-delims / ":" / "@" */
static FsmTransition pchar_fsm[] = {
      {-1, 1, FSM_MATCH_FSM(unreserved_fsm) , FSM_ACCEPT, NULL, NULL }
    , {-1, 2, FSM_MATCH_FSM(pct_encoded_fsm), FSM_ACCEPT, NULL, NULL }
    , {-1, 3, FSM_MATCH_FSM(sub_delims_fsm) , FSM_ACCEPT, NULL, NULL }
    , {-1,-1, FSM_MATCH_CHAR(_U(":@"))      , FSM_ACCEPT, NULL, NULL }
};


/* 1*pchar */
static FsmTransition segment_nz_fsm[] = {
	  { 1,-1, FSM_MATCH_FSM(pchar_fsm),     FSM_ACCEPT, NULL, NULL }
	, { 1,-1, FSM_MATCH_FSM(pchar_fsm),     FSM_ACCEPT, NULL, NULL }
};

/* *pchar */
static FsmTransition segment_fsm[] = {
      { 0, 1, FSM_MATCH_FSM(pchar_fsm), FSM_ACCEPT, NULL, NULL }
    , {-1,-1, FSM_MATCH_NOTHING,        FSM_ACCEPT, NULL, NULL }
};

/* "/" segment */
static FsmTransition slash_segment_fsm[] = {
      { 1,-1, FSM_MATCH_STR(_U("/"))    , FSM_NORMAL, NULL, NULL }
    , {-1,-1, FSM_MATCH_FSM(segment_fsm), FSM_ACCEPT, NULL, NULL }
};

/* segment-nz *slash_segment */
static FsmTransition segment_nz_slash_fsm[] = {
	  { 1,-1, FSM_MATCH_FSM(segment_nz_fsm)              , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_RPT_FSM(slash_segment_fsm, 0, -1), FSM_ACCEPT, NULL, NULL }
};

/* "/" [ segment-nz *slash_segment ] */
static FsmTransition path_absolute_fsm[] = {
	  { 1,-1, FSM_MATCH_STR(_U("/"))                  , FSM_NORMAL, NULL, NULL }
    , {-1,-1, FSM_MATCH_OPT_FSM(segment_nz_slash_fsm) , FSM_ACCEPT, NULL, NULL }
};

/* 1*( unreserved / pct-encoded / sub-delims / "@" )
   		; non-zero-length segment without any colon ":"
*/
static FsmTransition segment_nc_fsm[] = {
      {-1, 1, FSM_MATCH_FSM(unreserved_fsm) , FSM_ACCEPT, NULL, NULL }
    , {-1, 2, FSM_MATCH_FSM(pct_encoded_fsm), FSM_ACCEPT, NULL, NULL }
    , {-1, 3, FSM_MATCH_FSM(sub_delims_fsm) , FSM_ACCEPT, NULL, NULL }
    , {-1,-1, FSM_MATCH_CHAR(_U("@"))       , FSM_ACCEPT, NULL, NULL }
};

static FsmTransition segment_nz_nc_fsm[] = {
    {-1,-1, FSM_MATCH_RPT_FSM(segment_nc_fsm, 1, -1),  FSM_ACCEPT, NULL, NULL }
};

/* segment-nz-nc *slash-segment */
static FsmTransition path_noscheme_fsm[] = {
      { 1,-1, FSM_MATCH_FSM(segment_nz_nc_fsm), FSM_ACCEPT, NULL, NULL }
    , {-1,-1, FSM_MATCH_RPT_FSM(slash_segment_fsm, 0, -1), FSM_ACCEPT, NULL, NULL }
};

/* 0<pchar> */
static FsmTransition path_empty_fsm[] = {
    {-1,-1, FSM_MATCH_NOTHING, FSM_ACCEPT, NULL, NULL }
};

/* 1*4HEXDIG */
static FsmTransition h16_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(_HEXDIGIT), FSM_ACCEPT, NULL, NULL }
	, { 2,-1, FSM_MATCH_CHAR(_HEXDIGIT), FSM_ACCEPT, NULL, NULL }
	, { 3,-1, FSM_MATCH_CHAR(_HEXDIGIT), FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_CHAR(_HEXDIGIT), FSM_ACCEPT, NULL, NULL }
};

/* "25" %x30-35        ; 250-255 */
static FsmTransition dec_octet_fsm_4[] = {
	  { 1,-1, FSM_MATCH_STR(_U("25")),      FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_CHAR(_U("012345")), FSM_ACCEPT, NULL, NULL }
};

/* "2" %x30-34 DIGIT   ; 200-249 */
static FsmTransition dec_octet_fsm_3[] = {
	  { 1,-1, FSM_MATCH_STR(_U("2"))     , FSM_NORMAL, NULL, NULL }
	, { 2,-1, FSM_MATCH_CHAR(_U("01234")), FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_CHAR(_DIGIT)     , FSM_ACCEPT, NULL, NULL }
};

/* "1" 2DIGIT  ; 100-199 */
static FsmTransition dec_octet_fsm_2[] = {
	  { 1,-1, FSM_MATCH_STR(_U("1")), FSM_NORMAL, NULL, NULL }
	, { 2,-1, FSM_MATCH_CHAR(_DIGIT), FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_CHAR(_DIGIT), FSM_ACCEPT, NULL, NULL }
};

/* %x31-39 DIGIT       ; 10-99*/
static FsmTransition dec_octet_fsm_1[] = {
      { 1,-1, FSM_MATCH_CHAR(_U("123456789")), FSM_NORMAL, NULL, NULL }
    , {-1,-1, FSM_MATCH_CHAR(_DIGIT)         , FSM_ACCEPT, NULL, NULL }
};

/*
  DIGIT                 ; 0-9
  / %x31-39 DIGIT       ; 10-99
  / "1" 2DIGIT          ; 100-199
  / "2" %x30-34 DIGIT   ; 200-249
  / "25" %x30-35        ; 250-255
*/
static FsmTransition dec_octet_fsm[] = {
      {-1, 1, FSM_MATCH_FSM(dec_octet_fsm_4), FSM_ACCEPT, NULL, NULL } /* 250 - 255 */
    , {-1, 2, FSM_MATCH_FSM(dec_octet_fsm_3), FSM_ACCEPT, NULL, NULL } /* 200 - 249 */
    , {-1, 3, FSM_MATCH_FSM(dec_octet_fsm_2), FSM_ACCEPT, NULL, NULL } /* 100 - 199 */
    , {-1, 4, FSM_MATCH_FSM(dec_octet_fsm_1), FSM_ACCEPT, NULL, NULL } /* 10 - 99 */
    , {-1,-1, FSM_MATCH_CHAR(_DIGIT)        , FSM_ACCEPT, NULL, NULL } /* 0 - 9 */
};

/* dec-octet "." dec-octet "." dec-octet "." dec-octet */
static FsmTransition ipv4address_fsm[] = {
	  { 1,-1, FSM_MATCH_FSM(dec_octet_fsm), FSM_NORMAL, NULL, NULL }
	, { 2,-1, FSM_MATCH_STR(_U("."))      , FSM_NORMAL, NULL, NULL }
	, { 3,-1, FSM_MATCH_FSM(dec_octet_fsm), FSM_NORMAL, NULL, NULL }
	, { 4,-1, FSM_MATCH_STR(_U("."))      , FSM_NORMAL, NULL, NULL }
	, { 5,-1, FSM_MATCH_FSM(dec_octet_fsm), FSM_NORMAL, NULL, NULL }
	, { 6,-1, FSM_MATCH_STR(_U("."))      , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(dec_octet_fsm), FSM_ACCEPT, NULL, NULL }
};

/* ( h16 ":" h16 ) / IPv4address */
static FsmTransition ls32_fsm[] = {
	  {-1, 1, FSM_MATCH_FSM(ipv4address_fsm), FSM_ACCEPT, NULL, NULL }
	, { 2,-1, FSM_MATCH_FSM(h16_fsm)        , FSM_NORMAL, NULL, NULL }
	, { 3,-1, FSM_MATCH_STR(_U(":"))        , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(h16_fsm)        , FSM_ACCEPT, NULL, NULL }
};

/* h16 ":" */
static FsmTransition h16_semi_fsm[] = {
	  { 1,-1, FSM_MATCH_FSM(h16_fsm), FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_STR(_U(":")), FSM_ACCEPT, NULL, NULL }
};

/* *1( h16 ":" ) h16 */
static FsmTransition ipv6address_fsm_4_1[] = {
	  { 1,-1, FSM_MATCH_RPT_FSM(h16_semi_fsm, 0, 1), FSM_NORMAL, NULL, NULL }
	, {-1, 2, FSM_MATCH_FSM(h16_fsm)           , FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(h16_fsm)           , FSM_ACCEPT, NULL, NULL }
};

/* *2( h16 ":" ) h16 */
static FsmTransition ipv6address_fsm_5_1[] = {
	  { 1, 2, FSM_MATCH_RPT_FSM(h16_semi_fsm, 0, 2), FSM_NORMAL, NULL, NULL }
	, {-1, 2, FSM_MATCH_FSM(h16_fsm)           , FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(h16_fsm)           , FSM_ACCEPT, NULL, NULL }
};

/* *3( h16 ":" ) h16 */
static FsmTransition ipv6address_fsm_6_1[] = {
	  { 1, 2, FSM_MATCH_RPT_FSM(h16_semi_fsm, 0, 3), FSM_NORMAL, NULL, NULL }
	, {-1, 2, FSM_MATCH_FSM(h16_fsm)           , FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(h16_fsm)           , FSM_ACCEPT, NULL, NULL }
};

/* *4( h16 ":" ) h16 */
static FsmTransition ipv6address_fsm_7_1[] = {
	  { 1, 2, FSM_MATCH_RPT_FSM(h16_semi_fsm, 0, 4), FSM_NORMAL, NULL, NULL }
	, {-1, 2, FSM_MATCH_FSM(h16_fsm)           , FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(h16_fsm)           , FSM_ACCEPT, NULL, NULL }
};

/* *5( h16 ":" ) h16 */
static FsmTransition ipv6address_fsm_8_1[] = {
	  { 1, 2, FSM_MATCH_RPT_FSM(h16_semi_fsm, 0, 5), FSM_NORMAL, NULL, NULL }
	, {-1, 2, FSM_MATCH_FSM(h16_fsm)           , FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(h16_fsm)           , FSM_ACCEPT, NULL, NULL }
};

/* *6( h16 ":" ) h16 */
static FsmTransition ipv6address_fsm_9_1[] = {
	  { 1, 2, FSM_MATCH_RPT_FSM(h16_semi_fsm, 0, 6), FSM_NORMAL, NULL, NULL }
	, {-1, 2, FSM_MATCH_FSM(h16_fsm)           , FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(h16_fsm)           , FSM_ACCEPT, NULL, NULL }
};

/* 6( h16 ":" ) ls32 */
static FsmTransition ipv6address_fsm_1[] = {
      { 1,-1, FSM_MATCH_RPT_FSM(h16_semi_fsm, 6, 6), FSM_NORMAL, NULL, NULL }
    , {-1,-1, FSM_MATCH_FSM(ls32_fsm)          , FSM_ACCEPT, NULL, NULL }
};

/* "::" 5( h16 ":" ) ls32 */
static FsmTransition ipv6address_fsm_2[] = {
	  { 1,-1, FSM_MATCH_STR(_U("::"))          , FSM_NORMAL, NULL, NULL }
	, { 2,-1, FSM_MATCH_RPT_FSM(h16_semi_fsm, 5, 5), FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(ls32_fsm)          , FSM_ACCEPT, NULL, NULL }
};

/* [ h16 ] "::" 4( h16 ":" ) ls32 */
static FsmTransition ipv6address_fsm_3[] = {
      { 1, 1, FSM_MATCH_FSM(h16_fsm)           , FSM_NORMAL, NULL, NULL }
    , { 2,-1, FSM_MATCH_STR(_U("::"))          , FSM_NORMAL, NULL, NULL }
    , { 3,-1, FSM_MATCH_RPT_FSM(h16_semi_fsm, 4, 4), FSM_NORMAL, NULL, NULL }
    , {-1,-1, FSM_MATCH_FSM(ls32_fsm)          , FSM_ACCEPT, NULL, NULL }
};

/* [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32 */
static FsmTransition ipv6address_fsm_4[] = {
      { 1, 1, FSM_MATCH_FSM(ipv6address_fsm_4_1), FSM_NORMAL, NULL, NULL }

    , { 2,-1, FSM_MATCH_STR(_U("::"))           , FSM_NORMAL, NULL, NULL }
    , { 3,-1, FSM_MATCH_FSM(h16_semi_fsm)       , FSM_NORMAL, NULL, NULL }
    , { 4,-1, FSM_MATCH_FSM(h16_semi_fsm)       , FSM_NORMAL, NULL, NULL }
    , { 5,-1, FSM_MATCH_FSM(h16_semi_fsm)       , FSM_NORMAL, NULL, NULL }
    , {-1,-1, FSM_MATCH_FSM(ls32_fsm)           , FSM_ACCEPT, NULL, NULL }
};

/* [ *2( h16 ":" ) h16 ] "::" 2( h16 ":" ) ls32 */
static FsmTransition ipv6address_fsm_5[] = {
	  { 1, 2, FSM_MATCH_FSM(ipv6address_fsm_5_1), FSM_NORMAL, NULL, NULL }
	, { 2, 2, FSM_MATCH_FSM(ipv6address_fsm_5_1), FSM_NORMAL, NULL, NULL }

    , { 3,-1, FSM_MATCH_STR(_U("::"))           , FSM_NORMAL, NULL, NULL }
	, { 4,-1, FSM_MATCH_FSM(h16_semi_fsm)       , FSM_NORMAL, NULL, NULL }
	, { 5,-1, FSM_MATCH_FSM(h16_semi_fsm)       , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(ls32_fsm)           , FSM_ACCEPT, NULL, NULL }
};

/* [ *3( h16 ":" ) h16 ] "::" h16 ":"   ls32 */
static FsmTransition ipv6address_fsm_6[] = {
	  { 1, 3, FSM_MATCH_FSM(ipv6address_fsm_6_1), FSM_NORMAL, NULL, NULL }
	, { 2, 3, FSM_MATCH_FSM(ipv6address_fsm_6_1), FSM_NORMAL, NULL, NULL }
	, { 3, 3, FSM_MATCH_FSM(ipv6address_fsm_6_1), FSM_NORMAL, NULL, NULL }

	, { 4,-1, FSM_MATCH_STR(_U("::"))           , FSM_NORMAL, NULL, NULL }
	, { 5,-1, FSM_MATCH_FSM(h16_semi_fsm)       , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(ls32_fsm)           , FSM_ACCEPT, NULL, NULL }
};


/* [ *4( h16 ":" ) h16 ] "::" ls32 */
static FsmTransition ipv6address_fsm_7[] = {
	  { 1, 4, FSM_MATCH_FSM(ipv6address_fsm_7_1), FSM_NORMAL, NULL, NULL }
	, { 2, 4, FSM_MATCH_FSM(ipv6address_fsm_7_1), FSM_NORMAL, NULL, NULL }
	, { 3, 4, FSM_MATCH_FSM(ipv6address_fsm_7_1), FSM_NORMAL, NULL, NULL }
	, { 4, 4, FSM_MATCH_FSM(ipv6address_fsm_7_1), FSM_NORMAL, NULL, NULL }

	, { 5,-1, FSM_MATCH_STR(_U("::"))           , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(ls32_fsm)           , FSM_ACCEPT, NULL, NULL }
};

/* [ *5( h16 ":" ) h16 ] "::"              h16 */
static FsmTransition ipv6address_fsm_8[] = {
	  { 1, 5, FSM_MATCH_FSM(ipv6address_fsm_8_1), FSM_NORMAL, NULL, NULL }
	, { 2, 5, FSM_MATCH_FSM(ipv6address_fsm_8_1), FSM_NORMAL, NULL, NULL }
	, { 3, 5, FSM_MATCH_FSM(ipv6address_fsm_8_1), FSM_NORMAL, NULL, NULL }
	, { 4, 5, FSM_MATCH_FSM(ipv6address_fsm_8_1), FSM_NORMAL, NULL, NULL }
	, { 5, 5, FSM_MATCH_FSM(ipv6address_fsm_8_1), FSM_NORMAL, NULL, NULL }

	, { 6,-1, FSM_MATCH_STR(_U("::"))           , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(h16_fsm)            , FSM_ACCEPT, NULL, NULL }
};


/* [ *6( h16 ":" ) h16 ] "::" */
static FsmTransition ipv6address_fsm_9[] = {
	  { 1, 6, FSM_MATCH_FSM(ipv6address_fsm_9_1), FSM_NORMAL, NULL, NULL }
	, { 2, 6, FSM_MATCH_FSM(ipv6address_fsm_9_1), FSM_NORMAL, NULL, NULL }
	, { 3, 6, FSM_MATCH_FSM(ipv6address_fsm_9_1), FSM_NORMAL, NULL, NULL }
	, { 4, 6, FSM_MATCH_FSM(ipv6address_fsm_9_1), FSM_NORMAL, NULL, NULL }
	, { 5, 6, FSM_MATCH_FSM(ipv6address_fsm_9_1), FSM_NORMAL, NULL, NULL }
	, { 6, 6, FSM_MATCH_FSM(ipv6address_fsm_9_1), FSM_NORMAL, NULL, NULL }

	, {-1,-1, FSM_MATCH_STR(_U("::"))           , FSM_ACCEPT, NULL, NULL }
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
static FsmTransition ipv6address_fsm[] = {
	  {-1, 1, FSM_MATCH_FSM(ipv6address_fsm_1), FSM_ACCEPT, NULL, NULL }
	, {-1, 2, FSM_MATCH_FSM(ipv6address_fsm_2), FSM_ACCEPT, NULL, NULL }
	, {-1, 3, FSM_MATCH_FSM(ipv6address_fsm_3), FSM_ACCEPT, NULL, NULL }
	, {-1, 4, FSM_MATCH_FSM(ipv6address_fsm_4), FSM_ACCEPT, NULL, NULL }
	, {-1, 5, FSM_MATCH_FSM(ipv6address_fsm_5), FSM_ACCEPT, NULL, NULL }
	, {-1, 6, FSM_MATCH_FSM(ipv6address_fsm_6), FSM_ACCEPT, NULL, NULL }
	, {-1, 7, FSM_MATCH_FSM(ipv6address_fsm_7), FSM_ACCEPT, NULL, NULL }
	, {-1, 8, FSM_MATCH_FSM(ipv6address_fsm_8), FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(ipv6address_fsm_9), FSM_ACCEPT, NULL, NULL }
};

/* "v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" ) */
static FsmTransition ipvfuture_tail_fsm[] = {
	  {-1, 1, FSM_MATCH_FSM(unreserved_fsm), FSM_ACCEPT, NULL, NULL }
	, {-1, 2, FSM_MATCH_FSM(sub_delims_fsm), FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_STR(_U(":"))       , FSM_ACCEPT, NULL, NULL }
};

static FsmTransition ipvfuture_fsm[] = {
      { 1,-1, FSM_MATCH_STR(_U("v"))                  , FSM_NORMAL, NULL, NULL }
	, { 2,-1, FSM_MATCH_RPT_FSM(hexdig_fsm, 1, -1)        , FSM_NORMAL, NULL, NULL }
    , { 3,-1, FSM_MATCH_STR(_U("."))                  , FSM_NORMAL, NULL, NULL }
    , {-1,-1, FSM_MATCH_RPT_FSM(ipvfuture_tail_fsm, 1, -1), FSM_ACCEPT, NULL, NULL }
};

/* "[" ( IPv6address / IPvFuture  ) "]" */
static FsmTransition ip_literal_entry_fsm[] = {
	  {-1, 1, FSM_MATCH_FSM(ipv6address_fsm), FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(ipvfuture_fsm)  , FSM_ACCEPT, NULL, NULL }
};
static FsmTransition ip_literal_fsm[] = {
	  { 1,-1, FSM_MATCH_STR(_U("["))             , FSM_NORMAL, NULL, NULL }
	, { 2,-1, FSM_MATCH_FSM(ip_literal_entry_fsm), FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_STR(_U("]"))             , FSM_ACCEPT, NULL, NULL }
};

/* *( unreserved / pct-encoded / sub-delims ) */
static FsmTransition reg_name_entry_fsm[] = {
      {-1, 1, FSM_MATCH_FSM(unreserved_fsm),  FSM_ACCEPT, NULL, NULL }
    , {-1, 2, FSM_MATCH_FSM(pct_encoded_fsm), FSM_ACCEPT, NULL, NULL }
    , {-1,-1, FSM_MATCH_FSM(sub_delims_fsm),  FSM_ACCEPT, NULL, NULL }
};
static FsmTransition reg_name_fsm[] = {
      {-1,-1, FSM_MATCH_RPT_FSM(reg_name_entry_fsm, 0, -1), FSM_ACCEPT, NULL, NULL }
};

/*  host = IP-literal / IPv4address / reg-name */
static FsmTransition host_fsm[] = {
	  {-1, 1, FSM_MATCH_FSM(ip_literal_fsm), FSM_ACCEPT, set_host_is_ip, NULL}
	, {-1, 2, FSM_MATCH_FSM(ipv4address_fsm), FSM_ACCEPT, set_host_is_ip, NULL}

	/* OBSOLETE NOTE: this poses a parsing problem - all IPv4 addresses are valid
	reg_name as well. Fix this by doing the reg_name_fsm now,
	then on match, do a function to check if its an IPv4
	address */
	, {-1,-1, FSM_MATCH_FSM(reg_name_fsm),   FSM_ACCEPT, NULL, NULL }
};

/* *DIGIT */
static FsmTransition port_fsm[] = {
      {-1,-1, FSM_MATCH_RPT_FSM(digit_fsm, 0, -1), FSM_ACCEPT, NULL, NULL }
};

/* ":" port */
static FsmTransition authority_fsm_2[] = {
	  { 1,-1, FSM_MATCH_STR(_U(":")) , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(port_fsm), FSM_ACCEPT, set_port, NULL}
};

/* *( unreserved / pct-encoded / sub-delims / ":" ) */
static FsmTransition userinfo_fsm[] = {
	  { 0, 1, FSM_MATCH_FSM(unreserved_fsm) , FSM_ACCEPT, NULL, NULL }
	, { 0, 2, FSM_MATCH_FSM(pct_encoded_fsm), FSM_ACCEPT, NULL, NULL }
	, { 0, 3, FSM_MATCH_FSM(sub_delims_fsm) , FSM_ACCEPT, NULL, NULL }
	, { 0, 4, FSM_MATCH_STR(_U(":"))        , FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_NOTHING             , FSM_ACCEPT, NULL, NULL }
};

/* userinfo "@" */
static FsmTransition authority_fsm_1[] = {
	  { 1,-1, FSM_MATCH_FSM(userinfo_fsm), FSM_NORMAL, set_userinfo, NULL }
	, {-1,-1, FSM_MATCH_STR(_U("@"))     , FSM_ACCEPT, NULL, NULL }
};

/* [ userinfo "@" ] host [ ":" port ] */
/* [ authority_fsm_1 ] host [ authority_fsm_2 ] */
static FsmTransition authority_fsm[] = {
	  { 1, 3, FSM_MATCH_RPT_FSM(authority_fsm_1, 0, 1) , FSM_NORMAL, NULL, NULL }
	, { 2, 3, FSM_MATCH_FSM(host_fsm)                  , FSM_NORMAL, set_host, NULL }
	, {-1, 3, FSM_MATCH_RPT_FSM(authority_fsm_2, 0, 1) , FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_NOTHING                        , FSM_REJECT, unset_userinfo, NULL }
};

/*  *( "/" segment ) */
static FsmTransition path_abempty_fsm[] = {
      {-1,-1, FSM_MATCH_RPT_FSM(slash_segment_fsm, 0, -1), FSM_ACCEPT, NULL, NULL }
};

/*
relative-part = "//" authority path-abempty
              / path-absolute
              / path-noscheme`
              / path-empty
*/
static FsmTransition relative_part_fsm[] = {
	  { 1, 3, FSM_MATCH_STR(_U("//"))        , FSM_NORMAL, NULL, NULL }
	, { 2, 3, FSM_MATCH_FSM(authority_fsm)   , FSM_NORMAL, set_path, NULL }
	, {-1, 3, FSM_MATCH_FSM(path_abempty_fsm), FSM_ACCEPT, set_path, NULL }

	, {-1, 4, FSM_MATCH_FSM(path_absolute_fsm), FSM_ACCEPT, set_path, NULL }
	, {-1, 5, FSM_MATCH_FSM(path_noscheme_fsm), FSM_ACCEPT, set_path, NULL }
	, {-1,-1, FSM_MATCH_FSM(path_empty_fsm)   , FSM_ACCEPT, set_path, NULL }
};

/* *( pchar / "/" / "?" ) */
static FsmTransition query_chars_fsm[] = {
	  {-1, 1, FSM_MATCH_FSM(pchar_fsm), FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_CHAR(_U("/?")), FSM_ACCEPT, NULL, NULL }
};
static FsmTransition query_fsm[] = {
    {-1,-1, FSM_MATCH_RPT_FSM(query_chars_fsm, 0, -1), FSM_ACCEPT, NULL, NULL }
};

/* "?" query */
static FsmTransition relative_ref_fsm_1[] = {
	  { 1,-1, FSM_MATCH_STR(_U("?"))  , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(query_fsm), FSM_ACCEPT, set_query, NULL }
};

/* *( pchar / "/" / "?" ) */
static FsmTransition fragment_fsm[] = {
	{-1,-1, FSM_MATCH_RPT_FSM(query_chars_fsm, 0, -1), FSM_ACCEPT, NULL, NULL }
};

/* "#" fragment */
static FsmTransition relative_ref_fsm_2[] = {
	  { 1,-1, FSM_MATCH_STR(_U("#"))     , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(fragment_fsm), FSM_ACCEPT, set_fragment, NULL}
};

/* relative-part [ "?" query ] [ "#" fragment ] */
static FsmTransition relative_ref_fsm[] = {
      { 1,-1, FSM_MATCH_FSM(relative_part_fsm)       , FSM_NORMAL, NULL, NULL }
    , { 2,-1, FSM_MATCH_RPT_FSM(relative_ref_fsm_1, 0, 1), FSM_NORMAL, NULL, NULL }
    , {-1,-1, FSM_MATCH_RPT_FSM(relative_ref_fsm_2, 0, 1), FSM_ACCEPT, NULL, NULL }
};

/* ALPHA *( ALPHA / DIGIT / "+" / "-" / "." ) */
static FsmTransition scheme_chars_fsm[] = {
	  {-1, 1, FSM_MATCH_CHAR(_ALPHA)   , FSM_ACCEPT, NULL, NULL }
	, {-1, 2, FSM_MATCH_CHAR(_DIGIT)   , FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_CHAR(_U("+-.")), FSM_ACCEPT, NULL, NULL }
};
static FsmTransition scheme_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(_ALPHA), FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_RPT_FSM(scheme_chars_fsm, 0, -1), FSM_ACCEPT, NULL, NULL }
};

/* segment-nz *( "/" segment ) */
static FsmTransition path_rootless_fsm[] = {
	  { 1,-1, FSM_MATCH_FSM(segment_nz_fsm)          , FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_RPT_FSM(slash_segment_fsm, 0, -1), FSM_ACCEPT, NULL, NULL }
};

/*
    "//" authority path-abempty
	 	 / path-absolute
	 	 / path-rootless
	 	 / path-empty
*/
static FsmTransition hier_part_fsm[] = {
	  { 1, 3, FSM_MATCH_STR(_U("//"))        , FSM_NORMAL, NULL, NULL }
	, { 2, 3, FSM_MATCH_FSM(authority_fsm)   , FSM_NORMAL, set_path, NULL }
	, {-1, 3, FSM_MATCH_FSM(path_abempty_fsm), FSM_ACCEPT, set_path, NULL }

	, {-1, 4, FSM_MATCH_FSM(path_absolute_fsm), FSM_ACCEPT, set_path, NULL }
	, {-1, 5, FSM_MATCH_FSM(path_rootless_fsm), FSM_ACCEPT, set_path, NULL }
	, {-1,-1, FSM_MATCH_FSM(path_empty_fsm),    FSM_ACCEPT, set_path, NULL }
};


/* "?" query */
static FsmTransition uri_fsm_1[] = {
	  { 1,-1, FSM_MATCH_STR(_U("?"))  , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(query_fsm), FSM_ACCEPT, set_query, NULL }
};

/* "#" fragment */
static FsmTransition uri_fsm_2[] = {
	  { 1,-1, FSM_MATCH_STR(_U("#"))     , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(fragment_fsm), FSM_ACCEPT, set_fragment, NULL }
};


/* scheme ":" hier-part [ "?" query ] [ "#" fragment ] */
static FsmTransition uri_fsm[] = {
	  { 1,-1, FSM_MATCH_FSM(scheme_fsm)     , FSM_NORMAL, set_scheme, NULL }
    , { 2,-1, FSM_MATCH_STR(_U(":"))        , FSM_NORMAL, NULL, NULL }
	, { 3,-1, FSM_MATCH_FSM(hier_part_fsm)  , FSM_NORMAL, NULL, NULL }
	, { 4,-1, FSM_MATCH_RPT_FSM(uri_fsm_1, 0, 1), FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_RPT_FSM(uri_fsm_2, 0, 1), FSM_ACCEPT, NULL, NULL }
};


/* URI / relative-ref */
static FsmTransition uri_reference_fsm[] = {
	  {-1, 1, FSM_MATCH_FSM(uri_fsm),          FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(relative_ref_fsm), FSM_ACCEPT, NULL, NULL }
};

static bool set_port(const void *data, size_t len, void *context, void *action_args)
{
	if (context) {
		UriParseContext *ctx = reinterpret_cast<UriParseContext*>(context);
		String digits((const Char*)data, len);

		CWT_UNUSED(action_args);

		if (data && len > 0) {
			bool ok;
			ctx->uridata->port = digits.toUShort(&ok, 10);
			if (!ok) {
				ctx->status->addError(_Tr("invalid port"));
				return false;
			}
		}
	}
	return true;
}

static bool set_host_is_ip(const void *data, size_t len, void *context, void *action_args)
{
	CWT_UNUSED3(data, len, action_args);
	if (context) {
		UriParseContext *ctx = reinterpret_cast<UriParseContext*>(context);
		ctx->uridata->hostIsIp = true;
	}
	return true;
}

static bool unset_userinfo(const void *data, size_t len, void *context, void *action_args)
{
	CWT_UNUSED3(data, len, action_args);
	if (context) {
		UriParseContext *ctx = reinterpret_cast<UriParseContext*>(context);
		ctx->uridata->userinfo.clear();
	}
	return true;
}

#define _SET_STRING_HELPER(func_name, uri_property)                                           \
		static bool func_name(const void *data, size_t len, void *context, void *action_args) \
		{                                                                                     \
			CWT_UNUSED(action_args);                                                          \
			if (context) {                                                                    \
				UriParseContext *ctx = reinterpret_cast<UriParseContext*>(context);           \
				ctx->uridata->uri_property = String((const Char*)data, len);                  \
			}                                                                                 \
			return true;                                                                      \
		}

_SET_STRING_HELPER(set_scheme, scheme)
_SET_STRING_HELPER(set_userinfo, userinfo)
_SET_STRING_HELPER(set_host, host)
_SET_STRING_HELPER(set_path, path)
_SET_STRING_HELPER(set_query, query)
_SET_STRING_HELPER(set_fragment, fragment)

CWT_NS_END

#endif /* __CWT_URI_RFC3986_HPP__ */
