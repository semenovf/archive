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

static const int __ABNF_BEGIN_DOC      = 1;
static const int __ABNF_END_DOC_OK     = 2;
static const int __ABNF_END_DOC_FAIL   = 3;
static const int __ABNF_BEGIN_RULE     = 4;
static const int __ABNF_END_RULE       = 5;
static const int __ABNF_BEGIN_ALTERN   = 6;
static const int __ABNF_END_ALTERN     = 7;
static const int __ABNF_BEGIN_CONCAT   = 8;
static const int __ABNF_END_CONCAT     = 9;
static const int __ABNF_BEGIN_RPT      = 10;
static const int __ABNF_END_RPT        = 11;
static const int __ABNF_BEGIN_OPTION   = 12;
static const int __ABNF_END_OPTION     = 13;
static const int __ABNF_BEGIN_GROUP    = 14;
static const int __ABNF_END_GROUP      = 15;
static const int __ABNF_ELEM_RULE_REF  = 16;
static const int __ABNF_ELEM_CHAR_VAL  = 17;
static const int __ABNF_ELEM_NUM_VAL   = 18;
static const int __ABNF_ELEM_PROSE_VAL = 19;

static const int __ABNF_RPT_FROM   = 1;
static const int __ABNF_RPT_TO     = 2;
static const int __ABNF_RPT_FROMTO = 3;

static bool block (const String::const_iterator & begin, const String::const_iterator & end, void *context, void *);
static bool rulename (const String::const_iterator & begin, const String::const_iterator & end, void *context, void *);
//static bool comment (const String::const_iterator & begin, const String::const_iterator & end, void *context, void *);
static bool rpt (const String::const_iterator & begin, const String::const_iterator & end, void *context, void *);

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


static String __ABNF_ALPHA("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
static String __ABNF_DIGIT("0123456789");
static String __ABNF_HEXDIGIT("0123456789ABCDEFabcdef");
static String __ABNF_BIT("01");
static String __ABNF_DQUOTE("\"");
static String __ABNF_WSP(" \t");

/*
static ssize_t __end_of_data(FsmContext<String> * , void * , const String::const_iterator & begin, const String::const_iterator & end)
{
	return begin < end ? 0 : -1;
}
*/

static FsmTransition<String> nl_fsm[] = {
	  {-1, 1, FSM_MATCH_STR("\r\n"), FSM_ACCEPT, nullptr, nullptr } /* Windows/DOS */
	, {-1, 2, FSM_MATCH_STR("\n\r"), FSM_ACCEPT, nullptr, nullptr } /* Mac OS 9 */
	, {-1,-1, FSM_MATCH_STR("\n")  , FSM_ACCEPT, nullptr, nullptr } /* Unix */
};

/*
"<" *(%x20-3D / %x3F-7E) ">"
			; bracketed string of SP and VCHAR
			;  without angles
			; prose description, to be used as
			;  last resort
*/
static UChar prose_rg[] = {0x20, 0x3D, 0x3F, 0x7E};
static FsmTransition<String> prose_val_opt_fsm[] = {
	  {-1, 1, FSM_MATCH_RANGE(prose_rg[0], prose_rg[1]), FSM_ACCEPT, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_RANGE(prose_rg[2], prose_rg[3]), FSM_ACCEPT, nullptr, nullptr }
};
static FsmTransition<String> prose_val_fsm[] = {
	  { 1,-1, FSM_MATCH_STR("<")                         , FSM_NORMAL, nullptr, nullptr }
	, { 2,-1, FSM_MATCH_RPT_FSM(prose_val_opt_fsm, 0,-1) , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_STR(">")                         , FSM_ACCEPT, nullptr, nullptr }
};

/* ALPHA *(ALPHA / DIGIT / "-") */
static FsmTransition<String> rulename_opt_fsm[] = {
      {-1, 1, FSM_MATCH_CHAR(__ABNF_ALPHA) , FSM_ACCEPT, nullptr, nullptr }
    , {-1, 2, FSM_MATCH_CHAR(__ABNF_DIGIT) , FSM_ACCEPT, nullptr, nullptr }
    , {-1,-1, FSM_MATCH_CHAR("-")          , FSM_ACCEPT, nullptr, nullptr }
};
static FsmTransition<String> rulename_fsm[] = {
      { 1,-1, FSM_MATCH_CHAR(__ABNF_ALPHA)              , FSM_NORMAL, nullptr, nullptr }
    , {-1,-1, FSM_MATCH_RPT_FSM(rulename_opt_fsm, 0,-1) , FSM_ACCEPT, nullptr, nullptr }
};

/* ";" *(WSP / VCHAR) NL */
/* VCHAR = %x21-7E ; visible (printing) characters */
static UChar vchar_rg[2] = {0x21, 0x7E };
static FsmTransition<String> wsp_vchar_fsm[] = {
	  {-1, 1, FSM_MATCH_CHAR(__ABNF_WSP)               , FSM_ACCEPT, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_RANGE(vchar_rg[0], vchar_rg[1]), FSM_ACCEPT, nullptr, nullptr }
};
static FsmTransition<String> comment_fsm[] = {
      { 1,-1, FSM_MATCH_CHAR(";")                    , FSM_NORMAL, nullptr, nullptr }
    , { 2,-1, FSM_MATCH_RPT_FSM(wsp_vchar_fsm, 0,-1) , FSM_NORMAL, nullptr, nullptr }
    , {-1,-1, FSM_MATCH_FSM(nl_fsm)                  , FSM_ACCEPT, nullptr, nullptr }
};

/* comment / NL ; comment or newline */
static FsmTransition<String> c_nl_fsm[] = {
     {-1, 1, FSM_MATCH_FSM(comment_fsm) , FSM_ACCEPT, nullptr, nullptr }
   , {-1,-1, FSM_MATCH_FSM(nl_fsm)      , FSM_ACCEPT, nullptr, nullptr }
};

/*
 DQUOTE *(%x20-21 / %x23-7E) DQUOTE
                       ; quoted string of SP and VCHAR
                       ;  without DQUOTE
*/
static UChar char_val_rg[] = { 0x23, 0x7E };
static FsmTransition<String> char_val_opt_fsm[] = {
	  {-1, 1, FSM_MATCH_RANGE(char_val_rg[0], char_val_rg[1]) , FSM_ACCEPT, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_CHAR(" !")                            , FSM_ACCEPT, nullptr, nullptr }
};
static FsmTransition<String> char_val_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(__ABNF_DQUOTE)             , FSM_NORMAL, nullptr, nullptr }
	, { 2,-1, FSM_MATCH_RPT_FSM(char_val_opt_fsm, 0,-1) , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_CHAR(__ABNF_DQUOTE)             , FSM_ACCEPT, nullptr, nullptr }
};


/*
"b" 1*BIT [ 1*("." 1*BIT) / ("-" 1*BIT) ]
                       ; series of concatenated bit values
                       ;  or single ONEOF range
*/
/* "." 1*BIT */
/* "-" 1*BIT */
static FsmTransition<String> bin_dash_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR("-")                  , FSM_NORMAL , nullptr, nullptr }
	, {-1,-1, FSM_MATCH_RPT_CHAR(__ABNF_BIT, 1,-1) , FSM_ACCEPT , nullptr, nullptr }
};
static FsmTransition<String> bin_point_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(".")                  , FSM_NORMAL , nullptr, nullptr }
	, {-1,-1, FSM_MATCH_RPT_CHAR(__ABNF_BIT, 1,-1) , FSM_ACCEPT , nullptr, nullptr }
};
static FsmTransition<String> bin_dash_point_fsm[] = {
	  {-1, 1, FSM_MATCH_FSM(bin_dash_fsm)            , FSM_ACCEPT , nullptr, nullptr }
	, {-1,-1, FSM_MATCH_RPT_FSM(bin_point_fsm, 1,-1) , FSM_ACCEPT , nullptr, nullptr }
};
static FsmTransition<String> bin_val_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR("b")                   , FSM_NORMAL , nullptr, nullptr }
	, { 2,-1, FSM_MATCH_RPT_CHAR(__ABNF_BIT, 1,-1)  , FSM_NORMAL , nullptr, nullptr }
	, {-1,-1, FSM_MATCH_OPT_FSM(bin_dash_point_fsm) , FSM_ACCEPT , nullptr, nullptr }
};


/* "d" 1*DIGIT [ 1*("." 1*DIGIT) / ("-" 1*DIGIT) ] */
static FsmTransition<String> dec_dash_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR("-")                    , FSM_NORMAL , nullptr, nullptr }
	, {-1,-1, FSM_MATCH_RPT_CHAR(__ABNF_DIGIT, 1,-1) , FSM_ACCEPT , nullptr, nullptr }
};
static FsmTransition<String> dec_point_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(".")                    , FSM_NORMAL , nullptr, nullptr }
	, {-1,-1, FSM_MATCH_RPT_CHAR(__ABNF_DIGIT, 1,-1) , FSM_ACCEPT , nullptr, nullptr }
};
static FsmTransition<String> dec_dash_point_fsm[] = {
	  {-1, 1, FSM_MATCH_FSM(dec_dash_fsm)            , FSM_ACCEPT , nullptr, nullptr }
	, {-1,-1, FSM_MATCH_RPT_FSM(dec_point_fsm, 1,-1) , FSM_ACCEPT , nullptr, nullptr }
};
static FsmTransition<String> dec_val_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR("d")                    , FSM_NORMAL , nullptr, nullptr }
	, { 2,-1, FSM_MATCH_RPT_CHAR(__ABNF_DIGIT, 1,-1) , FSM_NORMAL , nullptr, nullptr }
	, {-1,-1, FSM_MATCH_OPT_FSM(dec_dash_point_fsm)  , FSM_ACCEPT , nullptr, nullptr }
};

/* "x" 1*HEXDIG [ 1*("." 1*HEXDIG) / ("-" 1*HEXDIG) ] */
static FsmTransition<String> hex_dash_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR("-")                       , FSM_NORMAL , nullptr, nullptr }
	, {-1,-1, FSM_MATCH_RPT_CHAR(__ABNF_HEXDIGIT, 1,-1) , FSM_ACCEPT , nullptr, nullptr }
};
static FsmTransition<String> hex_point_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(".")                       , FSM_NORMAL , nullptr, nullptr }
	, {-1,-1, FSM_MATCH_RPT_CHAR(__ABNF_HEXDIGIT, 1,-1) , FSM_ACCEPT , nullptr, nullptr }
};
static FsmTransition<String> hex_dash_point_fsm[] = {
	  {-1, 1, FSM_MATCH_FSM(hex_dash_fsm)            , FSM_ACCEPT , nullptr, nullptr }
	, {-1,-1, FSM_MATCH_RPT_FSM(hex_point_fsm, 1,-1) , FSM_ACCEPT , nullptr, nullptr }
};
static FsmTransition<String> hex_val_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR("x")                       , FSM_NORMAL , nullptr, nullptr }
	, { 2,-1, FSM_MATCH_RPT_CHAR(__ABNF_HEXDIGIT, 1,-1) , FSM_NORMAL , nullptr, nullptr }
	, {-1,-1, FSM_MATCH_OPT_FSM(hex_dash_point_fsm)     , FSM_ACCEPT , nullptr, nullptr }
};

/*  "%" (bin-val / dec-val / hex-val) */
static FsmTransition<String> num_val_opt_fsm[] = {
	  {-1, 1, FSM_MATCH_FSM(dec_val_fsm) , FSM_ACCEPT, nullptr, nullptr }
	, {-1, 2, FSM_MATCH_FSM(hex_val_fsm) , FSM_ACCEPT, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(bin_val_fsm) , FSM_ACCEPT, nullptr, nullptr }
};
static FsmTransition<String> num_val_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR("%")            , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(num_val_opt_fsm) , FSM_ACCEPT, nullptr, nullptr }
};

/* WSP / (c-nl WSP) */
/* c-nl WSP */
static FsmTransition<String> c_nl_wsp_fsm[] = {
     { 1,-1, FSM_MATCH_FSM(c_nl_fsm)    , FSM_NORMAL , nullptr , nullptr }
   , {-1,-1, FSM_MATCH_CHAR(__ABNF_WSP) , FSM_ACCEPT , nullptr , nullptr }
};
/* WSP / c-nl-wsp */
static FsmTransition<String> c_wsp_fsm[] = {
     {-1, 1, FSM_MATCH_FSM(c_nl_wsp_fsm) , FSM_ACCEPT , nullptr , nullptr }
   , {-1,-1, FSM_MATCH_CHAR(__ABNF_WSP)  , FSM_ACCEPT , nullptr , nullptr }
};

/*
  *c-wsp ("=" / "=/") *c-wsp
							; basic rules definition and
							;  incremental alternatives
*/
static FsmTransition<String> assign_fsm[] = {
	  {-1, 1, FSM_MATCH_STR("=/") , FSM_ACCEPT , nullptr, nullptr }
	, {-1,-1, FSM_MATCH_STR("=")  , FSM_ACCEPT , nullptr, nullptr }
};
static FsmTransition<String> defined_as_fsm[] = {
	  { 1,-1, FSM_MATCH_RPT_FSM(c_wsp_fsm, 0,-1) , FSM_NORMAL , nullptr, nullptr }
	, { 2,-1, FSM_MATCH_FSM(assign_fsm)          , FSM_NORMAL , nullptr, nullptr }
	, {-1,-1, FSM_MATCH_RPT_FSM(c_wsp_fsm, 0,-1) , FSM_ACCEPT , nullptr, nullptr }
};

/* *DIGIT "*" *DIGIT */
static FsmTransition<String> repeat_range_fsm[] = {
	  { 1, 1, FSM_MATCH_RPT_CHAR(__ABNF_DIGIT, 0,-1) , FSM_NORMAL, rpt, (void *) & __ABNF_RPT_FROM }
	, { 2,-1, FSM_MATCH_CHAR("*")                    , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_RPT_CHAR(__ABNF_DIGIT, 0,-1) , FSM_ACCEPT, rpt, (void *) & __ABNF_RPT_TO }
};
/* (*DIGIT "*" *DIGIT) / 1*DIGIT  */
static FsmTransition<String> repeat_fsm[] = {
	  {-1, 1, FSM_MATCH_FSM(repeat_range_fsm)  , FSM_ACCEPT, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_RPT_CHAR(__ABNF_DIGIT, 1,-1) , FSM_ACCEPT, rpt, (void *) & __ABNF_RPT_FROMTO }
};

extern FsmTransition<String> element_fsm[];
/* [repeat] element */
static FsmTransition<String> repetition_fsm[] = {
	  { 1,-1, FSM_MATCH_OPT_FSM(repeat_fsm), FSM_NORMAL, block, (void *) & __ABNF_BEGIN_RPT }
	, {-1,-1, FSM_MATCH_FSM(element_fsm)   , FSM_ACCEPT, block, (void *) & __ABNF_END_RPT }
};

/* concatenation = repetition *(1*c-wsp repetition) */
/* 1*c-wsp repetition */
static FsmTransition<String> repetition_c_wsp_fsm[] = {
	  { 1,-1, FSM_MATCH_RPT_FSM(c_wsp_fsm, 1,-1) , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(repetition_fsm)      , FSM_ACCEPT, nullptr, nullptr }
};
static FsmTransition<String> concatenation_fsm[] = {
	  { 1,-1, FSM_MATCH_NOTHING                            , FSM_NORMAL, block, (void *) & __ABNF_BEGIN_CONCAT }
	, { 2,-1, FSM_MATCH_FSM(repetition_fsm)                , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_RPT_FSM(repetition_c_wsp_fsm, 0,-1), FSM_ACCEPT, block, (void *) & __ABNF_END_CONCAT }
};

/* concatenation *(*c-wsp "/" *c-wsp concatenation) */
/* *c-wsp "/" *c-wsp concatenation */
static FsmTransition<String> concatenation_next_fsm[] = {
	  { 1,-1, FSM_MATCH_RPT_FSM(c_wsp_fsm, 0,-1) , FSM_NORMAL, nullptr, nullptr }
	, { 2,-1, FSM_MATCH_CHAR("/")                , FSM_NORMAL, nullptr, nullptr }
	, { 3,-1, FSM_MATCH_RPT_FSM(c_wsp_fsm, 0,-1) , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(concatenation_fsm)   , FSM_ACCEPT, nullptr, nullptr }
};

static FsmTransition<String> alternation_fsm[] = {
	  { 1,-1, FSM_MATCH_NOTHING                              , FSM_NORMAL, block, (void *) & __ABNF_BEGIN_ALTERN }
	, { 2,-1, FSM_MATCH_FSM(concatenation_fsm)               , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_RPT_FSM(concatenation_next_fsm, 0,-1), FSM_ACCEPT, block, (void *) & __ABNF_END_ALTERN }
};

/* elements = alternation *c-wsp */
static FsmTransition<String> elements_fsm[] = {
	  { 1,-1, FSM_MATCH_FSM(alternation_fsm)     , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_RPT_FSM(c_wsp_fsm, 0,-1) , FSM_ACCEPT, nullptr, nullptr }
};

/* "[" *c-wsp alternation *c-wsp "]" */
static FsmTransition<String> option_fsm[] = {
	  { 1,-1, FSM_MATCH_STR("[")                 , FSM_NORMAL, block, (void *) & __ABNF_BEGIN_OPTION }
	, { 2,-1, FSM_MATCH_RPT_FSM(c_wsp_fsm, 0,-1) , FSM_NORMAL, nullptr, nullptr }
	, { 3,-1, FSM_MATCH_FSM(alternation_fsm)     , FSM_NORMAL, nullptr, nullptr }
	, { 4,-1, FSM_MATCH_RPT_FSM(c_wsp_fsm, 0,-1) , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_STR("]")                 , FSM_ACCEPT, block, (void *) & __ABNF_END_OPTION }
};

/* "(" *c-wsp alternation *c-wsp ")" */
static FsmTransition<String> group_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR("(")                , FSM_NORMAL, block, (void *) & __ABNF_BEGIN_GROUP }
	, { 2,-1, FSM_MATCH_RPT_FSM(c_wsp_fsm, 0,-1) , FSM_NORMAL, nullptr, nullptr }
	, { 3,-1, FSM_MATCH_FSM(alternation_fsm)     , FSM_NORMAL, nullptr, nullptr }
	, { 4,-1, FSM_MATCH_RPT_FSM(c_wsp_fsm, 0,-1) , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_STR(")")                 , FSM_ACCEPT, block, (void *) & __ABNF_END_GROUP }
};

/* rulename / group / option / char-val / num-val / prose-val */
FsmTransition<String> element_fsm[] = {
	  {-1, 1, FSM_MATCH_FSM(rulename_fsm),  FSM_ACCEPT, block, (void *) & __ABNF_ELEM_RULE_REF }
	, {-1, 2, FSM_MATCH_FSM(group_fsm),     FSM_ACCEPT, nullptr, nullptr }
	, {-1, 3, FSM_MATCH_FSM(option_fsm),    FSM_ACCEPT, nullptr, nullptr }
	, {-1, 4, FSM_MATCH_FSM(char_val_fsm),  FSM_ACCEPT, block, (void *) & __ABNF_ELEM_CHAR_VAL }
	, {-1, 5, FSM_MATCH_FSM(num_val_fsm),   FSM_ACCEPT, block, (void *) & __ABNF_ELEM_NUM_VAL }
	, {-1,-1, FSM_MATCH_FSM(prose_val_fsm), FSM_ACCEPT, block, (void *) & __ABNF_ELEM_PROSE_VAL }
};

/*
 rulename defined-as elements c-nl
                       ; continues if next line starts
                       ;  with white space
*/
static FsmTransition<String> rule_fsm[] = {
	  { 1,-1, FSM_MATCH_FSM(rulename_fsm)   , FSM_NORMAL, rulename, nullptr }
	, { 2,-1, FSM_MATCH_FSM(defined_as_fsm) , FSM_NORMAL, block, (void *) & __ABNF_BEGIN_RULE }
	, { 3,-1, FSM_MATCH_FSM(elements_fsm)   , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(c_nl_fsm)       , FSM_ACCEPT, block, (void *) & __ABNF_END_RULE }
};

/* 1*( rule / (*c-wsp c-nl) ) */
/* *c-wsp c-nl */
static FsmTransition<String> c_wsp_nl_fsm[] = {
	  { 1,-1, FSM_MATCH_RPT_FSM(c_wsp_fsm, 0,-1) , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(c_nl_fsm)            , FSM_ACCEPT, nullptr, nullptr }
};

/* rule / (*c-wsp c-nl) */
static FsmTransition<String> rule_c_wsp_nl_fsm[] = {
	  {-1, 1, FSM_MATCH_FSM(c_wsp_nl_fsm) , FSM_ACCEPT, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(rule_fsm)     , FSM_ACCEPT, nullptr, nullptr }
};

static FsmTransition<String> rulelist_fsm[] = {
	  { 1,-1, FSM_MATCH_NOTHING                          , FSM_NORMAL, block, (void *) & __ABNF_BEGIN_DOC }
	, { 2, 3, FSM_MATCH_RPT_FSM(rule_c_wsp_nl_fsm, 1,-1) , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_NOTHING                          , FSM_ACCEPT, block, (void *) & __ABNF_END_RULE }
	, {-1,-1, FSM_MATCH_NOTHING                          , FSM_ACCEPT, block, (void *) & __ABNF_END_DOC_FAIL }
};


static bool block (const String::const_iterator & begin, const String::const_iterator & end, void * context, void * action_args)
{
	const int *flag = (const int *)action_args;

	if (!context)
		return true;

	AbnfParseContext *ctx = __CAST_PARSE_CTX(context);

	switch (*flag) {
	case __ABNF_BEGIN_DOC    : return ctx->api.beginDocument(ctx->userContext);
	case __ABNF_END_DOC_OK   : return ctx->api.endDocument(true, ctx->userContext);
	case __ABNF_END_DOC_FAIL : return ctx->api.endDocument(false, ctx->userContext);
	case __ABNF_BEGIN_RULE   :
		if (String(begin, end).contains("=/")) {
			return ctx->api.beginRule(ctx->rulename, true, ctx->userContext);
		}
		return ctx->api.beginRule(ctx->rulename, false, ctx->userContext);

	case __ABNF_END_RULE          : return ctx->api.endRule(ctx->userContext);
	case __ABNF_BEGIN_ALTERN : return ctx->api.beginAlternation(ctx->userContext);
	case __ABNF_END_ALTERN   : return ctx->api.endAlternation(ctx->userContext);
	case __ABNF_BEGIN_CONCAT      : return ctx->api.beginConcatenation(ctx->userContext);
	case __ABNF_END_CONCAT        : return ctx->api.endConcatenation(ctx->userContext);
	case __ABNF_BEGIN_RPT: {
		int from = ctx->rpt.from, to = ctx->rpt.to;
		if (ctx->rpt.to == CWT_INT_MIN)
			from = to = 1;
		ctx->rpt.from = ctx->rpt.to = CWT_INT_MIN;
		return ctx->api.beginRepetition(from, to, ctx->userContext);
	}
	case __ABNF_END_RPT           : return ctx->api.endRepetition(ctx->userContext);
	case __ABNF_BEGIN_OPTION      : return ctx->api.beginOption(ctx->userContext);
	case __ABNF_END_OPTION        : return ctx->api.endOption(ctx->userContext);
	case __ABNF_BEGIN_GROUP       : return ctx->api.beginGroup(ctx->userContext);
	case __ABNF_END_GROUP         : return ctx->api.endGroup(ctx->userContext);
	case __ABNF_ELEM_RULE_REF     : return ctx->api.ruleRef(String(begin, end), ctx->userContext);
	case __ABNF_ELEM_CHAR_VAL     : {
		String s(begin, end);
		CWT_ASSERT(s.startsWith("\""));
		CWT_ASSERT(s.endsWith("\""));
		return ctx->api.charVal(s.substr(1, s.length()-2), ctx->userContext);
	}
	case __ABNF_ELEM_NUM_VAL      : return ctx->api.numVal(String(begin, end), ctx->userContext);
	case __ABNF_ELEM_PROSE_VAL    : return ctx->api.proseVal(String(begin, end), ctx->userContext);

	default:
		break;
	}

	return false;
}

static bool rulename (const String::const_iterator & begin, const String::const_iterator & end, void * context, void * )
{
	if (context) {
		AbnfParseContext * ctx = __CAST_PARSE_CTX(context);
		ctx->rulename = String(begin, end);
	}
	return true;
}

/*
static bool comment (const String::const_iterator & begin, const String::const_iterator & end, void * context, void *)
{
	if (!context)
		return true;

	AbnfParseContext * ctx = __CAST_PARSE_CTX(context);
	return ctx->api.comment(String(begin, end), ctx->userContext);
}
*/

static bool rpt (const String::const_iterator & begin, const String::const_iterator & end, void * context, void * action_args)
{
	const int * flag = (const int *)action_args;

	if (context) {
		AbnfParseContext * ctx = __CAST_PARSE_CTX(context);
		bool ok = true;
		int value = 0;

		if (begin < end) {
			value = String(begin, end).toInt(& ok, 10);
			if (!ok) {
				Logger::error("invalid repetition value");
				return false;
			}
		}

		switch (*flag) {
		case __ABNF_RPT_FROM:
			ctx->rpt.from = begin < end ? value : 0;
			break;
		case __ABNF_RPT_TO:
			ctx->rpt.to = begin < end ? value : -1;
			break;
		case __ABNF_RPT_FROMTO:
			ctx->rpt.from = ctx->rpt.to = begin < end ? value : 1;
			break;
		default:
			return false;
		}
	}
	return true;
}

CWT_NS_END

#endif /* __CWT_ABNF_RFC5234_H__ */
