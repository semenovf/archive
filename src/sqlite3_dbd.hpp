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
};

#endif /* __CWT_DEBBY_SQLITE3_DBD_HPP__ */
