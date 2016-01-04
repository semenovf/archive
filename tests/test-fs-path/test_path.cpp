/**
 * @file   test.cpp
 * @author
 * @date
 *
 * @brief testing ...
 */

#include <pfs/test/test.hpp>
#include <pfs/fs/path.hpp>
#include <string>
#include <iostream>

using std::cout;
using std::endl;

typedef pfs::fs::path path_type;
typedef pfs::fs::path::string_type string_type;

extern void test_canonical ();

void test_base ()
{
	ADD_TESTS(5);

//	path_type path("/a/b/c/d");
//	path_type::iterator it = path.begin();
//	path_type::iterator itEnd = path.end();

//	for (; it != itEnd; ++it)
//		cout << *it << endl;

	TEST_OK(pfs::fs::exists(path_type("/usr/local/bin")));
	TEST_OK(!pfs::fs::exists(path_type("/unknown/local/bin")));
	TEST_OK(pfs::fs::is_directory(path_type("/usr/local/bin")));
	TEST_OK(path_type("/usr/local/bin").is_absolute());
	TEST_OK(path_type("usr/local/bin").is_relative());

	cout << "Temporary directory: " << pfs::fs::temp_directory_path().native() << endl;
}


int main(int argc, char *argv[])
{
	PFS_UNUSED(argc);
	PFS_UNUSED(argv);

	BEGIN_TESTS(0);

	test_base();
	test_canonical();

	END_TESTS;
}

