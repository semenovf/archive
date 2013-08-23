/*
 * dbh.hpp
 *
 *  Created on: Aug 19, 2013
 *      Author: wladt
 */

#ifndef __CWT_DBH_HPP__
#define __CWT_DBH_HPP__

#include <cwt/dbd.hpp>

CWT_NS_BEGIN

class DbStatement;

class DbHandler : public Errorable
{
	typedef DbDriver * (*driver_ctor) ();

protected:
	DbHandler() : Errorable(), m_dbh(nullptr) {}

public:
	~DbHandler() { close(); }

	static DbHandler *      open (const String & uri);
	void                    close ();

	bool                    query (const String & sql) { return m_dbh->driver->query(*m_dbh, sql); }   // cannot be used for statements that contain binary data
	DbStatement *           prepare (const String & sql);
	ulong_t                 rows ()       { return m_dbh->driver->rows(*m_dbh); }
	ulong_t                 lastId ()     { return m_dbh->driver->lastId(*m_dbh); }
	Vector<String>          tables ()     { return m_dbh->driver->tables(*m_dbh); }
	bool                    tableExists   (const String & name) { return m_dbh->driver->tableExists(*m_dbh, name); }
	bool                    setAutoCommit (bool on) { return m_dbh->driver->setAutoCommit(*m_dbh, on); }
	bool                    autoCommit () { return m_dbh->driver->autoCommit(*m_dbh); }
	bool                    begin ()      { return m_dbh->driver->begin(*m_dbh); }
	bool                    commit ()     { return m_dbh->driver->commit(*m_dbh); }
	bool                    rollback ()   { return m_dbh->driver->rollback(*m_dbh); }
	long_t                  errno ()      { return m_dbh->driver->errno(*m_dbh); }

	bool                    meta (const String & table, Vector<DbColumnMeta> & meta)
										  { return m_dbh->driver->meta(*m_dbh, table, meta); }

private:
	DbHandlerData * m_dbh;
};

CWT_NS_END

#endif /* __CWT_DBH_HPP__ */
