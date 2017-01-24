/**
 * @file   test.cpp
 * @author
 * @date
 *
 * @brief testing ...
 */

#include <pfs/test/test.hpp>
#include <pfs.hpp>
#include <pfs/mpl/impl/stdcxx/string_builder.hpp>
#include <pfs/mpl/impl/stdcxx/list.hpp>
#include <pfs/mpl/impl/stdc/string.hpp>
#include <pfs/mpl/fs/path.hpp>

typedef pfs::mpl::fs::traits<char const *, pfs::mpl::stdcxx::list> traits_type;
typedef pfs::mpl::fs::path<traits_type>   path_type;
typedef typename traits_type::string_type string_type;

#include "test_path.hpp"
#include "test_canonical.hpp"

int main (int argc, char *argv[])
{
	PFS_UNUSED(argc);
	PFS_UNUSED(argv);

	BEGIN_TESTS(0);

  	test::base();
	test::canonical();
    
	return END_TESTS;
}
