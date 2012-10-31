/**
 * @file   abnf.c
 * @author wladt
 * @date   Oct 31, 2012 15:54:00
 *
 * @brief
 */

#include <cwt/abnf.h>
#include <cwt/test.h>

int main(int argc, char *argv[])
{
	CwtAbnfNS *abnf;

	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	abnf = cwtAbnfNS();

	CWT_BEGIN_TESTS(1);

	CWT_UNUSED(abnf);

	CWT_END_TESTS;
}


