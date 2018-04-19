/*
 * abnf_rulelist.cpp
 *
 *  Created on: Oct 7, 2013
 *      Author: wladt
 */

#include <cwt/abnf.hpp>
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


void buildAbnfRuleList (AbnfRuleList & rulelist)
{
	String ABNF_ALPHA("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
	String ABNF_DIGIT("0123456789");
	String ABNF_HEXDIG("0123456789ABCDEFabcdef");
	String ABNF_BIT("01");
	String ABNF_DQUOTE("\"");
	String ABNF_WSP(" \t");

#define IS_ALTERN true

	// 1*( rule / (*c-wsp c-nl) )
	rulelist.newRule("rulelist").newRpt(1,-1)
		.newAltern()
			.add(Abnf::newRuleRef("rule"))
			.add(Abnf::newConcat()
				.add(Abnf::newRpt("c-wsp"))
				.add(Abnf::newRuleRef("c-nl")));

	//	rule = rulename defined-as elements c-nl
	//		; continues if next line starts
	//		; with white space
	rulelist.newRule("rule").newConcat()
			.add(Abnf::newRuleRef("rulename"))
			.add(Abnf::newRuleRef("defined-as"))
			.add(Abnf::newRuleRef("elements"))
			.add(Abnf::newRuleRef("c-nl"))
			.add(Abnf::newComment()) // append new line
			.add(Abnf::newComment("continues if next line starts"))
			.add(Abnf::newComment("with white space"));

	//	rulename =  ALPHA *(ALPHA / DIGIT / "-")
	rulelist.newRule("rulename").newConcat()
			.add(Abnf::newCharVal(ABNF_ALPHA, IS_ALTERN))
			.newRpt().newAltern()
					.add(Abnf::newCharVal(ABNF_ALPHA, IS_ALTERN))
					.add(Abnf::newCharVal(ABNF_DIGIT, IS_ALTERN))
					.add(Abnf::newCharVal("-"));

	//	defined-as = *c-wsp (\"=\" / \"=/\") *c-wsp
	//		   ; basic rules definition and
	//		   ; incremental alternatives
	rulelist.newRule("defined-as").newConcat()
			.add(Abnf::newRpt("c-wsp"))
			.add(Abnf::newAltern()
				.add(Abnf::newCharVal("="))
				.add(Abnf::newCharVal("=/")))
			.add(Abnf::newRpt("c-wsp"))
			.add(Abnf::newComment()) // append new line
			.add(Abnf::newComment("basic rules definition and"))
			.add(Abnf::newComment("incremental alternatives"));

	//	elements = alternation *c-wsp
	rulelist.newRule("elements").newConcat()
		.add("alternation")
		.add(Abnf::newRpt("c-wsp"));

	//	c-wsp =  WSP / (c-nl WSP)
	rulelist.newRule("c-wsp").newAltern()
			.add(Abnf::newCharVal(ABNF_WSP, IS_ALTERN))
			.add(Abnf::newConcat().add("c-nl").add(Abnf::newCharVal(ABNF_WSP, IS_ALTERN)));

	//	c-nl = comment / NL
	//			; comment or newline
	rulelist.newRule("c-nl").newAltern()
			.add("comment")
			.add(Abnf::newCharVal("\n")) // TODO see NL rule below
			.addComment()
			.addComment("comment or newline");

	//	comment =  ";" *(WSP / VCHAR) NL
	rulelist.newRule("comment").newConcat()
			.add(Abnf::newCharVal(";"))
			.add(Abnf::newRpt()
				.add(Abnf::newAltern()
					.add(Abnf::newCharVal(ABNF_WSP, IS_ALTERN))
					.add(Abnf::newNumVal(0x21, 0x7E, 16))))
			.add(Abnf::newCharVal("\n")); // TODO see NL below

	//	alternation = concatenation *( *c-wsp "/" *c-wsp concatenation )
	rulelist.newRule("alternation").newConcat()
			.add("concatenation")
			.add(Abnf::newRpt()
				.add(Abnf::newConcat()
					.add(Abnf::newRpt("c-wsp"))
					.add(Abnf::newCharVal("/"))
					.add(Abnf::newRpt("c-wsp"))
					.add("concatenation")));

	// concatenation = repetition *( 1*c-wsp repetition )
	rulelist.newRule("concatenation").newConcat()
			.add("repetition")
			.add(Abnf::newRpt()
				.add(Abnf::newConcat()
					.add(Abnf::newRpt("c-wsp", 1))
					.add("repetition")));

	// repetition = [ repeat ] element
	rulelist.newRule("repetition").newConcat()
			.add(Abnf::newRpt("repeat", 0, 1))
			.add("element");

	//	repeat = ( *DIGIT "*" *DIGIT ) / 1*DIGIT
	rulelist.newRule("repeat").newAltern()
		.add(Abnf::newConcat()
			.add(Abnf::newRpt().add(Abnf::newCharVal(ABNF_DIGIT, IS_ALTERN)))
			.add(Abnf::newCharVal("*"))
			.add(Abnf::newRpt().add(Abnf::newCharVal(ABNF_DIGIT, IS_ALTERN))))
		.add(Abnf::newRpt(1).add(Abnf::newCharVal(ABNF_DIGIT, IS_ALTERN)));

	//	element = rulename / group / option / char-val / num-val / prose-val
	rulelist.newRule("element").newAltern()
			.add("rulename")
			.add("group")
			.add("option")
			.add("char-val")
			.add("num-val")
			.add("prose-val");

	//	group = "(" *c-wsp alternation *c-wsp ")"
	rulelist.newRule("group").newConcat()
			.add(Abnf::newCharVal("("))
			.add(Abnf::newRpt("c-wsp"))
			.add("alternation")
			.add(Abnf::newRpt("c-wsp"))
			.add(Abnf::newCharVal(")"));

	//	option = "[" *c-wsp alternation *c-wsp "]"
	rulelist.newRule("option").newConcat()
			.add(Abnf::newCharVal("["))
			.add(Abnf::newRpt("c-wsp"))
			.add("alternation")
			.add(Abnf::newRpt("c-wsp"))
			.add(Abnf::newCharVal("]"));

	//	char-val = DQUOTE *( %x20-21 / %x23-7E ) DQUOTE
	//			; quoted string of SP and VCHAR
	//			; without DQUOTE
	rulelist.newRule("char-val").newConcat()
			.add(Abnf::newCharVal(ABNF_DQUOTE))
			.add(Abnf::newRpt()
				.add(Abnf::newAltern()
					.add(Abnf::newNumVal(0x20, 0x21, 16))
					.add(Abnf::newNumVal(0x23, 0x7E, 16))))
			.add(Abnf::newCharVal(ABNF_DQUOTE))
			.addComment()
			.addComment("quoted string of SP and VCHAR")
			.addComment("without DQUOTE");

	//	num-val = "%" ( bin-val / dec-val / hex-val )
	rulelist.newRule("num-val").newConcat()
		.add(Abnf::newCharVal("%"))
		.add(Abnf::newAltern()
			.add("bin-val")
			.add("dec-val")
			.add("hex-val"));

	//	bin-val = "b" 1*BIT [ 1*( "." 1*BIT ) / ( "-" 1*BIT ) ]
	//			; series of concatenated bit values
	//			; or single ONEOF range
	rulelist.newRule("bin-val").newConcat()
		.add(Abnf::newCharVal("b"))
		.add(Abnf::newRpt(1).add(Abnf::newCharVal(ABNF_BIT, IS_ALTERN)))
		.add(Abnf::newRpt(0, 1)
			.add(Abnf::newAltern()
				.add(Abnf::newRpt(1)
					.add(Abnf::newConcat()
						.add(Abnf::newCharVal("."))
						.add(Abnf::newRpt(1).add(Abnf::newCharVal(ABNF_BIT, IS_ALTERN)))))
				.add(Abnf::newConcat()
					.add(Abnf::newCharVal("-"))
					.add(Abnf::newRpt(1).add(Abnf::newCharVal(ABNF_BIT, IS_ALTERN))))))
			.addComment()
			.addComment("series of concatenated bit values")
			.addComment("or single ONEOF range");

	//	dec-val = "d" 1*DIGIT [ 1*( "." 1*DIGIT ) / ( "-" 1*DIGIT ) ]
	rulelist.newRule("dec-val").newConcat()
		.add(Abnf::newCharVal("d"))
		.add(Abnf::newRpt(1).add(Abnf::newCharVal(ABNF_DIGIT, IS_ALTERN)))
		.add(Abnf::newRpt(0, 1)
			.add(Abnf::newAltern()
				.add(Abnf::newRpt(1)
					.add(Abnf::newConcat()
						.add(Abnf::newCharVal("."))
						.add(Abnf::newRpt(1).add(Abnf::newCharVal(ABNF_DIGIT, IS_ALTERN)))))
				.add(Abnf::newConcat()
					.add(Abnf::newCharVal("-"))
					.add(Abnf::newRpt(1).add(Abnf::newCharVal(ABNF_DIGIT, IS_ALTERN))))));

	//	hex-val = "x" 1*HEXDIG [ 1*( "." 1*HEXDIG ) / ( "-" 1*HEXDIG ) ]
	rulelist.newRule("hex-val").newConcat()
		.add(Abnf::newCharVal("x"))
		.add(Abnf::newRpt(1).add(Abnf::newCharVal(ABNF_HEXDIG, IS_ALTERN)))
		.add(Abnf::newRpt(0, 1)
			.add(Abnf::newAltern()
				.add(Abnf::newRpt(1)
					.add(Abnf::newConcat()
						.add(Abnf::newCharVal("."))
						.add(Abnf::newRpt(1).add(Abnf::newCharVal(ABNF_HEXDIG, IS_ALTERN)))))
				.add(Abnf::newConcat()
					.add(Abnf::newCharVal("-"))
					.add(Abnf::newRpt(1).add(Abnf::newCharVal(ABNF_HEXDIG, IS_ALTERN))))));

	//	prose-val = "<" *( %x20-3D / %x3F-7E ) ">"
	//			; bracketed string of SP and VCHAR
	//			;  without angles
	//			; prose description, to be used as
	//			;  last resort
	rulelist.newRule("prose-val").newConcat()
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

/*
	rulelist.newRule("NL").newAltern()
			.add(Abnf::newCharVal("\r\n")) // Windows/DOS
			.add(Abnf::newCharVal("\n\r")) // Mac OS 9
			.add(Abnf::newCharVal("\n"));  // Unix
*/
}
