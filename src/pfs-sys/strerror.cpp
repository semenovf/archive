/**
 * @file strerror.cpp
 * @author wladt
 * @date Jun 25, 2014
 */

#include <pfs/string.hpp>

extern pfs::string pfs_strerror (int errn);

namespace pfs { namespace platform {

DLL_API pfs::string strerror (int errn)
{
	return pfs_strerror(errn);
}

}} // pfs::platform

