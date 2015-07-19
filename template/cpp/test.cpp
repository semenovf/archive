/**
 * @file   test.cpp
 * @author
 * @date
 *
 * @brief testing ...
 */

#include <pfs/test/test.hpp>

using std::cout;
using std::endl;

int main(int argc, char *argv[])
{
	PFS_UNUSED(argc);
	PFS_UNUSED(argv);

	BEGIN_TESTS(2);

	TEST_OK(true == true);
	TEST_FAIL(true == true);

	ADD_TESTS(2);
	TEST_OK2(true == true, "true == true");
	TEST_FAIL2(true == true, "true == true");

	END_TESTS;
}

