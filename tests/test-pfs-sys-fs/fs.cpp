/**
 * @file fs.cpp
 * @author wladt
 * @date Apr 24, 2015
 */

#include <pfs/test/test.hpp>
#include <pfs/string.hpp>
#include <pfs/stringlist.hpp>
#include <pfs/fs.hpp>
#include <pfs/map.hpp>
#include <iostream>

using std::cout;
using std::endl;
using pfs::string;
using pfs::stringlist;

extern void test_join ();
extern void test_normalize ();

void test_temp_file ()
{
	typedef pfs::map<string, bool> map_type;
	map_type tmpFilesMap;

    pfs::fs fs;
    int n = 10;
    int i = n;

    // FIXME need to generate unique temporary file for each iteration (modify `random' class).
    while (i-- > 0) {
    	pfs::string tmpFile = fs.tempFile(_l1("prefix"), _l1("suffix"), _l1(""));
    	if (tmpFilesMap.contains(tmpFile))
    		break;
    }

    TEST_OK2(tmpFilesMap.size() == n, "All temporary file names are unique");

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
	test_join();
	test_normalize();

	END_TESTS;
}



