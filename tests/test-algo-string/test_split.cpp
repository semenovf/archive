/*
 * test_splitcpp
 *
 *  Created on: Oct 25, 2015
 *      Author: wladt
 */

#include <iostream>
#include <pfs/test/test.hpp>
#include "pfs/cast/string.hpp"
#include "pfs/algo/string/join.hpp"
#include "pfs/algo/string/split.hpp"
//#include <pfs/string.hpp>
#include <string>
#include <vector>

using std::cout;
using std::endl;
//using pfs::string;
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
	  { "/"  , "/"  , "/", pfs::KeepEmpty, 2 }
	, { "/a" , "/a" , "/", pfs::KeepEmpty, 2 }
	, { "a/" , "a/" , "/", pfs::KeepEmpty, 2 }
    , { "/a/", "/a/", "/", pfs::KeepEmpty, 3 }
	, { "a/b", "a/b", "/", pfs::KeepEmpty, 2 }
	, { "/a/", "a"  , "/", pfs::DontKeepEmpty, 1 }
	, { "/a//b//c", "/a//b//c"  , "//", pfs::DontKeepEmpty, 3 }
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

		pfs::split(src.begin(), src.end(), sep.begin(), sep.end(), test->keepEmpty, result);

		string sample;

		pfs::join(result.begin(), result.end(), sep, sample);
		string desc;
		string b;

		desc.append("keepEmpty=");
		desc.append(pfs::lexical_cast<string>(test->keepEmpty, b));
		desc.append(" \"");
		desc.append(src);
		desc.append("\" => ");

		for (sequence_type::const_iterator it = result.begin(); it != result.end(); ++it) {
			desc.push_back('[');
			desc.append(*it);
			desc.push_back(']');
			desc.push_back(' ');
		}
//		cout << pfs::join(result.begin(), result.end(), sep, text) << endl;
		TEST_OK2(result.size() == test->nelems
				&& sample == string(test->sample)
				, desc.c_str());

		++test;
	}
}
