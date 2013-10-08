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

void test_abnf_normalize()
{
	AbnfRuleList rulelist;

	// 1*( rule / (*c-wsp c-nl) )
	AbnfRule * rule = & rulelist.newRule("rulelist");
	rule->newRpt(1,-1)
		.newAltern()
			.add(Abnf::newRuleRef("rule"))
			.add(Abnf::newConcat()
				.add(Abnf::newRpt("c-wsp"))
				.add(Abnf::newRuleRef("c-nl")));

//	rule = rulename defined-as elements c-nl
//		; continues if next line starts
//		; with white space
	rule = & rulelist.newRule("rule");
	rule->newConcat()
			.add(Abnf::newRuleRef("rulename"))
			.add(Abnf::newRuleRef("defined-as"))
			.add(Abnf::newRuleRef("elements"))
			.add(Abnf::newRuleRef("c-nl"))
			.add(Abnf::newComment()) // append new line
			.add(Abnf::newComment("continues if next line starts"))
			.add(Abnf::newComment("with white space"));

//	rulename =  ALPHA *(ALPHA / DIGIT / "-")
	rule = & rulelist.newRule("rulename");
	rule->newConcat()
			.add(Abnf::newRuleRef("ALPHA"))
			.newRpt().newAltern()
					.add(Abnf::newRuleRef("ALPHA"))
					.add(Abnf::newRuleRef("DIGIT"))
					.add(Abnf::newCharVal("-"));

//	defined-as = *c-wsp (\"=\" / \"=/\") *c-wsp
//		   ; basic rules definition and
//		   ; incremental alternatives
	rule = & rulelist.newRule("defined-as");
	rule->newConcat()
			.add(Abnf::newRpt("c-wsp"))
			.add(Abnf::newAltern()
				.add(Abnf::newCharVal("="))
				.add(Abnf::newCharVal("=/")))
			.add(Abnf::newRpt("c-wsp"))
			.add(Abnf::newComment()) // append new line
			.add(Abnf::newComment("basic rules definition and"))
			.add(Abnf::newComment("incremental alternatives"));

//	elements = alternation *c-wsp
	rule = & rulelist.newRule("elements");
	rule->newConcat()
		.add("alternation")
		.add(Abnf::newRpt("c-wsp"));

//	c-wsp =  WSP / (c-nl WSP)
	rule = & rulelist.newRule("c-wsp");
	rule->newAltern()
			.add("WSP")
			.add(Abnf::newConcat().add("c-nl").add("WSP"));

//	c-nl = comment / NL
//			; comment or newline
	rule = & rulelist.newRule("c-nl");
	rule->newAltern()
			.add("comment")
			.add("NL")
			.addComment()
			.addComment("comment or newline");

//	comment =  ";" *(WSP / VCHAR) NL
	rule = & rulelist.newRule("comment");
	rule->newConcat()
			.add(Abnf::newCharVal(";"))
			.add(Abnf::newRpt()
				.add(Abnf::newAltern()
					.add("WSP")
					.add("VCHAR")))
			.add("NL");

//	alternation = concatenation *( *c-wsp "/" *c-wsp concatenation )
	rule = & rulelist.newRule("alternation");
	rule->newConcat()
			.add("concatenation")
			.add(Abnf::newRpt()
				.add(Abnf::newConcat()
					.add(Abnf::newRpt("c-wsp"))
					.add(Abnf::newCharVal("/"))
					.add(Abnf::newRpt("c-wsp"))
					.add("concatenation")));

// concatenation = repetition *( 1*c-wsp repetition )
	rule = & rulelist.newRule("concatenation");
	rule->newConcat()
			.add("repetition")
			.add(Abnf::newRpt()
				.add(Abnf::newConcat()
					.add(Abnf::newRpt("c-wsp", 1))
					.add("repetition")));

// repetition = [ repeat ] element
	rule = & rulelist.newRule("repetition");
	rule->newConcat()
			.add(Abnf::newRpt("repeat", 0, 1))
			.add("element");

//	repeat = ( *DIGIT "*" *DIGIT ) / 1*DIGIT
	rule = & rulelist.newRule("repeat");
	rule->newAltern()
		.add(Abnf::newConcat()
			.add(Abnf::newRpt("DIGIT"))
			.add(Abnf::newCharVal("*"))
			.add(Abnf::newRpt("DIGIT")))
		.add(Abnf::newRpt("DIGIT", 1));

//	element = rulename / group / option / char-val / num-val / prose-val
	rule = & rulelist.newRule("element");
	rule->newAltern()
			.add("rulename")
			.add("group")
			.add("option")
			.add("char-val")
			.add("num-val")
			.add("prose-val");

//	group = "(" *c-wsp alternation *c-wsp ")"
	rule = & rulelist.newRule("group");
	rule->newConcat()
			.add(Abnf::newCharVal("("))
			.add(Abnf::newRpt("c-wsp"))
			.add("alternation")
			.add(Abnf::newRpt("c-wsp"))
			.add(Abnf::newCharVal(")"));

//	option = "[" *c-wsp alternation *c-wsp "]"
	rule = & rulelist.newRule("option");
	rule->newConcat()
			.add(Abnf::newCharVal("["))
			.add(Abnf::newRpt("c-wsp"))
			.add("alternation")
			.add(Abnf::newRpt("c-wsp"))
			.add(Abnf::newCharVal("]"));

//	char-val = DQUOTE *( %x20-21 / %x23-7E ) DQUOTE
//			; quoted string of SP and VCHAR
//			; without DQUOTE
	rule = & rulelist.newRule("char-val");
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

//	num-val = "%" ( bin-val / dec-val / hex-val )
	rule = & rulelist.newRule("num-val");
	rule->newConcat()
		.add(Abnf::newCharVal("%"))
		.add(Abnf::newAltern()
			.add("bin-val")
			.add("dec-val")
			.add("hex-val"));

//	bin-val = "b" 1*BIT [ 1*( "." 1*BIT ) / ( "-" 1*BIT ) ]
//			; series of concatenated bit values
//			; or single ONEOF range
	rule = & rulelist.newRule("bin-val");
	rule->newConcat()
		.add(Abnf::newCharVal("b"))
		.add(Abnf::newRpt("BIT", 1))
		.add(Abnf::newRpt(0, 1)
			.add(Abnf::newAltern()
				.add(Abnf::newRpt(1)
					.add(Abnf::newConcat()
						.add(Abnf::newCharVal("."))
						.add(Abnf::newRpt("BIT", 1))))
				.add(Abnf::newConcat()
					.add(Abnf::newCharVal("-"))
					.add(Abnf::newRpt("BIT", 1)))))
			.addComment()
			.addComment("series of concatenated bit values")
			.addComment("or single ONEOF range");

//	dec-val = "d" 1*DIGIT [ 1*( "." 1*DIGIT ) / ( "-" 1*DIGIT ) ]
	rule = & rulelist.newRule("dec-val");
	rule->newConcat()
		.add(Abnf::newCharVal("d"))
		.add(Abnf::newRpt("DIGIT", 1))
		.add(Abnf::newRpt(0, 1)
			.add(Abnf::newAltern()
				.add(Abnf::newRpt(1)
					.add(Abnf::newConcat()
						.add(Abnf::newCharVal("."))
						.add(Abnf::newRpt("DIGIT", 1))))
				.add(Abnf::newConcat()
					.add(Abnf::newCharVal("-"))
					.add(Abnf::newRpt("DIGIT", 1)))));

//	hex-val = "x" 1*HEXDIG [ 1*( "." 1*HEXDIG ) / ( "-" 1*HEXDIG ) ]
	rule = & rulelist.newRule("hex-val");
	rule->newConcat()
		.add(Abnf::newCharVal("x"))
		.add(Abnf::newRpt("HEXDIG", 1))
		.add(Abnf::newRpt(0, 1)
			.add(Abnf::newAltern()
				.add(Abnf::newRpt(1)
					.add(Abnf::newConcat()
						.add(Abnf::newCharVal("."))
						.add(Abnf::newRpt("HEXDIG", 1))))
				.add(Abnf::newConcat()
					.add(Abnf::newCharVal("-"))
					.add(Abnf::newRpt("HEXDIG", 1)))));

//	prose-val = "<" *( %x20-3D / %x3F-7E ) ">"
//			; bracketed string of SP and VCHAR
//			;  without angles
//			; prose description, to be used as
//			;  last resort
	rule = & rulelist.newRule("prose-val");
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

	CWT_TEST_OK(rulelist.normalize());

	std::cout << "== ABNF: ==========================" << std::endl;
	std::cout << rulelist.toString();// << std::endl;
	std::cout << "===================================" << std::endl;

	// 1*( rule / (*c-wsp c-nl) )
	CWT_TEST_OK(rulelist.find("rulelist") && rulelist.find("rulelist")->toString()
			== String("rulelist = 1*rulelist_0"));
	CWT_TEST_OK(rulelist.find("rulelist_0") && rulelist.find("rulelist_0")->toString()
			== String("rulelist_0 = rule / rulelist_1"));
	CWT_TEST_OK(rulelist.find("rulelist_1") && rulelist.find("rulelist_1")->toString()
			== String("rulelist_1 = *c_wsp c_nl"));

	//	rule = rulename defined-as elements c-nl
	//		; continues if next line starts
	//		; with white space
	CWT_TEST_OK(rulelist.find("rule") && rulelist.find("rule")->toString()
			== String("rule = rulename defined_as elements c_nl")
				+ String::EndOfLine
				+ String("\t\t; continues if next line starts") + String::EndOfLine
				+ String("\t\t; with white space") + String::EndOfLine);


	//	rulename =  ALPHA *( ALPHA / DIGIT / "-" )
	CWT_TEST_OK(rulelist.find("rulename") && rulelist.find("rulename")->toString()
			== String("rulename = ALPHA *rulename_0"));

	CWT_TEST_OK(rulelist.find("rulename_0") && rulelist.find("rulename_0")->toString()
			== String("rulename_0 = ALPHA / DIGIT / \"-\""));

	//	defined-as = *c-wsp (\"=\" / \"=/\") *c-wsp
	//		   ; basic rules definition and
	//		   ; incremental alternatives
	//CWT_TEST_FAIL(rulelist.find("defined-as"));
	//std::cout << '[' << rulelist.find("defined_as")->toString() << ']' << std::endl;
	CWT_TEST_OK(rulelist.find("defined_as") && rulelist.find("defined_as")->toString()
			== String("defined_as = *c_wsp defined_as_0 *c_wsp")
			+ String::EndOfLine
			+ String("\t\t; basic rules definition and") + String::EndOfLine
			+ String("\t\t; incremental alternatives") + String::EndOfLine);

	//	elements = alternation *c-wsp
	CWT_TEST_OK(rulelist.find("elements") && rulelist.find("elements")->toString()
			== String("elements = alternation *c_wsp"));

	//	c-wsp =  WSP / ( c-nl WSP )
	CWT_TEST_OK(rulelist.find("c_wsp") && rulelist.find("c_wsp")->toString()
			== String("c_wsp = WSP / c_wsp_0"));

	CWT_TEST_OK(rulelist.find("c_wsp_0") && rulelist.find("c_wsp_0")->toString()
			== String("c_wsp_0 = c_nl WSP"));

	//	c-nl = comment / NL
	//			; comment or newline
	CWT_TEST_OK(rulelist.find("c_nl") && rulelist.find("c_nl")->toString()
			== String("c_nl = comment / NL")
			+ String::EndOfLine
			+ String("\t\t; comment or newline") + String::EndOfLine);

	//	comment = ";" *( WSP / VCHAR ) NL
	CWT_TEST_OK(rulelist.find("comment") && rulelist.find("comment")->toString()
			== String("comment = \";\" *comment_0 NL"));

	CWT_TEST_OK(rulelist.find("comment_0") && rulelist.find("comment_0")->toString()
			== String("comment_0 = WSP / VCHAR"));

	//	alternation = concatenation *( *c-wsp "/" *c-wsp concatenation )
	CWT_TEST_OK(rulelist.find("alternation") && rulelist.find("alternation")->toString()
			== String("alternation = concatenation *alternation_0"));

	CWT_TEST_OK(rulelist.find("alternation_0") && rulelist.find("alternation_0")->toString()
			== String("alternation_0 = *c_wsp \"/\" *c_wsp concatenation"));

	// concatenation = repetition *( 1*c-wsp repetition )
	CWT_TEST_OK(rulelist.find("concatenation") && rulelist.find("concatenation")->toString()
			== String("concatenation = repetition *concatenation_0"));

	CWT_TEST_OK(rulelist.find("concatenation_0") && rulelist.find("concatenation_0")->toString()
			== String("concatenation_0 = 1*c_wsp repetition"));

	// repetition = [ repeat ] element
	CWT_TEST_OK(rulelist.find("repetition") && rulelist.find("repetition")->toString()
			== String("repetition = [ repeat ] element"));

	//	repeat = ( *DIGIT "*" *DIGIT ) / 1*DIGIT
	CWT_TEST_OK(rulelist.find("repeat") && rulelist.find("repeat")->toString()
			== String("repeat = repeat_0 / 1*DIGIT"));

	CWT_TEST_OK(rulelist.find("repeat_0") && rulelist.find("repeat_0")->toString()
			== String("repeat_0 = *DIGIT \"*\" *DIGIT"));

	//	element = rulename / group / option / char-val / num-val / prose-val
	CWT_TEST_OK(rulelist.find("element") && rulelist.find("element")->toString()
			== String("element = rulename / group / option / char_val / num_val / prose_val"));

	//	group = "(" *c-wsp alternation *c-wsp ")"
	CWT_TEST_OK(rulelist.find("group") && rulelist.find("group")->toString()
			== String("group = \"(\" *c_wsp alternation *c_wsp \")\""));

	//	option = "[" *c-wsp alternation *c-wsp "]"
	CWT_TEST_OK(rulelist.find("option") && rulelist.find("option")->toString()
			== String("option = \"[\" *c_wsp alternation *c_wsp \"]\""));

	//	char-val = DQUOTE *( %x20-21 / %x23-7E ) DQUOTE
	//			; quoted string of SP and VCHAR
	//			; without DQUOTE
	CWT_TEST_OK(rulelist.find("char_val") && rulelist.find("char_val")->toString()
			== String("char_val = DQUOTE *char_val_0 DQUOTE")
			+ String::EndOfLine
			+ String("\t\t; quoted string of SP and VCHAR") + String::EndOfLine
			+ String("\t\t; without DQUOTE") + String::EndOfLine);

	CWT_TEST_OK(rulelist.find("char_val_0") && rulelist.find("char_val_0")->toString()
			== String("char_val_0 = %x20-21 / %x23-7E"));

	//	num-val = "%" ( bin-val / dec-val / hex-val )
	CWT_TEST_OK(rulelist.find("num_val") && rulelist.find("num_val")->toString()
			== String("num_val = \"%\" num_val_0"));

	CWT_TEST_OK(rulelist.find("num_val_0") && rulelist.find("num_val_0")->toString()
			== String("num_val_0 = bin_val / dec_val / hex_val"));

	//	bin-val = "b" 1*BIT [ 1*( "." 1*BIT ) / ( "-" 1*BIT ) ]
	//			; series of concatenated bit values
	//			; or single ONEOF range
	CWT_TEST_OK(rulelist.find("bin_val") && rulelist.find("bin_val")->toString()
			== String("bin_val = \"b\" 1*BIT [ bin_val_0 ]")
			+ String::EndOfLine
			+ String("\t\t; series of concatenated bit values") + String::EndOfLine
			+ String("\t\t; or single ONEOF range") + String::EndOfLine);

	CWT_TEST_OK(rulelist.find("bin_val_0") && rulelist.find("bin_val_0")->toString()
			== String("bin_val_0 = 1*bin_val_1 / bin_val_2"));

	CWT_TEST_OK(rulelist.find("bin_val_1") && rulelist.find("bin_val_1")->toString()
			== String("bin_val_1 = \".\" 1*BIT"));

	CWT_TEST_OK(rulelist.find("bin_val_2") && rulelist.find("bin_val_2")->toString()
			== String("bin_val_2 = \"-\" 1*BIT"));

	//	dec-val = "d" 1*DIGIT [ 1*( "." 1*DIGIT ) / ( "-" 1*DIGIT ) ]
	CWT_TEST_OK(rulelist.find("dec_val") && rulelist.find("dec_val")->toString()
			== String("dec_val = \"d\" 1*DIGIT [ dec_val_0 ]"));

	CWT_TEST_OK(rulelist.find("dec_val_0") && rulelist.find("dec_val_0")->toString()
			== String("dec_val_0 = 1*dec_val_1 / dec_val_2"));

	CWT_TEST_OK(rulelist.find("dec_val_1") && rulelist.find("dec_val_1")->toString()
			== String("dec_val_1 = \".\" 1*DIGIT"));

	CWT_TEST_OK(rulelist.find("dec_val_2") && rulelist.find("dec_val_2")->toString()
			== String("dec_val_2 = \"-\" 1*DIGIT"));

	//	hex-val = "x" 1*HEXDIG [ 1*( "." 1*HEXDIG ) / ( "-" 1*HEXDIG ) ]
	CWT_TEST_OK(rulelist.find("hex_val") && rulelist.find("hex_val")->toString()
			== String("hex_val = \"x\" 1*HEXDIG [ hex_val_0 ]"));

	CWT_TEST_OK(rulelist.find("hex_val_0") && rulelist.find("hex_val_0")->toString()
			== String("hex_val_0 = 1*hex_val_1 / hex_val_2"));

	CWT_TEST_OK(rulelist.find("hex_val_1") && rulelist.find("hex_val_1")->toString()
			== String("hex_val_1 = \".\" 1*HEXDIG"));

	CWT_TEST_OK(rulelist.find("hex_val_2") && rulelist.find("hex_val_2")->toString()
			== String("hex_val_2 = \"-\" 1*HEXDIG"));

	//	prose-val = "<" *( %x20-3D / %x3F-7E ) ">"
	//			; bracketed string of SP and VCHAR
	//			;  without angles
	//			; prose description, to be used as
	//			;  last resort


	CWT_TEST_OK(rulelist.find("rulelist"));
	CWT_TEST_OK(rulelist.find("rule"));
	CWT_TEST_OK(rulelist.find("rulename"));
	CWT_TEST_OK(rulelist.find("defined_as"));
	CWT_TEST_OK(rulelist.find("elements"));
	CWT_TEST_OK(rulelist.find("c_wsp"));
	CWT_TEST_OK(rulelist.find("c_nl"));
	CWT_TEST_OK(rulelist.find("comment"));
	CWT_TEST_OK(rulelist.find("alternation"));
	CWT_TEST_OK(rulelist.find("concatenation"));
	CWT_TEST_OK(rulelist.find("repetition"));
	CWT_TEST_OK(rulelist.find("repeat"));
	CWT_TEST_OK(rulelist.find("element"));
	CWT_TEST_OK(rulelist.find("group"));
	CWT_TEST_OK(rulelist.find("option"));
	CWT_TEST_OK(rulelist.find("char_val"));
	CWT_TEST_OK(rulelist.find("num_val"));
	CWT_TEST_OK(rulelist.find("bin_val"));
	CWT_TEST_OK(rulelist.find("dec_val"));
	CWT_TEST_OK(rulelist.find("hex_val"));
	CWT_TEST_OK(rulelist.find("prose_val"));
}

int main(int argc, char *argv[])
{
	CWT_UNUSED(argc);
	CWT_UNUSED(argv);
	CWT_BEGIN_TESTS(61);

	test_abnf_normalize();

	CWT_END_TESTS;
}

