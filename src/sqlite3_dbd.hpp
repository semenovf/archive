/*
 * sqlite3_dbd.hpp
 *
 *  Created on: Nov 6, 2013
 *      Author: wladt
 */

#ifndef __CWT_DEBBY_SQLITE3_DBD_HPP__
#define __CWT_DEBBY_SQLITE3_DBD_HPP__

#include <cwt/debby/dbd.hpp>
#include "../sqlite3/sqlite3.h"

struct Sqlite3DbHandler : public cwt::debby::database_data
{
	sqlite3 * _dbh_native;
};

struct Sqlite3DbStatement : public cwt::debby::statement_data
{
	sqlite3_stmt * _sth_native;
	ulong_t _nrows;
	long_t  _lastId;
	int     _columnCount;

	Sqlite3DbStatement () : _sth_native(nullptr), _nrows(0), _lastId(0), _columnCount(0) {}
	void reset ()
	{
		_nrows = 0,
		_lastId = 0;
		_columnCount = 0;
	}
};

#endif /* __CWT_DEBBY_SQLITE3_DBD_HPP__ */
