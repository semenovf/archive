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

class DbHandler
{
	typedef DbDriver * (*driver_ctor) ();

protected:
	DbHandler() : m_dbh(nullptr) {}

public:
	~DbHandler() { close(); }

	static DbHandler *      open          (const String & uri);
	void                    close         ();

	bool                    query         (const String & sql) { return m_dbh->driver->query(*m_dbh, sql); }   // cannot be used for statements that contain binary data
	DbStatement *           prepare       (const String & sql);
	ulong_t                 rows          () { return m_dbh->driver->rows(*m_dbh); }
	bool                    setAutoCommit (bool on) { return m_dbh->driver->setAutoCommit(*m_dbh, on); }
	bool                    autoCommit    () { return m_dbh->driver->autoCommit(*m_dbh); }
	long_t                  errno         () { return m_dbh->driver->errno(*m_dbh); }
	String                  strerror      () { return m_dbh->driver->strerror(*m_dbh); }

/*
	bool                    func          (const String &, String *);
	void                    attr          (const String &, void *);
	Vector<String>          tables        ();
	bool                    tableExists   (const String & tname);
*/
	bool                    begin         () { return m_dbh->driver->begin(*m_dbh); }
	bool                    commit        () { return m_dbh->driver->commit(*m_dbh); }
	bool                    rollback      () { return m_dbh->driver->rollback(*m_dbh); }

private:
	DbHandlerData * m_dbh;
};

CWT_NS_END

#endif /* __CWT_DBH_HPP__ */
