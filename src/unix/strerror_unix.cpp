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

pfs::string __strerror (int_t errn)
{
#if defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L
	static const int __MaxBufLen = 128;

	char buffer[__MaxBufLen];
	strerror_r(errn, buffer, __MaxBufLen);
	return pfs::string::fromUtf8(buffer);
#else
	static mutex __mutex;
	auto_lock<> locker(&__mutex);
	return pfs::string::fromUtf8(strerror(errn));
#endif
}


