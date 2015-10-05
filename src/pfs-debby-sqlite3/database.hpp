/*
 * database.hpp
 *
 *  Created on: Oct 5, 2015
 *      Author: wladt
 */

#include <pfs/debby/database.hpp>
#include "sqlite3/sqlite3.h"

namespace pfs { namespace debby { namespace sqlite3 {

class database : public pfs::debby::database
{
	typedef sqlite3 * native_handle_type;

	native_handle_type _dbh_native;
};

struct statement : public cwt::debby::statement_data
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
