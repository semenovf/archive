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


#ifndef __CWT_URI_RFC3986_H__
#define __CWT_URI_RFC3986_H__

#include <cwt/fsm_common.h>

static void __set_scheme(const void *data, size_t len, void *context, void *action_args);
static void __set_query(const void *data, size_t len, void *context, void *action_args);
static void __set_fragment(const void *data, size_t len, void *context, void *action_args);
static void __set_path(const void *data, size_t len, void *context, void *action_args);
static void __set_userinfo(const void *data, size_t len, void *context, void *action_args);
static void __unset_userinfo(const void *data, size_t len, void *context, void *action_args);
static void __set_host(const void *data, size_t len, void *context, void *action_args);
static void __set_port(const void *data, size_t len, void *context, void *action_args);
static void __set_host_is_ip(const void *data, size_t len, void *context, void *action_args);
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

static CwtFsmRptBounds rpt_0more = { 0, -1 };
static CwtFsmRptBounds rpt_1more = { 1, -1 };
static CwtFsmRptBounds rpt_01 = { 0, 1 };
static CwtFsmRptBounds rpt_02 = { 0, 2 };
static CwtFsmRptBounds rpt_03 = { 0, 3 };
static CwtFsmRptBounds rpt_04 = { 0, 4 };
static CwtFsmRptBounds rpt_05 = { 0, 5 };
static CwtFsmRptBounds rpt_06 = { 0, 6 };
/*
static CwtFsmRptBounds rpt_11 = { 1, 1 };
static CwtFsmRptBounds rpt_22 = { 2, 2 };
static CwtFsmRptBounds rpt_33 = { 3, 3 };
*/
static CwtFsmRptBounds rpt_44 = { 4, 4 };
static CwtFsmRptBounds rpt_55 = { 5, 5 };
static CwtFsmRptBounds rpt_66 = { 6, 6 };

/* ALPHA / DIGIT / "-" / "." / "_" / "~" */
static CwtFsmTransition unreserved_fsm[] = {
      {-1, 1, FSM_MATCH_INLINE(ALPHA_FSM_INL), FSM_ACCEPT, NULL, NULL }
    , {-1, 2, FSM_MATCH_INLINE(DIGIT_FSM_INL), FSM_ACCEPT, NULL, NULL }
    , {-1,-1, FSM_MATCH_CHAR(_T("-._~"), 4),   FSM_ACCEPT, NULL, NULL }
};

/* "%" HEXDIG HEXDIG */
static CwtFsmTransition pct_encoded_fsm[] = {
      { 1,-1, FSM_MATCH_STR(_T("%"), 1), FSM_NORMAL, NULL, NULL }
    , { 2,-1, FSM_MATCH_FSM(HEXDIG_FSM), FSM_NORMAL, NULL, NULL }
    , {-1,-1, FSM_MATCH_FSM(HEXDIG_FSM), FSM_ACCEPT, NULL, NULL }
};

/* "!" / "$" / "&" / "'" / "(" / ")"
       / "*" / "+" / "," / ";" / "="
*/
static CwtFsmTransition sub_delims_fsm[] = {
    {-1,-1, FSM_MATCH_CHAR(_T("!$&'()*+,;="), 11), FSM_ACCEPT, NULL, NULL }
};

/* unreserved / pct-encoded / sub-delims / ":" / "@" */
static CwtFsmTransition pchar_fsm[] = {
      {-1, 1, FSM_MATCH_FSM(unreserved_fsm),  FSM_ACCEPT, NULL, NULL }
    , {-1, 2, FSM_MATCH_FSM(pct_encoded_fsm), FSM_ACCEPT, NULL, NULL }
    , {-1, 3, FSM_MATCH_FSM(sub_delims_fsm),  FSM_ACCEPT, NULL, NULL }
    , {-1,-1, FSM_MATCH_CHAR(_T(":@"), 2),    FSM_ACCEPT, NULL, NULL }
};


/* 1*pchar */
static CwtFsmTransition segment_nz_fsm[] = {
	  { 1,-1, FSM_MATCH_FSM(pchar_fsm),     FSM_ACCEPT, NULL, NULL }
	, { 1,-1, FSM_MATCH_FSM(pchar_fsm),     FSM_ACCEPT, NULL, NULL }
};

/* *pchar */
static CwtFsmTransition segment_fsm[] = {
      { 0, 1, FSM_MATCH_FSM(pchar_fsm), FSM_ACCEPT, NULL, NULL }
    , {-1,-1, FSM_MATCH_NOTHING,        FSM_ACCEPT, NULL, NULL }
};

/* "/" segment */
static CwtFsmTransition slash_segment_fsm[] = {
      { 1,-1, FSM_MATCH_STR(_T("/"), 1),        FSM_NORMAL, NULL, NULL }
    , {-1,-1, FSM_MATCH_FSM(segment_fsm),       FSM_ACCEPT, NULL, NULL }
};

/* "/" [ segment-nz *( "/" segment ) ] */
static CwtFsmTransition path_absolute_fsm[] = {
	  { 1, 3, FSM_MATCH_STR(_T("/"), 1),     FSM_NORMAL, NULL, NULL }
    , { 2, 3, FSM_MATCH_FSM(segment_nz_fsm), FSM_NORMAL, NULL, NULL }
    , {-1, 3, FSM_MATCH_RPT(slash_segment_fsm, &rpt_0more), FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_STR(_T("/"), 1),     FSM_ACCEPT, NULL, NULL }
};

/* 1*( unreserved / pct-encoded / sub-delims / "@" )
   		; non-zero-length segment without any colon ":"
*/
static CwtFsmTransition segment_nc_fsm[] = {
      {-1, 1, FSM_MATCH_FSM(unreserved_fsm),  FSM_ACCEPT, NULL, NULL }
    , {-1, 2, FSM_MATCH_FSM(pct_encoded_fsm), FSM_ACCEPT, NULL, NULL }
    , {-1, 3, FSM_MATCH_FSM(sub_delims_fsm),  FSM_ACCEPT, NULL, NULL }
    , {-1,-1, FSM_MATCH_CHAR(_T("@"), 1),     FSM_ACCEPT, NULL, NULL }
};

static CwtFsmTransition segment_nz_nc_fsm[] = {
    {-1,-1, FSM_MATCH_RPT(segment_nc_fsm, &rpt_1more),  FSM_ACCEPT, NULL, NULL }
};


/* segment-nz-nc *slash-segment */
static CwtFsmTransition path_noscheme_fsm[] = {
      { 1,-1, FSM_MATCH_FSM(segment_nz_nc_fsm), FSM_ACCEPT, NULL, NULL }
    , {-1,-1, FSM_MATCH_RPT(slash_segment_fsm, &rpt_0more), FSM_ACCEPT, NULL, NULL }
};

/* 0<pchar> */
static CwtFsmTransition path_empty_fsm[] = {
    {-1,-1, FSM_MATCH_NOTHING, FSM_ACCEPT, NULL, NULL }
};

/* 1*4HEXDIG */
static CwtFsmTransition h16_fsm[] = {
	  { 1,-1, FSM_MATCH_FSM(HEXDIG_FSM), FSM_ACCEPT, NULL, NULL }
	, { 2,-1, FSM_MATCH_FSM(HEXDIG_FSM), FSM_ACCEPT, NULL, NULL }
	, { 3,-1, FSM_MATCH_FSM(HEXDIG_FSM), FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(HEXDIG_FSM), FSM_ACCEPT, NULL, NULL }
};


/* "25" %x30-35        ; 250-255 */
static CwtFsmTransition dec_octet_fsm_4[] = {
	  { 1,-1, FSM_MATCH_STR(_T("25"), 2),      FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_CHAR(_T("012345"), 6), FSM_ACCEPT, NULL, NULL }
};

/* "2" %x30-34 DIGIT   ; 200-249 */
static CwtFsmTransition dec_octet_fsm_3[] = {
	  { 1,-1, FSM_MATCH_STR(_T("2"), 1),       FSM_NORMAL, NULL, NULL }
	, { 2,-1, FSM_MATCH_CHAR(_T("01234"), 5),  FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_INLINE(DIGIT_FSM_INL), FSM_ACCEPT, NULL, NULL }
};

/* "1" 2DIGIT  ; 100-199 */
static CwtFsmTransition dec_octet_fsm_2[] = {
	  { 1,-1, FSM_MATCH_STR(_T("1"), 1), FSM_NORMAL, NULL, NULL }
	, { 2,-1, FSM_MATCH_INLINE(DIGIT_FSM_INL),  FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_INLINE(DIGIT_FSM_INL),  FSM_ACCEPT, NULL, NULL }
};

/* %x31-39 DIGIT       ; 10-99*/
static CwtFsmTransition dec_octet_fsm_1[] = {
      { 1,-1, FSM_MATCH_CHAR(_T("123456789"), 9), FSM_NORMAL, NULL, NULL }
    , {-1,-1, FSM_MATCH_INLINE(DIGIT_FSM_INL),    FSM_ACCEPT, NULL, NULL }
};

/*
  DIGIT                 ; 0-9
  / %x31-39 DIGIT       ; 10-99
  / "1" 2DIGIT          ; 100-199
  / "2" %x30-34 DIGIT   ; 200-249
  / "25" %x30-35        ; 250-255
*/
static CwtFsmTransition dec_octet_fsm[] = {
      {-1, 1, FSM_MATCH_FSM(dec_octet_fsm_4),  FSM_ACCEPT, NULL, NULL } /* 250 - 255 */
    , {-1, 2, FSM_MATCH_FSM(dec_octet_fsm_3),  FSM_ACCEPT, NULL, NULL } /* 200 - 249 */
    , {-1, 3, FSM_MATCH_FSM(dec_octet_fsm_2),  FSM_ACCEPT, NULL, NULL } /* 100 - 199 */
    , {-1, 4, FSM_MATCH_FSM(dec_octet_fsm_1),  FSM_ACCEPT, NULL, NULL } /* 10 - 99 */
    , {-1,-1, FSM_MATCH_INLINE(DIGIT_FSM_INL), FSM_ACCEPT, NULL, NULL } /* 0 - 9 */
};

/* dec-octet "." dec-octet "." dec-octet "." dec-octet */
static CwtFsmTransition ipv4address_fsm[] = {
	  { 1,-1, FSM_MATCH_FSM(dec_octet_fsm), FSM_NORMAL, NULL, NULL }
	, { 2,-1, FSM_MATCH_STR(_T("."), 1),    FSM_NORMAL, NULL, NULL }
	, { 3,-1, FSM_MATCH_FSM(dec_octet_fsm), FSM_NORMAL, NULL, NULL }
	, { 4,-1, FSM_MATCH_STR(_T("."), 1),    FSM_NORMAL, NULL, NULL }
	, { 5,-1, FSM_MATCH_FSM(dec_octet_fsm), FSM_NORMAL, NULL, NULL }
	, { 6,-1, FSM_MATCH_STR(_T("."), 1),    FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(dec_octet_fsm), FSM_ACCEPT, NULL, NULL }
};

/* ( h16 ":" h16 ) / IPv4address */
static CwtFsmTransition ls32_fsm[] = {
	  {-1, 1, FSM_MATCH_FSM(ipv4address_fsm), FSM_ACCEPT, NULL, NULL }
	, { 2,-1, FSM_MATCH_FSM(h16_fsm),         FSM_NORMAL, NULL, NULL }
	, { 3,-1, FSM_MATCH_STR(_T(":"), 1),      FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(h16_fsm),         FSM_ACCEPT, NULL, NULL }
};

/* h16 ":" */
static CwtFsmTransition h16_semi_fsm[] = {
	  { 1,-1, FSM_MATCH_FSM(h16_fsm),    FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_STR(_T(":"), 1), FSM_ACCEPT, NULL, NULL }
};

/* *1( h16 ":" ) h16 */
static CwtFsmTransition ipv6address_fsm_4_1[] = {
	  { 1, 2, FSM_MATCH_RPT(h16_semi_fsm, &rpt_01), FSM_NORMAL, NULL, NULL }
	, {-1, 2, FSM_MATCH_FSM(h16_fsm),               FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(h16_fsm),               FSM_ACCEPT, NULL, NULL }
};

/* *2( h16 ":" ) h16 */
static CwtFsmTransition ipv6address_fsm_5_1[] = {
	  { 1, 2, FSM_MATCH_RPT(h16_semi_fsm, &rpt_02), FSM_NORMAL, NULL, NULL }
	, {-1, 2, FSM_MATCH_FSM(h16_fsm),               FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(h16_fsm),               FSM_ACCEPT, NULL, NULL }
};

/* *3( h16 ":" ) h16 */
static CwtFsmTransition ipv6address_fsm_6_1[] = {
	  { 1, 2, FSM_MATCH_RPT(h16_semi_fsm, &rpt_03), FSM_NORMAL, NULL, NULL }
	, {-1, 2, FSM_MATCH_FSM(h16_fsm),               FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(h16_fsm),               FSM_ACCEPT, NULL, NULL }
};

/* *4( h16 ":" ) h16 */
static CwtFsmTransition ipv6address_fsm_7_1[] = {
	  { 1, 2, FSM_MATCH_RPT(h16_semi_fsm, &rpt_04), FSM_NORMAL, NULL, NULL }
	, {-1, 2, FSM_MATCH_FSM(h16_fsm),               FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(h16_fsm),               FSM_ACCEPT, NULL, NULL }
};

/* *5( h16 ":" ) h16 */
static CwtFsmTransition ipv6address_fsm_8_1[] = {
	  { 1, 2, FSM_MATCH_RPT(h16_semi_fsm, &rpt_05), FSM_NORMAL, NULL, NULL }
	, {-1, 2, FSM_MATCH_FSM(h16_fsm),               FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(h16_fsm),               FSM_ACCEPT, NULL, NULL }
};

/* *6( h16 ":" ) h16 */
static CwtFsmTransition ipv6address_fsm_9_1[] = {
	  { 1, 2, FSM_MATCH_RPT(h16_semi_fsm, &rpt_06), FSM_NORMAL, NULL, NULL }
	, {-1, 2, FSM_MATCH_FSM(h16_fsm),               FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(h16_fsm),               FSM_ACCEPT, NULL, NULL }
};

/* 6( h16 ":" ) ls32 */
static CwtFsmTransition ipv6address_fsm_1[] = {
      { 1,-1, FSM_MATCH_RPT(h16_semi_fsm, &rpt_66), FSM_NORMAL, NULL, NULL }
    , {-1,-1, FSM_MATCH_FSM(ls32_fsm),     FSM_ACCEPT, NULL, NULL }
};

/* "::" 5( h16 ":" ) ls32 */
static CwtFsmTransition ipv6address_fsm_2[] = {
	  { 1,-1, FSM_MATCH_STR(_T("::"), 2),  FSM_NORMAL, NULL, NULL }
	, { 2,-1, FSM_MATCH_RPT(h16_semi_fsm, &rpt_55), FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(ls32_fsm),     FSM_ACCEPT, NULL, NULL }
};

/* [ h16 ] "::" 4( h16 ":" ) ls32 */
static CwtFsmTransition ipv6address_fsm_3[] = {
      { 1, 1, FSM_MATCH_FSM(h16_fsm),      FSM_NORMAL, NULL, NULL }
    , { 2,-1, FSM_MATCH_STR(_T("::"), 2),  FSM_NORMAL, NULL, NULL }
    , { 3,-1, FSM_MATCH_RPT(h16_semi_fsm, &rpt_44), FSM_NORMAL, NULL, NULL }
    , {-1,-1, FSM_MATCH_FSM(ls32_fsm),     FSM_ACCEPT, NULL, NULL }
};

/* [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32 */
static CwtFsmTransition ipv6address_fsm_4[] = {
      { 1, 1, FSM_MATCH_FSM(ipv6address_fsm_4_1), FSM_NORMAL, NULL, NULL }

    , { 2,-1, FSM_MATCH_STR(_T("::"), 2),         FSM_NORMAL, NULL, NULL }
    , { 3,-1, FSM_MATCH_FSM(h16_semi_fsm),        FSM_NORMAL, NULL, NULL }
    , { 4,-1, FSM_MATCH_FSM(h16_semi_fsm),        FSM_NORMAL, NULL, NULL }
    , { 5,-1, FSM_MATCH_FSM(h16_semi_fsm),        FSM_NORMAL, NULL, NULL }
    , {-1,-1, FSM_MATCH_FSM(ls32_fsm),            FSM_ACCEPT, NULL, NULL }
};

/* [ *2( h16 ":" ) h16 ] "::" 2( h16 ":" ) ls32 */
static CwtFsmTransition ipv6address_fsm_5[] = {
	  { 1, 2, FSM_MATCH_FSM(ipv6address_fsm_5_1), FSM_NORMAL, NULL, NULL }
	, { 2, 2, FSM_MATCH_FSM(ipv6address_fsm_5_1), FSM_NORMAL, NULL, NULL }

    , { 3,-1, FSM_MATCH_STR(_T("::"), 2),         FSM_NORMAL, NULL, NULL }
	, { 4,-1, FSM_MATCH_FSM(h16_semi_fsm),        FSM_NORMAL, NULL, NULL }
	, { 5,-1, FSM_MATCH_FSM(h16_semi_fsm),        FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(ls32_fsm),            FSM_ACCEPT, NULL, NULL }
};

/* [ *3( h16 ":" ) h16 ] "::" h16 ":"   ls32 */
static CwtFsmTransition ipv6address_fsm_6[] = {
	  { 1, 3, FSM_MATCH_FSM(ipv6address_fsm_6_1), FSM_NORMAL, NULL, NULL }
	, { 2, 3, FSM_MATCH_FSM(ipv6address_fsm_6_1), FSM_NORMAL, NULL, NULL }
	, { 3, 3, FSM_MATCH_FSM(ipv6address_fsm_6_1), FSM_NORMAL, NULL, NULL }

	, { 4,-1, FSM_MATCH_STR(_T("::"), 2),         FSM_NORMAL, NULL, NULL }
	, { 5,-1, FSM_MATCH_FSM(h16_semi_fsm),        FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(ls32_fsm),            FSM_ACCEPT, NULL, NULL }
};


/* [ *4( h16 ":" ) h16 ] "::" ls32 */
static CwtFsmTransition ipv6address_fsm_7[] = {
	  { 1, 4, FSM_MATCH_FSM(ipv6address_fsm_7_1), FSM_NORMAL, NULL, NULL }
	, { 2, 4, FSM_MATCH_FSM(ipv6address_fsm_7_1), FSM_NORMAL, NULL, NULL }
	, { 3, 4, FSM_MATCH_FSM(ipv6address_fsm_7_1), FSM_NORMAL, NULL, NULL }
	, { 4, 4, FSM_MATCH_FSM(ipv6address_fsm_7_1), FSM_NORMAL, NULL, NULL }

	, { 5,-1, FSM_MATCH_STR(_T("::"), 2),         FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(ls32_fsm),            FSM_ACCEPT, NULL, NULL }
};

/* [ *5( h16 ":" ) h16 ] "::"              h16 */
static CwtFsmTransition ipv6address_fsm_8[] = {
	  { 1, 5, FSM_MATCH_FSM(ipv6address_fsm_8_1), FSM_NORMAL, NULL, NULL }
	, { 2, 5, FSM_MATCH_FSM(ipv6address_fsm_8_1), FSM_NORMAL, NULL, NULL }
	, { 3, 5, FSM_MATCH_FSM(ipv6address_fsm_8_1), FSM_NORMAL, NULL, NULL }
	, { 4, 5, FSM_MATCH_FSM(ipv6address_fsm_8_1), FSM_NORMAL, NULL, NULL }
	, { 5, 5, FSM_MATCH_FSM(ipv6address_fsm_8_1), FSM_NORMAL, NULL, NULL }

	, { 6,-1, FSM_MATCH_STR(_T("::"), 2),         FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(h16_fsm),             FSM_ACCEPT, NULL, NULL }
};


/* [ *6( h16 ":" ) h16 ] "::" */
static CwtFsmTransition ipv6address_fsm_9[] = {
	  { 1, 6, FSM_MATCH_FSM(ipv6address_fsm_9_1), FSM_NORMAL, NULL, NULL }
	, { 2, 6, FSM_MATCH_FSM(ipv6address_fsm_9_1), FSM_NORMAL, NULL, NULL }
	, { 3, 6, FSM_MATCH_FSM(ipv6address_fsm_9_1), FSM_NORMAL, NULL, NULL }
	, { 4, 6, FSM_MATCH_FSM(ipv6address_fsm_9_1), FSM_NORMAL, NULL, NULL }
	, { 5, 6, FSM_MATCH_FSM(ipv6address_fsm_9_1), FSM_NORMAL, NULL, NULL }
	, { 6, 6, FSM_MATCH_FSM(ipv6address_fsm_9_1), FSM_NORMAL, NULL, NULL }

	, {-1,-1, FSM_MATCH_STR(_T("::"), 2),         FSM_ACCEPT, NULL, NULL }
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
static CwtFsmTransition ipv6address_fsm[] = {
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
static CwtFsmTransition ipvfuture_tail_fsm[] = {
	  {-1, 1, FSM_MATCH_FSM(unreserved_fsm), FSM_ACCEPT, NULL, NULL }
	, {-1, 2, FSM_MATCH_FSM(sub_delims_fsm), FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_STR(_T(":"), 1),     FSM_ACCEPT, NULL, NULL }
};
static CwtFsmTransition ipvfuture_fsm[] = {
      { 1,-1, FSM_MATCH_STR(_T("v"), 1),                     FSM_NORMAL, NULL, NULL }
	, { 2,-1, FSM_MATCH_RPT(HEXDIG_FSM, &rpt_1more),         FSM_NORMAL, NULL, NULL }
    , { 3,-1, FSM_MATCH_STR(_T("."), 1),                     FSM_NORMAL, NULL, NULL }
    , {-1,-1, FSM_MATCH_RPT(ipvfuture_tail_fsm, &rpt_1more), FSM_ACCEPT, NULL, NULL }
};

/* "[" ( IPv6address / IPvFuture  ) "]" */
static CwtFsmTransition ip_literal_entry_fsm[] = {
	  {-1, 1, FSM_MATCH_FSM(ipv6address_fsm), FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(ipvfuture_fsm),   FSM_ACCEPT, NULL, NULL }
};
static CwtFsmTransition ip_literal_fsm[] = {
	  { 1,-1, FSM_MATCH_STR(_T("["), 1),       FSM_NORMAL, NULL, NULL }
	, { 2,-1, FSM_MATCH_FSM(ip_literal_entry_fsm), FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_STR(_T("]"), 1),       FSM_ACCEPT, NULL, NULL }
};

/* *( unreserved / pct-encoded / sub-delims ) */
static CwtFsmTransition reg_name_entry_fsm[] = {
      {-1, 1, FSM_MATCH_FSM(unreserved_fsm),  FSM_ACCEPT, NULL, NULL }
    , {-1, 2, FSM_MATCH_FSM(pct_encoded_fsm), FSM_ACCEPT, NULL, NULL }
    , {-1,-1, FSM_MATCH_FSM(sub_delims_fsm),  FSM_ACCEPT, NULL, NULL }
};
static CwtFsmTransition reg_name_fsm[] = {
      {-1,-1, FSM_MATCH_RPT(reg_name_entry_fsm, &rpt_0more), FSM_ACCEPT, NULL, NULL }
};

/*  host = IP-literal / IPv4address / reg-name */
static CwtFsmTransition host_fsm[] = {
	  {-1, 1, FSM_MATCH_FSM(ip_literal_fsm), FSM_ACCEPT, __set_host_is_ip, NULL}
	, {-1, 2, FSM_MATCH_FSM(ipv4address_fsm), FSM_ACCEPT, __set_host_is_ip, NULL}

	/* OBSOLETE NOTE: this poses a parsing problem - all IPv4 addresses are valid
	reg_name as well. Fix this by doing the reg_name_fsm now,
	then on match, do a function to check if its an IPv4
	address */
	, {-1,-1, FSM_MATCH_FSM(reg_name_fsm),   FSM_ACCEPT, NULL, NULL }
};

/* *DIGIT */
static CwtFsmTransition port_fsm[] = {
      { 0, 1, FSM_MATCH_INLINE(DIGIT_FSM_INL), FSM_ACCEPT, NULL, NULL }
    , {-1,-1, FSM_MATCH_NOTHING,               FSM_ACCEPT, NULL, NULL }
};


/* ":" port */
static CwtFsmTransition authority_fsm_2[] = {
	  { 1,-1, FSM_MATCH_STR(_T(":"), 1), FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(port_fsm),   FSM_ACCEPT, __set_port, NULL}
};

/* *( unreserved / pct-encoded / sub-delims / ":" ) */
static CwtFsmTransition userinfo_fsm[] = {
	  { 0, 1, FSM_MATCH_FSM(unreserved_fsm),  FSM_ACCEPT, NULL, NULL }
	, { 0, 2, FSM_MATCH_FSM(pct_encoded_fsm), FSM_ACCEPT, NULL, NULL }
	, { 0, 3, FSM_MATCH_FSM(sub_delims_fsm),  FSM_ACCEPT, NULL, NULL }
	, { 0, 4, FSM_MATCH_STR(_T(":"), 1),      FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_NOTHING,              FSM_ACCEPT, NULL, NULL }
};

/* userinfo "@" */
static CwtFsmTransition authority_fsm_1[] = {
	  { 1,-1, FSM_MATCH_FSM(userinfo_fsm), FSM_NORMAL, __set_userinfo, NULL }
	, {-1,-1, FSM_MATCH_STR(_T("@"), 1),   FSM_ACCEPT, NULL, NULL }
};

/* [ userinfo "@" ] host [ ":" port ] */
/* [ authority_fsm_1 ] host [ authority_fsm_2 ] */
static CwtFsmTransition authority_fsm[] = {
	  { 1, 3, FSM_MATCH_RPT(authority_fsm_1, &rpt_01), FSM_NORMAL, NULL, NULL }
	, { 2, 3, FSM_MATCH_FSM(host_fsm),                 FSM_NORMAL, __set_host, NULL }
	, {-1, 3, FSM_MATCH_RPT(authority_fsm_2, &rpt_01), FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_NOTHING,                       FSM_REJECT, __unset_userinfo, NULL }
};

/*  *( "/" segment ) */
static CwtFsmTransition path_abempty_fsm[] = {
      {-1,-1, FSM_MATCH_RPT(slash_segment_fsm, &rpt_0more), FSM_ACCEPT, NULL, NULL }
};

/*
relative-part = "//" authority path-abempty
              / path-absolute
              / path-noscheme`
              / path-empty
*/
static CwtFsmTransition relative_part_fsm[] = {
	  { 1, 3, FSM_MATCH_STR(_T("//"), 2),       FSM_NORMAL, NULL, NULL }
	, { 2, 3, FSM_MATCH_FSM(authority_fsm),     FSM_NORMAL, __set_path, NULL }
	, {-1, 3, FSM_MATCH_FSM(path_abempty_fsm),  FSM_ACCEPT, __set_path, NULL }

	, {-1, 4, FSM_MATCH_FSM(path_absolute_fsm), FSM_ACCEPT, __set_path, NULL }
	, {-1, 5, FSM_MATCH_FSM(path_noscheme_fsm), FSM_ACCEPT, __set_path, NULL }
	, {-1,-1, FSM_MATCH_FSM(path_empty_fsm),    FSM_ACCEPT, __set_path, NULL }
};

/* *( pchar / "/" / "?" ) */
static CwtFsmTransition query_chars_fsm[] = {
	  {-1, 1, FSM_MATCH_FSM(pchar_fsm),    FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_CHAR(_T("/?"), 2), FSM_ACCEPT, NULL, NULL }
};
static CwtFsmTransition query_fsm[] = {
    {-1,-1, FSM_MATCH_RPT(query_chars_fsm, &rpt_0more), FSM_ACCEPT, NULL, NULL }
};

/* "?" query */
static CwtFsmTransition relative_ref_fsm_1[] = {
	  { 1,-1, FSM_MATCH_STR(_T("?"), 1), FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(query_fsm),  FSM_ACCEPT, __set_query, NULL }
};

/* *( pchar / "/" / "?" ) */
static CwtFsmTransition fragment_fsm[] = {
	{-1,-1, FSM_MATCH_RPT(query_chars_fsm, &rpt_0more), FSM_ACCEPT, NULL, NULL }
};

/* "#" fragment */
static CwtFsmTransition relative_ref_fsm_2[] = {
	  { 1,-1, FSM_MATCH_STR(_T("#"), 1),   FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(fragment_fsm), FSM_ACCEPT, __set_fragment, NULL}
};

/* relative-part [ "?" query ] [ "#" fragment ] */
static CwtFsmTransition relative_ref_fsm[] = {
      { 1,-1, FSM_MATCH_FSM(relative_part_fsm),           FSM_NORMAL, NULL, NULL }
    , { 2,-1, FSM_MATCH_RPT(relative_ref_fsm_1, &rpt_01), FSM_NORMAL, NULL, NULL }
    , {-1,-1, FSM_MATCH_RPT(relative_ref_fsm_2, &rpt_01), FSM_ACCEPT, NULL, NULL }
};

/* ALPHA *( ALPHA / DIGIT / "+" / "-" / "." ) */
static CwtFsmTransition scheme_chars_fsm[] = {
	  {-1, 1, FSM_MATCH_INLINE(ALPHA_FSM_INL), FSM_ACCEPT, NULL, NULL }
	, {-1, 2, FSM_MATCH_INLINE(DIGIT_FSM_INL), FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_CHAR(_T("+-."), 3),    FSM_ACCEPT, NULL, NULL }
};
static CwtFsmTransition scheme_fsm[] =
{
	  { 1,-1, FSM_MATCH_INLINE(ALPHA_FSM_INL), FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_RPT(scheme_chars_fsm, &rpt_0more), FSM_ACCEPT, NULL, NULL }
};

/* segment-nz *( "/" segment ) */
static CwtFsmTransition path_rootless_fsm[] = {
	  { 1,-1, FSM_MATCH_FSM(segment_nz_fsm),                FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_RPT(slash_segment_fsm, &rpt_0more), FSM_ACCEPT, NULL, NULL }
};

/*
	   "//" authority path-abempty
	 / path-absolute
	 / path-rootless
	 / path-empty
*/
static CwtFsmTransition hier_part_fsm[] = {
	  { 1, 3, FSM_MATCH_STR(_T("//"), 2),       FSM_NORMAL, NULL, NULL }
	, { 2, 3, FSM_MATCH_FSM(authority_fsm),     FSM_NORMAL, __set_path, NULL }
	, {-1, 3, FSM_MATCH_FSM(path_abempty_fsm),  FSM_ACCEPT, __set_path, NULL }

	, {-1, 4, FSM_MATCH_FSM(path_absolute_fsm), FSM_ACCEPT, __set_path, NULL }
	, {-1, 5, FSM_MATCH_FSM(path_rootless_fsm), FSM_ACCEPT, __set_path, NULL }
	, {-1,-1, FSM_MATCH_FSM(path_empty_fsm),    FSM_ACCEPT, __set_path, NULL }
};


/* "?" query */
static CwtFsmTransition uri_fsm_1[] = {
	  { 1,-1, FSM_MATCH_STR(_T("?"), 1), FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(query_fsm),  FSM_ACCEPT, __set_query, NULL }
};

/* "#" fragment */
static CwtFsmTransition uri_fsm_2[] = {
	  { 1,-1, FSM_MATCH_STR(_T("#"), 1),   FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(fragment_fsm), FSM_ACCEPT, __set_fragment, NULL }
};


/* scheme ":" hier-part [ "?" query ] [ "#" fragment ] */
static CwtFsmTransition uri_fsm[] = {
	  { 1,-1, FSM_MATCH_FSM(scheme_fsm),    FSM_NORMAL, __set_scheme, NULL }
    , { 2,-1, FSM_MATCH_STR(_T(":"), 1),    FSM_NORMAL, NULL, NULL }
	, { 3,-1, FSM_MATCH_FSM(hier_part_fsm), FSM_NORMAL, NULL, NULL }
	, { 4,-1, FSM_MATCH_RPT(uri_fsm_1, &rpt_01), FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_RPT(uri_fsm_2, &rpt_01), FSM_ACCEPT, NULL, NULL }
};


/* URI / relative-ref */
static CwtFsmTransition uri_reference_fsm[] = {
	  {-1, 1, FSM_MATCH_FSM(uri_fsm),          FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(relative_ref_fsm), FSM_ACCEPT, NULL, NULL }
};

static BOOL __parse_uint_digits(const CWT_CHAR *s, size_t len, int radix, UINT *d)
{
	CwtStrNS *strNS = cwt_str_ns();
	BOOL ok;
	CWT_CHAR buf[64];
	UINT n;
	size_t i;

	strNS->strNcpy(buf, s, len);
	buf[len] = 0;

	for( i = 0; i < len; i++ )
		if( !strNS->isDigit(buf[i]) )
			return FALSE;

	n = strNS->toUINT(buf, radix, &ok);

	if( ok && d )
		*d = n;
	return ok;
}


static void __set_port(const void *data, size_t len, void *context, void *action_args)
{
	if( context ) {
		CwtUri *uri = (CwtUri*)context;
		UINT port = 0;

		CWT_UNUSED(action_args);

		if( __parse_uint_digits((const CWT_CHAR*)data, len, 10, &port) )
			uri->port = (UINT16)port;
	}
}

static void __set_host_is_ip(const void *data, size_t len, void *context, void *action_args)
{
	if( context ) {
		CwtUri *uri = (CwtUri*)context;
		CWT_UNUSED(data);
		CWT_UNUSED(len);
		CWT_UNUSED(action_args);

		uri->host_is_ip = TRUE;
	}
}

/*
static void __check_host_is_ip(const void *data, size_t len, void *context, void *action_args)
{
	ssize_t ret;
	CwtFsm fsm;

	FSM_INIT(fsm, CWT_CHAR, ipv4address_fsm, NULL, cwtBelongCwtChar, cwtExactCwtChar, cwtRangeCwtChar);
	ret = cwtFsmNS()->exec(&fsm, 0, data, len);

	if( ret > 0 && (size_t)ret == len ) {
		__set_host_is_ip(data, len, context, action_args);
	}
}
*/


static void __set_scheme(const void *data, size_t len, void *context, void *action_args)
{
	if( context ) {
		CwtUri *uri = (CwtUri*)context;
		CWT_UNUSED(action_args);
		if( uri->scheme )
			CWT_FREE(uri->scheme);
		uri->scheme = cwt_str_ns()->strNdup((const CWT_CHAR*)data, len);
	}
}

static void __unset_userinfo(const void *data, size_t len, void *context, void *action_args)
{
	if( context ) {
		CwtUri *uri = (CwtUri*)context;

		CWT_UNUSED(data);
		CWT_UNUSED(len);
		CWT_UNUSED(action_args);

		if( uri->userinfo ) {
			CWT_FREE(uri->userinfo);
			uri->userinfo = NULL;
		}
	}
}

static void __set_uri_var(CWT_CHAR **pvar, const void *data, size_t len, void *action_args)
{
	CWT_UNUSED(action_args);
	CWT_ASSERT(pvar);

	if( *pvar )
		CWT_FREE(*pvar);

	*pvar = cwt_str_ns()->strNdup((const CWT_CHAR*)data, len);
}


static void __set_userinfo(const void *data, size_t len, void *context, void *action_args)
{
	if( context )
		__set_uri_var(&((CwtUri*)context)->userinfo, data, len, action_args);
}


static void __set_host(const void *data, size_t len, void *context, void *action_args)
{
	if( context )
		__set_uri_var(&((CwtUri*)context)->host, data, len, action_args);
}

static void __set_path(const void *data, size_t len, void *context, void *action_args)
{
	if( context )
		__set_uri_var(&((CwtUri*)context)->path, data, len, action_args);
}

static void __set_query(const void *data, size_t len, void *context, void *action_args)
{
	if( context )
		__set_uri_var(&((CwtUri*)context)->query, data, len, action_args);
}

static void __set_fragment(const void *data, size_t len, void *context, void *action_args)
{
	if( context )
		__set_uri_var(&((CwtUri*)context)->fragment, data, len, action_args);
}

#endif /* __CWT_URI_RFC3986_H__ */
