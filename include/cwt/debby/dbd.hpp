/*
 * dbd.hpp
 *
 *  Created on: Aug 19, 2013
 *      Author: wladt
 */

#ifndef __CWT_DEBBY_DBD_HPP__
#define __CWT_DEBBY_DBD_HPP__

#include <cwt/vector.hpp>
#include <cwt/map.hpp>
#include <cwt/unitype.hpp>

CWT_NS_BEGIN

class DbDriver;

struct DbHandlerData   { DbDriver * driver; };
struct DbStatementData { DbDriver * driver; };
struct DbColumnMeta
{
	String            column_name;
	UniType::TypeEnum column_type;
	String            native_type;
};

class DbDriver
{
public:
	DbHandlerData *         (*open)          (const String & path
			, const String & username
			, const String & password
			, const Map<String, String> & params);
	void                    (*close)         (DbHandlerData *);

	bool                    (*query)         (DbHandlerData &, const String & sql);   // cannot be used for statements that contain binary data
	DbStatementData *       (*prepare)       (DbHandlerData &, const String & sql);
	ulong_t                 (*rows)          (DbHandlerData &);
	ulong_t 				(*lastId)        (DbHandlerData &);
	Vector<String>          (*tables)        (DbHandlerData &);
	bool                    (*tableExists)   (DbHandlerData &, const String & name);
	bool                    (*setAutoCommit) (DbHandlerData &, bool);
	bool                    (*autoCommit)    (DbHandlerData &);
	bool                    (*begin)         (DbHandlerData &);
	bool                    (*commit)        (DbHandlerData &);
	bool                    (*rollback)      (DbHandlerData &);

	long_t                  (*errno)         (DbHandlerData &);

	bool                    (*meta)          (DbHandlerData &, const String & table, Vector<DbColumnMeta> & meta);

// Statement routines
	void					(*closeStmt)     (DbStatementData *);
	bool					(*execStmt)      (DbStatementData &);
	bool                    (*fetchRowArray) (DbStatementData &, Vector<UniType> & row);
	bool                    (*fetchRowHash)  (DbStatementData &, Map<String, UniType> & row);
	bool                    (*bind)          (DbStatementData &, size_t index, const UniType & param);

	bool                    (*dropScheme)    (DbHandlerData *); // drop scheme and close connection
};

CWT_NS_END

#endif /* __CWT_DEBBY_DBD_HPP__ */
