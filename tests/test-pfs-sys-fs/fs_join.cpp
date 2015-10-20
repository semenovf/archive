/*
 * fs_join.cpp
 *
 *  Created on: Oct 20, 2015
 *      Author: wladt
 */

#include <iostream>
#include <pfs/test/test.hpp>
#include <pfs/stringlist.hpp>
#include "pfs/fs.hpp"

using std::cout;
using std::endl;
using pfs::string;
using pfs::stringlist;

struct TestJoin
{
	const char * items[32];
	const char * result;
};

static TestJoin test_join_data[] = {
	  { { "/",  0 },  "/" }
	, { { "/", "/", 0 },  "/" }
	, { { "/", "/", "/", 0 },  "/" }
	, { { "/", "a", "/", 0 },  "/a/" }
	, { { "/", "a", 0 },  "/a" }
	, { { "/", "a", "b", 0 }, "/a/b" }
	, { { "/", "a", "b", "c", 0 }, "/a/b/c" }
	, { { "a", "b", "c", 0 }, "a/b/c" }
	, { { "a", "b", "/", 0 }, "a/b/" }
	, { { "a", "b", "/", "/", 0 }, "a/b/" }
	, { { "a", "b", "/", "/", "/", 0 }, "a/b/" }
	, { { 0 }, 0 }
};

void test_join ()
{
	pfs::fs fs;
	fs.setSeparatorChar('/');

	ADD_TESTS(sizeof(test_join_data)/sizeof(test_join_data[0]) - 1);

	TestJoin * test = & test_join_data[0];

	while (test->result) {
		const char ** item = & test->items[0];

		stringlist dirs;

		while (*item) {
			dirs << *item;
			++item;
		}

		string result = fs.join(dirs);
		string text;
		text << test->result << "==" << result;
		TEST_OK2(string(test->result) == result, text.c_str());

		++test;
	}
}
