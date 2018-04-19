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
#include <iostream>

using namespace cwt;

extern void buildAbnfRuleList (AbnfRuleList & rulelist);

void test_abnf_normalize()
{
	AbnfRuleList rulelist;
	buildAbnfRuleList(rulelist);

	CWT_TEST_OK(rulelist.normalize());

	std::cout << "== ABNF: ==========================" << std::endl;
	std::cout << rulelist.toString();// << std::endl;
	std::cout << "===================================" << std::endl;

	// 1*( rule / (*c-wsp c-nl) )
	CWT_TEST_OK(rulelist.find("rulelist") && rulelist.find("rulelist")->toString()
			== String("rulelist = 1*rulelist-0"));
	CWT_TEST_OK(rulelist.find("rulelist-0") && rulelist.find("rulelist-0")->toString()
			== String("rulelist-0 = rule / rulelist-1"));
	CWT_TEST_OK(rulelist.find("rulelist-1") && rulelist.find("rulelist-1")->toString()
			== String("rulelist-1 = *c-wsp c-nl"));

	//	rule = rulename defined-as elements c-nl
	//		; continues if next line starts
	//		; with white space
	CWT_TEST_OK(rulelist.find("rule") && rulelist.find("rule")->toString()
			== String("rule = rulename defined-as elements c-nl")
				+ String::EndOfLine
				+ String("\t\t; continues if next line starts") + String::EndOfLine
				+ String("\t\t; with white space") + String::EndOfLine);

	//	rulename =  ALPHA *( ALPHA / DIGIT / "-" )
	CWT_TEST_OK(rulelist.find("rulename") && rulelist.find("rulename")->toString()
			== String("rulename = ALPHA *rulename-0"));

	CWT_TEST_OK(rulelist.find("rulename-0") && rulelist.find("rulename-0")->toString()
			== String("rulename-0 = ALPHA / DIGIT / \"-\""));

	//	defined-as = *c-wsp (\"=\" / \"=/\") *c-wsp
	//		   ; basic rules definition and
	//		   ; incremental alternatives
	//CWT_TEST_FAIL(rulelist.find("defined-as"));
	//std::cout << '[' << rulelist.find("defined-as")->toString() << ']' << std::endl;
	CWT_TEST_OK(rulelist.find("defined-as") && rulelist.find("defined-as")->toString()
			== String("defined-as = *c-wsp defined-as-0 *c-wsp")
			+ String::EndOfLine
			+ String("\t\t; basic rules definition and") + String::EndOfLine
			+ String("\t\t; incremental alternatives") + String::EndOfLine);

	//	elements = alternation *c-wsp
	CWT_TEST_OK(rulelist.find("elements") && rulelist.find("elements")->toString()
			== String("elements = alternation *c-wsp"));

	//	c-wsp =  WSP / ( c-nl WSP )
	CWT_TEST_OK(rulelist.find("c-wsp") && rulelist.find("c-wsp")->toString()
			== String("c-wsp = WSP / c-wsp-0"));

	CWT_TEST_OK(rulelist.find("c-wsp-0") && rulelist.find("c-wsp-0")->toString()
			== String("c-wsp-0 = c-nl WSP"));

	//	c-nl = comment / NL
	//			; comment or newline
	CWT_TEST_OK(rulelist.find("c-nl") && rulelist.find("c-nl")->toString()
			== String("c-nl = comment / NL")
			+ String::EndOfLine
			+ String("\t\t; comment or newline") + String::EndOfLine);

	//	comment = ";" *( WSP / VCHAR ) NL
	CWT_TEST_OK(rulelist.find("comment") && rulelist.find("comment")->toString()
			== String("comment = \";\" *comment-0 NL"));

	CWT_TEST_OK(rulelist.find("comment-0") && rulelist.find("comment-0")->toString()
			== String("comment-0 = WSP / VCHAR"));

	//	alternation = concatenation *( *c-wsp "/" *c-wsp concatenation )
	CWT_TEST_OK(rulelist.find("alternation") && rulelist.find("alternation")->toString()
			== String("alternation = concatenation *alternation-0"));

	CWT_TEST_OK(rulelist.find("alternation-0") && rulelist.find("alternation-0")->toString()
			== String("alternation-0 = *c-wsp \"/\" *c-wsp concatenation"));

	// concatenation = repetition *( 1*c-wsp repetition )
	CWT_TEST_OK(rulelist.find("concatenation") && rulelist.find("concatenation")->toString()
			== String("concatenation = repetition *concatenation-0"));

	CWT_TEST_OK(rulelist.find("concatenation-0") && rulelist.find("concatenation-0")->toString()
			== String("concatenation-0 = 1*c-wsp repetition"));

	// repetition = [ repeat ] element
	CWT_TEST_OK(rulelist.find("repetition") && rulelist.find("repetition")->toString()
			== String("repetition = [ repeat ] element"));

	//	repeat = ( *DIGIT "*" *DIGIT ) / 1*DIGIT
	CWT_TEST_OK(rulelist.find("repeat") && rulelist.find("repeat")->toString()
			== String("repeat = repeat-0 / 1*DIGIT"));

	CWT_TEST_OK(rulelist.find("repeat-0") && rulelist.find("repeat-0")->toString()
			== String("repeat-0 = *DIGIT \"*\" *DIGIT"));

	//	element = rulename / group / option / char-val / num-val / prose-val
	CWT_TEST_OK(rulelist.find("element") && rulelist.find("element")->toString()
			== String("element = rulename / group / option / char-val / num-val / prose-val"));

	//	group = "(" *c-wsp alternation *c-wsp ")"
	CWT_TEST_OK(rulelist.find("group") && rulelist.find("group")->toString()
			== String("group = \"(\" *c-wsp alternation *c-wsp \")\""));

	//	option = "[" *c-wsp alternation *c-wsp "]"
	CWT_TEST_OK(rulelist.find("option") && rulelist.find("option")->toString()
			== String("option = \"[\" *c-wsp alternation *c-wsp \"]\""));

	//	char-val = DQUOTE *( %x20-21 / %x23-7E ) DQUOTE
	//			; quoted string of SP and VCHAR
	//			; without DQUOTE
	CWT_TEST_OK(rulelist.find("char-val") && rulelist.find("char-val")->toString()
			== String("char-val = DQUOTE *char-val-0 DQUOTE")
			+ String::EndOfLine
			+ String("\t\t; quoted string of SP and VCHAR") + String::EndOfLine
			+ String("\t\t; without DQUOTE") + String::EndOfLine);

	CWT_TEST_OK(rulelist.find("char-val-0") && rulelist.find("char-val-0")->toString()
			== String("char-val-0 = %x20-21 / %x23-7E"));

	//	num-val = "%" ( bin-val / dec-val / hex-val )
	CWT_TEST_OK(rulelist.find("num-val") && rulelist.find("num-val")->toString()
			== String("num-val = \"%\" num-val-0"));

	CWT_TEST_OK(rulelist.find("num-val-0") && rulelist.find("num-val-0")->toString()
			== String("num-val-0 = bin-val / dec-val / hex-val"));

	//	bin-val = "b" 1*BIT [ 1*( "." 1*BIT ) / ( "-" 1*BIT ) ]
	//			; series of concatenated bit values
	//			; or single ONEOF range
	CWT_TEST_OK(rulelist.find("bin-val") && rulelist.find("bin-val")->toString()
			== String("bin-val = \"b\" 1*BIT [ bin-val-0 ]")
			+ String::EndOfLine
			+ String("\t\t; series of concatenated bit values") + String::EndOfLine
			+ String("\t\t; or single ONEOF range") + String::EndOfLine);

	CWT_TEST_OK(rulelist.find("bin-val-0") && rulelist.find("bin-val-0")->toString()
			== String("bin-val-0 = 1*bin-val-1 / bin-val-2"));

	CWT_TEST_OK(rulelist.find("bin-val-1") && rulelist.find("bin-val-1")->toString()
			== String("bin-val-1 = \".\" 1*BIT"));

	CWT_TEST_OK(rulelist.find("bin-val-2") && rulelist.find("bin-val-2")->toString()
			== String("bin-val-2 = \"-\" 1*BIT"));

	//	dec-val = "d" 1*DIGIT [ 1*( "." 1*DIGIT ) / ( "-" 1*DIGIT ) ]
	CWT_TEST_OK(rulelist.find("dec-val") && rulelist.find("dec-val")->toString()
			== String("dec-val = \"d\" 1*DIGIT [ dec-val-0 ]"));

	CWT_TEST_OK(rulelist.find("dec-val-0") && rulelist.find("dec-val-0")->toString()
			== String("dec-val-0 = 1*dec-val-1 / dec-val-2"));

	CWT_TEST_OK(rulelist.find("dec-val-1") && rulelist.find("dec-val-1")->toString()
			== String("dec-val-1 = \".\" 1*DIGIT"));

	CWT_TEST_OK(rulelist.find("dec-val-2") && rulelist.find("dec-val-2")->toString()
			== String("dec-val-2 = \"-\" 1*DIGIT"));

	//	hex-val = "x" 1*HEXDIG [ 1*( "." 1*HEXDIG ) / ( "-" 1*HEXDIG ) ]
	CWT_TEST_OK(rulelist.find("hex-val") && rulelist.find("hex-val")->toString()
			== String("hex-val = \"x\" 1*HEXDIG [ hex-val-0 ]"));

	CWT_TEST_OK(rulelist.find("hex-val-0") && rulelist.find("hex-val-0")->toString()
			== String("hex-val-0 = 1*hex-val-1 / hex-val-2"));

	CWT_TEST_OK(rulelist.find("hex-val-1") && rulelist.find("hex-val-1")->toString()
			== String("hex-val-1 = \".\" 1*HEXDIG"));

	CWT_TEST_OK(rulelist.find("hex-val-2") && rulelist.find("hex-val-2")->toString()
			== String("hex-val-2 = \"-\" 1*HEXDIG"));

	//	prose-val = "<" *( %x20-3D / %x3F-7E ) ">"
	//			; bracketed string of SP and VCHAR
	//			;  without angles
	//			; prose description, to be used as
	//			;  last resort

}

int main(int argc, char *argv[])
{
	CWT_UNUSED(argc);
	CWT_UNUSED(argv);
	CWT_BEGIN_TESTS(40);

	test_abnf_normalize();

	CWT_END_TESTS;
}

