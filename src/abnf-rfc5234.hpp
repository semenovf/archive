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
#include <cwt/logger.hpp>
#include "abnf_p.hpp"

CWT_NS_BEGIN

static const int _BEGIN_DOC         = 1;
static const int _END_DOC_OK        = 2;
static const int _END_DOC_FAIL      = 3;
static const int _BEGIN_RULE        = 4;
static const int _END_RULE          = 5;
static const int _BEGIN_ALTERNATION = 6;
static const int _END_ALTERNATION   = 7;
static const int _BEGIN_CONCAT      = 8;
static const int _END_CONCAT        = 9;
static const int _BEGIN_RPT         = 10;
static const int _END_RPT           = 11;
static const int _BEGIN_OPTION      = 12;
static const int _END_OPTION        = 13;
static const int _BEGIN_GROUP       = 14;
static const int _END_GROUP         = 15;
static const int _ELEM_RULE_REF     = 16;
static const int _ELEM_CHAR_VAL     = 17;
static const int _ELEM_NUM_VAL      = 18;
static const int _ELEM_PROSE_VAL    = 19;

static const int _RPT_FROM   = 1;
static const int _RPT_TO     = 2;
static const int _RPT_FROMTO = 3;

static bool block(const void *data, size_t len, void *context, void *action_args);
static bool rulename(const void *data, size_t len, void *context, void *action_args);
static bool comment(const void *data, size_t len, void *context, void *action_args);
static bool rpt(const void *data, size_t len, void *context, void *action_args);
//static bool plain_element(const void *data, size_t len, void *context, void *action_args);
/*

static bool prose_val(const void *data, size_t len, void *context, void *action_args);
*/


/*static bool _trace(const void *data, size_t len, void *context, void *action_args)
{
	CWT_UNUSED(context);
	if (action_args) {
		CWT_TRACE(String().sprintf("%s: [%ls]"
				, reinterpret_cast<const char*>(action_args)
				, String((const Char*)data, len).data()).toUtf8().data());
	}
	return true;
}*/

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

static String _ALPHA    = _U("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
static String _DIGIT    = _U("0123456789");
static String _HEXDIGIT = _U("0123456789ABCDEFabcdef");
static String _BIT      = _U("01");
static String _DQUOTE   = _U("\"");
static String _WSP      = _U(" \t");
static String _NL       = _U("\n\r");


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
     {-1, 1, FSM_MATCH_FSM(comment_fsm) , FSM_ACCEPT, comment, NULL }
   , {-1,-1, FSM_MATCH_FSM(nl_fsm)      , FSM_ACCEPT, NULL, NULL }
};

/*
 DQUOTE *(%x20-21 / %x23-7E) DQUOTE
                       ; quoted string of SP and VCHAR
                       ;  without DQUOTE
*/
static Char char_val_rg[] = { 0x23, 0x7E };
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
	  { 1,-1, FSM_MATCH_CHAR(_U("-"))          , FSM_NORMAL , NULL, NULL }
	, {-1,-1, FSM_MATCH_RPT_CHAR(_DIGIT, 1,-1) , FSM_ACCEPT , NULL, NULL }
};
static FsmTransition dec_point_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(_U("."))          , FSM_NORMAL , NULL, NULL }
	, {-1,-1, FSM_MATCH_RPT_CHAR(_DIGIT, 1,-1) , FSM_ACCEPT , NULL, NULL }
};
static FsmTransition dec_dash_point_fsm[] = {
	  {-1, 1, FSM_MATCH_FSM(dec_dash_fsm)            , FSM_ACCEPT , NULL, NULL }
	, {-1,-1, FSM_MATCH_RPT_FSM(dec_point_fsm, 1,-1) , FSM_ACCEPT , NULL, NULL }
};
static FsmTransition dec_val_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(_U("d"))               , FSM_NORMAL , NULL, NULL }
	, { 2,-1, FSM_MATCH_RPT_CHAR(_DIGIT, 1,-1)      , FSM_NORMAL , NULL, NULL }
	, {-1,-1, FSM_MATCH_OPT_FSM(dec_dash_point_fsm) , FSM_ACCEPT , NULL, NULL }
};

/* "x" 1*HEXDIG [ 1*("." 1*HEXDIG) / ("-" 1*HEXDIG) ] */
static FsmTransition hex_dash_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(_U("-"))             , FSM_NORMAL , NULL, NULL }
	, {-1,-1, FSM_MATCH_RPT_CHAR(_HEXDIGIT, 1,-1) , FSM_ACCEPT , NULL, NULL }
};
static FsmTransition hex_point_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(_U("."))             , FSM_NORMAL , NULL, NULL }
	, {-1,-1, FSM_MATCH_RPT_CHAR(_HEXDIGIT, 1,-1) , FSM_ACCEPT , NULL, NULL }
};
static FsmTransition hex_dash_point_fsm[] = {
	  {-1, 1, FSM_MATCH_FSM(hex_dash_fsm)            , FSM_ACCEPT , NULL, NULL }
	, {-1,-1, FSM_MATCH_RPT_FSM(hex_point_fsm, 1,-1) , FSM_ACCEPT , NULL, NULL }
};
static FsmTransition hex_val_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(_U("x"))               , FSM_NORMAL , NULL, NULL }
	, { 2,-1, FSM_MATCH_RPT_CHAR(_HEXDIGIT, 1,-1)   , FSM_NORMAL , NULL, NULL }
	, {-1,-1, FSM_MATCH_OPT_FSM(hex_dash_point_fsm) , FSM_ACCEPT , NULL, NULL }
};

/*  "%" (bin-val / dec-val / hex-val) */
static FsmTransition num_val_opt_fsm[] = {
	  {-1, 1, FSM_MATCH_FSM(dec_val_fsm) , FSM_ACCEPT, NULL, NULL }
	, {-1, 2, FSM_MATCH_FSM(hex_val_fsm) , FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(bin_val_fsm) , FSM_ACCEPT, NULL, NULL }
};
static FsmTransition num_val_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(_U("%"))        , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(num_val_opt_fsm) , FSM_ACCEPT, NULL, NULL }
};

/* WSP / (c-nl WSP) */
/* c-nl WSP */
static FsmTransition c_nl_wsp_fsm[] = {
     { 1,-1, FSM_MATCH_FSM(c_nl_fsm) , FSM_NORMAL , NULL , NULL }
   , {-1,-1, FSM_MATCH_CHAR(_WSP)    , FSM_ACCEPT , NULL , NULL }
};
/* WSP / c-nl-wsp */
static FsmTransition c_wsp_fsm[] = {
     {-1, 1, FSM_MATCH_FSM(c_nl_wsp_fsm) , FSM_ACCEPT , NULL , NULL }
   , {-1,-1, FSM_MATCH_CHAR(_WSP)        , FSM_ACCEPT , NULL , NULL }
};

/*
  *c-wsp ("=" / "=/") *c-wsp
							; basic rules definition and
							;  incremental alternatives
*/
static FsmTransition assign_fsm[] = {
	  {-1, 1, FSM_MATCH_STR(_U("=/")) , FSM_ACCEPT , NULL, NULL }
	, {-1,-1, FSM_MATCH_STR(_U("="))  , FSM_ACCEPT , NULL, NULL }
};
static FsmTransition defined_as_fsm[] = {
	  { 1,-1, FSM_MATCH_RPT_FSM(c_wsp_fsm, 0,-1) , FSM_NORMAL , NULL, NULL }
	, { 2,-1, FSM_MATCH_FSM(assign_fsm)          , FSM_NORMAL , NULL, NULL }
	, {-1,-1, FSM_MATCH_RPT_FSM(c_wsp_fsm, 0,-1) , FSM_ACCEPT , NULL, NULL }
};

/* *DIGIT "*" *DIGIT */
static FsmTransition repeat_range_fsm[] = {
	  { 1, 1, FSM_MATCH_RPT_CHAR(_DIGIT, 0,-1) , FSM_NORMAL, rpt, (void*)&_RPT_FROM }
	, { 2,-1, FSM_MATCH_CHAR(_U("*"))          , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_RPT_CHAR(_DIGIT, 0,-1) , FSM_ACCEPT, rpt, (void*)&_RPT_TO }
};
/* (*DIGIT "*" *DIGIT) / 1*DIGIT  */
static FsmTransition repeat_fsm[] = {
	  {-1, 1, FSM_MATCH_FSM(repeat_range_fsm)  , FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_RPT_CHAR(_DIGIT, 1,-1) , FSM_ACCEPT, rpt, (void*)&_RPT_FROMTO }
};

extern FsmTransition element_fsm[];
/* [repeat] element */
static FsmTransition repetition_fsm[] = {
	  { 1,-1, FSM_MATCH_OPT_FSM(repeat_fsm), FSM_NORMAL, block, (void*)&_BEGIN_RPT }
	, {-1,-1, FSM_MATCH_FSM(element_fsm)   , FSM_ACCEPT, block, (void*)&_END_RPT }
};

/* concatenation = repetition *(1*c-wsp repetition) */
/* 1*c-wsp repetition */
static FsmTransition repetition_c_wsp_fsm[] = {
	  { 1,-1, FSM_MATCH_RPT_FSM(c_wsp_fsm, 1,-1) , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(repetition_fsm)      , FSM_ACCEPT, NULL, NULL }
};
static FsmTransition concatenation_fsm[] = {
	  { 1,-1, FSM_MATCH_NOTHING                            , FSM_NORMAL, block, (void*)&_BEGIN_CONCAT }
	, { 2,-1, FSM_MATCH_FSM(repetition_fsm)                , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_RPT_FSM(repetition_c_wsp_fsm, 0,-1), FSM_ACCEPT, block, (void*)&_END_CONCAT }
};

/* concatenation *(*c-wsp "/" *c-wsp concatenation) */
/* *c-wsp "/" *c-wsp concatenation */
static FsmTransition concatenation_next_fsm[] = {
	  { 1,-1, FSM_MATCH_RPT_FSM(c_wsp_fsm, 0,-1) , FSM_NORMAL, NULL, NULL }
	, { 2,-1, FSM_MATCH_CHAR(_U("/"))            , FSM_NORMAL, NULL, NULL }
	, { 3,-1, FSM_MATCH_RPT_FSM(c_wsp_fsm, 0,-1) , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(concatenation_fsm)   , FSM_ACCEPT, NULL, NULL }
};

static FsmTransition alternation_fsm[] = {
	  { 1,-1, FSM_MATCH_NOTHING                              , FSM_NORMAL, block, (void*)&_BEGIN_ALTERNATION }
	, { 2,-1, FSM_MATCH_FSM(concatenation_fsm)               , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_RPT_FSM(concatenation_next_fsm, 0,-1), FSM_ACCEPT, block, (void*)&_END_ALTERNATION }
};

/* elements = alternation *c-wsp */
static FsmTransition elements_fsm[] = {
	  { 1,-1, FSM_MATCH_FSM(alternation_fsm)     , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_RPT_FSM(c_wsp_fsm, 0,-1) , FSM_ACCEPT, NULL, NULL }
};

/* "[" *c-wsp alternation *c-wsp "]" */
static FsmTransition option_fsm[] = {
	  { 1,-1, FSM_MATCH_STR(_U("["))             , FSM_NORMAL, block, (void*)&_BEGIN_OPTION }
	, { 2,-1, FSM_MATCH_RPT_FSM(c_wsp_fsm, 0,-1) , FSM_NORMAL, NULL, NULL }
	, { 3,-1, FSM_MATCH_FSM(alternation_fsm)     , FSM_NORMAL, NULL, NULL }
	, { 4,-1, FSM_MATCH_RPT_FSM(c_wsp_fsm, 0,-1) , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_STR(_U("]"))             , FSM_ACCEPT, block, (void*)&_END_OPTION }
};

/* "(" *c-wsp alternation *c-wsp ")" */
static FsmTransition group_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(_U("("))            , FSM_NORMAL, block, (void*)&_BEGIN_GROUP }
	, { 2,-1, FSM_MATCH_RPT_FSM(c_wsp_fsm, 0,-1) , FSM_NORMAL, NULL, NULL }
	, { 3,-1, FSM_MATCH_FSM(alternation_fsm)     , FSM_NORMAL, NULL, NULL }
	, { 4,-1, FSM_MATCH_RPT_FSM(c_wsp_fsm, 0,-1) , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_STR(_U(")"))             , FSM_ACCEPT, block, (void*)&_END_GROUP }
};

/* rulename / group / option / char-val / num-val / prose-val */
FsmTransition element_fsm[] = {
	  {-1, 1, FSM_MATCH_FSM(rulename_fsm),  FSM_ACCEPT, block, (void*)&_ELEM_RULE_REF }
	, {-1, 2, FSM_MATCH_FSM(group_fsm),     FSM_ACCEPT, NULL, NULL }
	, {-1, 3, FSM_MATCH_FSM(option_fsm),    FSM_ACCEPT, NULL, NULL }
	, {-1, 4, FSM_MATCH_FSM(char_val_fsm),  FSM_ACCEPT, block, (void*)&_ELEM_CHAR_VAL }
	, {-1, 5, FSM_MATCH_FSM(num_val_fsm),   FSM_ACCEPT, block, (void*)&_ELEM_NUM_VAL }
	, {-1,-1, FSM_MATCH_FSM(prose_val_fsm), FSM_ACCEPT, block, (void*)&_ELEM_PROSE_VAL }
};

/*
 rulename defined-as elements c-nl
                       ; continues if next line starts
                       ;  with white space
*/
static FsmTransition rule_fsm[] = {
	  { 1,-1, FSM_MATCH_FSM(rulename_fsm)   , FSM_NORMAL, rulename, NULL }
	, { 2,-1, FSM_MATCH_FSM(defined_as_fsm) , FSM_NORMAL, block, (void*)&_BEGIN_RULE }
	, { 3,-1, FSM_MATCH_FSM(elements_fsm)   , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(c_nl_fsm)       , FSM_ACCEPT, block, (void*)&_END_RULE }
};

/* 1*( rule / (*c-wsp c-nl) ) */
/* *c-wsp c-nl */
static FsmTransition c_wsp_nl_fsm[] = {
	  { 1,-1, FSM_MATCH_RPT_FSM(c_wsp_fsm, 0,-1) , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(c_nl_fsm)            , FSM_ACCEPT, NULL, NULL }
};

/* rule / (*c-wsp c-nl) */
static FsmTransition rule_c_wsp_nl_fsm[] = {
	  {-1, 1, FSM_MATCH_FSM(c_wsp_nl_fsm) , FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(rule_fsm)     , FSM_ACCEPT, NULL, NULL }
};

static FsmTransition rulelist_fsm[] = {
	  { 1,-1, FSM_MATCH_NOTHING                          , FSM_NORMAL, block, (void*)&_BEGIN_DOC }
	, { 2, 3, FSM_MATCH_RPT_FSM(rule_c_wsp_nl_fsm, 1,-1) , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_NOTHING                          , FSM_ACCEPT, block, (void*)&_END_DOC_OK }
	, {-1,-1, FSM_MATCH_NOTHING                          , FSM_ACCEPT, block, (void*)&_END_DOC_FAIL }
};


static bool block(const void *data, size_t len, void *context, void *action_args)
{
	CWT_UNUSED2(data, len);
	const int *flag = (const int *)action_args;

	if (!context)
		return true;

	AbnfParseContext *ctx = _CAST_PARSE_CTX(context);

	switch (*flag) {
	case _BEGIN_DOC         : return ctx->api.beginDocument(ctx->userContext);
	case _END_DOC_OK        : return ctx->api.endDocument(true, ctx->userContext);
	case _END_DOC_FAIL      : return ctx->api.endDocument(false, ctx->userContext);
	case _BEGIN_RULE        :
		if (String((const Char*)data, len).contains(_U("=/"))) {
			return ctx->api.beginRule(ctx->rulename, true, ctx->userContext);
		}
		return ctx->api.beginRule(ctx->rulename, false, ctx->userContext);

	case _END_RULE          : return ctx->api.endRule(ctx->userContext);
	case _BEGIN_ALTERNATION : return ctx->api.beginAlternation(ctx->userContext);
	case _END_ALTERNATION   : return ctx->api.endAlternation(ctx->userContext);
	case _BEGIN_CONCAT      : return ctx->api.beginConcatenation(ctx->userContext);
	case _END_CONCAT        : return ctx->api.endConcatenation(ctx->userContext);
	case _BEGIN_RPT: {
		int from = ctx->rpt.from, to = ctx->rpt.to;
		if (ctx->rpt.to == CWT_INT_MIN)
			from = to = 1;
		ctx->rpt.from = ctx->rpt.to = CWT_INT_MIN;
		return ctx->api.beginRepetition(from, to, ctx->userContext);
	}
	case _END_RPT           : return ctx->api.endRepetition(ctx->userContext);
	case _BEGIN_OPTION      : return ctx->api.beginOption(ctx->userContext);
	case _END_OPTION        : return ctx->api.endOption(ctx->userContext);
	case _BEGIN_GROUP       : return ctx->api.beginGroup(ctx->userContext);
	case _END_GROUP         : return ctx->api.endGroup(ctx->userContext);
	case _ELEM_RULE_REF     : return ctx->api.ruleRef(String((const Char*)data, len), ctx->userContext);
	case _ELEM_CHAR_VAL     : {
		String s((const Char*)data, len);
		CWT_ASSERT(s.startsWith(Char('\"')));
		CWT_ASSERT(s.endsWith(Char('\"')));
		return ctx->api.charVal(s.substr(1, s.length()-2), ctx->userContext);
	}
	case _ELEM_NUM_VAL      : return ctx->api.numVal(String((const Char*)data, len), ctx->userContext);
	case _ELEM_PROSE_VAL    : return ctx->api.proseVal(String((const Char*)data, len), ctx->userContext);

	default:
		break;
	}

	return false;
}

static bool rulename(const void *data, size_t len, void *context, void *action_args)
{
	CWT_UNUSED(action_args);
	if (context) {
		AbnfParseContext *ctx = _CAST_PARSE_CTX(context);
		ctx->rulename = String((const Char*)data, len);
	}
	return true;
}

static bool comment(const void *data, size_t len, void *context, void *action_args)
{
	CWT_UNUSED(action_args);
	if (!context)
		return true;

	AbnfParseContext *ctx = _CAST_PARSE_CTX(context);
	return ctx->api.comment(String((const Char*)data, len), ctx->userContext);
}

static bool rpt(const void *data, size_t len, void *context, void *action_args)
{
	CWT_UNUSED(action_args);
	const int *flag = (const int *)action_args;

	if (context) {
		AbnfParseContext *ctx = _CAST_PARSE_CTX(context);
		bool ok = true;
		int value = 0;

		if (len) {
			value = String((const Char*)data, len).toInt(&ok, 10);
			if (!ok) {
				Logger::error("invalid repetition value");
				return false;
			}
		}

		switch(*flag) {
		case _RPT_FROM:
			ctx->rpt.from = len > 0 ? value : 0;
			break;
		case _RPT_TO:
			ctx->rpt.to = len > 0 ? value : -1;
			break;
		case _RPT_FROMTO:
			ctx->rpt.from = ctx->rpt.to = len > 0 ? value : 1;
			break;
		default:
			return false;
		}
	}
	return true;
}

CWT_NS_END

#endif /* __CWT_ABNF_RFC5234_H__ */
