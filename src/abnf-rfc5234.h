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
 * abnf-rfc5234.h
 *
 *  Created on: 31.10.2012
 *      Author: Fedor Semenov
 */


#ifndef __CWT_ABNF_RFC5234_H__
#define __CWT_ABNF_RFC5234_H__

#include <cwt/fsm_common.h>

#ifdef __COMMENT__
static void __set_scheme(const void *data, size_t len, void *context, void *action_args);
#endif

/*
   All CRLF entries replaced by NL !!!

   RFC 5234: Augmented BNF for Syntax Specifications: ABNF
   -----------------------------------------------------------------------
         rulelist       =  1*( rule / (*c-wsp c-nl) )

         rule           =  rulename defined-as elements c-nl
                                ; continues if next line starts
                                ;  with white space

         rulename       =  ALPHA *(ALPHA / DIGIT / "-")


         defined-as     =  *c-wsp ("=" / "=/") *c-wsp
                                ; basic rules definition and
                                ;  incremental alternatives

         elements       =  alternation *c-wsp

         c-wsp          =  WSP / (c-nl WSP)

         c-nl           =  comment / NL
                                ; comment or newline

         comment        =  ";" *(WSP / VCHAR) NL

         alternation    =  concatenation
                           *(*c-wsp "/" *c-wsp concatenation)

         concatenation  =  repetition *(1*c-wsp repetition)

         repetition     =  [repeat] element

         repeat         =  1*DIGIT / (*DIGIT "*" *DIGIT)

         element        =  rulename / group / option /
                           char-val / num-val / prose-val

         group          =  "(" *c-wsp alternation *c-wsp ")"

         option         =  "[" *c-wsp alternation *c-wsp "]"

         char-val       =  DQUOTE *(%x20-21 / %x23-7E) DQUOTE
                                ; quoted string of SP and VCHAR
                                ;  without DQUOTE

         num-val        =  "%" (bin-val / dec-val / hex-val)

         bin-val        =  "b" 1*BIT
                           [ 1*("." 1*BIT) / ("-" 1*BIT) ]
                                ; series of concatenated bit values
                                ;  or single ONEOF range

         dec-val        =  "d" 1*DIGIT
                           [ 1*("." 1*DIGIT) / ("-" 1*DIGIT) ]

         hex-val        =  "x" 1*HEXDIG
                           [ 1*("." 1*HEXDIG) / ("-" 1*HEXDIG) ]

         prose-val      =  "<" *(%x20-3D / %x3F-7E) ">"
                                ; bracketed string of SP and VCHAR
                                ;  without angles
                                ; prose description, to be used as
                                ;  last resort
  */


/* "<" *(%x20-3D / %x3F-7E) ">"
                       ; bracketed string of SP and VCHAR
                       ;  without angles
                       ; prose description, to be used as
                       ;  last resort
*/
static CWT_CHAR prose_rg[4] = {(CWT_CHAR)0x20, (CWT_CHAR)0x3D, (CWT_CHAR)0x3F, (CWT_CHAR)0x7E };
static CwtFsmTransition prose_val_fsm[] = {
	  { 1,-1, FSM_MATCH_STR(_T("<"), 1),  FSM_NORMAL, NULL, NULL }
	, { 1, 2, FSM_MATCH_RANGE(&prose_rg[0], &prose_rg[1]), FSM_NORMAL, NULL, NULL }
	, { 1, 3, FSM_MATCH_RANGE(&prose_rg[2], &prose_rg[3]), FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_STR(_T(">"), 1),  FSM_ACCEPT, NULL, NULL }
};


/* ALPHA *(ALPHA / DIGIT / "-") */
static CwtFsmTransition rulename_fsm[] = {
      { 1,-1, FSM_MATCH_INLINE(ALPHA_FSM_INL), FSM_ACCEPT, NULL, NULL }
    , { 1, 2, FSM_MATCH_INLINE(ALPHA_FSM_INL), FSM_NORMAL, NULL, NULL }
    , { 1, 3, FSM_MATCH_INLINE(DIGIT_FSM_INL), FSM_NORMAL, NULL, NULL }
    , { 1, 4, FSM_MATCH_STR(_T("-"), 1),       FSM_NORMAL, NULL, NULL }
    , {-1,-1, FSM_MATCH_NOTHING,               FSM_ACCEPT, NULL, NULL }
};


/* ";" *(WSP / VCHAR) NL */
static CwtFsmTransition comment_fsm[] = {
      { 1,-1, FSM_MATCH_STR(_T(";"), 1),       FSM_ACCEPT, NULL, NULL }
    , { 1, 2, FSM_MATCH_INLINE(WSP_FSM_INL),   FSM_ACCEPT, NULL, NULL }
    , { 1, 3, FSM_MATCH_INLINE(VCHAR_FSM_INL), FSM_ACCEPT, NULL, NULL }
    , {-1, 4, FSM_MATCH_FSM(NL_FSM),           FSM_ACCEPT, NULL, NULL }
    , {-1,-1, FSM_MATCH_NOTHING,         	   FSM_REJECT, NULL, NULL }
};

/* comment / NL ; comment or newline */
static CwtFsmTransition c_nl_fsm[] = {
     {-1, 1, FSM_MATCH_FSM(comment_fsm), FSM_ACCEPT, NULL, NULL }
   , {-1,-1, FSM_MATCH_FSM(NL_FSM),      FSM_ACCEPT, NULL, NULL }
};

/*
 DQUOTE *(%x20-21 / %x23-7E) DQUOTE
                       ; quoted string of SP and VCHAR
                       ;  without DQUOTE
*/
static CWT_CHAR char_val_rg[2] = {(CWT_CHAR)0x23, (CWT_CHAR)0x7E };
static CwtFsmTransition char_val_fsm[] = {
	{ 1,-1, FSM_MATCH_INLINE(DQUOTE_FSM_INL), FSM_NORMAL, NULL, NULL }
  , { 1, 2, FSM_MATCH_RANGE(&char_val_rg[0], &char_val_rg[1]), FSM_NORMAL, NULL, NULL }
  , { 1, 3, FSM_MATCH_CHAR(_T(" !"), 2), FSM_NORMAL, NULL, NULL }
  , {-1,-1, FSM_MATCH_INLINE(DQUOTE_FSM_INL), FSM_ACCEPT, NULL, NULL }
};


/*
"b" 1*BIT [ 1*("." 1*BIT) / ("-" 1*BIT) ]
                       ; series of concatenated bit values
                       ;  or single ONEOF range
*/
/* "." 1*BIT */
static CwtFsmRepetitionContext __bit_rpt = { BIT_FSM, 1, -1 };
/* "-" 1*BIT */
static CwtFsmTransition bit_dash_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(_T("-"), 1), FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_RPT(&__bit_rpt),  FSM_ACCEPT, NULL, NULL }
};
static CwtFsmTransition bit_point_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(_T("."), 1), FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_RPT(&__bit_rpt),  FSM_ACCEPT, NULL, NULL }
};
static CwtFsmRepetitionContext __bit_point_rpt = { bit_point_fsm, 1, -1 };
static CwtFsmTransition bin_val_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(_T("b"), 1),      FSM_NORMAL, NULL, NULL }
	, { 2,-1, FSM_MATCH_RPT(&__bit_rpt),       FSM_ACCEPT, NULL, NULL }
	, { 4, 3, FSM_MATCH_FSM(bit_dash_fsm),     FSM_NORMAL, NULL, NULL }
	, { 4, 4, FSM_MATCH_RPT(&__bit_point_rpt), FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_NOTHING,               FSM_ACCEPT, NULL, NULL }
};


/* "d" 1*DIGIT [ 1*("." 1*DIGIT) / ("-" 1*DIGIT) ] */
static CwtFsmRepetitionContext __dec_rpt = { DIGIT_FSM, 1, -1 };
static CwtFsmTransition dec_dash_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(_T("-"), 1), FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_RPT(&__dec_rpt),  FSM_ACCEPT, NULL, NULL }
};
static CwtFsmTransition dec_point_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(_T("."), 1), FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_RPT(&__dec_rpt),  FSM_ACCEPT, NULL, NULL }
};
static CwtFsmRepetitionContext __dec_point_rpt = { dec_point_fsm, 1, -1 };
static CwtFsmTransition dec_val_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(_T("d"), 1),      FSM_NORMAL, NULL, NULL }
	, { 2,-1, FSM_MATCH_RPT(&__dec_rpt),       FSM_ACCEPT, NULL, NULL }
	, { 4, 3, FSM_MATCH_FSM(dec_dash_fsm),     FSM_NORMAL, NULL, NULL }
	, { 4, 4, FSM_MATCH_RPT(&__dec_point_rpt), FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_NOTHING,               FSM_ACCEPT, NULL, NULL }
};

/* "x" 1*HEXDIG [ 1*("." 1*HEXDIG) / ("-" 1*HEXDIG) ] */
static CwtFsmRepetitionContext __hex_rpt = { HEXDIG_FSM, 1, -1 };
static CwtFsmTransition hex_dash_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(_T("-"), 1), FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_RPT(&__hex_rpt),  FSM_ACCEPT, NULL, NULL }
};
static CwtFsmTransition hex_point_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(_T("."), 1), FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_RPT(&__hex_rpt),  FSM_ACCEPT, NULL, NULL }
};

static CwtFsmRepetitionContext __hex_point_rpt = { hex_point_fsm, 1, -1 };
static CwtFsmTransition hex_val_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(_T("x"), 1),      FSM_NORMAL, NULL, NULL }
	, { 2,-1, FSM_MATCH_RPT(&__hex_rpt),       FSM_ACCEPT, NULL, NULL }
	, { 4, 3, FSM_MATCH_FSM(hex_dash_fsm),     FSM_NORMAL, NULL, NULL }
	, { 4, 4, FSM_MATCH_RPT(&__hex_point_rpt), FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_NOTHING,               FSM_ACCEPT, NULL, NULL }
};


/*  "%" (bin-val / dec-val / hex-val) */
static CwtFsmTransition num_val_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(_T("%"), 1), FSM_NORMAL, NULL, NULL }
	, { 4, 2, FSM_MATCH_FSM(dec_val_fsm), FSM_NORMAL, NULL, NULL }
	, { 4, 3, FSM_MATCH_FSM(hex_val_fsm), FSM_NORMAL, NULL, NULL }
	, { 4,-1, FSM_MATCH_FSM(bin_val_fsm), FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_NOTHING,          FSM_ACCEPT, NULL, NULL }
};

/* WSP / (c-nl WSP) */
static CwtFsmTransition c_wsp_fsm[] = {
     { 1, 2, FSM_MATCH_FSM(c_nl_fsm), FSM_NORMAL, NULL, NULL }
   , {-1, 2, FSM_MATCH_INLINE(WSP_FSM_INL),  FSM_ACCEPT, NULL, NULL }
   , {-1,-1, FSM_MATCH_INLINE(WSP_FSM_INL),  FSM_ACCEPT, NULL, NULL }
};

static CwtFsmRepetitionContext c_wsp_0more_rpt = { c_wsp_fsm,-1,-1 };
static CwtFsmRepetitionContext c_wsp_1more_rpt = { c_wsp_fsm, 1,-1 };


/*
  *c-wsp ("=" / "=/") *c-wsp
							; basic rules definition and
							;  incremental alternatives
*/
static CwtFsmRepetitionContext __defined_as_rpt = { c_wsp_fsm, -1, -1 };
static CwtFsmTransition defined_as_fsm[] = {
	  { 1, 1, FSM_MATCH_RPT(&__defined_as_rpt), FSM_ACCEPT, NULL, NULL }
	, { 3, 2, FSM_MATCH_STR(_T("=/"), 2),       FSM_NORMAL, NULL, NULL }
	, { 3,-1, FSM_MATCH_STR(_T("="), 1),        FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_RPT(&__defined_as_rpt), FSM_ACCEPT, NULL, NULL }
};

static CwtFsmRepetitionContext __dec_1more_rpt = { DIGIT_FSM, 1, -1 };
static CwtFsmRepetitionContext __dec_0more_rpt = { DIGIT_FSM,-1, -1 };
/* *DIGIT "*" *DIGIT */
static CwtFsmTransition repeat_range_fsm[] = {
	  { 1, 1, FSM_MATCH_RPT(&__dec_0more_rpt),  FSM_NORMAL, NULL, NULL }
	, { 2,-1, FSM_MATCH_STR(_T("*"), 1),        FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_RPT(&__dec_0more_rpt),  FSM_ACCEPT, NULL, NULL }
};
/* (*DIGIT "*" *DIGIT) / 1*DIGIT  */
static CwtFsmTransition repeat_fsm[] = {
	  {-1, 1, FSM_MATCH_FSM(repeat_range_fsm),  FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_RPT(&__dec_1more_rpt),  FSM_ACCEPT, NULL, NULL }
};

static CwtFsmTransition element_fsm[];
/* [repeat] element */
static CwtFsmTransition repetition_fsm[] = {
	  { 1, 1, FSM_MATCH_FSM(repeat_fsm),  FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(element_fsm), FSM_ACCEPT, NULL, NULL }
};

/* repetition *(1*c-wsp repetition) */
/* 1*c-wsp repetition */
static CwtFsmTransition repetition_c_wsp_fsm[] = {
	  { 1,-1, FSM_MATCH_RPT(&c_wsp_1more_rpt), FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(repetition_fsm),   FSM_ACCEPT, NULL, NULL }
};
static CwtFsmTransition concatenation_fsm[] = {
	  { 1,-1, FSM_MATCH_FSM(repetition_fsm),       FSM_ACCEPT, NULL, NULL }
	, { 1,-1, FSM_MATCH_FSM(repetition_c_wsp_fsm), FSM_ACCEPT, NULL, NULL }
};


/* concatenation *(*c-wsp "/" *c-wsp concatenation) */
/* *c-wsp "/" *c-wsp concatenation */
static CwtFsmTransition concatenation_c_wsp_fsm[] = {
	  { 1,-1, FSM_MATCH_RPT(&c_wsp_0more_rpt),  FSM_NORMAL, NULL, NULL }
	, { 2,-1, FSM_MATCH_STR(_T("/"), 1),        FSM_NORMAL, NULL, NULL }
	, { 3,-1, FSM_MATCH_RPT(&c_wsp_0more_rpt),  FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(concatenation_fsm), FSM_ACCEPT, NULL, NULL }
};

static CwtFsmTransition alternation_fsm[] = {
	  { 1,-1, FSM_MATCH_FSM(concatenation_fsm),       FSM_ACCEPT, NULL, NULL }
	, { 1,-1, FSM_MATCH_FSM(concatenation_c_wsp_fsm), FSM_ACCEPT, NULL, NULL }
};

/* alternation *c-wsp */
static CwtFsmTransition elements_fsm[] = {
	  { 1,-1, FSM_MATCH_FSM(alternation_fsm),   FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_RPT(&c_wsp_0more_rpt),  FSM_ACCEPT, NULL, NULL }
};

/* "[" *c-wsp alternation *c-wsp "]" */
static CwtFsmTransition option_fsm[] = {
	  { 1,-1, FSM_MATCH_STR(_T("["), 1),       FSM_NORMAL, NULL, NULL }
	, { 2,-1, FSM_MATCH_RPT(&c_wsp_0more_rpt), FSM_NORMAL, NULL, NULL }
	, { 3,-1, FSM_MATCH_FSM(alternation_fsm),  FSM_NORMAL, NULL, NULL }
	, { 4,-1, FSM_MATCH_RPT(&c_wsp_0more_rpt), FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_STR(_T("]"), 1),       FSM_ACCEPT, NULL, NULL }
};

/* "(" *c-wsp alternation *c-wsp ")" */
static CwtFsmTransition group_fsm[] = {
	  { 1,-1, FSM_MATCH_STR(_T("("), 1),       FSM_NORMAL, NULL, NULL }
	, { 2,-1, FSM_MATCH_RPT(&c_wsp_0more_rpt), FSM_NORMAL, NULL, NULL }
	, { 3,-1, FSM_MATCH_FSM(alternation_fsm),  FSM_NORMAL, NULL, NULL }
	, { 4,-1, FSM_MATCH_RPT(&c_wsp_0more_rpt), FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_STR(_T(")"), 1),       FSM_ACCEPT, NULL, NULL }
};

/* rulename / group / option / char-val / num-val / prose-val */
static CwtFsmTransition element_fsm[] = {
	  {-1, 1, FSM_MATCH_FSM(rulename_fsm),  FSM_ACCEPT, NULL, NULL }
	, {-1, 2, FSM_MATCH_FSM(group_fsm),     FSM_ACCEPT, NULL, NULL }
	, {-1, 3, FSM_MATCH_FSM(option_fsm),    FSM_ACCEPT, NULL, NULL }
	, {-1, 4, FSM_MATCH_FSM(char_val_fsm),  FSM_ACCEPT, NULL, NULL }
	, {-1, 5, FSM_MATCH_FSM(num_val_fsm),   FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(prose_val_fsm), FSM_ACCEPT, NULL, NULL }
};

/*
 rulename defined-as elements c-nl
                       ; continues if next line starts
                       ;  with white space
*/
static CwtFsmTransition rule_fsm[] = {
	  { 1,-1, FSM_MATCH_FSM(rulename_fsm),   FSM_NORMAL, NULL, NULL }
	, { 2,-1, FSM_MATCH_FSM(defined_as_fsm), FSM_NORMAL, NULL, NULL }
	, { 3,-1, FSM_MATCH_FSM(elements_fsm),   FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(c_nl_fsm),       FSM_ACCEPT, NULL, NULL }
};

/* 1*( rule / (*c-wsp c-nl) ) */
/* *c-wsp c-nl */
static CwtFsmTransition c_wsp_nl_fsm[] = {
	  { 1,-1, FSM_MATCH_RPT(&c_wsp_0more_rpt), FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(c_nl_fsm),         FSM_ACCEPT, NULL, NULL }
};

/* rule / (*c-wsp c-nl) */
static CwtFsmTransition rule_c_wsp_nl_fsm[] = {
	  {-1, 1, FSM_MATCH_FSM(c_wsp_nl_fsm), FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(rule_fsm),     FSM_ACCEPT, NULL, NULL }
};

static CwtFsmRepetitionContext rulelist_1more_rpt = { rule_c_wsp_nl_fsm, 1, -1 };
static CwtFsmTransition rulelist_fsm[] = {
	{-1,-1, FSM_MATCH_RPT(&rulelist_1more_rpt), FSM_ACCEPT, NULL, NULL }
};

#endif /* __CWT_ABNF_RFC5234_H__ */
