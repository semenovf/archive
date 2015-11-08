/*
 * strerror.hpp
 *
 *  Created on: Nov 8, 2015
 *      Author: wladt
 */

#ifndef __PFS_PLATFORM_STRERROR_HPP__
#define __PFS_PLATFORM_STRERROR_HPP__

#include <pfs/platform/string.hpp>

namespace pfs { namespace platform {

DLL_API string_type & strerror (int errn, string_type & result);

inline string_type strerror (int errn)
{
	string_type r;
	return strerror(errn, r);
}

}} // pfs::platform


#endif /* __PFS_PLATFORM_STRERROR_HPP__ */
