/*
 * dbd.hpp
 *
 *  Created on: Aug 19, 2013
 *      Author: wladt
 */

#ifndef __CWT_DBD_HPP__
#define __CWT_DBD_HPP__

#include <cwt/vector.hpp>
#include <cwt/errorable.hpp>
#include <cwt/hash.hpp>
#include <cwt/unitype.hpp>

CWT_NS_BEGIN

class DbDriver;

struct DbHandlerData   { DbDriver * driver; };
struct DbStatementData { DbDriver * driver; };

class DbDriver
{
public:
	DbHandlerData *         (*open)          (const String & path
			, const String & username
			, const String & password
			, const Hash<String, String> & params);
	void                    (*close)         (DbHandlerData *);

	bool                    (*query)         (DbHandlerData &, const String & sql);   // cannot be used for statements that contain binary data
	DbStatementData *       (*prepare)       (DbHandlerData &, const String & sql);
	ulong_t                 (*rows)          (DbHandlerData &);
	bool                    (*setAutoCommit) (DbHandlerData &, bool);
	bool                    (*autoCommit)    (DbHandlerData &);
	long_t                  (*errno)         (DbHandlerData &);
	String                  (*strerror)      (DbHandlerData &);

/*
	bool                    (*func)          (DbHandlerData & , const String &, String *);
	void                    (*attr)          (DbHandlerData & , const String &, void *);
	Vector<String>          (*tables)        (DbHandlerData &);
	bool                    (*tableExists)   (DbHandlerData &, const String & tname);
*/

	bool                    (*begin)         (DbHandlerData &); // begin transaction
	bool                    (*commit)        (DbHandlerData &);
	bool                    (*rollback)      (DbHandlerData &);

// Statement routines
	void					(*closeStmt)     (DbStatementData *);
	bool					(*execStmt)      (DbStatementData &);
	Vector<UniType>         (*fetchRowArray) (DbStatementData &);
	Hash<String, UniType>   (*fetchRowHash)  (DbStatementData &);
};

CWT_NS_END

#endif /* __CWT_DBD_HPP__ */
