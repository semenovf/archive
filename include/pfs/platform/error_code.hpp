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

DLL_API string & lexical_cast (const error_code & ex, string & result);

}} // pfs::platform

#endif /* __PFS_ERROR_CODE_HPP__ */
