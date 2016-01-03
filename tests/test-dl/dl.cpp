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
#include <pfs/fs/path.hpp>
#include <iostream>

using pfs::string;

int main(int argc, char *argv[])
{
    PFS_UNUSED2(argc, argv);
	BEGIN_TESTS(3);

	pfs::dl dl;
	pfs::fs::path dlfile = dl.build_filename(string("pfs-sys"));
	pfs::dl::handle dlhandle;
	typedef int (*dl_test_fn)(void);


	pfs::fs::pathlist searchdirs;

	searchdirs.push_back(pfs::fs::path("."));
	searchdirs.push_back(pfs::fs::path(".."));

	string errstr;
	pfs::error_code ex;

	bool rc = dl.open(dlfile, searchdirs, & ex, & errstr);

	if (!rc) {
		std::cerr << errstr << std::endl;
	}

	TEST_FAIL2(rc, "Open/load library");

	dl_test_fn dltest = reinterpret_cast<dl_test_fn>(dl.resolve("dl_only_for_testing_purpose"
			, & ex
			, & errstr));

	if (ex) {
		std::cerr << errstr << std::endl;
	}

	TEST_FAIL2(!ex, "'dl_only_for_testing_purpose': symbol (function pointer) found");

	TEST_OK2(dltest() == dl_only_for_testing_purpose(), "run plugin/library function");
	dl.close();

	END_TESTS;
}
