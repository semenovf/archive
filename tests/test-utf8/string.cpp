/*
 * string.cpp
 *
 *  Created on: Dec 6, 2015
 *      Author: wladt
 */

#include <pfs/test/test.hpp>
#include "pfs/iostream.hpp"
#include "pfs/sstream.hpp"
#include "pfs/string.hpp"

using pfs::string;
using pfs::ostringstream;

void test_string ()
{
	ADD_TESTS(1);

	ostringstream oss;
	string s("Hello, World!");
	oss << s;

	TEST_OK(s == oss.str());

	// TODO Implement more tests
}
