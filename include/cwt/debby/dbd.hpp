/*
 * dbd.hpp
 *
 *  Created on: Aug 19, 2013
 *      Author: wladt
 */

#ifndef __CWT_DEBBY_DBD_HPP__
#define __CWT_DEBBY_DBD_HPP__

#include <pfs/vector.hpp>
#include <pfs/map.hpp>
#include <pfs/unitype.hpp>

namespace cwt {

namespace debby
{

class schema;
struct driver;

struct handler_data   { driver * driver; };
struct statement_data { driver * driver; };

enum type {
	  Null
	, Bool
	, Boolean = Bool
	, Byte
	, Short
	, Int
	, Integer = Int
	, Long
	, Float
	, Double
	, String
	, Text    = String
	, Blob
	, Time
	, Date
	, DateTime

	, Mask     = 0xF000
	, Unsigned = 0x1000
};

struct column_meta
{
	column_meta ()
		: column_type       (Null)
		, has_pk            (false, false)
		, has_autoinc       (false, 0)
		, has_not_null      (false, true)
        , has_unique        (false, false)
		, has_default_value (false, pfs::unitype())
		, has_size          (false, 0)
		, has_decimals      (false, 0)
		, has_unsigned      (false, false)
		, has_timestamp     (false, false)
		, has_index         (false, false)
	{}
	pfs::string             column_name;
	cwt::debby::type        column_type;
	pfs::string             native_type;
	std::pair<bool, bool>   has_pk;
	std::pair<bool, uint_t> has_autoinc;  // > 0 if column is autoincremented
	std::pair<bool, bool>   has_not_null; // has not_null value, value set in 'not_null' property
	std::pair<bool, bool>   has_unique;
	std::pair<bool, pfs::unitype> has_default_value;
	std::pair<bool, size_t> has_size;
	std::pair<bool, size_t> has_decimals;
	std::pair<bool, bool>   has_unsigned;
	std::pair<bool, bool>   has_timestamp;
	std::pair<bool, bool>   has_index;
};

struct driver
{
	handler_data *          (*open)          (const pfs::string & path
			, const pfs::string & username
			, const pfs::string & password
			, const pfs::map<pfs::string, pfs::string> & params);
	void                     (*close)         (handler_data *);

	bool                     (*query)         (handler_data &, const pfs::string & sql);   // cannot be used for statements that contain binary data
	statement_data *         (*prepare)       (handler_data &, const pfs::string & sql);
	ulong_t                  (*rows)          (handler_data &);
	ulong_t 				 (*lastId)        (handler_data &);
	pfs::vector<pfs::string> (*tables)        (handler_data &);
	bool                     (*tableExists)   (handler_data &, const pfs::string & name);
	bool                     (*setAutoCommit) (handler_data &, bool);
	bool                     (*autoCommit)    (handler_data &);
	bool                     (*begin)         (handler_data &);
	bool                     (*commit)        (handler_data &);
	bool                     (*rollback)      (handler_data &);

	long_t                   (*errno)         (handler_data &);

	bool                     (*meta)          (handler_data &, const pfs::string & table, pfs::vector<column_meta> & meta);

// Statement routines
	void					 (*closeStmt)     (statement_data *);
	bool					 (*execStmt)      (statement_data &);
	bool                     (*fetchRowArray) (statement_data &, pfs::vector<pfs::unitype> & row);
	bool                     (*fetchRowHash)  (statement_data &, pfs::map<pfs::string, pfs::unitype> & row);
	bool                     (*bind)          (statement_data &, size_t index, const pfs::unitype & param);

//	bool                     (*createSchema)  (handler_data &, const Schema & schema); // create schema
//	bool                     (*dropSchema)    (handler_data &, const Schema & schema); // drop schema and close connection
};

}} // cwt::debby

#endif /* __CWT_DEBBY_DBD_HPP__ */
