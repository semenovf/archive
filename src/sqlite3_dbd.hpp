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

using namespace cwt;
using namespace cwt::debby;

struct Sqlite3DbHandler : public DbHandlerData
{
	sqlite3 * dbh_native;
};

struct Sqlite3DbStatement : public DbStatementData
{
	sqlite3_stmt * sth_native;
};


bool s3_dbd_query (DbHandlerData & dbh, const String & sql);
/*
bool s3_create_schema (DbHandlerData & dbh, const Schema & schema);
bool s3_drop_schema (DbHandlerData & dbh, const Schema & schema);
*/

#endif /* __CWT_DEBBY_SQLITE3_DBD_HPP__ */
