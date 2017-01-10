/**
 * @file   test.cpp
 * @author
 * @date
 *
 * @brief Testing pfs::mpl::string.hpp.
 */

#include <pfs/test/test.hpp>
#include <pfs.hpp>

#include <cstring>
#include <cwchar>
#include <pfs/mpl/stdc/string.hpp>
#include <pfs/mpl/stdcxx/string.hpp>
#include "test_basic.hpp"
#include "test_compare.hpp"

int main (int argc, char *argv[])
{
 	PFS_UNUSED(argc);
	PFS_UNUSED(argv);
    
	BEGIN_TESTS(0);
    
    test_basic<char const *>();
    test_basic<wchar_t const *>();
    test_basic<std::string>();
    test_basic<std::wstring>();

    test_compare<char const *>();
    test_compare<wchar_t const *>();
    test_compare<std::string>();
    test_compare<std::wstring>();

	return END_TESTS;
}
