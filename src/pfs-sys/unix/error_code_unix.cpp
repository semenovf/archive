/**
 * @file   error_code_unix.cpp
 * @author wladt
 * @date   Nov 10, 2015
 *
 * @brief
 */

#include "pfs/platform/error_code.hpp"

#if !(defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L)
#	include <pfs/mutex.hpp>
#endif

// FIXME Need to support system locale

namespace pfs {

string & lexical_cast (const pfs::platform::error_code & ex, string & result)
{
#if defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L
	static const int __MaxBufLen = 256;
	char buffer[__MaxBufLen];

// XSI-compliant version
#	if(_POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600) && ! _GNU_SOURCE

	strerror_r(ex.native(), buffer, __MaxBufLen);
	result = buffer;

#	else // GNU-specific version

	result = strerror_r(ex.native(), buffer, __MaxBufLen);

#	endif

#else
	static mutex __mtx;
	lock_guard<pfs::mutex> locker(__mtx);
	result = strerror(ex.native());
#endif

	return result;
}


string lexical_cast (const pfs::platform::error_code & ex)
{
#if defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L
	static const int __MaxBufLen = 256;
	char buffer[__MaxBufLen];

// XSI-compliant version
#	if(_POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600) && ! _GNU_SOURCE

	strerror_r(ex.native(), buffer, __MaxBufLen);
	string r(buffer);

#	else // GNU-specific version

	string r(strerror_r(ex.native(), buffer, __MaxBufLen));

#	endif

#else
	static mutex __mtx;
	lock_guard<pfs::mutex> locker(__mtx);
	string r(strerror(ex.native()));
#endif

	return r;
}

} // pfs

