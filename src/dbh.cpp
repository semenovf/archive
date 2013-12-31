/*
 * dbh.cpp
 *
 *  Created on: Aug 19, 2013
 *      Author: wladt
 */

#include "../include/cwt/debby/dbh.hpp"
#include "../include/cwt/debby/sth.hpp"
#include <cwt/dl.hpp>
#include <cwt/logger.hpp>
#include <cwt/safeformat.hpp>
#include <cwt/uri.hpp>

namespace cwt { namespace debby {

DbHandlerPtr DbHandler::open (const cwt::String & uri_str)
{
	cwt::Uri uri;

	if (!uri.parse(uri_str)) {
		cwt::Logger::error(_Fr("Invalid URI specified for DB driver: %s") % uri_str);
		return DbHandlerPtr();
	}

	cwt::String debby_name = uri.scheme();

	if (debby_name.isEmpty()) {
		Logger::error(_Tr("Invalid URI specified for DB driver: DB driver name is empty."));
		return DbHandlerPtr();
	}

	debby_name.prepend(cwt::String("cwt-debby-"));
	DbDriver * driver = nullptr;

	cwt::String dlpath = cwt::Dl::buildDlFileName(debby_name);
	if (!cwt::Dl::pluginOpen(debby_name, dlpath, & driver)) {
		cwt::Logger::error(_Fr("Fatal error while loading DB driver for %s from %s") % uri.scheme() % dlpath);
		return DbHandlerPtr();
	}

	cwt::Vector<cwt::String> userinfo = uri.userinfo().split(":");
	cwt::Map<cwt::String, cwt::String> params;

	DbHandlerData * driverData = driver->open (uri.path()
			, userinfo.size() > 0 ? userinfo[0] : cwt::String() // login
			, userinfo.size() > 1 ? userinfo[1] : cwt::String() // password
			, uri.queryItems());

	if (!driverData)
		return DbHandlerPtr();

	DbHandlerPtr dbh(new DbHandler);
	dbh->_dbhData = driverData;

	return dbh;
}

/*
bool DbHandler::deploySchema (DbHandler & dbh, const Schema & schema)
{
	if (!dbh._dbhData)
		return false;
	return dbh._dbhData->driver->createSchema(*dbh._dbhData, schema);
}

bool DbHandler::dropSchema (DbHandler & dbh, const Schema & schema)
{
	if (!dbh._dbhData)
		return false;
	// Drop schema (connection closed automatically)
	if (dbh._dbhData->driver->dropSchema(*dbh._dbhData, schema)) {
		dbh._dbhData = nullptr;
		return true;
	}
	return false;
}
*/

void DbHandler::close ()
{
	if (_dbhData) {
		_dbhData->driver->close(_dbhData);
		_dbhData = nullptr;
	}
}

StatementPtr DbHandler::prepare (const cwt::String & sql)
{
	DbStatementData * d = _dbhData->driver->prepare(*_dbhData, sql);
	if (d) {
		StatementPtr sth(new Statement);
		sth->m_sth = d;
		sth->m_bindCursor = 0; // reset counter of bind parameters
		return sth;
	}

	return StatementPtr();
}

}} // cwt::debby


