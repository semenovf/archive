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
 * expr = 1*DIGIT "+" op
 * op = 1*DIGIT
 * DIGIT = "0" / "1" / "2" / "3" / "4" / "5" / "6" / "7" / "8" / "9"
 */
void test_abnf_normalize()
{
	AbnfRuleSet * ruleset = Abnf::createRuleSet ();

	ruleset->newRule("expr")
		->newConcat()
			.add("op")
			.add(Abnf::newCharVal("+"))
			.add("op");

	ruleset->newRule("op")
		->newRpt("DIGIT", 1);

	ruleset->newRule("DIGIT")
		->newAltern()
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

	CWT_TEST_OK(ruleset->normalize());

	std::cout << "ABNF: =============================" << std::endl;
	std::cout << ruleset->toString();// << std::endl;
	std::cout << "===================================" << std::endl;

	Abnf::destroyRuleSet (ruleset);
}

int main(int argc, char *argv[])
{
	CWT_UNUSED(argc);
	CWT_UNUSED(argv);
	CWT_BEGIN_TESTS(1);

	test_abnf_normalize();

	CWT_END_TESTS;
}

