/*
 * error_code.hpp
 *
 *  Created on: Nov 10, 2015
 *      Author: wladt
 */

#ifndef __PFS_ERROR_CODE_HPP__
#define __PFS_ERROR_CODE_HPP__

#include <pfs/string.hpp>

namespace pfs { namespace platform {

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

}} // pfs::platform

namespace pfs {

// XXX DEPRECATED
DLL_API string & lexical_cast (const pfs::platform::error_code & ex, string & result);

DLL_API string lexical_cast (const pfs::platform::error_code & ex);

} // pfs

#endif /* __PFS_ERROR_CODE_HPP__ */
