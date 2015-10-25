/*
 * test_find.cpp
 *
 *  Created on: Oct 21, 2015
 *      Author: wladt
 */

#include <iostream>
#include <pfs/test/test.hpp>
#include "pfs/algo/string/find.hpp"
#include "pfs/cast/string.hpp"
//#include <pfs/string.hpp>
#include <string>

using std::cout;
using std::endl;
//using pfs::string;
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
	  {{ "a", -1, -1 },  { "a", -1, -1 },  0 }
	, {{ "ab", -1, -1 },  { "b", -1, -1 },  1 }
	, {{ "abcdf", -1, -1 },  { "c", -1, -1 },  2 }
	, {{ "abcdf", 2, -1 },  { "c", -1, -1 },  0 }
	, {{ "abcdf", -1, -1 },  { "df", -1, -1 },  3 }
	, {{ "abcdf", -1, -1 },  { "dfa", -1, -1 },  -1 }
	, {{ 0, -1, -1 },  { 0, -1, -1 },  -1 }
};

void test_find ()
{
	ADD_TESTS(sizeof(test_data)/sizeof(test_data[0]) - 1);

	TestData * test = & test_data[0];

	while (test->haystack.s) {

		string text;

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

		string::iterator it = pfs::find(
			  haystackBegin
			, haystackEnd
			, needleBegin
			, needleEnd);

		if (test->pos >= 0) {
			string tmp;

			text.append("\"");
			text.append(test->needle.s);
			text.append("\" found in \"");
			text.append(test->haystack.s);
			text.append("\" at pos ");
			text.append(pfs::lexical_cast(test->pos, tmp));

			TEST_OK2(it == haystackBegin + test->pos, text.c_str());
		} else {
			text.append("\"");
			text.append(test->needle.s);
			text.append("\" not found in \"");
			text.append(test->haystack.s);
			text.append("\"");

			TEST_OK2(it == haystackEnd, text.c_str());
		}

		++test;
	}
}
