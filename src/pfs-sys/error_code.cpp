/*
 * error_code.cpp
 *
 *  Created on: Jan 3, 2016
 *      Author: wladt
 */
#include <cerrno>
#include "pfs/error_code.hpp"
#include "pfs/platform.hpp"

#if !(defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L)
#	include <pfs/mutex.hpp>
#endif

// FIXME Need to support system locale

namespace pfs {

string to_string (const error_code & ex)
{
	if (!ex)
		return string("no error");

	if (ex.native() < CustomErrorBase) {
		return platform::to_string(ex);
	}

	switch(ex.native()) {
	case InvalidArgument:
		return _u8("invalid argument");

	case DlSymbolNotFoundError:
		return _u8("symbol not found");

	case ZlibStreamError:
		return _u8("invalid parameter or inconsistent stream state");
	case ZlibDataError:
		return _u8("data corrupted or inconsistent");
	case ZlibMemError:
		return _u8("not enough memory");
	case ZlibBufError:
		return _u8("buffer error, may be not enough memory, or I/O error");
	case ZlibVersionError:
		return _u8("incompatible zlib version");
	}

	string r("unknown error: ");
	r.append(to_string(ex.native()));
	return r;
}

} // pfs
