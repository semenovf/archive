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

namespace debby
{

class Schema;
struct DbDriver;

struct DbHandlerData   { DbDriver * driver; };
struct DbStatementData { DbDriver * driver; };
struct DbColumnMeta
{
	cwt::String            column_name;
	cwt::UniType::TypeEnum column_type;
	cwt::String            native_type;
};

struct DbDriver
{
	DbHandlerData *          (*open)          (const cwt::String & path
			, const cwt::String & username
			, const cwt::String & password
			, const cwt::Map<cwt::String, cwt::String> & params);
	void                     (*close)         (DbHandlerData *);

	bool                     (*query)         (DbHandlerData &, const cwt::String & sql);   // cannot be used for statements that contain binary data
	DbStatementData *        (*prepare)       (DbHandlerData &, const cwt::String & sql);
	ulong_t                  (*rows)          (DbHandlerData &);
	ulong_t 				 (*lastId)        (DbHandlerData &);
	cwt::Vector<cwt::String> (*tables)        (DbHandlerData &);
	bool                     (*tableExists)   (DbHandlerData &, const cwt::String & name);
	bool                     (*setAutoCommit) (DbHandlerData &, bool);
	bool                     (*autoCommit)    (DbHandlerData &);
	bool                     (*begin)         (DbHandlerData &);
	bool                     (*commit)        (DbHandlerData &);
	bool                     (*rollback)      (DbHandlerData &);

	long_t                   (*errno)         (DbHandlerData &);

	bool                     (*meta)          (DbHandlerData &, const cwt::String & table, cwt::Vector<DbColumnMeta> & meta);

// Statement routines
	void					 (*closeStmt)     (DbStatementData *);
	bool					 (*execStmt)      (DbStatementData &);
	bool                     (*fetchRowArray) (DbStatementData &, cwt::Vector<cwt::UniType> & row);
	bool                     (*fetchRowHash)  (DbStatementData &, cwt::Map<cwt::String, cwt::UniType> & row);
	bool                     (*bind)          (DbStatementData &, size_t index, const cwt::UniType & param);

	bool                     (*createSchema)  (DbHandlerData &, const Schema & schema); // create schema
	bool                     (*dropSchema)    (DbHandlerData &, const Schema & schema); // drop schema and close connection
};

} // namespace debby

CWT_NS_END

#endif /* __CWT_DEBBY_DBD_HPP__ */
