/*
 * abnf_ruleset.cpp
 *
 *  Created on: Oct 7, 2013
 *      Author: wladt
 */

#include "../../include/cwt/abnf.hpp"
#include <iostream>

using namespace cwt;
/*
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

String generateAbnfTransitions()
{
	AbnfRuleList ruleset;

	// 1*( rule / (*c-wsp c-nl) )
	AbnfRule * rule = & ruleset.newRule("rulelist");
	rule->newRpt(1,-1)
		.newAltern()
			.add(Abnf::newRuleRef("rule"))
			.add(Abnf::newConcat()
				.add(Abnf::newRpt("c-wsp"))
				.add(Abnf::newRuleRef("c-nl")));

//	rule = rulename defined-as elements c-nl
//		; continues if next line starts
//		; with white space
	rule = & ruleset.newRule("rule");
	rule->newConcat()
			.add(Abnf::newRuleRef("rulename"))
			.add(Abnf::newRuleRef("defined-as"))
			.add(Abnf::newRuleRef("elements"))
			.add(Abnf::newRuleRef("c-nl"))
			.add(Abnf::newComment()) // append new line
			.add(Abnf::newComment("continues if next line starts"))
			.add(Abnf::newComment("with white space"));

//	rulename =  ALPHA *(ALPHA / DIGIT / "-")
	rule = & ruleset.newRule("rulename");
	rule->newConcat()
			.add(Abnf::newRuleRef("ALPHA"))
			.newRpt().newAltern()
					.add(Abnf::newRuleRef("ALPHA"))
					.add(Abnf::newRuleRef("DIGIT"))
					.add(Abnf::newCharVal("-"));

//	defined-as = *c-wsp (\"=\" / \"=/\") *c-wsp
//		   ; basic rules definition and
//		   ; incremental alternatives

	rule = & ruleset.newRule("defined-as");
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
	rule = & ruleset.newRule("elements");
	rule->newConcat()
		.add("alternation")
		.add(Abnf::newRpt("c-wsp"));

//	c-wsp =  WSP / (c-nl WSP)
	rule = & ruleset.newRule("c-wsp");
	rule->newAltern()
			.add("WSP")
			.add(Abnf::newConcat().add("c-nl").add("WSP"));

//	c-nl = comment / NL
//			; comment or newline
	rule = & ruleset.newRule("c-nl");
	rule->newAltern()
			.add("comment")
			.add("NL")
			.addComment()
			.addComment("comment or newline");

//	comment =  ";" *(WSP / VCHAR) NL
	rule = & ruleset.newRule("comment");
	rule->newConcat()
			.add(Abnf::newCharVal(";"))
			.add(Abnf::newRpt()
				.add(Abnf::newAltern()
					.add("WSP")
					.add("VCHAR")))
			.add("NL");

//	alternation = concatenation *( *c-wsp "/" *c-wsp concatenation )
	rule = & ruleset.newRule("alternation");
	rule->newConcat()
			.add("concatenation")
			.add(Abnf::newRpt()
				.add(Abnf::newConcat()
					.add(Abnf::newRpt("c-wsp"))
					.add(Abnf::newCharVal("/"))
					.add(Abnf::newRpt("c-wsp"))
					.add("concatenation")));

// concatenation = repetition *( 1*c-wsp repetition )
	rule = & ruleset.newRule("concatenation");
	rule->newConcat()
			.add("repetition")
			.add(Abnf::newRpt()
				.add(Abnf::newConcat()
					.add(Abnf::newRpt("c-wsp", 1))
					.add("repetition")));

// repetition = [ repeat ] element
	rule = & ruleset.newRule("repetition");
	rule->newConcat()
			.add(Abnf::newRpt("repeat", 0, 1))
			.add("element");

//	repeat = ( *DIGIT "*" *DIGIT ) / 1*DIGIT
	rule = & ruleset.newRule("repeat");
	rule->newAltern()
		.add(Abnf::newConcat()
			.add(Abnf::newRpt("DIGIT"))
			.add(Abnf::newCharVal("*"))
			.add(Abnf::newRpt("DIGIT")))
		.add(Abnf::newRpt("DIGIT", 1));

//	element = rulename / group / option / char-val / num-val / prose-val
	rule = & ruleset.newRule("element");
	rule->newAltern()
			.add("rulename")
			.add("group")
			.add("option")
			.add("char-val")
			.add("num-val")
			.add("prose-val");

//	group = "(" *c-wsp alternation *c-wsp ")"
	rule = & ruleset.newRule("group");
	rule->newConcat()
			.add(Abnf::newCharVal("("))
			.add(Abnf::newRpt("c-wsp"))
			.add("alternation")
			.add(Abnf::newRpt("c-wsp"))
			.add(Abnf::newCharVal(")"));

//	option = "[" *c-wsp alternation *c-wsp "]"
	rule = & ruleset.newRule("option");
	rule->newConcat()
			.add(Abnf::newCharVal("["))
			.add(Abnf::newRpt("c-wsp"))
			.add("alternation")
			.add(Abnf::newRpt("c-wsp"))
			.add(Abnf::newCharVal("]"));

//	char-val = DQUOTE *( %x20-21 / %x23-7E ) DQUOTE
//			; quoted string of SP and VCHAR
//			; without DQUOTE
	rule = & ruleset.newRule("char-val");
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
	rule = & ruleset.newRule("num-val");
	rule->newConcat()
		.add(Abnf::newCharVal("%"))
		.add(Abnf::newAltern()
			.add("bin-val")
			.add("dec-val")
			.add("hex-val"));

//	bin-val = "b" 1*BIT [ 1*( "." 1*BIT ) / ( "-" 1*BIT ) ]
//			; series of concatenated bit values
//			; or single ONEOF range
	rule = & ruleset.newRule("bin-val");
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
	rule = & ruleset.newRule("dec-val");
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
	rule = & ruleset.newRule("hex-val");
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

	rule = & ruleset.newRule("prose-val");
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

	//ruleset.normalize();
	std::cout << "===================================" << std::endl;
	std::cout << ruleset.toString();// << std::endl;
	std::cout << "===================================" << std::endl;

	AbnfGenContext genCtx(ruleset);
	genCtx.compactCharValues(true);
	return genCtx.generate();
}
