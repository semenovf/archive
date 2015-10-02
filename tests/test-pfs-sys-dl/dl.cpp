/**
 * @file   dl.cpp
 * @author wladt
 * @date   Feb 1, 2013 15:18:36 AM
 *
 * @brief URI testing
 */

#include <pfs/test/test.hpp>
#include <pfs/dl.hpp>
#include <pfs/string.hpp>
#include <iostream>

int main(int argc, char *argv[])
{
    PFS_UNUSED2(argc, argv);
	BEGIN_TESTS(3);

	pfs::dl & dl = pfs::dl::getDL();
	pfs::string dlname = dl.buildDlFileName(pfs::string("pfs-sys"));
	pfs::dl::handle dlhandle;
	typedef int (*dl_test_fn)(void);
	dl_test_fn dltest;

	dl.addSearchPath(pfs::string("."));
	dl.addSearchPath(pfs::string(".."));
	dlhandle = dl.open(dlname);
	if (!dlhandle) {
		std::cerr << dl.lastErrorText() << std::endl;
	}
	TEST_FAIL2(dlhandle, "Open library/plugin");
	TEST_FAIL2(dltest = (dl_test_fn)dl.ptr(dlhandle, "dl_only_for_testing_purpose")
			, "'dl_only_for_testing_purpose': symbol (function pointer) found");
	TEST_OK2(dltest() == dl_only_for_testing_purpose(), "run plugin/library function");
	dl.close(dlhandle);

	END_TESTS;
}
