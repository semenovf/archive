/**
 * @file   system.c
 * @author wladt
 * @date   Apr 11, 2013 11:37:39 AM
 *
 * @brief
 */

#include <string.h>
#include <pfs/string.hpp>
#include <pfs/mt.hpp>

// FIXME Need to support system locale

pfs::string pfs_strerror (int errn)
{
#if defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L
	static const int __MaxBufLen = 128;
	char buffer[__MaxBufLen];

// XSI-compliant version
#	if(_POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600) && ! _GNU_SOURCE

	strerror_r(errn, buffer, __MaxBufLen);
	return pfs::string::fromUtf8(buffer);

#	else // GNU-specific version

	return pfs::string::fromUtf8(strerror_r(errn, buffer, __MaxBufLen));

#	endif

#else
	static pfs::mutex __mutex;
	pfs::auto_lock<pfs::mutex> locker(&__mutex);
	return pfs::string::fromUtf8(strerror(errn));
#endif
}


