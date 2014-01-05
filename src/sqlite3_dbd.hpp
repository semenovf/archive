/*
 * sqlite3_dbd.hpp
 *
 *  Created on: Nov 6, 2013
 *      Author: wladt
 */

#ifndef __CWT_DEBBY_SQLITE3_DBD_HPP__
#define __CWT_DEBBY_SQLITE3_DBD_HPP__

#include <cwt/debby/dbd.hpp>
#include <cwt/debby/dbh.hpp>
#include "../sqlite3/sqlite3.h"

struct Sqlite3DbHandler : public cwt::debby::handler_data
{
	sqlite3 * dbh_native;
};

struct Sqlite3DbStatement : public cwt::debby::statement_data
{
	sqlite3_stmt * sth_native;
};


bool s3_dbd_query (cwt::debby::handler_data & dbh, const pfs::string & sql);
/*
bool s3_create_schema (cwt::debby::handler_data & dbh, const Schema & schema);
bool s3_drop_schema (cwt::debby::handler_data & dbh, const Schema & schema);
*/

#endif /* __CWT_DEBBY_SQLITE3_DBD_HPP__ */
