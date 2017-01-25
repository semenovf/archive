/**
 * @file   test.cpp
 * @author
 * @date
 *
 * @brief testing ...
 */

namespace test {

void base ()
{
	ADD_TESTS(5);

	TEST_OK(pfs::fs::exists(path_type("/usr/local/bin")));
	TEST_OK(!pfs::fs::exists(path_type("/unknown/local/bin")));
	TEST_OK(pfs::fs::is_directory(path_type("/usr/local/bin")));
	TEST_OK(path_type("/usr/local/bin").is_absolute());
	TEST_OK(path_type("usr/local/bin").is_relative());

//	cout << "Temporary directory: " << pfs::fs::temp_directory_path().native() << endl;
}

}