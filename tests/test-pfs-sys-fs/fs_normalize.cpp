/*
 * fs_normalize.cpp
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

struct TestNormalize
{
	const char * source;
	const char * result;
};

static TestNormalize test_norm_data[] = {
      { "/a/b/c/.././../d/f/", "/a/d/f" }
	, { "/", "/" }
    , { "//", "/" }
    , { "///", "/" }
//	, { { "/",  0 },  "/" }
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
	, { 0 , 0 }
};

string normalize (const string & path)
{

	if (path.isEmpty())
		return string();

	pfs::fs fs;
	stringlist sl = path.split(fs.separatorChar(), string::DontKeepEmpty);

	stringlist::const_iterator it = sl.cbegin();
	stringlist::const_iterator itEnd = sl.cend();

	stringlist result;

	if (path.startsWith(fs.separatorChar()))
		result.append(string(1, fs.separatorChar()));

	while (it != itEnd) {
		cout << "[" << static_cast<string>(*it) << "]" << endl;

		if (*it == string("..")) {
			++it;
		} else if (*it == string(".")) {
			; // nothing to do
		} else {
			result.append(*it);
		}

		if (it != itEnd)
			++it;
	}

	return fs.join(result);
}

void test_normalize ()
{
	pfs::fs fs;
	fs.setSeparatorChar('/');

	ADD_TESTS(sizeof(test_norm_data)/sizeof(test_norm_data[0]) - 1);

	TestNormalize * test = & test_norm_data[0];

	while (test->source) {
		string result = normalize(string(test->source));
		string text;
		text << test->result << "==" << result;
		TEST_OK2(string(test->result) == result, text.c_str());

		++test;
	}
}
