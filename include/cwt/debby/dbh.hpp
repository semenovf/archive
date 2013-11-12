/*
 * dbh.hpp
 *
 *  Created on: Aug 19, 2013
 *      Author: wladt
 */

#ifndef __CWT_DEBBY_DBH_HPP__
#define __CWT_DEBBY_DBH_HPP__

#include <cwt/debby/dbd.hpp>
#include <cwt/vector.hpp>
#include <cwt/shared_ptr.hpp>

CWT_NS_BEGIN

namespace debby
{

class Statement;
class DbHandler;

typedef cwt::shared_ptr<DbHandler> DbHandlerPtr;

class DbHandler
{
protected:
	DbHandler() : _dbhData(nullptr) {}

public:
	~DbHandler() { close(); }

	static DbHandlerPtr open (const cwt::String & uri);
	static bool         deploySchema (DbHandler & dbh, const Schema & schema);
	static bool         dropSchema (DbHandler & dbh, const Schema & schema);

	bool     opened() const { return _dbhData != nullptr; }
	void     close ();

	bool     query (const cwt::String & sql) { return _dbhData->driver->query(*_dbhData, sql); }   // cannot be used for statements that contain binary data

	cwt::shared_ptr<Statement>
		     prepare (const cwt::String & sql);

	ulong_t  rows ()       { return _dbhData->driver->rows(*_dbhData); }
	ulong_t  lastId ()     { return _dbhData->driver->lastId(*_dbhData); }

	cwt::Vector<cwt::String>
		     tables ()    { return _dbhData->driver->tables(*_dbhData); }

	bool     tableExists   (const cwt::String & name) { return _dbhData->driver->tableExists(*_dbhData, name); }
	bool     setAutoCommit (bool on) { return _dbhData->driver->setAutoCommit(*_dbhData, on); }
	bool     autoCommit () { return _dbhData->driver->autoCommit(*_dbhData); }
	bool     begin ()      { return _dbhData->driver->begin(*_dbhData); }
	bool     commit ()     { return _dbhData->driver->commit(*_dbhData); }
	bool     rollback ()   { return _dbhData->driver->rollback(*_dbhData); }
	long_t   errno ()      { return _dbhData->driver->errno(*_dbhData); }

	bool     meta (const cwt::String & table, cwt::Vector<DbColumnMeta> & meta)
			               { return _dbhData->driver->meta(*_dbhData, table, meta); }

private:
	DbHandlerData * _dbhData;
};

} // namespace debby

CWT_NS_END

#endif /* __CWT_DEBBY_DBH_HPP__ */
