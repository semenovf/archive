/*
 * error_code.hpp
 *
 *  Created on: Nov 10, 2015
 *      Author: wladt
 */

#ifndef __PFS_ERROR_CODE_HPP__
#define __PFS_ERROR_CODE_HPP__

#include <pfs/string.hpp>

namespace pfs {

enum error_code_enum
{
	  CustomErrorBase = 10000

	, InvalidArgument

	, DlErrorBase
	, DlOpenError = DlErrorBase
	, DlSymbolNotFoundError

	, ZlibErrorBase
	, ZlibStreamError = ZlibErrorBase   ///< Invalid parameter or inconsistent stream state (Z_STREAM_ERROR)
	, ZlibDataError                     ///< Data corrupted or inconsistent (Z_DATA_ERROR)
	, ZlibMemError                      ///< Not enough memory (Z_MEM_ERROR)
	, ZlibBufError                      ///< Buffer error (Z_BUF_ERROR), may be not enough memory, or I/O error
	, ZlibVersionError                  ///< Incompatible zlib version (Z_VERSION_ERROR)
};


class error_code
{
	int _native;

public:
	error_code ()
		: _native(0)
	{}

	error_code (int errn)
		: _native(errn)
	{}

	operator bool () const
	{
		return _native != 0;
	}

	error_code & operator = (int errn)
	{
		_native = errn;
		return *this;
	}

	int native () const
	{
		return _native;
	}
};

DLL_API string to_string (const error_code & ex);

} // pfs

#endif /* __PFS_ERROR_CODE_HPP__ */
