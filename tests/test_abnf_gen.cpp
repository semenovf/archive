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

/*
 * expr = op "+" op
 * op = 1*DIGIT [ 1*ALPHA ]
 * DIGIT = "0" / "1" / "2" / "3" / "4" / "5" / "6" / "7" / "8" / "9"
 * ALPHA = "a" / "b" / "z" / 1*1%x19 / %x20-FF / "Z" / %x100.101.102.103.104
 */
void test_abnf_gen()
{
	AbnfRuleList ruleset;

	ruleset.newRule("expr")
		.newConcat()
			.add("op")
			.add(Abnf::newCharVal("+"))
			.add(Abnf::newRpt("DIGIT", 1));

	ruleset.newRule("op")
		.newConcat()
			.add(Abnf::newRpt("DIGIT", 1))
			.newRpt(1).add(Abnf::newRuleRef("ALPHA"));

	ruleset.newRule("DIGIT")
		.newAltern()
			.add(Abnf::newCharVal("0"))
			.add(Abnf::newCharVal("1"))
			.add(Abnf::newCharVal("2"))
			.add(Abnf::newCharVal("3"))
			.add(Abnf::newCharVal("4"))
			.add(Abnf::newCharVal("5"))
			.add(Abnf::newCharVal("6"))
			.add(Abnf::newCharVal("7"))
			.add(Abnf::newCharVal("8"))
			.add(Abnf::newCharVal("9"));

	ruleset.newRule("ALPHA")
		.newAltern()
			.add(Abnf::newCharVal("a"))
			.add(Abnf::newCharVal("b"))
			.add(Abnf::newCharVal("z"))
			.add(Abnf::newRpt(1, 1).add(Abnf::newNumVal(0x19, 16)))
			.add(Abnf::newNumVal(0x20, 0xFF, 16))
			.add(Abnf::newCharVal("Z"))
			.add(Abnf::newNumVal(0x100, 16))
			.add(Abnf::newNumVal(0x101, 16))
			.add(Abnf::newNumVal(0x102, 16))
			.add(Abnf::newNumVal(0x103, 16))
			.add(Abnf::newNumVal(0x104, 16));


	AbnfGenContext genCtx("String", ruleset);
	genCtx.setCompactCharValues(true);
	String trans = genCtx.generate();
	CWT_TEST_FAIL(!trans.isEmpty());

	std::cout << "ABNF: =============================" << std::endl;
	std::cout << ruleset.toString();
	std::cout << "===================================" << std::endl;

	std::cout << "Transitions: ======================" << std::endl;
	std::cout << trans << std::endl;
	std::cout << "===================================" << std::endl;
}

int main(int argc, char *argv[])
{
	CWT_UNUSED(argc);
	CWT_UNUSED(argv);
	CWT_BEGIN_TESTS(1);

	test_abnf_gen();

	CWT_END_TESTS;
}

