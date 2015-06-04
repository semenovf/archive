/**
 * @file fs.cpp
 * @author wladt
 * @date Apr 24, 2015
 */

#include <pfs/test/test.hpp>
#include <pfs/string.hpp>
#include <pfs/fs.hpp>
#include <iostream>

using std::cout;
using std::endl;

void test_temp_file ()
{
    pfs::fs fs;
    pfs::string tmpFile = fs.tempFile(_l1("prefix"), _l1("suffix"), _l1(""));

    TEST_FAIL(!tmpFile.isEmpty());
    cout << "Temporary file name: \"" << tmpFile << '"' << endl;

    tmpFile = fs.tempFile(_l1("prefix"), _l1("suffix"));

    TEST_FAIL(!tmpFile.isEmpty());
    cout << "Temporary file name: \"" << tmpFile << '"' << endl;
}

int main (int argc, char *argv[])
{
	PFS_UNUSED2(argc, argv);
	BEGIN_TESTS(2);

	test_temp_file();

	END_TESTS;
}



