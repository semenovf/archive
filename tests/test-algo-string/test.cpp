/*
 * mbcs_string.cpp
 *
 *  Created on: Nov 15, 2013
 *  Author: wladt
 */

#include <pfs/test/test.hpp>
#include <pfs.hpp>

extern void test_find ();
extern void test_split ();

int main(int argc, char *argv[])
{
    PFS_UNUSED2(argc, argv);
	BEGIN_TESTS(0);
	test_find();
	test_split();
    END_TESTS;
}
