/*
 * dbd.hpp
 *
 *  Created on: Aug 19, 2013
 *      Author: wladt
 */

#ifndef __PFS_DEBBY_DBD_HPP__
#define __PFS_DEBBY_DBD_HPP__

#include <pfs/vector.hpp>
#include <pfs/map.hpp>
#include <pfs/string.hpp>
#include <pfs/byte_string.hpp>
#include <pfs/variant.hpp>
#include <pfs/time.hpp>
#include <pfs/date.hpp>
#include <pfs/datetime.hpp>
#include <utility>

namespace pfs { namespace debby {

class schema;
struct driver;

typedef pfs::variant<bool
		, integral_t
		, real_t
		, pfs::string
		, pfs::byte_string
		, pfs::time
		, pfs::date
		, pfs::datetime> variant_type;

struct database_data
{
	driver * _driver;
	database_data () : _driver(nullptr) {}
};

struct statement_data
{
	driver * _driver;
	size_t   _bindCursor; // current bind index

	statement_data () : _driver(nullptr), _bindCursor(0)
	{}

	statement_data (const statement_data & other)
		: _driver(other._driver)
		, _bindCursor(other._bindCursor)
	{}
};

enum column_type
{
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
		, has_default_value (false, variant_type())
		, has_size          (false, 0)
		, has_decimals      (false, 0)
		, has_unsigned      (false, false)
		, has_timestamp     (false, false)
		, has_index         (false, false)
	{}

	string             column_name;
	debby::column_type column_type;
	string             native_type;
	std::pair<bool, bool>   has_pk;
	std::pair<bool, unsigned int> has_autoinc;  // > 0 if column is autoincremented
	std::pair<bool, bool>   has_not_null; // has not_null value, value set in 'not_null' property
	std::pair<bool, bool>   has_unique;
	std::pair<bool, variant_type> has_default_value;
	std::pair<bool, size_t> has_size;
	std::pair<bool, size_t> has_decimals;
	std::pair<bool, bool>   has_unsigned;
	std::pair<bool, bool>   has_timestamp;
	std::pair<bool, bool>   has_index;
};

struct driver
{
	virtual ~driver () {}

	virtual database_data * open  (const pfs::string & path
			, const string & username
			, const string & password
			, const map<string, string> & params
			, string & errstr) = 0;
	virtual void close (database_data *) = 0;

	virtual bool             query         (database_data &, const string & sql, string & errstr) = 0;   // cannot be used for statements that contain binary data
	virtual statement_data * prepare       (database_data &, const string & sql, string & errstr) = 0;
	virtual vector<string>   tables        (database_data &) = 0;
	virtual bool             tableExists   (database_data &, const string & name) = 0;
	virtual bool             setAutoCommit (database_data &, bool) = 0;
	virtual bool             autoCommit    (database_data &) = 0;
	virtual bool             begin         (database_data &, string & errstr) = 0;
	virtual bool             commit        (database_data &, string & errstr) = 0;
	virtual bool             rollback      (database_data &, string & errstr) = 0;

	virtual integral_t       errorCode    (database_data &) = 0;

	virtual bool             meta          (database_data &, const string & table
											   , vector<column_meta> & meta
											   , string & errstr) = 0;

// Statement routines
	virtual void		closeStmt     (statement_data *) = 0;
	virtual bool		execStmt      (statement_data &, string & errstr) = 0;
	virtual uintegral_t rows          (statement_data &) = 0;
	virtual integral_t 	lastId        (statement_data &) = 0;
	virtual bool        fetchRowArray (statement_data &, vector<variant_type> & row) = 0;
	virtual bool        fetchRowHash  (statement_data &, map<string, variant_type> & row) = 0;
	virtual bool        bind          (statement_data &, size_t index, const variant_type & param) = 0;
	virtual size_t      columnCount   (statement_data &) = 0;
	virtual pfs::string columnName    (statement_data &, size_t index) = 0;
	virtual column_type columnType    (statement_data &, size_t index) = 0;
};

}} // pfs::debby

#endif /* __PFS_DEBBY_DBD_HPP__ */
