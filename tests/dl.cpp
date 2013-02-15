/**
 * @file   dl.cpp
 * @author wladt
 * @date   Feb 1, 2013 15:18:36 AM
 *
 * @brief URI testing
 */

#include <cwt/dl.hpp>
#include <cwt/test.h>

using namespace cwt;


int main(int argc, char *argv[])
{
	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(3);

	String dlname = Dl::buildDlFileName(_U("cwt"));
	Dl::Handle dlhandle;
	typedef int (*dl_test_fn)(void);
	dl_test_fn dltest;

	CWT_TEST_FAIL(dlhandle = Dl::open(dlname));
	CWT_TEST_FAIL(dltest = (dl_test_fn)Dl::ptr(dlhandle, "dl_only_for_testing_purpose"));
	CWT_TEST_OK(dltest() == dl_only_for_testing_purpose());
	Dl::close(dlhandle);

	CWT_END_TESTS;
}
