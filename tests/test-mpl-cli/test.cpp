/**
 * @file   test.cpp
 * @author
 * @date
 *
 * @brief testing ...
 */

#include <pfs/test/test.hpp>
#include <pfs.hpp>
#include "test_cmdline.hpp"
//#include "test_route.hpp"

int main (int argc, char *argv[])
{
	PFS_UNUSED(argc);
	PFS_UNUSED(argv);

	BEGIN_TESTS(0);

    test::cli::test_iterator();
    test::cli::test_tokenizer();
//    test::cli::test_route();
    
	return END_TESTS;
}

