/**
 * @file   test.cpp
 * @author
 * @date
 *
 * @brief testing ...
 */

#include <pfs/test/test.hpp>
#include <pfs.hpp>

extern void test_poll ();
extern void test_pool_dispatcher ();

int main(int argc, char *argv[])
{
	PFS_UNUSED(argc);
	PFS_UNUSED(argv);

	BEGIN_TESTS(0);

	test_poll();
	test_pool_dispatcher();

	END_TESTS;
}

