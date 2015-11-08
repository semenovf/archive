/**
 * @file   system.c
 * @author wladt
 * @date   Apr 11, 2013 11:37:39 AM
 *
 * @brief
 */

#include <string.h>
#include <pfs/platform/string.hpp>

#if !(defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L)
#	include <pfs/mutex.hpp>
#endif

// FIXME Need to support system locale

namespace pfs { namespace platform {

string_type & strerror (int errn, string_type & result)
{
#if defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L
	static const int __MaxBufLen = 256;
	char_type buffer[__MaxBufLen];

// XSI-compliant version
#	if(_POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600) && ! _GNU_SOURCE

	strerror_r(errn, buffer, __MaxBufLen);
	result = string_type(buffer);

#	else // GNU-specific version

	result = string_type(strerror_r(errn, buffer, __MaxBufLen));

#	endif

#else
	static mutex mtx;
	lock_guard<pfs::mutex> locker(mtx);
	result = string_type(strerror(errn));
#endif

	return result;
}

}} // pfs::platform

