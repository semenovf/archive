/**
 * @file strerror.cpp
 * @author wladt
 * @date Jun 25, 2014
 */

#include <pfs/string.hpp>

extern pfs::string __cwt_strerror (int_t errn);

namespace cwt { namespace platform {

DLL_API pfs::string strerror (int errn)
{
	return __cwt_strerror(errn);
}

}}

