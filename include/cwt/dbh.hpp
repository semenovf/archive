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

class DbHandler
{
	shared_ptr<DbHandler>   connect       (const String & dsn, const String & username, const String & password);
	void                    disconnect    ();
	bool                    func          (const String &, String *);
	void                    attr          (const String &, void *);
	bool                    setAutoCommit (bool);
	bool                    autoCommit    ( );
	long_t                  errno         ();
	String                  strerror      ();
	String                  state         ();
	bool                    query         (const String & sql);   /* cannot be used for statements that contain binary data */
	shared_ptr<DbStatement> prepare       (const String & sql);
	ulong_t                 rows          ();
	Vector<String>          tables        ();
	bool                    tableExists   (const String & tname);

	char *                  encode_n      (const String & s, size_t n);
	String                  decode_n      (const char * s, size_t n);
	char *                  encode        (const String & s);
	String                  decode        (const char * s);

	bool                    begin         (); /* begin transaction */
	bool                    commit        ();
	bool                    rollback      ();

	static bool loadDriver (const String & name);
private:
	DbDriver      * m_driver;
	DbHandlerData * m_dbh;
};

CWT_NS_END

#endif /* __CWT_DBH_HPP__ */
