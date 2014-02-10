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

struct database_data
{
	driver * _driver;
	database_data () : _driver(nullptr) {}
};

struct statement_data
{
	driver *    _driver;
	size_t      _bindCursor; // current bind index
	statement_data () : _driver(nullptr), _bindCursor(0) {}
	statement_data (const statement_data & other)
		: _driver(other._driver)
		, _bindCursor(other._bindCursor)
	{}
	//~statement_data ();
};

enum column_type {
	  Null
	, Bool
	, Boolean = Bool
	, Integer
	, Double
	, Float   = Double
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
	cwt::debby::column_type        column_type;
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
	database_data *           (* open)          (const pfs::string & path
			, const pfs::string & username
			, const pfs::string & password
			, const pfs::map<pfs::string, pfs::string> & params
			, pfs::string & errstr);
	void                     (* close)         (database_data *);

	bool                     (* query)         (database_data &, const pfs::string & sql, pfs::string & errstr);   // cannot be used for statements that contain binary data
	statement_data *         (* prepare)       (database_data &, const pfs::string & sql, pfs::string & errstr);
	ulong_t                  (* rows)          (database_data &);
	ulong_t 				 (* lastId)        (database_data &);
	pfs::vector<pfs::string> (* tables)        (database_data &);
	bool                     (* tableExists)   (database_data &, const pfs::string & name);
	bool                     (* setAutoCommit) (database_data &, bool);
	bool                     (* autoCommit)    (database_data &);
	bool                     (* begin)         (database_data &, pfs::string & errstr);
	bool                     (* commit)        (database_data &, pfs::string & errstr);
	bool                     (* rollback)      (database_data &, pfs::string & errstr);

	long_t                   (* errno)         (database_data &);

	bool                     (* meta)          (database_data &, const pfs::string & table
			, pfs::vector<column_meta> & meta
			, pfs::string & errstr);

// Statement routines
	void					 (* closeStmt)     (statement_data *);
	bool					 (* execStmt)      (statement_data &, pfs::string & errstr);
	bool                     (* fetchRowArray) (statement_data &, pfs::vector<pfs::unitype> & row);
	bool                     (* fetchRowHash)  (statement_data &, pfs::map<pfs::string, pfs::unitype> & row);
	bool                     (* bind)          (statement_data &, size_t index, const pfs::unitype & param);
};

}} // cwt::debby

#endif /* __CWT_DEBBY_DBD_HPP__ */
