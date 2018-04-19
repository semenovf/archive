/*
 * test_splitcpp
 *
 *  Created on: Oct 25, 2015
 *      Author: wladt
 */

#include <iostream>
#include "pfs/test/test.hpp"
#include "pfs/string.hpp"
#include "pfs/mpl/algo/join.hpp"
#include "pfs/mpl/algo/split.hpp"
#include "pfs/string.hpp"
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::vector;

struct TestData
{
	const char * source;
	const char * sample;
	const char * separator;
	bool keepEmpty;
	int nelems;
};

static TestData test_data[] = {
	  { "/"  , "/"  , "/", pfs::mpl::keep_empty, 2 }
	, { "/a" , "/a" , "/", pfs::mpl::keep_empty, 2 }
	, { "a/" , "a/" , "/", pfs::mpl::keep_empty, 2 }
    , { "/a/", "/a/", "/", pfs::mpl::keep_empty, 3 }
	, { "a/b", "a/b", "/", pfs::mpl::keep_empty, 2 }
	, { "/a/", "a"  , "/", pfs::mpl::dont_keep_empty, 1 }
	, { "/a//b//c", "/a//b//c"  , "//", pfs::mpl::dont_keep_empty, 3 }
	, { 0    ,     0, 0  , false }
};

void test_split ()
{
	typedef vector<string> sequence_type;

	ADD_TESTS(sizeof(test_data)/sizeof(test_data[0]) - 1);

	TestData * test = & test_data[0];

	while (test->source) {
		sequence_type result;

		string src(test->source);
		string sep(test->separator);

		pfs::mpl::split(src.begin(), src.end(), sep.begin(), sep.end(), test->keepEmpty, & result);

		string sample;

		pfs::mpl::join(result.begin(), result.end(), sep, & sample);
		std::ostringstream desc;

		desc << "keepEmpty="
			 << test->keepEmpty
			 << " \"" << src << "\" => ";

		for (sequence_type::const_iterator it = result.begin(); it != result.end(); ++it) {
			desc << '[' << *it << ']' << ' ';
		}
//		cout << pfs::join(result.begin(), result.end(), sep, text) << endl;
		TEST_OK2(result.size() == test->nelems
				&& sample == string(test->sample), desc.str().c_str());

		++test;
	}
}
