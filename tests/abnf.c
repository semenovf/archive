/**
 * @file   abnf.c
 * @author wladt
 * @date   Oct 31, 2012 15:54:00
 *
 * @brief
 */

#include <cwt/abnf.h>
#include <cwt/fsm_test.h>

#include "../src/abnf-rfc5234.h"

static struct _FsmTestEntry __fsmTestEntries[] = {
	/* ";" *(WSP / VCHAR) NL */
	  { VHEADER(comment_fsm)
		, { _T(";\r\n"), _T("; comment line \r\n"), _T("; comment\r\n"), _T("; \t\t \r\n"), VNULL }
		, {   {-1, _T(";") }
			, {-1, _T(";\n") }
			, {-1, _T(";\r") }
			, {-1, _T("comment\r\n") }
			, INULL }}

	/* WSP / (c-nl WSP) */
	, { VHEADER(c_wsp_fsm)
		, { _T("\t"), _T("; comment line \r\n "), _T("\r\n\t"), _T("; \t\t \r\n "), VNULL }
		, {   { 3, _T("\r\n\t ") }
			, {-1, _T(";\n") }
			, {-1, _T(";\r") }
			, {-1, _T("comment\r\n") }
			, INULL }}

	/*  *c-wsp ("=" / "=/") *c-wsp */
	, { VHEADER(defined_as_fsm)
		, { _T("\t ="), _T(" \t = \t "), _T("\t =/"), _T(" \t =/ \t "), VNULL }
		, { INULL }}

	, { VHEADER(defined_as_fsm)
		, {   _T("; comment line \r\n =")
			, _T("; comment line \r\n ; comment line \r\n\t=/")
			, _T("; comment line \r\n ; comment line \r\n\t= \r\n\t")
			, VNULL }
		, { INULL }}

	, { VHEADER(defined_as_fsm)
		, { _T("\r\n\t ="), _T("\r\n\t = \r\n\t\r\n\t"), _T("\r\n\t \r\n\t =/"), _T("\r\n\t =/ \r\n\t\r\n\t"), VNULL }
		, { INULL }}

	/* "x" 1*HEXDIG [ 1*("." 1*HEXDIG) / ("-" 1*HEXDIG) ] */
	, { VHEADER(hex_val_fsm)
		, { _T("xA1B2C3D4"), _T("xA1B2.C3D4"), _T("xDEAD.BEEF.321.12345"), _T("xA5-987"), VNULL }
		, {   {-1, _T("FE2345") }
			, {-1, _T("xQWER") }
			, { 6, _T("xA5-DE-AD") }
		    , {-1, _T("x") }
			, INULL }}

	/* "d" 1*DIGIT [ 1*("." 1*DIGIT) / ("-" 1*DIGIT) ] */
	, { VHEADER(dec_val_fsm)
		, { _T("d1234567890"), _T("d987.654"), _T("d987.654.321.12345"), _T("d123-987"), VNULL }
		, {   {-1, _T("12345") }
			, {-1, _T("dABC") }
			, { 6, _T("d12-34-56") }
			, INULL }}

	/* "b" 1*BIT [ 1*("." 1*BIT) / ("-" 1*BIT) ] */
	, { VHEADER(bin_val_fsm)
		, { _T("b1010101"), _T("b10101.101"), _T("b10101.000.001.11100"), _T("b10101-11111"), VNULL }
		, {   {-1, _T("010101") }
			, {-1, _T("b234") }
			, { 6, _T("b00-01-10") }
			, INULL }}

	/*  "%" (bin-val / dec-val / hex-val) */
	, { VHEADER(num_val_fsm)
		, { _T("%xDEADBEEF"), _T("%d987654"), _T("%b0101"), _T("%xDEAD.BEEF.321.12345"), VNULL }
		, {   {-1, _T("%dABC") }
			, {-1, _T("%xQWERTY") }
			, { 8, _T("%xABCD98?") }
			, {-1, _T("%%%") }
			, INULL }}

	/*"<" *(%x20-3D / %x3F-7E) ">"*/
	, { VHEADER(prose_val_fsm)
		, { _T("<Hello, World!>"), _T("<--! HTML comment -->"), _T("</end-tag>"), _T("<empty-tag/>"), VNULL }
		, {   {-1, _T("Hello,>") }
			, {-1, _T("<World") }
			, {15, _T("<Hello, World!> \n") }
			, {-1, _T("<\n>") }
			, INULL }}


	, { VHEADER(char_val_fsm)
		, { _T("\"\""), _T("\"?!@#$%^\""), _T("\" !0!1!2!3! \""), _T("\"  Hello, World!  \""), VNULL }
		, {   {-1, _T("\"0123  ") }
			, {-1, _T("\"\n\"") }
			, {-1, _T("\"  \t  \"") }
			, INULL }}

	/* comment / CRLF ; comment or newline */
	, { VHEADER(c_nl_fsm)
		, { _T(";\r\n"), _T("; comment line \r\n"), _T("\r\n"), _T("; \t\t \r\n"), VNULL }
		, {   { 2, _T("\r\n  ") }
			, {-1, _T(";\n") }
			, {-1, _T(";\r") }
			, {-1, _T("comment\r\n") }
			, INULL }}

	/* ALPHA *(ALPHA / DIGIT / "-") */
	, { VHEADER(rulename_fsm)
		, { _T("A"), _T("A9"), _T("A-"), _T("A-9"), VNULL }
		, {   {-1, _T("9A") }
			, {-1, _T("-A") }
			, {-1, _T("?%") }
			, {-1, _T("/") }
			, INULL }}

	/* 1*DIGIT / (*DIGIT "*" *DIGIT) */
	, { VHEADER(repeat_fsm)
		, { _T("1"), _T("*5"), _T("1234"), _T("5*"), VNULL }
		, { INULL }}

	, { VHEADER(repeat_fsm)
		, { _T("1*5"), _T("1234*2345"), VNULL }
		, { INULL }}

	/* [repeat] element*/
	, { VHEADER(repetition_fsm)
		, { _T("element-name"), _T("1one-elem"), _T("1*one-more-elem"), _T("2*5two-to-five-elems"), VNULL }
		, { INULL }}

	/* repetition *(1*c-wsp repetition) */
	, { VHEADER(concatenation_fsm)
		, {   _T("1one-elem")
			, _T("1one-elem element-name")
			, _T("1*one-more-elem  \t  2*5two-to-five-elems")
			, _T("2*5two-to-five-elems")
			, VNULL }
		, { INULL }}

	/* concatenation *(*c-wsp "/" *c-wsp concatenation) */
	, { VHEADER(alternation_fsm)
		, {   _T("alter0 / alter1")
			, _T("2*5two-to-five-elems")
			, _T("1*one-more-elem  \t  2*5two-to-five-elems / 2*5two-to-five-elems")
			, _T("1*one-more-elem  \t  2*5two-to-five-elems / 2*5two-to-five-elems / 1one-elem element-name")
			, VNULL }
		, { INULL }}

	/* "(" *c-wsp alternation *c-wsp ")" */
	, { VHEADER(group_fsm)
		, {   _T("( alter0 / alter1 )")
			, _T("( 2*5two-to-five-elems )")
			, _T("( 1*one-more-elem  \t  2*5two-to-five-elems / 2*5two-to-five-elems )")
			, _T("( 1*one-more-elem  \t  2*5two-to-five-elems / 2*5two-to-five-elems / 1one-elem element-name )")
			, VNULL }
		, { INULL }}

	/* "[" *c-wsp alternation *c-wsp "]" */
	, { VHEADER(option_fsm)
		, {   _T("[ alter0 / alter1 ]")
			, _T("[ 2*5two-to-five-elems ]")
			, _T("[ 1*one-more-elem  \t  2*5two-to-five-elems / 2*5two-to-five-elems ]")
			, _T("[ 1*one-more-elem  \t  2*5two-to-five-elems / 2*5two-to-five-elems / 1one-elem element-name ]")
			, VNULL }
		, { INULL }}

	/* rulename / group / option / char-val / num-val / prose-val */
	, { VHEADER(element_fsm)
		, {   _T("rule-name")
			, _T("( 1*one-more-elem  \t  2*5two-to-five-elems / 2*5two-to-five-elems / 1one-elem element-name )")
			, _T("[ 1*one-more-elem  \t  2*5two-to-five-elems / 2*5two-to-five-elems / 1one-elem element-name ]")
			, VNULL }
		, { INULL }}

	, { VHEADER(element_fsm)
		, {   _T("\"  Hello, World!  \"")
			, _T("%xDEAD.BEEF.321.12345")
			, _T("b10101-11111")
			, _T("<--! HTML comment -->")
			, VNULL }
		, { INULL }}

	/* alternation *c-wsp */
	, { VHEADER(elements_fsm)
		, {   _T("alter0 / alter1 \t ")
			, _T("2*5two-to-five-elems   ")
			, _T("1*one-more-elem  \t  2*5two-to-five-elems / 2*5two-to-five-elems")
			, _T("1*one-more-elem  \t  2*5two-to-five-elems / 2*5two-to-five-elems / 1one-elem element-name\t\t  \t\t  ")
			, VNULL }
		, { INULL }}

	/* rulename defined-as elements c-nl
						   ; continues if next line starts
						   ;  with white space
	*/
	, { VHEADER(rule_fsm)
		, { _T("comment        =  \";\" *(WSP / VCHAR) CRLF\r\n")
			, _T("element        =  rulename / group / option /\r\n\tchar-val / num-val / prose-val\r\n")
			, VNULL }
		, { INULL }}

	/* 1*( rule / (*c-wsp c-nl) ) */
	, { VHEADER(rulelist_fsm)
		, { _T("comment        =  \";\" *(WSP / VCHAR) CRLF\r\n"
		       "element        =  rulename / group / option /\r\n\tchar-val / num-val / prose-val\r\n")
			, VNULL }
		, { INULL }}

};


int main(int argc, char *argv[])
{
	static CwtFsm fsm;
	int i;
	int nentries = sizeof(__fsmTestEntries)/sizeof(__fsmTestEntries[0]);

	CWT_CHAR *nl = _T("\r\n");

	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	cwt_fsm_ns()->init(&fsm, sizeof(CWT_CHAR), NULL, NULL, cwt_fsm_belong_cwtchar, cwt_fsm_exact_cwtchar, cwt_fsm_range_cwtchar);
	cwt_fsm_ns()->setCustomNewLine(&fsm, nl);

	CWT_BEGIN_TESTS(128);

	for( i = 0; i < nentries; i++ )
		fsm_test_entries(&fsm, &__fsmTestEntries[i]);

	cwt_fsm_ns()->destroy(&fsm);

	CWT_END_TESTS;
}

