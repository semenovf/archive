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
#include <pfs/fsm/aliases.hpp>

namespace pfs {

struct parse_context
{
	uri_data   *uridata;
};

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
   reserved      = gen-delims / sub-delims
   gen-delims    = ":" / "/" / "?" / "#" / "[" / "]" / "@"
   sub-delims    = "!" / "$" / "&" / "'" / "(" / ")"
                 / "*" / "+" / "," / ";" / "="
  */

typedef pfs::fsm::m<pfs::string> M;

static string _URI_ALPHA("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
static string _URI_DIGIT("0123456789");
static string _URI_HEXDIGIT("0123456789ABCDEFabcdef"); /* DIGIT / "A" / "B" / "C" / "D" / "E" / "F" */

static fsm::transition<string> digit_fsm[] = {
    {-1,-1, M::chr(_URI_DIGIT).m, FSM_ACCEPT, 0, 0 }
};

static fsm::transition<string> hexdig_fsm[] = {
    {-1,-1, M::chr(_URI_HEXDIGIT).m, FSM_ACCEPT, 0, 0 }
};

/* ALPHA / DIGIT / "-" / "." / "_" / "~" */
static fsm::transition<string> unreserved_fsm[] = {
      {-1, 1, M::chr(_URI_ALPHA).m , FSM_ACCEPT, 0, 0 }
    , {-1, 2, M::chr(_URI_DIGIT).m , FSM_ACCEPT, 0, 0 }
    , {-1,-1, M::chr(_u8("-._~")).m, FSM_ACCEPT, 0, 0 }
};

/* "%" HEXDIG HEXDIG */
static fsm::transition<string> pct_encoded_fsm[] = {
      { 1,-1, M::str(_u8("%")).m      , FSM_NORMAL, 0, 0 }
    , { 2,-1, M::chr(_URI_HEXDIGIT).m, FSM_NORMAL, 0, 0 }
    , {-1,-1, M::chr(_URI_HEXDIGIT).m, FSM_ACCEPT, 0, 0 }
};

/* "!" / "$" / "&" / "'" / "(" / ")"
       / "*" / "+" / "," / ";" / "="
*/
static fsm::transition<string> sub_delims_fsm[] = {
    {-1,-1, M::chr(_u8("!$&'()*+,;=")).m, FSM_ACCEPT, 0, 0 }
};

/* unreserved / pct-encoded / sub-delims / ":" / "@" */
static fsm::transition<string> pchar_fsm[] = {
      {-1, 1, M::tr(unreserved_fsm).m , FSM_ACCEPT, 0, 0 }
    , {-1, 2, M::tr(pct_encoded_fsm).m, FSM_ACCEPT, 0, 0 }
    , {-1, 3, M::tr(sub_delims_fsm).m , FSM_ACCEPT, 0, 0 }
    , {-1,-1, M::chr(_u8(":@")).m     , FSM_ACCEPT, 0, 0 }
};


/* 1*pchar */
static fsm::transition<string> segment_nz_fsm[] = {
	  { 1,-1, M::tr(pchar_fsm).m,     FSM_ACCEPT, 0, 0 }
	, { 1,-1, M::tr(pchar_fsm).m,     FSM_ACCEPT, 0, 0 }
};

/* *pchar */
static fsm::transition<string> segment_fsm[] = {
      { 0, 1, M::tr(pchar_fsm).m, FSM_ACCEPT, 0, 0 }
    , {-1,-1, M::nothing().m    , FSM_ACCEPT, 0, 0 }
};

/* "/" segment */
static fsm::transition<string> slash_segment_fsm[] = {
      { 1,-1, M::str(_u8("/")).m   , FSM_NORMAL, 0, 0 }
    , {-1,-1, M::tr(segment_fsm).m, FSM_ACCEPT, 0, 0 }
};

/* segment-nz *slash_segment */
static fsm::transition<string> segment_nz_slash_fsm[] = {
	  { 1,-1, M::tr(segment_nz_fsm).m              , FSM_NORMAL, 0, 0 }
	, {-1,-1, M::rpt_tr(slash_segment_fsm, 0, -1).m, FSM_ACCEPT, 0, 0 }
};

/* "/" [ segment-nz *slash_segment ] */
static fsm::transition<string> path_absolute_fsm[] = {
	  { 1,-1, M::str(_u8("/")).m                 , FSM_NORMAL, 0, 0 }
    , {-1,-1, M::opt_tr(segment_nz_slash_fsm).m , FSM_ACCEPT, 0, 0 }
};

/* 1*( unreserved / pct-encoded / sub-delims / "@" )
   		; non-zero-length segment without any colon ":"
*/
static fsm::transition<string> segment_nc_fsm[] = {
      {-1, 1, M::tr(unreserved_fsm).m , FSM_ACCEPT, 0, 0 }
    , {-1, 2, M::tr(pct_encoded_fsm).m, FSM_ACCEPT, 0, 0 }
    , {-1, 3, M::tr(sub_delims_fsm).m , FSM_ACCEPT, 0, 0 }
    , {-1,-1, M::chr(_u8("@")).m      , FSM_ACCEPT, 0, 0 }
};

static fsm::transition<string> segment_nz_nc_fsm[] = {
    {-1,-1, M::rpt_tr(segment_nc_fsm, 1, -1).m,  FSM_ACCEPT, 0, 0 }
};

/* segment-nz-nc *slash-segment */
static fsm::transition<string> path_noscheme_fsm[] = {
      { 1,-1, M::tr(segment_nz_nc_fsm).m, FSM_ACCEPT, 0, 0 }
    , {-1,-1, M::rpt_tr(slash_segment_fsm, 0, -1).m, FSM_ACCEPT, 0, 0 }
};

/* 0<pchar> */
static fsm::transition<string> path_empty_fsm[] = {
    {-1,-1, M::nothing().m, FSM_ACCEPT, 0, 0 }
};

/* 1*4HEXDIG */
static fsm::transition<string> h16_fsm[] = {
	  { 1,-1, M::chr(_URI_HEXDIGIT).m, FSM_ACCEPT, 0, 0 }
	, { 2,-1, M::chr(_URI_HEXDIGIT).m, FSM_ACCEPT, 0, 0 }
	, { 3,-1, M::chr(_URI_HEXDIGIT).m, FSM_ACCEPT, 0, 0 }
	, {-1,-1, M::chr(_URI_HEXDIGIT).m, FSM_ACCEPT, 0, 0 }
};

/* "25" %x30-35        ; 250-255 */
static fsm::transition<string> dec_octet_fsm_4[] = {
	  { 1,-1, M::str(_u8("25")).m      , FSM_NORMAL, 0, 0 }
	, {-1,-1, M::chr(_u8("012345")).m , FSM_ACCEPT, 0, 0 }
};

/* "2" %x30-34 DIGIT   ; 200-249 */
static fsm::transition<string> dec_octet_fsm_3[] = {
	  { 1,-1, M::str(_u8("2")).m        , FSM_NORMAL, 0, 0 }
	, { 2,-1, M::chr(_u8("01234")).m   , FSM_NORMAL, 0, 0 }
	, {-1,-1, M::chr(_URI_DIGIT).m, FSM_ACCEPT, 0, 0 }
};

/* "1" 2DIGIT  ; 100-199 */
static fsm::transition<string> dec_octet_fsm_2[] = {
	  { 1,-1, M::str(_u8("1")).m   , FSM_NORMAL, 0, 0 }
	, { 2,-1, M::chr(_URI_DIGIT).m, FSM_NORMAL, 0, 0 }
	, {-1,-1, M::chr(_URI_DIGIT).m, FSM_ACCEPT, 0, 0 }
};

/* %x31-39 DIGIT       ; 10-99*/
static fsm::transition<string> dec_octet_fsm_1[] = {
      { 1,-1, M::chr(_u8("123456789")).m, FSM_NORMAL, 0, 0 }
    , {-1,-1, M::chr(_URI_DIGIT).m , FSM_ACCEPT, 0, 0 }
};

/*
  DIGIT                 ; 0-9
  / %x31-39 DIGIT       ; 10-99
  / "1" 2DIGIT          ; 100-199
  / "2" %x30-34 DIGIT   ; 200-249
  / "25" %x30-35        ; 250-255
*/
static fsm::transition<string> dec_octet_fsm[] = {
      {-1, 1, M::tr(dec_octet_fsm_4).m, FSM_ACCEPT, 0, 0 } /* 250 - 255 */
    , {-1, 2, M::tr(dec_octet_fsm_3).m, FSM_ACCEPT, 0, 0 } /* 200 - 249 */
    , {-1, 3, M::tr(dec_octet_fsm_2).m, FSM_ACCEPT, 0, 0 } /* 100 - 199 */
    , {-1, 4, M::tr(dec_octet_fsm_1).m, FSM_ACCEPT, 0, 0 } /* 10 - 99 */
    , {-1,-1, M::chr(_URI_DIGIT).m        , FSM_ACCEPT, 0, 0 } /* 0 - 9 */
};

/* dec-octet "." dec-octet "." dec-octet "." dec-octet */
static fsm::transition<string> ipv4address_fsm[] = {
	  { 1,-1, M::tr(dec_octet_fsm).m, FSM_NORMAL, 0, 0 }
	, { 2,-1, M::str(_u8(".")).m     , FSM_NORMAL, 0, 0 }
	, { 3,-1, M::tr(dec_octet_fsm).m, FSM_NORMAL, 0, 0 }
	, { 4,-1, M::str(_u8(".")).m     , FSM_NORMAL, 0, 0 }
	, { 5,-1, M::tr(dec_octet_fsm).m, FSM_NORMAL, 0, 0 }
	, { 6,-1, M::str(_u8(".")).m     , FSM_NORMAL, 0, 0 }
	, {-1,-1, M::tr(dec_octet_fsm).m, FSM_ACCEPT, 0, 0 }
};

/* ( h16 ":" h16 ) / IPv4address */
static fsm::transition<string> ls32_fsm[] = {
	  {-1, 1, M::tr(ipv4address_fsm).m, FSM_ACCEPT, 0, 0 }
	, { 2,-1, M::tr(h16_fsm).m        , FSM_NORMAL, 0, 0 }
	, { 3,-1, M::str(_u8(":")).m       , FSM_NORMAL, 0, 0 }
	, {-1,-1, M::tr(h16_fsm).m        , FSM_ACCEPT, 0, 0 }
};

/* h16 ":" */
static fsm::transition<string> h16_semi_fsm[] = {
	  { 1,-1, M::tr(h16_fsm).m  , FSM_NORMAL, 0, 0 }
	, {-1,-1, M::str(_u8(":")).m , FSM_ACCEPT, 0, 0 }
};

/* *1( h16 ":" ) h16 */
static fsm::transition<string> ipv6address_fsm_4_1[] = {
	  { 1,-1, M::rpt_tr(h16_semi_fsm, 0, 1).m, FSM_NORMAL, 0, 0 }
	, {-1, 2, M::tr(h16_fsm).m           , FSM_ACCEPT, 0, 0 }
	, {-1,-1, M::tr(h16_fsm).m           , FSM_ACCEPT, 0, 0 }
};

/* *2( h16 ":" ) h16 */
static fsm::transition<string> ipv6address_fsm_5_1[] = {
	  { 1, 2, M::rpt_tr(h16_semi_fsm, 0, 2).m, FSM_NORMAL, 0, 0 }
	, {-1, 2, M::tr(h16_fsm).m           , FSM_ACCEPT, 0, 0 }
	, {-1,-1, M::tr(h16_fsm).m           , FSM_ACCEPT, 0, 0 }
};

/* *3( h16 ":" ) h16 */
static fsm::transition<string> ipv6address_fsm_6_1[] = {
	  { 1, 2, M::rpt_tr(h16_semi_fsm, 0, 3).m, FSM_NORMAL, 0, 0 }
	, {-1, 2, M::tr(h16_fsm).m           , FSM_ACCEPT, 0, 0 }
	, {-1,-1, M::tr(h16_fsm).m           , FSM_ACCEPT, 0, 0 }
};

/* *4( h16 ":" ) h16 */
static fsm::transition<string> ipv6address_fsm_7_1[] = {
	  { 1, 2, M::rpt_tr(h16_semi_fsm, 0, 4).m, FSM_NORMAL, 0, 0 }
	, {-1, 2, M::tr(h16_fsm).m           , FSM_ACCEPT, 0, 0 }
	, {-1,-1, M::tr(h16_fsm).m           , FSM_ACCEPT, 0, 0 }
};

/* *5( h16 ":" ) h16 */
static fsm::transition<string> ipv6address_fsm_8_1[] = {
	  { 1, 2, M::rpt_tr(h16_semi_fsm, 0, 5).m, FSM_NORMAL, 0, 0 }
	, {-1, 2, M::tr(h16_fsm).m               , FSM_ACCEPT, 0, 0 }
	, {-1,-1, M::tr(h16_fsm).m               , FSM_ACCEPT, 0, 0 }
};

/* *6( h16 ":" ) h16 */
static fsm::transition<string> ipv6address_fsm_9_1[] = {
	  { 1, 2, M::rpt_tr(h16_semi_fsm, 0, 6).m, FSM_NORMAL, 0, 0 }
	, {-1, 2, M::tr(h16_fsm).m               , FSM_ACCEPT, 0, 0 }
	, {-1,-1, M::tr(h16_fsm).m               , FSM_ACCEPT, 0, 0 }
};

/* 6( h16 ":" ) ls32 */
static fsm::transition<string> ipv6address_fsm_1[] = {
      { 1,-1, M::rpt_tr(h16_semi_fsm, 6, 6).m, FSM_NORMAL, 0, 0 }
    , {-1,-1, M::tr(ls32_fsm).m              , FSM_ACCEPT, 0, 0 }
};

/* "::" 5( h16 ":" ) ls32 */
static fsm::transition<string> ipv6address_fsm_2[] = {
	  { 1,-1, M::str(_u8("::")).m             , FSM_NORMAL, 0, 0 }
	, { 2,-1, M::rpt_tr(h16_semi_fsm, 5, 5).m, FSM_NORMAL, 0, 0 }
	, {-1,-1, M::tr(ls32_fsm).m              , FSM_ACCEPT, 0, 0 }
};

/* [ h16 ] "::" 4( h16 ":" ) ls32 */
static fsm::transition<string> ipv6address_fsm_3[] = {
      { 1, 1, M::tr(h16_fsm).m               , FSM_NORMAL, 0, 0 }
    , { 2,-1, M::str(_u8("::")).m             , FSM_NORMAL, 0, 0 }
    , { 3,-1, M::rpt_tr(h16_semi_fsm, 4, 4).m, FSM_NORMAL, 0, 0 }
    , {-1,-1, M::tr(ls32_fsm).m              , FSM_ACCEPT, 0, 0 }
};

/* [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32 */
static fsm::transition<string> ipv6address_fsm_4[] = {
      { 1, 1, M::tr(ipv6address_fsm_4_1).m, FSM_NORMAL, 0, 0 }

    , { 2,-1, M::str(_u8("::")).m    , FSM_NORMAL, 0, 0 }
    , { 3,-1, M::tr(h16_semi_fsm).m , FSM_NORMAL, 0, 0 }
    , { 4,-1, M::tr(h16_semi_fsm).m , FSM_NORMAL, 0, 0 }
    , { 5,-1, M::tr(h16_semi_fsm).m , FSM_NORMAL, 0, 0 }
    , {-1,-1, M::tr(ls32_fsm).m     , FSM_ACCEPT, 0, 0 }
};

/* [ *2( h16 ":" ) h16 ] "::" 2( h16 ":" ) ls32 */
static fsm::transition<string> ipv6address_fsm_5[] = {
	  { 1, 2, M::tr(ipv6address_fsm_5_1).m, FSM_NORMAL, 0, 0 }
	, { 2, 2, M::tr(ipv6address_fsm_5_1).m, FSM_NORMAL, 0, 0 }

    , { 3,-1, M::str(_u8("::")).m    , FSM_NORMAL, 0, 0 }
	, { 4,-1, M::tr(h16_semi_fsm).m , FSM_NORMAL, 0, 0 }
	, { 5,-1, M::tr(h16_semi_fsm).m , FSM_NORMAL, 0, 0 }
	, {-1,-1, M::tr(ls32_fsm).m     , FSM_ACCEPT, 0, 0 }
};

/* [ *3( h16 ":" ) h16 ] "::" h16 ":"   ls32 */
static fsm::transition<string> ipv6address_fsm_6[] = {
	  { 1, 3, M::tr(ipv6address_fsm_6_1).m, FSM_NORMAL, 0, 0 }
	, { 2, 3, M::tr(ipv6address_fsm_6_1).m, FSM_NORMAL, 0, 0 }
	, { 3, 3, M::tr(ipv6address_fsm_6_1).m, FSM_NORMAL, 0, 0 }

	, { 4,-1, M::str(_u8("::")).m    , FSM_NORMAL, 0, 0 }
	, { 5,-1, M::tr(h16_semi_fsm).m , FSM_NORMAL, 0, 0 }
	, {-1,-1, M::tr(ls32_fsm).m     , FSM_ACCEPT, 0, 0 }
};


/* [ *4( h16 ":" ) h16 ] "::" ls32 */
static fsm::transition<string> ipv6address_fsm_7[] = {
	  { 1, 4, M::tr(ipv6address_fsm_7_1).m, FSM_NORMAL, 0, 0 }
	, { 2, 4, M::tr(ipv6address_fsm_7_1).m, FSM_NORMAL, 0, 0 }
	, { 3, 4, M::tr(ipv6address_fsm_7_1).m, FSM_NORMAL, 0, 0 }
	, { 4, 4, M::tr(ipv6address_fsm_7_1).m, FSM_NORMAL, 0, 0 }

	, { 5,-1, M::str(_u8("::")).m, FSM_NORMAL, 0, 0 }
	, {-1,-1, M::tr(ls32_fsm).m , FSM_ACCEPT, 0, 0 }
};

/* [ *5( h16 ":" ) h16 ] "::"              h16 */
static fsm::transition<string> ipv6address_fsm_8[] = {
	  { 1, 5, M::tr(ipv6address_fsm_8_1).m, FSM_NORMAL, 0, 0 }
	, { 2, 5, M::tr(ipv6address_fsm_8_1).m, FSM_NORMAL, 0, 0 }
	, { 3, 5, M::tr(ipv6address_fsm_8_1).m, FSM_NORMAL, 0, 0 }
	, { 4, 5, M::tr(ipv6address_fsm_8_1).m, FSM_NORMAL, 0, 0 }
	, { 5, 5, M::tr(ipv6address_fsm_8_1).m, FSM_NORMAL, 0, 0 }

	, { 6,-1, M::str(_u8("::")).m , FSM_NORMAL, 0, 0 }
	, {-1,-1, M::tr(h16_fsm).m   , FSM_ACCEPT, 0, 0 }
};


/* [ *6( h16 ":" ) h16 ] "::" */
static fsm::transition<string> ipv6address_fsm_9[] = {
	  { 1, 6, M::tr(ipv6address_fsm_9_1).m, FSM_NORMAL, 0, 0 }
	, { 2, 6, M::tr(ipv6address_fsm_9_1).m, FSM_NORMAL, 0, 0 }
	, { 3, 6, M::tr(ipv6address_fsm_9_1).m, FSM_NORMAL, 0, 0 }
	, { 4, 6, M::tr(ipv6address_fsm_9_1).m, FSM_NORMAL, 0, 0 }
	, { 5, 6, M::tr(ipv6address_fsm_9_1).m, FSM_NORMAL, 0, 0 }
	, { 6, 6, M::tr(ipv6address_fsm_9_1).m, FSM_NORMAL, 0, 0 }
	, {-1,-1, M::str(_u8("::")).m          , FSM_ACCEPT, 0, 0 }
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
	  {-1, 1, M::tr(ipv6address_fsm_1).m, FSM_ACCEPT, 0, 0 }
	, {-1, 2, M::tr(ipv6address_fsm_2).m, FSM_ACCEPT, 0, 0 }
	, {-1, 3, M::tr(ipv6address_fsm_3).m, FSM_ACCEPT, 0, 0 }
	, {-1, 4, M::tr(ipv6address_fsm_4).m, FSM_ACCEPT, 0, 0 }
	, {-1, 5, M::tr(ipv6address_fsm_5).m, FSM_ACCEPT, 0, 0 }
	, {-1, 6, M::tr(ipv6address_fsm_6).m, FSM_ACCEPT, 0, 0 }
	, {-1, 7, M::tr(ipv6address_fsm_7).m, FSM_ACCEPT, 0, 0 }
	, {-1, 8, M::tr(ipv6address_fsm_8).m, FSM_ACCEPT, 0, 0 }
	, {-1,-1, M::tr(ipv6address_fsm_9).m, FSM_ACCEPT, 0, 0 }
};

/* "v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" ) */
static fsm::transition<string> ipvfuture_tail_fsm[] = {
	  {-1, 1, M::tr(unreserved_fsm).m, FSM_ACCEPT, 0, 0 }
	, {-1, 2, M::tr(sub_delims_fsm).m, FSM_ACCEPT, 0, 0 }
	, {-1,-1, M::str(_u8(":")).m      , FSM_ACCEPT, 0, 0 }
};

static fsm::transition<string> ipvfuture_fsm[] = {
      { 1,-1, M::str(_u8("v")).m                     , FSM_NORMAL, 0, 0 }
	, { 2,-1, M::rpt_tr(hexdig_fsm, 1, -1).m        , FSM_NORMAL, 0, 0 }
    , { 3,-1, M::str(_u8(".")).m                     , FSM_NORMAL, 0, 0 }
    , {-1,-1, M::rpt_tr(ipvfuture_tail_fsm, 1, -1).m, FSM_ACCEPT, 0, 0 }
};

/* "[" ( IPv6address / IPvFuture  ) "]" */
static fsm::transition<string> ip_literal_entry_fsm[] = {
	  {-1, 1, M::tr(ipv6address_fsm).m, FSM_ACCEPT, 0, 0 }
	, {-1,-1, M::tr(ipvfuture_fsm).m  , FSM_ACCEPT, 0, 0 }
};
static fsm::transition<string> ip_literal_fsm[] = {
	  { 1,-1, M::str(_u8("[")).m            , FSM_NORMAL, 0, 0 }
	, { 2,-1, M::tr(ip_literal_entry_fsm).m, FSM_NORMAL, 0, 0 }
	, {-1,-1, M::str(_u8("]")).m            , FSM_ACCEPT, 0, 0 }
};

/* *( unreserved / pct-encoded / sub-delims ) */
static fsm::transition<string> reg_name_entry_fsm[] = {
      {-1, 1, M::tr(unreserved_fsm).m,  FSM_ACCEPT, 0, 0 }
    , {-1, 2, M::tr(pct_encoded_fsm).m, FSM_ACCEPT, 0, 0 }
    , {-1,-1, M::tr(sub_delims_fsm).m,  FSM_ACCEPT, 0, 0 }
};
static fsm::transition<string> reg_name_fsm[] = {
      {-1,-1, M::rpt_tr(reg_name_entry_fsm, 0, -1).m, FSM_ACCEPT, 0, 0 }
};

/*  host = IP-literal / IPv4address / reg-name */
static fsm::transition<string> host_fsm[] = {
	  {-1, 1, M::tr(ip_literal_fsm).m, FSM_ACCEPT, set_host_is_ip, 0}
	, {-1, 2, M::tr(ipv4address_fsm).m, FSM_ACCEPT, set_host_is_ip, 0}

	/* OBSOLETE NOTE: this poses a parsing problem - all IPv4 addresses are valid
	reg_name as well. Fix this by doing the reg_name_fsm now,
	then on match, do a function to check if its an IPv4
	address */
	, {-1,-1, M::tr(reg_name_fsm).m,   FSM_ACCEPT, 0, 0 }
};

/* *DIGIT */
static fsm::transition<string> port_fsm[] = {
      {-1,-1, M::rpt_tr(digit_fsm, 0, -1).m, FSM_ACCEPT, 0, 0 }
};

/* ":" port */
static fsm::transition<string> authority_fsm_2[] = {
	  { 1,-1, M::str(_u8(":")).m, FSM_NORMAL, 0, 0 }
	, {-1,-1, M::tr(port_fsm).m, FSM_ACCEPT, set_port, 0}
};

/* *( unreserved / pct-encoded / sub-delims / ":" ) */
static fsm::transition<string> userinfo_fsm[] = {
	  { 0, 1, M::tr(unreserved_fsm).m , FSM_ACCEPT, 0, 0 }
	, { 0, 2, M::tr(pct_encoded_fsm).m, FSM_ACCEPT, 0, 0 }
	, { 0, 3, M::tr(sub_delims_fsm).m , FSM_ACCEPT, 0, 0 }
	, { 0, 4, M::str(_u8(":")).m       , FSM_ACCEPT, 0, 0 }
	, {-1,-1, M::nothing().m           , FSM_ACCEPT, 0, 0 }
};

/* userinfo "@" */
static fsm::transition<string> authority_fsm_1[] = {
	  { 1,-1, M::tr(userinfo_fsm).m, FSM_NORMAL, set_userinfo, 0 }
	, {-1,-1, M::str(_u8("@")).m    , FSM_ACCEPT, 0, 0 }
};

/* [ userinfo "@" ] host [ ":" port ] */
/* [ authority_fsm_1 ] host [ authority_fsm_2 ] */
static fsm::transition<string> authority_fsm[] = {
	  { 1, 3, M::opt_tr(authority_fsm_1).m , FSM_NORMAL, 0, 0 }
	, { 2, 3, M::tr(host_fsm).m            , FSM_NORMAL, set_host, 0 }
	, {-1, 3, M::opt_tr(authority_fsm_2).m , FSM_ACCEPT, 0, 0 }
	, {-1,-1, M::nothing().m               , FSM_REJECT, unset_userinfo, 0 }
};

/*  *( "/" segment ) */
static fsm::transition<string> path_abempty_fsm[] = {
      {-1,-1, M::rpt_tr(slash_segment_fsm, 0, -1).m, FSM_ACCEPT, 0, 0 }
};

/*
relative-part = "//" authority path-abempty
              / path-absolute
              / path-noscheme`
              / path-empty
*/
static fsm::transition<string> relative_part_fsm[] = {
	  { 1, 3, M::str(_u8("//")).m       , FSM_NORMAL, 0, 0 }
	, { 2, 3, M::tr(authority_fsm).m   , FSM_NORMAL, set_path, 0 }
	, {-1, 3, M::tr(path_abempty_fsm).m, FSM_ACCEPT, set_path, 0 }

	, {-1, 4, M::tr(path_absolute_fsm).m, FSM_ACCEPT, set_path, 0 }
	, {-1, 5, M::tr(path_noscheme_fsm).m, FSM_ACCEPT, set_path, 0 }
	, {-1,-1, M::tr(path_empty_fsm).m   , FSM_ACCEPT, set_path, 0 }
};

/* *( pchar / "/" / "?" ) */
static fsm::transition<string> query_chars_fsm[] = {
	  {-1, 1, M::tr(pchar_fsm).m  , FSM_ACCEPT, 0, 0 }
	, {-1,-1, M::chr(_u8("/?")).m , FSM_ACCEPT, 0, 0 }
};
static fsm::transition<string> query_fsm[] = {
    {-1,-1, M::rpt_tr(query_chars_fsm, 0, -1).m, FSM_ACCEPT, 0, 0 }
};

/* "?" query */
static fsm::transition<string> relative_ref_fsm_1[] = {
	  { 1,-1, M::str(_u8("?")).m , FSM_NORMAL, 0, 0 }
	, {-1,-1, M::tr(query_fsm).m, FSM_ACCEPT, set_query, 0 }
};

/* *( pchar / "/" / "?" ) */
static fsm::transition<string> fragment_fsm[] = {
	{-1,-1, M::rpt_tr(query_chars_fsm, 0, -1).m, FSM_ACCEPT, 0, 0 }
};

/* "#" fragment */
static fsm::transition<string> relative_ref_fsm_2[] = {
	  { 1,-1, M::str(_u8("#")).m    , FSM_NORMAL, 0, 0 }
	, {-1,-1, M::tr(fragment_fsm).m, FSM_ACCEPT, set_fragment, 0}
};

/* relative-part [ "?" query ] [ "#" fragment ] */
static fsm::transition<string> relative_ref_fsm[] = {
      { 1,-1, M::tr(relative_part_fsm).m       , FSM_NORMAL, 0, 0 }
    , { 2,-1, M::rpt_tr(relative_ref_fsm_1, 0, 1).m, FSM_NORMAL, 0, 0 }
    , {-1,-1, M::rpt_tr(relative_ref_fsm_2, 0, 1).m, FSM_ACCEPT, 0, 0 }
};

/* ALPHA *( ALPHA / DIGIT / "+" / "-" / "." ) */
static fsm::transition<string> scheme_chars_fsm[] = {
	  {-1, 1, M::chr(_URI_ALPHA).m , FSM_ACCEPT, 0, 0 }
	, {-1, 2, M::chr(_URI_DIGIT).m , FSM_ACCEPT, 0, 0 }
	, {-1,-1, M::chr(_u8("+-.")).m , FSM_ACCEPT, 0, 0 }
};
static fsm::transition<string> scheme_fsm[] = {
	  { 1,-1, M::chr(_URI_ALPHA).m, FSM_NORMAL, 0, 0 }
	, {-1,-1, M::rpt_tr(scheme_chars_fsm, 0, -1).m, FSM_ACCEPT, 0, 0 }
};

/* segment-nz *( "/" segment ) */
static fsm::transition<string> path_rootless_fsm[] = {
	  { 1,-1, M::tr(segment_nz_fsm).m              , FSM_ACCEPT, 0, 0 }
	, {-1,-1, M::rpt_tr(slash_segment_fsm, 0, -1).m, FSM_ACCEPT, 0, 0 }
};

/*
    "//" authority path-abempty
	 	 / path-absolute
	 	 / path-rootless
	 	 / path-empty
*/
static fsm::transition<string> hier_part_fsm[] = {
	  { 1, 3, M::str(_u8("//")).m       , FSM_NORMAL, 0, 0 }
	, { 2, 3, M::tr(authority_fsm).m   , FSM_NORMAL, set_path, 0 }
	, {-1, 3, M::tr(path_abempty_fsm).m, FSM_ACCEPT, set_path, 0 }

	, {-1, 4, M::tr(path_absolute_fsm).m, FSM_ACCEPT, set_path, 0 }
	, {-1, 5, M::tr(path_rootless_fsm).m, FSM_ACCEPT, set_path, 0 }
	, {-1,-1, M::tr(path_empty_fsm).m,    FSM_ACCEPT, set_path, 0 }
};


/* "?" query */
static fsm::transition<string> uri_fsm_1[] = {
	  { 1,-1, M::str(_u8("?")).m , FSM_NORMAL, 0, 0 }
	, {-1,-1, M::tr(query_fsm).m, FSM_ACCEPT, set_query, 0 }
};

/* "#" fragment */
static fsm::transition<string> uri_fsm_2[] = {
	  { 1,-1, M::str(_u8("#")).m    , FSM_NORMAL, 0, 0 }
	, {-1,-1, M::tr(fragment_fsm).m, FSM_ACCEPT, set_fragment, 0 }
};


/* scheme ":" hier-part [ "?" query ] [ "#" fragment ] */
static fsm::transition<string> uri_fsm[] = {
	  { 1,-1, M::tr(scheme_fsm).m         , FSM_NORMAL, set_scheme, 0 }
    , { 2,-1, M::str(_u8(":")).m           , FSM_NORMAL, 0, 0 }
	, { 3,-1, M::tr(hier_part_fsm).m      , FSM_NORMAL, 0, 0 }
	, { 4,-1, M::rpt_tr(uri_fsm_1, 0, 1).m, FSM_NORMAL, 0, 0 }
	, {-1,-1, M::rpt_tr(uri_fsm_2, 0, 1).m, FSM_ACCEPT, 0, 0 }
};


/* URI / relative-ref */
static fsm::transition<string> uri_reference_fsm[] = {
	  {-1, 1, M::tr(uri_fsm).m         , FSM_ACCEPT, 0, 0 }
	, {-1,-1, M::tr(relative_ref_fsm).m, FSM_ACCEPT, 0, 0 }
};


static bool set_port (string::const_iterator begin
		, string::const_iterator end
		, void * context
		, void *)
{
	if (context) {
		parse_context *ctx = reinterpret_cast<parse_context*>(context);
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
		parse_context *ctx = reinterpret_cast<parse_context*>(context);
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
		parse_context *ctx = reinterpret_cast<parse_context*>(context);
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
		reinterpret_cast<parse_context*>(context)->uridata->scheme = string(begin, end);
	}
	return true;
}

static bool set_userinfo (string::const_iterator begin
		, string::const_iterator end
		, void * context
		, void *)
{
	if (context) {
		reinterpret_cast<parse_context*>(context)->uridata->userinfo = string(begin, end);
	}
	return true;
}

static bool set_host (string::const_iterator begin
		, string::const_iterator end
		, void * context
		, void *)
{
	if (context) {
		reinterpret_cast<parse_context*>(context)->uridata->host = string(begin, end);
	}
	return true;
}

static bool set_path (string::const_iterator begin
		, string::const_iterator end
		, void * context
		, void *)
{
	if (context) {
		reinterpret_cast<parse_context*>(context)->uridata->path = string(begin, end);
	}
	return true;
}

static bool set_query (string::const_iterator begin
		, string::const_iterator end
		, void * context
		, void *)
{
	if (context) {
		reinterpret_cast<parse_context*>(context)->uridata->query = string(begin, end);
	}
	return true;
}

static bool set_fragment (string::const_iterator begin
		, string::const_iterator end
		, void * context
		, void *)
{
	if (context) {
		reinterpret_cast<parse_context*>(context)->uridata->fragment = string(begin, end);
	}
	return true;
}

} // pfs

#endif /* __PFS_URI_RFC3986_HPP__ */
