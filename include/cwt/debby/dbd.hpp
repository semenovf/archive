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
	DbColumnMeta()
		: column_type(cwt::UniType::NullValue)
		, has_pk            (false, false)
		, has_autoinc       (false, 0)
		, has_not_null      (false, true)
        , has_unique        (false, false)
		, has_default_value (false, cwt::UniType())
		, has_size          (false, 0)
		, has_decimals      (false, 0)
		, has_unsigned      (false, false)
		, has_timestamp     (false, false)
	{}
	cwt::String             column_name;
	cwt::UniType::TypeEnum  column_type;
	cwt::String             native_type;
	std::pair<bool, bool>   has_pk;
	std::pair<bool, uint_t> has_autoinc; // > 0 if column is autoincremented
	std::pair<bool, bool>   has_not_null; // has not_null value, value set in 'not_null' property
	std::pair<bool, bool>   has_unique;
	std::pair<bool, cwt::UniType> has_default_value;
	std::pair<bool, size_t> has_size;
	std::pair<bool, size_t> has_decimals;
	std::pair<bool, bool>   has_unsigned;
	std::pair<bool, bool>   has_timestamp;
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
