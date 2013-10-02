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

#define __CWT_TEST_RULE(orig,rule)                  \
		CWT_TEST_FAIL2((orig) == (rule)->toString() \
				, String(_F("[%s] == [%s]") % (orig) % (rule)->toString()).c_str());

void test_abnf_gen()
{
	AbnfRuleSet * ruleset = Abnf::createRuleSet ();

	// 1*( rule / (*c-wsp c-nl) )
	AbnfRule * rule = ruleset->newRule("rulelist");
	rule->newRpt(1,-1)
				.newGroup()
					.newAltern()
						.add(Abnf::newRuleRef("rule"))
						.add(Abnf::newGroup().add(Abnf::newConcat()
							.add(Abnf::newRpt().add(Abnf::newRuleRef("c-wsp")))
							.add(Abnf::newRuleRef("c-nl"))));

	String s("rulelist = 1*( rule / ( *c-wsp c-nl ) )");
	__CWT_TEST_RULE(s, rule);

//	rule = rulename defined-as elements c-nl
//		; continues if next line starts
//		; with white space
	rule = ruleset->newRule("rule");
	rule->newConcat()
			.add(Abnf::newRuleRef("rulename"))
			.add(Abnf::newRuleRef("defined-as"))
			.add(Abnf::newRuleRef("elements"))
			.add(Abnf::newRuleRef("c-nl"))
			.add(Abnf::newComment()) // append new line
			.add(Abnf::newComment("continues if next line starts"))
			.add(Abnf::newComment("with white space"));

	s = String("rule = rulename defined-as elements c-nl") + String::EndOfLine
				+ String("\t\t; continues if next line starts") + String::EndOfLine
				+ String("\t\t; with white space") + String::EndOfLine;
	__CWT_TEST_RULE(s, rule);

//	rulename =  ALPHA *(ALPHA / DIGIT / "-")
	rule = ruleset->newRule("rulename");
	rule->newConcat()
			.add(Abnf::newRuleRef("ALPHA"))
			.newRpt().newAltern()
						.add(Abnf::newRuleRef("ALPHA"))
						.add(Abnf::newRuleRef("DIGIT"))
						.add(Abnf::newCharVal("-"));
	s = String("rulename = ALPHA *( ALPHA / DIGIT / \"-\" )");
	__CWT_TEST_RULE(s, rule);

//	defined-as = *c-wsp (\"=\" / \"=/\") *c-wsp
//		   ; basic rules definition and
//		   ; incremental alternatives

	rule = ruleset->newRule("defined-as");
	rule->newConcat()
			.add(Abnf::newRpt("c-wsp"))
			.add(Abnf::newGroup()
				.add(Abnf::newAltern()
						.add(Abnf::newCharVal("="))
						.add(Abnf::newCharVal("=/"))
				))
			.add(Abnf::newRpt("c-wsp"))
			.add(Abnf::newComment()) // append new line
			.add(Abnf::newComment("basic rules definition and"))
			.add(Abnf::newComment("incremental alternatives"));

	s = String("defined-as = *c-wsp ( \"=\" / \"=/\" ) *c-wsp") + String::EndOfLine
				+ String("\t\t; basic rules definition and") + String::EndOfLine
				+ String("\t\t; incremental alternatives") + String::EndOfLine;
	__CWT_TEST_RULE(s, rule);

//	elements = alternation *c-wsp
	rule = ruleset->newRule("elements");
	rule->newConcat()
			.add("alternation")
			.add(Abnf::newRpt("c-wsp"));

	s = String("elements = alternation *c-wsp");
	__CWT_TEST_RULE(s, rule);

//	c-wsp =  WSP / (c-nl WSP)
	rule = ruleset->newRule("c-wsp");
	rule->newAltern()
			.add("WSP")
			.add(Abnf::newGroup()
				.add(Abnf::newConcat().add("c-nl").add("WSP")));

	s = String("c-wsp = WSP / ( c-nl WSP )");
	__CWT_TEST_RULE(s, rule);

//	c-nl = comment / NL
//			; comment or newline
	rule = ruleset->newRule("c-nl");
	rule->newAltern()
			.add("comment")
			.add("NL")
			.addComment()
			.addComment("comment or newline");

	s = String("c-nl = comment / NL") + String::EndOfLine
			+ String("\t\t; comment or newline") + String::EndOfLine;
	__CWT_TEST_RULE(s, rule);

//	comment =  ";" *(WSP / VCHAR) NL
	rule = ruleset->newRule("comment");
	rule->newConcat()
			.add(Abnf::newCharVal(";"))
			.add(Abnf::newRpt()
				.add(Abnf::newAltern()
					.add("WSP")
					.add("VCHAR")))
			.add("NL");

	s = String("comment = \";\" *( WSP / VCHAR ) NL");
	__CWT_TEST_RULE(s, rule);

//	alternation = concatenation *( *c-wsp "/" *c-wsp concatenation )
	rule = ruleset->newRule("alternation");
	rule->newConcat()
			.add("concatenation")
			.add(Abnf::newRpt()
				.add(Abnf::newConcat()
					.add(Abnf::newRpt("c-wsp"))
					.add(Abnf::newCharVal("/"))
					.add(Abnf::newRpt("c-wsp"))
					.add("concatenation")));

	s = String("alternation = concatenation *( *c-wsp \"/\" *c-wsp concatenation )");
	__CWT_TEST_RULE(s, rule);

// concatenation = repetition *( 1*c-wsp repetition )
	rule = ruleset->newRule("concatenation");
	rule->newConcat()
			.add("repetition")
			.add(Abnf::newRpt()
				.add(Abnf::newConcat()
					.add(Abnf::newRpt("c-wsp", 1))
					.add("repetition")));

	s = String("concatenation = repetition *( 1*c-wsp repetition )");
	__CWT_TEST_RULE(s, rule);

// repetition = [ repeat ] element
	rule = ruleset->newRule("repetition");
	rule->newConcat()
			.add(Abnf::newOption().add("repeat"))
			.add("element");

	s = String("repetition = [ repeat ] element");
	__CWT_TEST_RULE(s, rule);

//	repeat = ( *DIGIT "*" *DIGIT ) / 1*DIGIT
	rule = ruleset->newRule("repeat");
	rule->newAltern()
			.add(Abnf::newGroup()
				.add(Abnf::newRpt("DIGIT"))
				.add(Abnf::newCharVal("*"))
				.add(Abnf::newRpt("DIGIT")))
			.add(Abnf::newRpt("DIGIT", 1));

	s = String("repeat = ( *DIGIT \"*\" *DIGIT ) / 1*DIGIT");
	__CWT_TEST_RULE(s, rule);

//	element = rulename / group / option / char-val / num-val / prose-val
	rule = ruleset->newRule("element");
	rule->newAltern()
			.add("rulename")
			.add("group")
			.add("option")
			.add("char-val")
			.add("num-val")
			.add("prose-val");

	s = String("element = rulename / group / option / char-val / num-val / prose-val");
	__CWT_TEST_RULE(s, rule);

//	group = "(" *c-wsp alternation *c-wsp ")"
	rule = ruleset->newRule("group");
	rule->newConcat()
			.add(Abnf::newCharVal("("))
			.add(Abnf::newRpt("c-wsp"))
			.add("alternation")
			.add(Abnf::newRpt("c-wsp"))
			.add(Abnf::newCharVal(")"));

	s = String("group = \"(\" *c-wsp alternation *c-wsp \")\"");
	__CWT_TEST_RULE(s, rule);

//	option = "[" *c-wsp alternation *c-wsp "]"
	rule = ruleset->newRule("option");
	rule->newConcat()
			.add(Abnf::newCharVal("["))
			.add(Abnf::newRpt("c-wsp"))
			.add("alternation")
			.add(Abnf::newRpt("c-wsp"))
			.add(Abnf::newCharVal("]"));

	s = String("option = \"[\" *c-wsp alternation *c-wsp \"]\"");
	__CWT_TEST_RULE(s, rule);

//	char-val = DQUOTE *( %x20-21 / %x23-7E ) DQUOTE
//			; quoted string of SP and VCHAR
//			; without DQUOTE
	rule = ruleset->newRule("char-val");
	rule->newConcat()
			.add("DQUOTE")
			.add(Abnf::newRpt()
				.add(Abnf::newAltern()
					.add(Abnf::newNumVal(0x20, 0x21, 16))
					.add(Abnf::newNumVal(0x23, 0x7E, 16))
				)
			)
			.add("DQUOTE")
			.addComment()
			.addComment("quoted string of SP and VCHAR")
			.addComment("without DQUOTE");

	s = String("char-val = DQUOTE *( %x20-21 / %x23-7E ) DQUOTE") + String::EndOfLine
		+ String("\t\t; quoted string of SP and VCHAR") + String::EndOfLine
		+ String("\t\t; without DQUOTE") + String::EndOfLine;
	__CWT_TEST_RULE(s, rule);


//	num-val = "%" ( bin-val / dec-val / hex-val )
	rule = ruleset->newRule("num-val");
	rule->newConcat()
		.add(Abnf::newCharVal("%"))
		.add(Abnf::newGroup()
			.add(Abnf::newAltern()
				.add("bin-val")
				.add("dec-val")
				.add("hex-val")));

	s = String("num-val = \"%\" ( bin-val / dec-val / hex-val )");
	__CWT_TEST_RULE(s, rule);

//	bin-val = "b" 1*BIT [ 1*( "." 1*BIT ) / ( "-" 1*BIT ) ]
//			; series of concatenated bit values
//			; or single ONEOF range
	rule = ruleset->newRule("bin-val");
	rule->newConcat()
			.add(Abnf::newCharVal("b"))
			.add(Abnf::newRpt("BIT", 1))
			.add(Abnf::newOption()
				.add(Abnf::newAltern()
					.add(Abnf::newRpt(1)
						.add(Abnf::newGroup()
							.add(Abnf::newConcat()
								.add(Abnf::newCharVal("."))
								.add(Abnf::newRpt("BIT", 1)))))
					.add(Abnf::newGroup()
						.add(Abnf::newConcat()
							.add(Abnf::newCharVal("-"))
							.add(Abnf::newRpt("BIT", 1))))))
			.addComment()
			.addComment("series of concatenated bit values")
			.addComment("or single ONEOF range");

	s = String("bin-val = \"b\" 1*BIT [ 1*( \".\" 1*BIT ) / ( \"-\" 1*BIT ) ]") + String::EndOfLine
		+ String("\t\t; series of concatenated bit values") + String::EndOfLine
		+ String("\t\t; or single ONEOF range") + String::EndOfLine;
	__CWT_TEST_RULE(s, rule);

//	dec-val = "d" 1*DIGIT [ 1*( "." 1*DIGIT ) / ( "-" 1*DIGIT ) ]
	rule = ruleset->newRule("dec-val");
	rule->newConcat()
			.add(Abnf::newCharVal("d"))
			.add(Abnf::newRpt("DIGIT", 1))
			.add(Abnf::newOption()
				.add(Abnf::newAltern()
					.add(Abnf::newRpt(1)
						.add(Abnf::newGroup()
							.add(Abnf::newConcat()
								.add(Abnf::newCharVal("."))
								.add(Abnf::newRpt("DIGIT", 1)))))
					.add(Abnf::newGroup()
						.add(Abnf::newConcat()
							.add(Abnf::newCharVal("-"))
							.add(Abnf::newRpt("DIGIT", 1))))));

	s = String("dec-val = \"d\" 1*DIGIT [ 1*( \".\" 1*DIGIT ) / ( \"-\" 1*DIGIT ) ]");
	__CWT_TEST_RULE(s, rule);

//	hex-val = "x" 1*HEXDIG [ 1*( "." 1*HEXDIG ) / ( "-" 1*HEXDIG ) ]
	rule = ruleset->newRule("hex-val");
	rule->newConcat()
			.add(Abnf::newCharVal("x"))
			.add(Abnf::newRpt("HEXDIG", 1))
			.add(Abnf::newOption()
				.add(Abnf::newAltern()
					.add(Abnf::newRpt(1)
						.add(Abnf::newGroup()
							.add(Abnf::newConcat()
								.add(Abnf::newCharVal("."))
								.add(Abnf::newRpt("HEXDIG", 1)))))
					.add(Abnf::newGroup()
						.add(Abnf::newConcat()
							.add(Abnf::newCharVal("-"))
							.add(Abnf::newRpt("HEXDIG", 1))))));

	s = String("hex-val = \"x\" 1*HEXDIG [ 1*( \".\" 1*HEXDIG ) / ( \"-\" 1*HEXDIG ) ]");
	__CWT_TEST_RULE(s, rule);

//	prose-val = "<" *( %x20-3D / %x3F-7E ) ">"
//			; bracketed string of SP and VCHAR
//			;  without angles
//			; prose description, to be used as
//			;  last resort

	rule = ruleset->newRule("prose-val");
	rule->newConcat()
			.add(Abnf::newCharVal("<"))
			.add(Abnf::newRpt()
				.add(Abnf::newAltern()
					.add(Abnf::newNumVal(0x20, 0x3D, 16))
					.add(Abnf::newNumVal(0x3F, 0x7E, 16))))
			.add(Abnf::newCharVal(">"))
			.addComment()
			.addComment("bracketed string of SP and VCHAR")
			.addComment(" without angles")
			.addComment("prose description, to be used as")
			.addComment(" last resort");

	s = String("prose-val = \"<\" *( %x20-3D / %x3F-7E ) \">\"") + String::EndOfLine
		+ String("\t\t; bracketed string of SP and VCHAR") + String::EndOfLine
		+ String("\t\t;  without angles") + String::EndOfLine
		+ String("\t\t; prose description, to be used as") + String::EndOfLine
		+ String("\t\t;  last resort") + String::EndOfLine;
	__CWT_TEST_RULE(s, rule);


	CWT_TEST_OK(ruleset->find("rulelist"));
	CWT_TEST_OK(ruleset->find("rule"));
	CWT_TEST_OK(ruleset->find("rulename"));
	CWT_TEST_OK(ruleset->find("defined-as"));
	CWT_TEST_OK(ruleset->find("elements"));
	CWT_TEST_OK(ruleset->find("c-wsp"));
	CWT_TEST_OK(ruleset->find("c-nl"));
	CWT_TEST_OK(ruleset->find("comment"));
	CWT_TEST_OK(ruleset->find("alternation"));
	CWT_TEST_OK(ruleset->find("concatenation"));
	CWT_TEST_OK(ruleset->find("repetition"));
	CWT_TEST_OK(ruleset->find("repeat"));
	CWT_TEST_OK(ruleset->find("element"));
	CWT_TEST_OK(ruleset->find("group"));
	CWT_TEST_OK(ruleset->find("option"));
	CWT_TEST_OK(ruleset->find("char-val"));
	CWT_TEST_OK(ruleset->find("num-val"));
	CWT_TEST_OK(ruleset->find("bin-val"));
	CWT_TEST_OK(ruleset->find("dec-val"));
	CWT_TEST_OK(ruleset->find("hex-val"));
	CWT_TEST_OK(ruleset->find("prose-val"));

	std::cout << "===================================" << std::endl;
	std::cout << ruleset->toString();// << std::endl;
	std::cout << "===================================" << std::endl;

	Abnf::destroyRuleSet (ruleset);
}

int main(int argc, char *argv[])
{
	CWT_UNUSED(argc);
	CWT_UNUSED(argv);
	CWT_BEGIN_TESTS(42);

	while(1)
		test_abnf_gen();

	CWT_END_TESTS;
}

