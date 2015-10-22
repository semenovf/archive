/*
 * test_find.cpp
 *
 *  Created on: Oct 21, 2015
 *      Author: wladt
 */

#include <iostream>
#include <pfs/test/test.hpp>
#include <pfs/string.hpp>
#include "pfs/algo/string/find.hpp"

using std::cout;
using std::endl;
using pfs::string;

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

		string::const_iterator haystackBegin = haystack.cbegin();
		string::const_iterator haystackEnd   = haystack.cend();
		string::const_iterator needleBegin   = needle.cbegin();
		string::const_iterator needleEnd     = needle.cend();

		if (test->haystack.start > 0)
			haystackBegin += test->haystack.start;

		if (test->haystack.end > 0)
			haystackEnd =  haystackBegin + test->haystack.end;

		if (test->needle.start > 0)
			needleBegin += test->needle.start;

		if (test->needle.end > 0)
			needleEnd =  needleBegin + test->needle.end;

		string::iterator it = pfs::find<string>(
				  haystackBegin
				, haystackEnd
				, needleBegin
				, needleEnd);

		if (test->pos >= 0) {
			text << "\""
					<<	test->needle.s
					<< "\" found in \""
					<< test->haystack.s << "\" at pos "
					<< string::toString(test->pos);

			TEST_OK2(it == haystackBegin + test->pos, text.c_str());
		} else {
			text << "\"" <<	test->needle.s
					<< "\" not found in \""
					<< test->haystack.s << "\"";
			TEST_OK2(it == haystackEnd, text.c_str());
		}


		++test;
	}
}




