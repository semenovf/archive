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

static const string_type SEPARATOR("/");

int main(int argc, char *argv[])
{
	PFS_UNUSED(argc);
	PFS_UNUSED(argv);

	BEGIN_TESTS(5);

	path_type path(string_type("/a/b/c/d"), SEPARATOR);
	path_type::iterator it = path.begin();
	path_type::iterator itEnd = path.end();

//	for (; it != itEnd; ++it)
//		cout << *it << endl;

	TEST_OK(pfs::fs::exists(path_type("/usr/local/bin", SEPARATOR)));
	TEST_OK(!pfs::fs::exists(path_type("/unknown/local/bin", SEPARATOR)));
	TEST_OK(pfs::fs::is_directory(path_type("/usr/local/bin", SEPARATOR)));
	TEST_OK(path_type("/usr/local/bin", SEPARATOR).is_absolute());
	TEST_OK(path_type("usr/local/bin", SEPARATOR).is_relative());

	cout << "Temporary directory: " << pfs::fs::temp_directory_path().native() << endl;

	END_TESTS;
}

