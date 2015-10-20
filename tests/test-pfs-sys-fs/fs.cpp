/**
 * @file fs.cpp
 * @author wladt
 * @date Apr 24, 2015
 */

#include <pfs/test/test.hpp>
#include <pfs/string.hpp>
#include <pfs/stringlist.hpp>
#include <pfs/fs.hpp>
#include <iostream>

using std::cout;
using std::endl;
using pfs::string;
using pfs::stringlist;

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

void test_join ()
{
	pfs::fs fs;
	stringlist paths;
	paths << string(1, fs.separator()) << string("a");
	//paths0 << fs.separator() << "a" << "b" << ".." << ""

	string sample;
	sample = string(1, fs.separator());
	sample.append("a");
	string desc(sample);
	desc.append("==");
	desc.append(fs.join(paths));
	TEST_OK2(sample == fs.join(paths), desc.c_str());
}

int main (int argc, char *argv[])
{
	PFS_UNUSED2(argc, argv);
	BEGIN_TESTS(2);

	test_temp_file();
	test_join();

	END_TESTS;
}



