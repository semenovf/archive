/**
 * @file   dl.cpp
 * @author wladt
 * @date   Feb 1, 2013 15:18:36 AM
 *
 * @brief URI testing
 */

#include <pfs/dl.hpp>
#include <pfs/test/test.hpp>
#include <pfs/string.hpp>
#include <pfs/fs/path.hpp>
#include <iostream>

using pfs::string;

int main(int argc, char *argv[])
{
    PFS_UNUSED2(argc, argv);
	BEGIN_TESTS(4);

	typedef int (*dl_test_fn)(void);

	pfs::dynamic_library dl;
	pfs::fs::path dlfile1 = dl.build_filename(string("pfs-sys"));
	pfs::fs::path dlfile2 = dl.build_filename(string("pfs-sys-d"));
	pfs::fs::path * pdlfile = 0;

	pfs::fs::pathlist searchdirs;

	searchdirs.push_back(pfs::fs::path("."));
	searchdirs.push_back(pfs::fs::path(".."));

	if (!pfs::fs::search_file(dlfile1, searchdirs).empty()) {
		pdlfile = & dlfile1;
	} else if (!pfs::fs::search_file(dlfile2, searchdirs).empty()) {
		pdlfile = & dlfile2;
	}

	TEST_FAIL2(pdlfile != 0, "Candidate for dynamic library found (pfs-sys or pfs-sys-d)");

	string errstr;
	pfs::error_code ex;

	bool rc = dl.open(*pdlfile, searchdirs, & ex, & errstr);

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

	END_TESTS;
}
