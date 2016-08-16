/*
 * error_code.cpp
 *
 *  Created on: Jan 3, 2016
 *      Author: wladt
 */
#include <cerrno>
#include "pfs/error_code.hpp"

#if PFS_OS_POSIX
#   include <cstring>

#   if !(defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L)
#       include <pfs/mutex.hpp>
#   endif
#else // ! PFS_OS_POSIX
#   error "Need to implement 'to_string (error_code const & ex)'"        
#endif

// FIXME Need to support system locale

namespace pfs {

string to_string (error_code const & ex)
{
	if (!ex)
		return string("No error");

	if (ex.native() < CustomErrorBase) {
        
#if PFS_OS_POSIX
#   if defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L
		static const int __MaxBufLen = 256;
		char buffer[__MaxBufLen];

// XSI-compliant version
#       if(_POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600) && ! _GNU_SOURCE

		strerror_r(ex.native(), buffer, __MaxBufLen);
		return string(buffer);

#       else // GNU-specific version

		return string(strerror_r(ex.native(), buffer, __MaxBufLen));

#       endif

#   else
		static mutex __mtx;
		lock_guard<pfs::mutex> locker(__mtx);
		return string(strerror(ex.native()));
#   endif

#endif
	}

	switch(ex.native()) {
	case InvalidArgumentError:
		return _u8("Invalid argument");

	case DlOpenError:
		return _u8("Dynamic library open error");
	case DlSymbolNotFoundError:
		return _u8("Symbol not found");

	case ZlibStreamError:
		return _u8("Invalid parameter or inconsistent stream state");
	case ZlibDataError:
		return _u8("Data corrupted or inconsistent");
	case ZlibMemError:
		return _u8("Not enough memory");
	case ZlibBufError:
		return _u8("Buffer error, may be not enough memory, or I/O error");
	case ZlibVersionError:
		return _u8("Incompatible zlib version");
	}

	string r("Unknown error: ");
	r.append(to_string(ex.native()));
	return r;
}

} // pfs
