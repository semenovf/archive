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
#include <pfs/errorable.hpp>
#include <pfs/pluggable.hpp>
#include <pfs/string.hpp>
#include <pfs/byte_string.hpp>
#include <pfs/time.hpp>
#include <pfs/date.hpp>
#include <pfs/datetime.hpp>
#include <pfs/variant.hpp>

namespace pfs { namespace debby {

typedef pfs::variant<bool
		, integral_t
		, real_t
		, pfs::string
		, pfs::byte_string
		, pfs::time
		, pfs::date
		, pfs::datetime> variant_type;

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

struct database_impl : public pfs::pluggable
{
	virtual ~database_impl () {}

	virtual database_impl * open  (const pfs::string & path
			, const string & username
			, const string & password
			, const map<string, string> & params
			, errorable * ex) = 0;

	virtual void close () = 0;

	virtual bool query (const string & sql) = 0;
	virtual statement_impl * prepare (const string & sql) = 0;
	virtual stringlist tables () = 0;
	virtual bool tableExists (const string & name) = 0;
	virtual bool setAutoCommit (bool) = 0;
	virtual bool autoCommit () = 0;
	virtual bool begin () = 0;
	virtual bool commit () = 0;
	virtual bool rollback () = 0;
	virtual integral_t errorCode () const = 0;
	virtual bool meta (const string & table, vector<column_meta> & meta) = 0;
};

struct statement_impl
{
	virtual ~statement_impl () {}

	virtual void close () = 0;
	virtual bool exec () = 0;
	virtual uintegral_t rows () const = 0;
	virtual integral_t lastId () const = 0;
	virtual vector<variant_type> fetchRowArray () = 0;
	virtual map<string, variant_type> fetchRowHash () = 0;
	virtual bool bind (size_t index, const variant_type & param) = 0;
	virtual size_t columnCount () = 0;
	virtual string columnName (size_t index) const = 0;
	virtual column_type columnType (size_t index) const = 0;
};

}} // pfs::debby

#endif /* __PFS_DEBBY_DBD_HPP__ */
