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
//#include <cwt/mt.hpp>
#include <cwt/safeformat.hpp>
#include <cwt/uri.hpp>

CWT_NS_BEGIN

//static const char * __db_driver_ctor_sym = "__open__";
//static CWT_DEFAULT_MT_POLICY g_mutex;

DbHandler * DbHandler::open (const String & uri_str)
{
/*
	static CWT_DEFAULT_MT_POLICY mutex;
	AutoLock<> locker(& mutex);
*/

//	DbHandler::driver_ctor db_driver_ctor;
	Uri uri;

	if (!uri.parse(uri_str)) {
		Logger::error(_Fr("Invalid URI specified for DB driver: %s") % uri_str);
		return nullptr;
	}

	String debby_name = uri.scheme();

	if (debby_name.isEmpty()) {
		Logger::error(_Tr("Invalid URI specified for DB driver: DB driver name is empty."));
		return nullptr;
	}

	debby_name.prepend(String("cwt-debby-"));
	DbDriver * driver = nullptr;

	String dlpath = Dl::buildDlFileName(debby_name);
	if (!Dl::pluginOpen(debby_name, dlpath, & driver)) {
		Logger::error(_Fr("Fatal error while loading DB driver for %s from %s") % uri.scheme() % dlpath);
		return nullptr;
	}
/*
	String dlpath = Dl::buildDlFileName(debby_name);
	Dl::Handle dlh = Dl::open(dlpath);

	if (!dlh) {
		Logger::error(_Fr("Unable to load DB driver for %s from %s") % uri.scheme() % dlpath);
		return nullptr;
	}

	db_driver_ctor = reinterpret_cast<driver_ctor>(Dl::symbol(dlh, __db_driver_ctor_sym));
	if (!db_driver_ctor) {
		Logger::error(_Fr("DB driver constructor (%s) not found for %s at %s") % __db_driver_ctor_sym % uri.scheme() % dlpath);
		return nullptr;
	}

	DbDriver * driver = !db_driver_ctor(& driver);
	if (!driver) {
		Logger::error(_Fr("Fatal error while loading DB driver for %s from %s") % uri.scheme() % dlpath);
		return nullptr;
	}
*/

	Vector<String> userinfo = uri.userinfo().split(":");
	Map<String, String> params;

	DbHandlerData * driverData = driver->open (uri.path()
			, userinfo.size() > 0 ? userinfo[0] : String() // login
			, userinfo.size() > 1 ? userinfo[1] : String() // password
			, uri.queryItems());

	if (!driverData)
		return nullptr;

	DbHandler * dbh = new DbHandler;

	dbh->m_dbh = driverData;

	return dbh;
}


bool DbHandler::dropScheme (DbHandler * dbh)
{
/*
	static CWT_DEFAULT_MT_POLICY mutex;
	AutoLock<> locker(& mutex);
*/
	// Drop scheme (connection closed automatically)
	if (dbh->m_dbh->driver->dropScheme(dbh->m_dbh)) {
		dbh->m_dbh = nullptr;
		return true;
	}
	return false;
}

void DbHandler::close ()
{
/*
	static CWT_DEFAULT_MT_POLICY mutex;
	AutoLock<> locker(& mutex);
*/

	if (m_dbh) {
		m_dbh->driver->close(m_dbh);
		m_dbh = nullptr;
	}
}

DbStatement * DbHandler::prepare (const String & sql)
{
	DbStatementData * d = m_dbh->driver->prepare(*m_dbh, sql);
	if (d) {
		DbStatement * sth = new DbStatement;
		sth->m_sth = d;
		sth->m_bindCursor = 0; // reset counter of bind parameters
		return sth;
	}

	return nullptr;
}

CWT_NS_END


