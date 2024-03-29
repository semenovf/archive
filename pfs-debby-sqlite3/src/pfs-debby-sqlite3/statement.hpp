/*
 * statement.hpp
 *
 *  Created on: Oct 6, 2015
 *      Author: wladt
 */

#ifndef __PFS_DEBBY_SQLITE3_STATEMENT_HPP__
#define __PFS_DEBBY_SQLITE3_STATEMENT_HPP__

#include <pfs/debby/dbd.hpp>
#include "sqlite3/sqlite3.h"

namespace pfs { namespace debby { namespace sqlite3 {

struct statement : public pfs::debby::statement_impl
{
	typedef sqlite3_stmt * native_handle_type;

	native_handle_type _native_handler;
	uintegral_t _nrows;
	integral_t  _lastId;
	int         _columnCount;

	statement ()
		: _native_handler(nullptr)
		, _nrows(0)
		, _lastId(0)
		, _columnCount(0)
	{}

	void reset ()
	{
		_nrows = 0,
		_lastId = 0;
		_columnCount = 0;
	}
};

}}} // pfs::debby::sqlite3

#endif /* __PFS_DEBBY_SQLITE3_STATEMENT_HPP__ */
