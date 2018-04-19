/*
 * fs_join.cpp
 *
 *  Created on: Oct 20, 2015
 *      Author: wladt
 */

#include <iostream>
#include <sstream>
#include <pfs/test/test.hpp>
#include <pfs/stringlist.hpp>
#include "pfs/fs/path.hpp"

using std::cout;
using std::endl;
using pfs::string;
using pfs::stringlist;

static const string SEPARATOR("/");

struct test_data_t
{
	const string path_str;
	const string canonical;
};

static test_data_t test_data[] = {
	  { _u8("abc/../../abc/file.txt") , _u8("abc/file.txt") }
	, { _u8("/")                   , _u8("/") }
	, { _u8("/abc/../abc/file.txt"), _u8("/abc/file.txt") }
	, { _u8("abc/../abc/file.txt") , _u8("abc/file.txt") }
	, { _u8("abc/../../abc/file.txt") , _u8("abc/file.txt") }
	, { string(), string() }


//	  { { "/",  0 },  "/" }
//	, { { "/", "/", 0 },  "/" }
//	, { { "/", "/", "/", 0 },  "/" }
//	, { { "/", "a", "/", 0 },  "/a/" }
//	, { { "/", "a", 0 },  "/a" }
//	, { { "/", "a", "b", 0 }, "/a/b" }
//	, { { "/", "a", "b", "c", 0 }, "/a/b/c" }
//	, { { "a", "b", "c", 0 }, "a/b/c" }
//	, { { "a", "b", "/", 0 }, "a/b/" }
//	, { { "a", "b", "/", "/", 0 }, "a/b/" }
//	, { { "a", "b", "/", "/", "/", 0 }, "a/b/" }
//	, { { 0 }, 0 }
};

void test_canonical ()
{
	ADD_TESTS(1 + sizeof(test_data)/sizeof(test_data[0]) - 1);

	TEST_OK(pfs::fs::path().canonical() == pfs::fs::path());

	test_data_t * test = & test_data[0];

	while (!test->path_str.empty()) {
		std::ostringstream oss;

		pfs::fs::path canonical = pfs::fs::path(test->path_str).canonical(SEPARATOR);

		oss << "'" << canonical.native()
			<< "' is canonical for '"
			<< test->path_str << "'";

		bool result = canonical == pfs::fs::path(test->canonical);

		if (!result) {
			oss << "' but expected '" << test->canonical << "'";
		}

		TEST_OK2(result, oss.str().c_str());

		++test;
	}
}
