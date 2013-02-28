/**
 * @file   abnf.c
 * @author wladt
 * @date   Oct 31, 2012 15:54:00
 *
 * @brief
 */


#include <cwt/abnf.hpp>
#include <cwt/logger.hpp>

using namespace cwt;

static void test_abnf()
{
	Abnf abnf;
	abnf.parse(String().fromUtf8(abnf_test_str));
}

int main(int argc, char *argv[])
{
	Logger::init();

	CWT_UNUSED(argc);
	CWT_UNUSED(argv);
	CWT_BEGIN_TESTS(125);

	test_abnf_fsm();
	test_abnf();

	CWT_END_TESTS;
}

