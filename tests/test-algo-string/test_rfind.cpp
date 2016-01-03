/*
 * test_find.cpp
 *
 *  Created on: Oct 21, 2015
 *      Author: wladt
 */

#include <iostream>
#include <pfs/test/test.hpp>
#include "pfs/algo/find.hpp"
#include "pfs/string.hpp"
#include <string>

using std::cout;
using std::endl;
using std::string;

struct StringParm
{
	const char * s;
	int start;
	int end;
};

struct TestData
{
	StringParm haystack;
	StringParm needle;
	int pos;
};

static TestData test_data[] = {
	  {{ "a"     , -1, -1 },  { "a"   , -1, -1 },  0 }
	, {{ "ab"    , -1, -1 },  { "b"   , -1, -1 },  1 }
	, {{ "ab"    , -1, -1 },  { "aba" , -1, -1 }, -1 }
	, {{ "abc"   , -1, -1 },  { "c"   , -1, -1 },  2 }
    , {{ "abc"   , -1, -1 },  { "d"   , -1, -1 }, -1 }
	, {{ "abcd"  ,  2, -1 },  { "c"   , -1, -1 },  0 }
	, {{ "abcabc", -1, -1 },  { "abc" , -1, -1 },  3 }
    , {{ "abcabc", -1, -1 },  { "cb"  , -1, -1 }, -1 }
	, {{ "abcabc", -1, -1 },  { "cb"  , -1, -1 }, -1 }
	, {{ 0, -1, -1 },  { 0, -1, -1 },  -1 }
};

void test_rfind ()
{
	ADD_TESTS(sizeof(test_data)/sizeof(test_data[0]) - 1);

	TestData * test = & test_data[0];

	while (test->haystack.s) {
		string haystack(test->haystack.s);
		string needle(test->needle.s);

		string::iterator haystackBegin = haystack.begin();
		string::iterator haystackEnd   = haystack.end();
		string::iterator needleBegin   = needle.begin();
		string::iterator needleEnd     = needle.end();

		if (test->haystack.start > 0)
			haystackBegin += test->haystack.start;

		if (test->haystack.end > 0)
			haystackEnd = haystackBegin + test->haystack.end;

		if (test->needle.start > 0)
			needleBegin += test->needle.start;

		if (test->needle.end > 0)
			needleEnd = needleBegin + test->needle.end;

		string::iterator it = pfs::rfind(
			  haystackBegin
			, haystackEnd
			, needleBegin
			, needleEnd);

		if (test->pos >= 0) {
			std::stringstream oss;

			oss << "\""
			    << test->needle.s
			    << "\" found in \""
			    << test->haystack.s
			    << "\" at pos "
			    << test->pos;

			TEST_OK2(it == haystackBegin + test->pos, oss.str().c_str());
		} else {
			std::stringstream oss;
			oss << "\""
			    << test->needle.s
			    << "\" not found in \""
			    << test->haystack.s
			    << "\"";

			TEST_OK2(it == haystackEnd, oss.str().c_str());
		}

		++test;
	}
}
