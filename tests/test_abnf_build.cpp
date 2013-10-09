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

void test_abnf_build()
{
	AbnfRuleList rulelist;
	buildAbnfRuleList(rulelist);

#define __CWT_TEST_RULE(orig,rulename) {                                             \
		AbnfRule * __rule = rulelist.find(rulename);                                 \
		CWT_TEST_FAIL2(__rule                                                        \
	             && (orig) == __rule->toString()                                     \
				, String(_F("[%s] == [%s]") % (orig) % __rule->toString()).c_str()); \
	}

	String s("rulelist = 1*( rule / ( *c-wsp c-nl ) )");
	__CWT_TEST_RULE(s, "rulelist");

	s = String("rule = rulename defined-as elements c-nl") + String::EndOfLine
				+ String("\t\t; continues if next line starts") + String::EndOfLine
				+ String("\t\t; with white space") + String::EndOfLine;
	__CWT_TEST_RULE(s, "rule");

	s = String("rulename = ALPHA *( ALPHA / DIGIT / \"-\" )");
	__CWT_TEST_RULE(s, "rulename");

	s = String("defined-as = *c-wsp ( \"=\" / \"=/\" ) *c-wsp") + String::EndOfLine
				+ String("\t\t; basic rules definition and") + String::EndOfLine
				+ String("\t\t; incremental alternatives") + String::EndOfLine;
	__CWT_TEST_RULE(s, "defined-as");

	s = String("elements = alternation *c-wsp");
	__CWT_TEST_RULE(s, "elements");

	s = String("c-wsp = WSP / ( c-nl WSP )");
	__CWT_TEST_RULE(s, "c-wsp");

	s = String("c-nl = comment / NL") + String::EndOfLine
			+ String("\t\t; comment or newline") + String::EndOfLine;
	__CWT_TEST_RULE(s, "c-nl");

	s = String("comment = \";\" *( WSP / VCHAR ) NL");
	__CWT_TEST_RULE(s, "comment");

	s = String("alternation = concatenation *( *c-wsp \"/\" *c-wsp concatenation )");
	__CWT_TEST_RULE(s, "alternation");

	s = String("concatenation = repetition *( 1*c-wsp repetition )");
	__CWT_TEST_RULE(s, "concatenation");

	s = String("repetition = [ repeat ] element");
	__CWT_TEST_RULE(s, "repetition");

	s = String("repeat = ( *DIGIT \"*\" *DIGIT ) / 1*DIGIT");
	__CWT_TEST_RULE(s, "repeat");

	s = String("element = rulename / group / option / char-val / num-val / prose-val");
	__CWT_TEST_RULE(s, "element");

	s = String("group = \"(\" *c-wsp alternation *c-wsp \")\"");
	__CWT_TEST_RULE(s, "group");

	s = String("option = \"[\" *c-wsp alternation *c-wsp \"]\"");
	__CWT_TEST_RULE(s, "option");

	s = String("char-val = DQUOTE *( %x20-21 / %x23-7E ) DQUOTE") + String::EndOfLine
		+ String("\t\t; quoted string of SP and VCHAR") + String::EndOfLine
		+ String("\t\t; without DQUOTE") + String::EndOfLine;
	__CWT_TEST_RULE(s, "char-val");

	s = String("num-val = \"%\" ( bin-val / dec-val / hex-val )");
	__CWT_TEST_RULE(s, "num-val");

	s = String("bin-val = \"b\" 1*BIT [ 1*( \".\" 1*BIT ) / ( \"-\" 1*BIT ) ]") + String::EndOfLine
		+ String("\t\t; series of concatenated bit values") + String::EndOfLine
		+ String("\t\t; or single ONEOF range") + String::EndOfLine;
	__CWT_TEST_RULE(s, "bin-val");

	s = String("dec-val = \"d\" 1*DIGIT [ 1*( \".\" 1*DIGIT ) / ( \"-\" 1*DIGIT ) ]");
	__CWT_TEST_RULE(s, "dec-val");

	s = String("hex-val = \"x\" 1*HEXDIG [ 1*( \".\" 1*HEXDIG ) / ( \"-\" 1*HEXDIG ) ]");
	__CWT_TEST_RULE(s, "hex-val");

	s = String("prose-val = \"<\" *( %x20-3D / %x3F-7E ) \">\"") + String::EndOfLine
		+ String("\t\t; bracketed string of SP and VCHAR") + String::EndOfLine
		+ String("\t\t;  without angles") + String::EndOfLine
		+ String("\t\t; prose description, to be used as") + String::EndOfLine
		+ String("\t\t;  last resort") + String::EndOfLine;
	__CWT_TEST_RULE(s, "prose-val");

	std::cout << "===================================" << std::endl;
	std::cout << rulelist.toString();// << std::endl;
	std::cout << "===================================" << std::endl;
}

int main(int argc, char *argv[])
{
	CWT_UNUSED(argc);
	CWT_UNUSED(argv);
	CWT_BEGIN_TESTS(21);

	test_abnf_build();

	CWT_END_TESTS;
}

