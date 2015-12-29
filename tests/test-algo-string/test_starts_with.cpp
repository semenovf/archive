/*
 * test_starts_with.cpp
 *
 *  Created on: Nov 8, 2015
 *      Author: wladt
 */
#include <iostream>
#include <pfs/test/test.hpp>
#include "pfs/algo/find.hpp"
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
	bool ok;
};

static TestData test_data[] = {
	  {{ "a", -1, -1 },  { "a", -1, -1 },  true }
	, {{ "ab", -1, -1 },  { "b", -1, -1 },  false }
	, {{ "abcdf", -1, -1 },  { "ab", -1, -1 },  true }
	, {{ "abcdf", -1, -1 },  { "abcdfe", -1, -1 },  false }
	, {{ "abcdf", -1, -1 },  { "abcd", -1, -1 },  true }
	, {{ "abcdf", -1, -1 },  { "abcdf", -1, -1 },  true }
	, {{ 0, -1, -1 },  { 0, -1, -1 },  -1 }
};

void test_starts_with ()
{
	size_t ntests = sizeof(test_data)/sizeof(test_data[0]) - 1;

	ADD_TESTS(ntests);

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

		bool result = pfs::starts_with(
			  haystackBegin
			, haystackEnd
			, needleBegin
			, needleEnd);


		if (test->ok) {
			text.append("\"");
			text.append(test->haystack.s);
			text.append("\" starts with \"");
			text.append(test->needle.s);
			text.append("\"");
		} else {
			text.append("\"");
			text.append(test->haystack.s);
			text.append("\" do not starts with \"");
			text.append(test->needle.s);
			text.append("\"");
		}

		TEST_OK2(test->ok == result, text.c_str());

		++test;
	}
}



