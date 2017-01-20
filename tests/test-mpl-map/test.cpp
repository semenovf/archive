/**
 * @file   test.cpp
 * @author
 * @date
 *
 * @brief testing ...
 */

#include <pfs/test/test.hpp>
#include <pfs.hpp>

#include <pfs/mpl/impl/stdcxx/map.hpp>
#include <pfs/mpl/impl/stdcxx/multimap.hpp>

// Enabled by `qt_enable`
#ifdef QT_CORE_LIB
#   include <pfs/mpl/impl/qt/map.hpp>
#   include <pfs/mpl/impl/qt/multimap.hpp>
#endif

#include "test_access.hpp"
#include "test_compare_swap.hpp"

int main (int argc, char *argv[])
{
 	PFS_UNUSED(argc);
	PFS_UNUSED(argv);
    
	BEGIN_TESTS(0);
    
    test_access<pfs::mpl::map, pfs::mpl::stdcxx::map>();
    test_compare<pfs::mpl::map, pfs::mpl::stdcxx::map>();
    test_swap<pfs::mpl::map, pfs::mpl::stdcxx::map>();
    
    test_access<pfs::mpl::multimap, pfs::mpl::stdcxx::multimap>();
//    test_compare<pfs::mpl::multimap, pfs::mpl::stdcxx::multimap>();
//    test_swap<pfs::mpl::multimap, pfs::mpl::stdcxx::multimap>();
    
#ifdef QT_CORE_LIB
    test_access<pfs::mpl::map, QMap>();
    test_compare<pfs::mpl::map, QMap>();
    test_swap<pfs::mpl::map, QMap>();

//    test_access<pfs::mpl::multimap, QMultiMap>();
//    test_compare<pfs::mpl::multimap, QMultiMap>();
//    test_swap<pfs::mpl::multimap, QMultiMap>();
#endif    

	return END_TESTS;
}
