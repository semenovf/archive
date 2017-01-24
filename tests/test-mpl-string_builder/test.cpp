/**
 * @file   test.cpp
 * @author
 * @date
 *
 * @brief testing ...
 */

#include <pfs.hpp>
#include <pfs/test/test.hpp>
#include <pfs/mpl/impl/stdcxx/string.hpp>
#include <pfs/mpl/impl/stdcxx/string_builder.hpp>
#include <iostream>

typedef pfs::mpl::string_builder<char> string_builder;
typedef pfs::mpl::string<std::string>  string_type;


using std::cout;
using std::endl;

int main(int argc, char *argv[])
{
	PFS_UNUSED(argc);
	PFS_UNUSED(argv);

	BEGIN_TESTS(0);

    string_builder sb;
    sb.push_back("Hello");
    sb.push_back(',');
    sb.push_back(' ');
    sb.push_back(std::string("World"));
    sb.push_back('!');
    sb.push_back(string_type(" Yeh!"));

	ADD_TESTS(1);
    
    TEST_OK(sb.str<std::string>() == "Hello, World! Yeh!");

	return END_TESTS;
}

