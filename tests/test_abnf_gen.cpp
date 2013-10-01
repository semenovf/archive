/**
 * @file   test_abnf_gen.cpp
 * @author wladt
 * @date   Oct 1, 2013
 *
 * @brief
 */

#include <cwt/test.h>
#include <cwt/abnf.hpp>
#include <cwt/logger.hpp>

using namespace cwt;

/*
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

void test_abnf_gen()
{
	AbnfRuleSet & ruleset = Abnf::createRuleSet ();

	AbnfRule & rule = ruleset.addRule("rulelist");

	Abnf::destroyRuleSet (ruleset);
}

int main(int argc, char *argv[])
{
	CWT_UNUSED(argc);
	CWT_UNUSED(argv);
	CWT_BEGIN_TESTS(0);

	test_abnf_gen();

	CWT_END_TESTS;
}

