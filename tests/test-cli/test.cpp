/**
 * @file   test.cpp
 * @author
 * @date
 *
 * @brief testing ...
 */

#include <pfs/test/test.hpp>
#include <iostream>
#include "test_basic.hpp"

using std::cout;
using std::endl;

int main(int argc, char *argv[])
{
	PFS_UNUSED(argc);
	PFS_UNUSED(argv);

	BEGIN_TESTS(0);

    test::cli::test_basic();
    
	return END_TESTS;
}

