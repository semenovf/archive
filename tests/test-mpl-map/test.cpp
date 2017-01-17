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

// Enabled by `qt_enable`
#ifdef QT_CORE_LIB
#   include <pfs/mpl/impl/qt/map.hpp>
#endif


#include "test_access.hpp"

int main (int argc, char *argv[])
{
 	PFS_UNUSED(argc);
	PFS_UNUSED(argv);
    
	BEGIN_TESTS(0);
    
    test_access<pfs::mpl::stdcxx::map>();
    
#ifdef QT_CORE_LIB
    test_access<QMap>();
#endif    

	return END_TESTS;
}
