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

#include <cwt/fsm.hpp>

CWT_NS_BEGIN

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


#define FSM_MATCH_CHAR(s)         FsmMatch(new FsmMatchStringChar(s))
#define FSM_MATCH_STR(s)          FsmMatch(new FsmMatchStringStr(s))
#define FSM_MATCH_RPT_CHAR(s,f,t) FsmMatch(new FsmMatchRpt(FSM_MATCH_CHAR(s),f,t))
#define FSM_MATCH_OPT_CHAR(s)     FsmMatch(new FsmMatchRpt(FSM_MATCH_CHAR(s),0,1))

String _ALPHA(_U("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));
String _DIGIT(_U("0123456789"));
String _BIT(_U("01"));
String _DQUOTE(_U("\""));
String _WSP(_U(" \t"));
String _NL(_U("\n\r"));


static FsmTransition nl_fsm[] = {
	  {-1, 1, FSM_MATCH_STR(_U("\r\n")), FSM_ACCEPT, NULL, NULL } /* Windows/DOS */
	, {-1, 2, FSM_MATCH_STR(_U("\n\r")), FSM_ACCEPT, NULL, NULL } /* Mac OS 9 */
	, {-1,-1, FSM_MATCH_STR(_U("\n"))  , FSM_ACCEPT, NULL, NULL } /* Unix */
};


/*
"<" *(%x20-3D / %x3F-7E) ">"
			; bracketed string of SP and VCHAR
			;  without angles
			; prose description, to be used as
			;  last resort
*/
static Char prose_rg[] = {0x20, 0x3D, 0x3F, 0x7E };
static FsmTransition prose_val_opt_fsm[] = {
	  {-1, 1, FSM_MATCH_RANGE(&prose_rg[0], &prose_rg[1]), FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_RANGE(&prose_rg[2], &prose_rg[3]), FSM_ACCEPT, NULL, NULL }
};
static FsmTransition prose_val_fsm[] = {
	  { 1,-1, FSM_MATCH_STR(_U("<"))                     , FSM_NORMAL, NULL, NULL }
	, { 2,-1, FSM_MATCH_RPT_FSM(prose_val_opt_fsm, 0,-1) , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_STR(_U(">"))                     , FSM_ACCEPT, NULL, NULL }
};

/* ALPHA *(ALPHA / DIGIT / "-") */
static FsmTransition rulename_opt_fsm[] = {
      {-1, 1, FSM_MATCH_CHAR(_ALPHA) , FSM_ACCEPT, NULL, NULL }
    , {-1, 2, FSM_MATCH_CHAR(_DIGIT) , FSM_ACCEPT, NULL, NULL }
    , {-1,-1, FSM_MATCH_CHAR(_U("-")), FSM_ACCEPT, NULL, NULL }
};
static FsmTransition rulename_fsm[] = {
      { 1,-1, FSM_MATCH_CHAR(_ALPHA)                    , FSM_NORMAL, NULL, NULL }
    , {-1,-1, FSM_MATCH_RPT_FSM(rulename_opt_fsm, 0,-1) , FSM_ACCEPT, NULL, NULL }
};

/* ";" *(WSP / VCHAR) NL */
/* VCHAR = %x21-7E ; visible (printing) characters */
static Char vchar_rg[2] = {0x21, 0x7E };
static FsmTransition wsp_vchar_fsm[] = {
	  {-1, 1, FSM_MATCH_CHAR(_WSP)                       , FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_RANGE(&vchar_rg[0], &vchar_rg[1]), FSM_ACCEPT, NULL, NULL }
};
static FsmTransition comment_fsm[] = {
      { 1,-1, FSM_MATCH_CHAR(_U(";"))                , FSM_NORMAL, NULL, NULL }
    , { 2,-1, FSM_MATCH_RPT_FSM(wsp_vchar_fsm, 0,-1) , FSM_NORMAL, NULL, NULL }
    , {-1,-1, FSM_MATCH_FSM(nl_fsm)                  , FSM_ACCEPT, NULL, NULL }
};

/* comment / NL ; comment or newline */
static FsmTransition c_nl_fsm[] = {
     {-1, 1, FSM_MATCH_FSM(comment_fsm) , FSM_ACCEPT, NULL, NULL }
   , {-1,-1, FSM_MATCH_FSM(nl_fsm)      , FSM_ACCEPT, NULL, NULL }
};

/*
 DQUOTE *(%x20-21 / %x23-7E) DQUOTE
                       ; quoted string of SP and VCHAR
                       ;  without DQUOTE
*/
static Char char_val_rg[] = {0x23, 0x7E };
static FsmTransition char_val_opt_fsm[] = {
	  {-1, 1, FSM_MATCH_RANGE(&char_val_rg[0], &char_val_rg[1]) , FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_CHAR(_U(" !"))                          , FSM_ACCEPT, NULL, NULL }
};
static FsmTransition char_val_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(_DQUOTE)                   , FSM_NORMAL, NULL, NULL }
	, { 2,-1, FSM_MATCH_RPT_FSM(char_val_opt_fsm, 0,-1) , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_CHAR(_DQUOTE)                   , FSM_ACCEPT, NULL, NULL }
};


/*
"b" 1*BIT [ 1*("." 1*BIT) / ("-" 1*BIT) ]
                       ; series of concatenated bit values
                       ;  or single ONEOF range
*/
/* "." 1*BIT */
/* "-" 1*BIT */
static FsmTransition bin_dash_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(_U("-"))        , FSM_NORMAL , NULL, NULL }
	, {-1,-1, FSM_MATCH_RPT_CHAR(_BIT, 1,-1) , FSM_ACCEPT , NULL, NULL }
};
static FsmTransition bin_point_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(_U("."))        , FSM_NORMAL , NULL, NULL }
	, {-1,-1, FSM_MATCH_RPT_CHAR(_BIT, 1,-1) , FSM_ACCEPT , NULL, NULL }
};
static FsmTransition bin_dash_point_fsm[] = {
	  {-1, 1, FSM_MATCH_FSM(bin_dash_fsm)            , FSM_ACCEPT , NULL, NULL }
	, {-1,-1, FSM_MATCH_RPT_FSM(bin_point_fsm, 1,-1) , FSM_ACCEPT , NULL, NULL }
};
static FsmTransition bin_val_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(_U("b"))               , FSM_NORMAL , NULL, NULL }
	, { 2,-1, FSM_MATCH_RPT_CHAR(_BIT, 1,-1)        , FSM_NORMAL , NULL, NULL }
	, {-1,-1, FSM_MATCH_OPT_FSM(bin_dash_point_fsm) , FSM_ACCEPT , NULL, NULL }
};


/* "d" 1*DIGIT [ 1*("." 1*DIGIT) / ("-" 1*DIGIT) ] */
static FsmTransition dec_dash_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(_T("-"), 1)           , FSM_NORMAL , NULL, NULL }
	, {-1,-1, FSM_MATCH_RPT(DIGIT_FSM, &rpt_1more) , FSM_ACCEPT , NULL, NULL }
};
static FsmTransition dec_point_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(_T("."), 1)           , FSM_NORMAL , NULL, NULL }
	, {-1,-1, FSM_MATCH_RPT(DIGIT_FSM, &rpt_1more) , FSM_ACCEPT , NULL, NULL }
};
static FsmTransition dec_dash_point_fsm[] = {
	  {-1, 1, FSM_MATCH_FSM(dec_dash_fsm)              , FSM_ACCEPT , NULL, NULL }
	, {-1,-1, FSM_MATCH_RPT(dec_point_fsm, &rpt_1more) , FSM_ACCEPT , NULL, NULL }
};
static FsmTransition dec_val_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(_T("d"), 1)           , FSM_NORMAL , NULL, NULL }
	, { 2,-1, FSM_MATCH_RPT(DIGIT_FSM, &rpt_1more) , FSM_ACCEPT , NULL, NULL }
	, { 3, 3, FSM_MATCH_FSM(dec_dash_point_fsm)    , FSM_NORMAL , NULL, NULL }
	, {-1,-1, FSM_MATCH_NOTHING                    , FSM_ACCEPT , NULL, NULL }
};

/* "x" 1*HEXDIG [ 1*("." 1*HEXDIG) / ("-" 1*HEXDIG) ] */
static FsmTransition hex_dash_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(_T("-"), 1)            , FSM_NORMAL , NULL, NULL }
	, {-1,-1, FSM_MATCH_RPT(HEXDIG_FSM, &rpt_1more) , FSM_ACCEPT , NULL, NULL }
};
static FsmTransition hex_point_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(_T("."), 1)            , FSM_NORMAL , NULL, NULL }
	, {-1,-1, FSM_MATCH_RPT(HEXDIG_FSM, &rpt_1more) , FSM_ACCEPT , NULL, NULL }
};
static FsmTransition hex_dash_point_fsm[] = {
	  {-1, 1, FSM_MATCH_FSM(hex_dash_fsm)              , FSM_ACCEPT , NULL, NULL }
	, {-1,-1, FSM_MATCH_RPT(hex_point_fsm, &rpt_1more) , FSM_ACCEPT , NULL, NULL }
};
static FsmTransition hex_val_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(_T("x"), 1)            , FSM_NORMAL , NULL, NULL }
	, { 2,-1, FSM_MATCH_RPT(HEXDIG_FSM, &rpt_1more) , FSM_ACCEPT , NULL, NULL }
	, { 3, 3, FSM_MATCH_FSM(hex_dash_point_fsm)     , FSM_NORMAL , NULL, NULL }
	, {-1,-1, FSM_MATCH_NOTHING                     , FSM_ACCEPT , NULL, NULL }
};

/*  "%" (bin-val / dec-val / hex-val) */
static FsmTransition num_val_opt_fsm[] = {
	  {-1, 1, FSM_MATCH_FSM(dec_val_fsm) , FSM_ACCEPT, NULL, NULL }
	, {-1, 2, FSM_MATCH_FSM(hex_val_fsm) , FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(bin_val_fsm) , FSM_ACCEPT, NULL, NULL }
};
static FsmTransition num_val_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(_T("%"), 1)     , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(num_val_opt_fsm) , FSM_ACCEPT, NULL, NULL }
};


/* WSP / (c-nl WSP) */
/* c-nl WSP */
static FsmTransition c_nl_wsp_fsm[] = {
     { 1,-1, FSM_MATCH_FSM(c_nl_fsm)       , FSM_NORMAL , NULL , NULL }
   , {-1,-1, FSM_MATCH_INLINE(WSP_FSM_INL) , FSM_ACCEPT , NULL , NULL }
};
/* WSP / c-nl-wsp */
static FsmTransition c_wsp_fsm[] = {
     {-1, 1, FSM_MATCH_FSM(c_nl_wsp_fsm)   , FSM_ACCEPT , NULL , NULL }
   , {-1,-1, FSM_MATCH_INLINE(WSP_FSM_INL) , FSM_ACCEPT , NULL , NULL }
};

/*
  *c-wsp ("=" / "=/") *c-wsp
							; basic rules definition and
							;  incremental alternatives
*/
static FsmTransition assign_fsm[] = {
	  {-1, 1, FSM_MATCH_STR(_T("=/"), 2)           , FSM_ACCEPT , NULL, NULL }
	, {-1,-1, FSM_MATCH_STR(_T("="), 1)            , FSM_ACCEPT , NULL, NULL }
};
static FsmTransition defined_as_fsm[] = {
	  { 1,-1, FSM_MATCH_RPT(c_wsp_fsm, &rpt_0more) , FSM_NORMAL , NULL, NULL }
	, { 2,-1, FSM_MATCH_FSM(assign_fsm)            , FSM_NORMAL , NULL, NULL }
	, {-1,-1, FSM_MATCH_RPT(c_wsp_fsm, &rpt_0more) , FSM_ACCEPT , NULL, NULL }
};

/*
static CwtFsmRepetitionContext __dec_1more_rpt = { DIGIT_FSM, 1, -1 };
static CwtFsmRepetitionContext __dec_0more_rpt = { DIGIT_FSM,-1, -1 };
*/
/* *DIGIT "*" *DIGIT */
static FsmTransition repeat_range_fsm[] = {
	  { 1, 1, FSM_MATCH_RPT(DIGIT_FSM, &rpt_0more) , FSM_NORMAL, NULL, NULL }
	, { 2,-1, FSM_MATCH_STR(_T("*"), 1)            , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_RPT(DIGIT_FSM, &rpt_0more) , FSM_ACCEPT, NULL, NULL }
};
/* (*DIGIT "*" *DIGIT) / 1*DIGIT  */
static FsmTransition repeat_fsm[] = {
	  {-1, 1, FSM_MATCH_FSM(repeat_range_fsm)      , FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_RPT(DIGIT_FSM, &rpt_1more) , FSM_ACCEPT, NULL, NULL }
};

static FsmTransition element_fsm[];
/* [repeat] element */
static FsmTransition repetition_fsm[] = {
	  { 1, 1, FSM_MATCH_FSM(repeat_fsm),  FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(element_fsm), FSM_ACCEPT, NULL, NULL }
};

/* repetition *(1*c-wsp repetition) */
/* 1*c-wsp repetition */
static FsmTransition repetition_c_wsp_fsm[] = {
	  { 1,-1, FSM_MATCH_RPT(c_wsp_fsm, &rpt_1more) , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(repetition_fsm)        , FSM_ACCEPT, NULL, NULL }
};
static FsmTransition concatenation_fsm[] = {
	  { 1,-1, FSM_MATCH_FSM(repetition_fsm),       FSM_ACCEPT, NULL, NULL }
	, { 1,-1, FSM_MATCH_FSM(repetition_c_wsp_fsm), FSM_ACCEPT, NULL, NULL }
};


/* concatenation *(*c-wsp "/" *c-wsp concatenation) */
/* *c-wsp "/" *c-wsp concatenation */
static FsmTransition concatenation_c_wsp_fsm[] = {
	  { 1,-1, FSM_MATCH_RPT(c_wsp_fsm, &rpt_0more) , FSM_NORMAL, NULL, NULL }
	, { 2,-1, FSM_MATCH_STR(_T("/"), 1)            , FSM_NORMAL, NULL, NULL }
	, { 3,-1, FSM_MATCH_RPT(c_wsp_fsm, &rpt_0more) , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(concatenation_fsm)     , FSM_ACCEPT, NULL, NULL }
};

static FsmTransition alternation_fsm[] = {
	  { 1,-1, FSM_MATCH_FSM(concatenation_fsm),       FSM_ACCEPT, NULL, NULL }
	, { 1,-1, FSM_MATCH_FSM(concatenation_c_wsp_fsm), FSM_ACCEPT, NULL, NULL }
};

/* alternation *c-wsp */
static FsmTransition elements_fsm[] = {
	  { 1,-1, FSM_MATCH_FSM(alternation_fsm)       , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_RPT(c_wsp_fsm, &rpt_0more) , FSM_ACCEPT, NULL, NULL }
};

/* "[" *c-wsp alternation *c-wsp "]" */
static FsmTransition option_fsm[] = {
	  { 1,-1, FSM_MATCH_STR(_T("["), 1)            , FSM_NORMAL, NULL, NULL }
	, { 2,-1, FSM_MATCH_RPT(c_wsp_fsm, &rpt_0more) , FSM_NORMAL, NULL, NULL }
	, { 3,-1, FSM_MATCH_FSM(alternation_fsm)       , FSM_NORMAL, NULL, NULL }
	, { 4,-1, FSM_MATCH_RPT(c_wsp_fsm, &rpt_0more) , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_STR(_T("]"), 1)            , FSM_ACCEPT, NULL, NULL }
};

/* "(" *c-wsp alternation *c-wsp ")" */
static FsmTransition group_fsm[] = {
	  { 1,-1, FSM_MATCH_STR(_T("("), 1)            , FSM_NORMAL, NULL, NULL }
	, { 2,-1, FSM_MATCH_RPT(c_wsp_fsm, &rpt_0more) , FSM_NORMAL, NULL, NULL }
	, { 3,-1, FSM_MATCH_FSM(alternation_fsm)       , FSM_NORMAL, NULL, NULL }
	, { 4,-1, FSM_MATCH_RPT(c_wsp_fsm, &rpt_0more) , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_STR(_T(")"), 1)            , FSM_ACCEPT, NULL, NULL }
};

/* rulename / group / option / char-val / num-val / prose-val */
static FsmTransition element_fsm[] = {
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
static FsmTransition rule_fsm[] = {
	  { 1,-1, FSM_MATCH_FSM(rulename_fsm)   , FSM_NORMAL, NULL, NULL }
	, { 2,-1, FSM_MATCH_FSM(defined_as_fsm) , FSM_NORMAL, NULL, NULL }
	, { 3,-1, FSM_MATCH_FSM(elements_fsm)   , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(c_nl_fsm)       , FSM_ACCEPT, NULL, NULL }
};

/* 1*( rule / (*c-wsp c-nl) ) */
/* *c-wsp c-nl */
static FsmTransition c_wsp_nl_fsm[] = {
	  { 1,-1, FSM_MATCH_RPT(c_wsp_fsm, &rpt_0more) , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(c_nl_fsm)              , FSM_ACCEPT, NULL, NULL }
};

/* rule / (*c-wsp c-nl) */
static FsmTransition rule_c_wsp_nl_fsm[] = {
	  {-1, 1, FSM_MATCH_FSM(c_wsp_nl_fsm) , FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(rule_fsm)     , FSM_ACCEPT, NULL, NULL }
};

static FsmTransition rulelist_fsm[] = {
	{-1,-1, FSM_MATCH_RPT(rule_c_wsp_nl_fsm, &rpt_1more) , FSM_ACCEPT, NULL, NULL }
};

CWT_NS_END

#endif /* __CWT_ABNF_RFC5234_H__ */
