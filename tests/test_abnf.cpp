/**
 * @file   abnf.c
 * @author wladt
 * @date   Oct 31, 2012 15:54:00
 *
 * @brief
 */


#include <cwt/abnf.hpp>
#include <cwt/fsm_test.hpp>
#include <cwt/logger.hpp>
#include "../src/abnf-rfc5234.hpp"

static FsmTestEntry __fsmTestEntries[] = {
	/* ";" *(WSP / VCHAR) NL */
	  { VHEADER(comment_fsm)
		, { ";\r\n", "; comment line \r\n", "; comment\r\n", "; \t\t \r\n", VNULL }
		, {   {-1, ";" }
/*			, {-1, ";\n" }*/
/*			, {-1, ";\r" }*/
			, {-1, "comment\r\n" }
			, INULL }}

	/* WSP / (c-nl WSP) */
	, { VHEADER(c_wsp_fsm)
		, { "\t", "; comment line \r\n ", "\r\n\t", "; \t\t \r\n ", VNULL }
		, {   { 3, "\r\n\t " }
			, {-1, ";\n" }
			, {-1, ";\r" }
			, {-1, "comment\r\n" }
			, INULL }}

	/*  *c-wsp ("=" / "=/") *c-wsp */
	, { VHEADER(defined_as_fsm)
		, { "\t =", " \t = \t ", "\t =/", " \t =/ \t ", VNULL }
		, { INULL }}

	, { VHEADER(defined_as_fsm)
		, {   "; comment line \r\n ="
			, "; comment line \r\n ; comment line \r\n\t=/"
			, "; comment line \r\n ; comment line \r\n\t= \r\n\t"
			, VNULL }
		, { INULL }}

	, { VHEADER(defined_as_fsm)
		, { "\r\n\t =", "\r\n\t = \r\n\t\r\n\t", "\r\n\t \r\n\t =/", "\r\n\t =/ \r\n\t\r\n\t", VNULL }
		, { INULL }}

	/* "x" 1*HEXDIG [ 1*("." 1*HEXDIG) / ("-" 1*HEXDIG) ] */
	, { VHEADER(hex_val_fsm)
		, { "xA1B2C3D4", "xA1B2.C3D4", "xDEAD.BEEF.321.12345", "xA5-987", VNULL }
		, {   {-1, "FE2345" }
			, {-1, "xQWER" }
			, { 6, "xA5-DE-AD" }
		    , {-1, "x" }
			, INULL }}

	/* "d" 1*DIGIT [ 1*("." 1*DIGIT) / ("-" 1*DIGIT) ] */
	, { VHEADER(dec_val_fsm)
		, { "d1234567890", "d987.654", "d987.654.321.12345", "d123-987", VNULL }
		, {   {-1, "12345" }
			, {-1, "dABC" }
			, { 6, "d12-34-56" }
			, INULL }}

	/* "b" 1*BIT [ 1*("." 1*BIT) / ("-" 1*BIT) ] */
	, { VHEADER(bin_val_fsm)
		, { "b1010101", "b10101.101", "b10101.000.001.11100", "b10101-11111", VNULL }
		, {   {-1, "010101" }
			, {-1, "b234" }
			, { 6, "b00-01-10" }
			, INULL }}

	/*  "%" (bin-val / dec-val / hex-val) */
	, { VHEADER(num_val_fsm)
		, { "%xDEADBEEF", "%d987654", "%b0101", "%xDEAD.BEEF.321.12345", VNULL }
		, {   {-1, "%dABC" }
			, {-1, "%xQWERTY" }
			, { 8, "%xABCD98?" }
			, {-1, "%%%" }
			, INULL }}

	/*"<" *(%x20-3D / %x3F-7E) ">"*/
	, { VHEADER(prose_val_fsm)
		, { "<Hello, World!>", "<--! HTML comment -->", "</end-tag>", "<empty-tag/>", VNULL }
		, {   {-1, "Hello,>" }
			, {-1, "<World" }
			, {15, "<Hello, World!> \n" }
			, {-1, "<\n>" }
			, INULL }}


	, { VHEADER(char_val_fsm)
		, { "\"\"", "\"?!@#$%^\"", "\" !0!1!2!3! \"", "\"  Hello, World!  \"", VNULL }
		, {   {-1, "\"0123  " }
			, {-1, "\"\n\"" }
			, {-1, "\"  \t  \"" }
			, INULL }}

	/* comment / CRLF ; comment or newline */
	, { VHEADER(c_nl_fsm)
		, { ";\r\n", "; comment line \r\n", "\r\n", "; \t\t \r\n", VNULL }
		, {   { 2, "\r\n  " }
/*
			, {-1, ";\n" }
			, {-1, ";\r" }
*/
			, {-1, "comment\r\n" }
			, INULL }}

	/* ALPHA *(ALPHA / DIGIT / "-") */
	, { VHEADER(rulename_fsm)
		, { "A", "A9", "A-", "A-9", VNULL }
		, {   {-1, "9A" }
			, {-1, "-A" }
			, {-1, "?%" }
			, {-1, "/" }
			, INULL }}

	/* 1*DIGIT / (*DIGIT "*" *DIGIT) */
	, { VHEADER(repeat_fsm)
		, { "1", "*5", "1234", "5*", VNULL }
		, { INULL }}

	, { VHEADER(repeat_fsm)
		, { "1*5", "1234*2345", VNULL }
		, { INULL }}

	/* [repeat] element*/
	, { VHEADER(repetition_fsm)
		, { "element-name", "1one-elem", "1*one-more-elem", "2*5two-to-five-elems", VNULL }
		, { INULL }}

	/* repetition *(1*c-wsp repetition) */
	, { VHEADER(concatenation_fsm)
		, {   "1one-elem"
			, "1one-elem element-name"
			, "1*one-more-elem  \t  2*5two-to-five-elems"
			, "2*5two-to-five-elems"
			, VNULL }
		, { INULL }}

	/* concatenation *(*c-wsp "/" *c-wsp concatenation) */
	, { VHEADER(alternation_fsm)
		, {   "alter0 / alter1"
			, "2*5two-to-five-elems"
			, "1*one-more-elem  \t  2*5two-to-five-elems / 2*5two-to-five-elems"
			, "1*one-more-elem  \t  2*5two-to-five-elems / 2*5two-to-five-elems / 1one-elem element-name"
			, VNULL }
		, { INULL }}

	/* "(" *c-wsp alternation *c-wsp ")" */
	, { VHEADER(group_fsm)
		, {   "( alter0 / alter1 )"
			, "( 2*5two-to-five-elems )"
			, "( 1*one-more-elem  \t  2*5two-to-five-elems / 2*5two-to-five-elems )"
			, "( 1*one-more-elem  \t  2*5two-to-five-elems / 2*5two-to-five-elems / 1one-elem element-name )"
			, VNULL }
		, { INULL }}

	/* "[" *c-wsp alternation *c-wsp "]" */
	, { VHEADER(option_fsm)
		, {   "[ alter0 / alter1 ]"
			, "[ 2*5two-to-five-elems ]"
			, "[ 1*one-more-elem  \t  2*5two-to-five-elems / 2*5two-to-five-elems ]"
			, "[ 1*one-more-elem  \t  2*5two-to-five-elems / 2*5two-to-five-elems / 1one-elem element-name ]"
			, VNULL }
		, { INULL }}

	/* rulename / group / option / char-val / num-val / prose-val */
	, { VHEADER(element_fsm)
		, {   "rule-name"
			, "( 1*one-more-elem  \t  2*5two-to-five-elems / 2*5two-to-five-elems / 1one-elem element-name )"
			, "[ 1*one-more-elem  \t  2*5two-to-five-elems / 2*5two-to-five-elems / 1one-elem element-name ]"
			, VNULL }
		, { INULL }}

	, { VHEADER(element_fsm)
		, {   "\"  Hello, World!  \""
			, "%xDEAD.BEEF.321.12345"
			, "b10101-11111"
			, "<--! HTML comment -->"
			, VNULL }
		, { INULL }}

	/* alternation *c-wsp */
	, { VHEADER(elements_fsm)
		, {   "alter0 / alter1 \t "
			, "2*5two-to-five-elems   "
			, "1*one-more-elem  \t  2*5two-to-five-elems / 2*5two-to-five-elems"
			, "1*one-more-elem  \t  2*5two-to-five-elems / 2*5two-to-five-elems / 1one-elem element-name\t\t  \t\t  "
			, VNULL }
		, { INULL }}

	/* rulename defined-as elements c-nl
						   ; continues if next line starts
						   ;  with white space
	*/
	, { VHEADER(rule_fsm)
		, {  "comment        =  \";\" *(WSP / VCHAR) CRLF\r\n"
			,"element        =  rulename / group / option /\r\n\tchar-val / num-val / prose-val\r\n"
			, VNULL }
		, { INULL }}

	/* 1*( rule / (*c-wsp c-nl) ) */
	, { VHEADER(rulelist_fsm)
		, {  "comment        =  \";\" *(WSP / VCHAR) CRLF\r\n"
		     "element        =  rulename / group / option /\r\n\tchar-val / num-val / prose-val\r\n"
			, VNULL }
		, { INULL }}

};


static void test_abnf_fsm()
{
	static Fsm<String> fsm(nullptr, nullptr);
	int nentries = sizeof(__fsmTestEntries)/sizeof(__fsmTestEntries[0]);

	for( int i = 0; i < nentries; i++ )
		fsm_test_entries(fsm, &__fsmTestEntries[i]);
}


static const char *abnf_test_str = "a = b;c";
//static const char *abnf_test_str = "comment        =  \";\" *(WSP / VCHAR) CRLF\r\n";

/*
static const char *abnf_test_str = "\n\
rulelist       =  1*( rule / (*c-wsp c-nl) )                     \n\
																 \n\
rule           =  rulename defined-as elements c-nl              \n\
					   ; continues if next line starts           \n\
					   ;  with white space                       \n\
																 \n\
rulename       =  ALPHA *(ALPHA / DIGIT / \"-\")                 \n\
																 \n\
defined-as     =  *c-wsp (\"=\" / \"=/\") *c-wsp                 \n\
					   ; basic rules definition and              \n\
					   ;  incremental alternatives               \n\
																 \n\
elements       =  alternation *c-wsp                             \n\
																 \n\
c-wsp          =  WSP / (c-nl WSP)                               \n\
																 \n\
c-nl           =  comment / NL                                   \n\
					   ; comment or newline                      \n\
																 \n\
comment        =  \";\" *(WSP / VCHAR) NL                        \n\
																 \n\
alternation    =  concatenation                                  \n\
				  *(*c-wsp \"/\" *c-wsp concatenation)           \n\
																 \n\
concatenation  =  repetition *(1*c-wsp repetition)               \n\
																 \n\
repetition     =  [repeat] element                               \n\
																 \n\
repeat         =  *DIGIT \"*\" *DIGIT                            \n\
repeat         =/ 1*DIGIT                                        \n\
																 \n\
element        =  rulename / group / option /                    \n\
				  char-val / num-val / prose-val                 \n\
																 \n\
group          =  \"(\" *c-wsp alternation *c-wsp \")\"          \n\
																 \n\
option         =  \"[\" *c-wsp alternation *c-wsp \"]\"          \n\
																 \n\
char-val       =  DQUOTE *(%x20-21 / %x23-7E) DQUOTE             \n\
					  ; quoted string of SP and VCHAR            \n\
					   ;  without DQUOTE                         \n\
																 \n\
num-val        =  \"%\" (bin-val / dec-val / hex-val)            \n\
																 \n\
bin-val        =  \"b\" 1*BIT                                    \n\
				  [ 1*(\".\" 1*BIT) / (\"-\" 1*BIT) ]            \n\
					   ; series of concatenated bit values       \n\
					   ;  or single ONEOF range                  \n\
																 \n\
dec-val        =  \"d\" 1*DIGIT                                  \n\
				  [ 1*(\".\" 1*DIGIT) / (\"-\" 1*DIGIT) ]        \n\
																 \n\
hex-val        =  \"x\" 1*HEXDIG                                 \n\
				  [ 1*(\".\" 1*HEXDIG) / (\"-\" 1*HEXDIG) ]      \n\
																 \n\
prose-val      =  \"<\" *(%x20-3D / %x3F-7E) \">\"               \n\
					   ; bracketed string of SP and VCHAR        \n\
					   ;  without angles                         \n\
					   ; prose description, to be used as        \n\
					   ;  last resort                            \n\
";
*/

static void test_abnf()
{
	Abnf abnf;
	AbnfRuleList ruleset;
	CWT_TEST_FAIL(abnf.parse(String::fromUtf8(abnf_test_str), ruleset));
}

int main(int argc, char *argv[])
{
	CWT_UNUSED(argc);
	CWT_UNUSED(argv);
	CWT_BEGIN_TESTS(125);

	if (0) test_abnf_fsm();
	if (1) test_abnf();

	CWT_END_TESTS;
}

