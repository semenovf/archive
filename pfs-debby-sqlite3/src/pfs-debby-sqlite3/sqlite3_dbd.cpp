/**
 * @file   sqlite3_dbd.cpp
 * @author wladt
 * @date   Aug 19, 2013 14:45
 *
 * @brief
 */

#include <pfs/mutex.hpp>
#include <pfs/string.hpp>
#include <pfs/map.hpp>
#include <pfs/byte_string.hpp>
#include <pfs/vector.hpp>
#include "sqlite3_dbd.hpp"

static int __refs = 0;
static pfs::debby::driver * __dbd = 0;
static pfs::mutex __mutex;

class mappings
{
	pfs::map<pfs::string, pfs::debby::column_type> _columnTypes;
	pfs::map<int, pfs::debby::column_type>         _sqliteColumnTypes;

public:
	mappings ();

	pfs::debby::column_type columnType (const pfs::string & type) { return _columnTypes.value(type, cwt::debby::Null); }
	pfs::debby::column_type columnType (int sqlitetype) { return _sqliteColumnTypes.value(sqlitetype, cwt::debby::Null); }
};

static mappings __mappings;

const int MaxSqlTimeout      = 10000; /* 10 seconds */
const int MaxExecRetryCount = 10;
const int MaxExecRetrySleep = 200;

/* DBI API functions implementations */
pfs::debby::database_data *
			   s3_dbd_open  (const pfs::string & driver_uri
		, const pfs::string & username
		, const pfs::string & password
		, const pfs::map<pfs::string, pfs::string> & params
		, pfs::string & errstr);
static void    s3_dbd_close           (cwt::debby::database_data * dbh);
static bool    s3_dbd_set_auto_commit (cwt::debby::database_data & dbh, bool on);
static bool    s3_dbd_auto_commit     (cwt::debby::database_data & dbh);
static long_t  s3_dbd_errno           (cwt::debby::database_data & dbh);
static bool    s3_dbd_query           (cwt::debby::database_data & dbh, const pfs::string & sql, pfs::string & errstr);
static cwt::debby::statement_data *
		       s3_dbd_prepare         (cwt::debby::database_data & dbh, const pfs::string & sql, pfs::string & errstr);

static pfs::vector<pfs::string>
			   s3_dbd_tables          (cwt::debby::database_data & dbh);
static bool    s3_dbd_table_exists    (cwt::debby::database_data & dbh, const pfs::string & name);
static bool    s3_dbd_begin           (cwt::debby::database_data & dbh, pfs::string & errstr);
static bool    s3_dbd_commit          (cwt::debby::database_data & dbh, pfs::string & errstr);
static bool    s3_dbd_rollback        (cwt::debby::database_data & dbh, pfs::string & errstr);
static bool    s3_dbd_meta            (cwt::debby::database_data & dbh
		, const pfs::string & table
		, pfs::vector<cwt::debby::column_meta> & meta
		, pfs::string & errstr);

static void    s3_dbd_stmt_close (cwt::debby::statement_data * sth);
static bool    s3_dbd_stmt_exec  (cwt::debby::statement_data & sth, pfs::string & errstr);
static ulong_t s3_dbd_stmt_affected_rows   (cwt::debby::statement_data & sth);
static long_t  s3_dbd_stmt_last_id         (cwt::debby::statement_data & sth);
static bool    s3_dbd_stmt_fetch_row_array (cwt::debby::statement_data & sth, pfs::vector<pfs::unitype> & row);
static bool    s3_dbd_stmt_fetch_row_hash (cwt::debby::statement_data & sth, pfs::map<pfs::string, pfs::unitype> & row);
static bool    s3_dbd_stmt_bind (cwt::debby::statement_data & sth, size_t index, const pfs::unitype & param);
static size_t  s3_dbd_stmt_column_count (cwt::debby::statement_data & sth);
static pfs::string s3_dbd_stmt_column_name (cwt::debby::statement_data &, size_t index);
static cwt::debby::column_type s3_dbd_stmt_column_type (cwt::debby::statement_data &, size_t index);


mappings::mappings ()
{
	_columnTypes.insert(_l1("BOOL")      , pfs::debby::Bool);
	_columnTypes.insert(_l1("BOOLEAN")   , pfs::debby::Bool);

	_columnTypes.insert(_l1("INT")       , pfs::debby::Integer);
	_columnTypes.insert(_l1("INTEGER")   , cwt::debby::Integer);
	_columnTypes.insert(_l1("TINYINT")   , cwt::debby::Integer);
	_columnTypes.insert(_l1("SMALLINT")  , cwt::debby::Integer);
	_columnTypes.insert(_l1("MEDIUMINT") , cwt::debby::Integer);
	_columnTypes.insert(_l1("BIGINT")    , cwt::debby::Integer);
	_columnTypes.insert(_l1("UNSIGNED")  , cwt::debby::Integer);

	_columnTypes.insert(_l1("DATE")      , cwt::debby::Date);
	_columnTypes.insert(_l1("DATETIME")  , cwt::debby::DateTime);
	_columnTypes.insert(_l1("TIME")      , cwt::debby::Time);

	_columnTypes.insert(_l1("REAL")      , cwt::debby::Double);
	_columnTypes.insert(_l1("DOUBLE")    , cwt::debby::Double);
	_columnTypes.insert(_l1("FLOAT")     , cwt::debby::Double);
	_columnTypes.insert(_l1("NUMERIC")   , cwt::debby::Double);
	_columnTypes.insert(_l1("DECIMAL")   , cwt::debby::Double);

	_columnTypes.insert(_l1("CHAR")      , cwt::debby::String);
	_columnTypes.insert(_l1("VARCHAR")   , cwt::debby::String);
	_columnTypes.insert(_l1("VARYING")   , cwt::debby::String);
	_columnTypes.insert(_l1("NCHAR")     , cwt::debby::String);
	_columnTypes.insert(_l1("NATIVE")    , cwt::debby::String);
	_columnTypes.insert(_l1("NVARCHAR")  , cwt::debby::String);
	_columnTypes.insert(_l1("TEXT")      , cwt::debby::String);
	_columnTypes.insert(_l1("CLOB")      , cwt::debby::String);

	_columnTypes.insert(_l1("BLOB")      , cwt::debby::Blob);
	_columnTypes.insert(_l1("BINARY")    , cwt::debby::Blob);


	_sqliteColumnTypes.insert(SQLITE_INTEGER, cwt::debby::Integer);
	_sqliteColumnTypes.insert(SQLITE_FLOAT  , cwt::debby::Double);
	_sqliteColumnTypes.insert(SQLITE_TEXT   , cwt::debby::String);
	_sqliteColumnTypes.insert(SQLITE_BLOB   , cwt::debby::Blob);
	_sqliteColumnTypes.insert(SQLITE_NULL   , cwt::debby::Null);
}

inline pfs::string __s3_stmt_errmsg (Sqlite3DbStatement * s3_sth)
{
	sqlite3 * dbh_native = sqlite3_db_handle(s3_sth->_native_handler);
	return pfs::string(sqlite3_errmsg(dbh_native));
}

//extern "C" bool __cwt_plugin_ctor__ (void * pluggable)
//{
//	PFS_ASSERT(pluggable);
//	cwt::debby::driver ** pdbd = static_cast<cwt::debby::driver **>(pluggable);
//
//	pfs::auto_lock<> lock(& __mutex);
//	if (!__dbd) {
//		__dbd = new cwt::debby::driver;
//
//		__dbd->open          = s3_dbd_open;
//		__dbd->close         = s3_dbd_close;
//		__dbd->autoCommit    = s3_dbd_auto_commit;
//		__dbd->setAutoCommit = s3_dbd_set_auto_commit;
//		__dbd->errno         = s3_dbd_errno;
//		__dbd->tables        = s3_dbd_tables;
//		__dbd->tableExists   = s3_dbd_table_exists;
//		__dbd->query         = s3_dbd_query;
//		__dbd->prepare       = s3_dbd_prepare;
//		__dbd->begin         = s3_dbd_begin;
//		__dbd->commit        = s3_dbd_commit;
//		__dbd->rollback      = s3_dbd_rollback;
//		__dbd->meta          = s3_dbd_meta;
//		__dbd->closeStmt     = s3_dbd_stmt_close;
//		__dbd->execStmt      = s3_dbd_stmt_exec;
//		__dbd->rows          = s3_dbd_stmt_affected_rows;
//		__dbd->lastId        = s3_dbd_stmt_last_id;
//		__dbd->fetchRowArray = s3_dbd_stmt_fetch_row_array;
//		__dbd->fetchRowHash  = s3_dbd_stmt_fetch_row_hash;
//		__dbd->bind          = s3_dbd_stmt_bind;
//		__dbd->columnCount   = s3_dbd_stmt_column_count;
//		__dbd->columnName    = s3_dbd_stmt_column_name;
//		__dbd->columnType    = s3_dbd_stmt_column_type;
//	}
//
//	*pdbd = __dbd;
//
//	return __dbd != nullptr;
//}
//
//extern "C" bool __cwt_plugin_dtor__(void * pluggable)
//{
//	PFS_UNUSED(pluggable);
//	return true;
//}

/**
 *
 * @brief Connects to sqlite3 databases.
 *
 * @param driver_dsn Details see SQLite docs on sqlite3_open_v2 function.
 *				Examples:
 *              	file:data.db
 *                 		Open the file "data.db" in the current directory.
 *
 *                  sqlite3:/home/fred/data.db
 *                  sqlite3:///home/fred/data.db
 *                  sqlite3://localhost/home/fred/data.db
 *                  	Open the database file "/home/fred/data.db".
 *
 *                  sqlite3:///C:/Documents%20and%20Settings/fred/Desktop/data.db
 *                  	Windows only: Open the file "data.db" on fred's desktop on drive C:.
 *                  	Note that the %20 escaping in this example is not
 *                  	strictly necessary - space characters can be used literally
 *                  	in URI filenames.
 *
 *                  sqlite3:data.db?mode=ro&cache=private
 *                  	Open file "data.db" in the current directory for read-only access.
 *                  	Regardless of whether or not shared-cache mode is enabled
 *                  	by default, use a private cache.
 *
 *                  sqlite3:/home/fred/data.db?vfs=unix-nolock
 *                  	Open file "/home/fred/data.db". Use the special VFS "unix-nolock".
 *
 *                  sqlite3:data.db?mode=readonly
 *                  	An error. "readonly" is not a valid option for the "mode" parameter.
 *
 *				Mode values:
 *					mode=ro (default)
 *				    mode=rw
 *                  mode=rwc
 *                  mode=memory
 *
 *              Cache values:
 *              	cache=shared
 *              	cache=private
 *
 * @param username   Unused.
 * @param password   Unused.
 * @return DBI connection to specified sqlite3 databases.
 *
 * @note  Autocommit mode is on by default.
 */
cwt::debby::database_data * s3_dbd_open(const pfs::string & path
		, const pfs::string & username
		, const pfs::string & password
		, const pfs::map<pfs::string, pfs::string> & params
		, pfs::string & errstr)
{
	Sqlite3DbHandler * dbh = nullptr;
	sqlite3 *  dbh_native = nullptr;
	int        rc = SQLITE_OK;
	int        s3_flags;
	int        s3_flag_mode;

	PFS_UNUSED2(username, password);
	errstr.clear();

	if (path.isEmpty()) {
		errstr = _u8(_Tr("Path to database does not specified"));
		return nullptr;
	}

    s3_flags = SQLITE_OPEN_URI;
	s3_flag_mode = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;

	pfs::map<pfs::string, pfs::string>::const_iterator mode = params.find(pfs::string("mode"));

	if (mode != params.cend()) {
		if (mode->second == "ro")
			s3_flag_mode = SQLITE_OPEN_READONLY;
		else if (mode->second == "rw")
			s3_flag_mode = SQLITE_OPEN_READWRITE;
		else if (mode->second == "rwc")
			s3_flag_mode = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
		else if (mode->second == "memory")
			s3_flag_mode = SQLITE_OPEN_MEMORY;
	}

	s3_flags |= s3_flag_mode;

    rc = sqlite3_open_v2(path.c_str(), & dbh_native, s3_flags, NULL);

	if (rc != SQLITE_OK) {
		if (!dbh_native) {
			errstr = _u8(_Tr("Unable to allocate memory for database handler."));
			errstr << _Tr("Internal error code: ") << pfs::string::number(rc);
		} else {
			switch( rc ) {
				case SQLITE_CANTOPEN:
					errstr << _Tr("Unable to open the database file. Try to check path ") << path;
					break;
				default: break;
			}
			sqlite3_close(dbh_native);
			dbh_native = nullptr;
		}
	} else {
		PFS_ASSERT(__dbd);
		dbh = new Sqlite3DbHandler;

		// TODO what for this call ?
		sqlite3_busy_timeout(dbh_native, MaxSqlTimeout);

		// Enable extended result codes
		sqlite3_extended_result_codes(dbh_native, 1);

		dbh->_driver = __dbd;
		dbh->_dbh_native = dbh_native;
	}

	if (dbh) {
		pfs::auto_lock<> lock(& __mutex);
		++__refs;
	}

    return static_cast<pfs::debby::database_data *>(dbh);
}


static void s3_dbd_close (cwt::debby::database_data * dbh)
{
	Sqlite3DbHandler * s3_dbh = static_cast<Sqlite3DbHandler*>(dbh);

	if (!dbh )
		return;

/*
	while ((stmt = sqlite3_next_stmt(s3_dbh->dbh_native, 0)) != 0)
		sqlite3_finalize(stmt);
*/

	sqlite3_close_v2(s3_dbh->_dbh_native);
	s3_dbh->_driver = nullptr;
	s3_dbh->_dbh_native = nullptr;
	delete s3_dbh;

	pfs::auto_lock<> lock(& __mutex);
	--__refs;

	PFS_ASSERT(__dbd);
	PFS_ASSERT(__refs >= 0);
	if (!__refs) {
		delete __dbd;
		__dbd = nullptr;
	}
}

bool s3_dbd_set_auto_commit (cwt::debby::database_data & , bool)
{
	return true;
}

bool s3_dbd_auto_commit (cwt::debby::database_data & dbh)
{
	Sqlite3DbHandler * s3_dbh = static_cast<Sqlite3DbHandler *>(& dbh);
	return 0 == sqlite3_get_autocommit(s3_dbh->_dbh_native)
			? false
			: true;
}

long_t s3_dbd_errno (cwt::debby::database_data & dbh)
{
	Sqlite3DbHandler * s3_dbh = static_cast<Sqlite3DbHandler *>(& dbh);
	int rc = sqlite3_errcode(s3_dbh->_dbh_native);
	if (rc == SQLITE_OK
			|| rc == SQLITE_DONE
			|| rc == SQLITE_ROW)
		rc = 0;
	return long_t(rc);
}

/*
pfs::string s3_dbd_strerror (cwt::debby::handler_data & dbh)
{
	Sqlite3DbHandler * s3_dbh = reinterpret_cast<Sqlite3DbHandler *>(& dbh);
	return pfs::string(sqlite3_errmsg(s3_dbh->dbh_native));
}
*/

bool s3_dbd_query (cwt::debby::database_data & dbh, const pfs::string & sql, pfs::string & errstr)
{
	Sqlite3DbHandler * s3_dbh = static_cast<Sqlite3DbHandler *>(& dbh);
	char * errmsg;
	errstr.clear();
	int rc = sqlite3_exec(s3_dbh->_dbh_native, sql.c_str(), NULL, NULL, & errmsg);

	if (SQLITE_OK != rc) {
		if (errmsg) {
			errstr = pfs::string(errmsg);
			sqlite3_free(errmsg);
		}
		return false;
	}

	return true;
}


cwt::debby::statement_data * s3_dbd_prepare (cwt::debby::database_data & dbh, const pfs::string & sql, pfs::string & errstr)
{
	Sqlite3DbHandler * s3_dbh = static_cast<Sqlite3DbHandler *>(& dbh);
	sqlite3_stmt * sth_native = nullptr;

	errstr.clear();

	int rc = sqlite3_prepare_v2(
			s3_dbh->_dbh_native
			, sql.c_str()      /* SQL statement, UTF-8 encoded */
			, -1               /* Maximum length of zSql in bytes. */
			, & sth_native     /* OUT: Statement handle */
			, NULL );          /* OUT: Pointer to unused portion of zSql */

	if (rc != SQLITE_OK) {
		errstr = pfs::string(sqlite3_errmsg(s3_dbh->_dbh_native));
		return nullptr;
	}

	Sqlite3DbStatement * sth = new Sqlite3DbStatement;
	PFS_ASSERT(__dbd);
	sth->_driver = __dbd;
	sth->_native_handler = sth_native;

//	int column_count = sqlite3_column_count(sth->sth_native);
//	if (column_count > 0) { /* SELECT statement */
//	}

	return static_cast<cwt::debby::statement_data*>(sth);
}

pfs::vector<pfs::string> s3_dbd_tables (cwt::debby::database_data & dbh)
{
	pfs::vector<pfs::string> r;
	pfs::string errstr;

	cwt::debby::statement_data * sth = s3_dbd_prepare(dbh
			, pfs::string("SELECT name FROM sqlite_master WHERE type='table' ORDER BY name")
			, errstr);

	if (sth) {
		if (s3_dbd_stmt_exec(*sth, errstr)) {
			pfs::vector<pfs::unitype> row;
			while (s3_dbd_stmt_fetch_row_array (*sth, row)) {
				pfs::string table = row[0]->toString();

				// ignore system tables
				if (table == "sqlite_sequence")
					continue;

				r.append(table);
				row.clear();
			}
		}
		s3_dbd_stmt_close(sth);
	}

	PFS_VERIFY_X(errstr.isEmpty(), errstr.c_str());

	return r;
}

bool s3_dbd_table_exists (cwt::debby::database_data & dbh, const pfs::string & name)
{
	bool r = false;
	pfs::string sql("SELECT COUNT(*) as table_count FROM sqlite_master WHERE type = 'table' AND name = ");
	sql << pfs::string(1,'\'') << name << pfs::string(1,'\'');
	pfs::string errstr;
	cwt::debby::statement_data * sth = s3_dbd_prepare(dbh, sql, errstr);

	if (sth) {
		if (s3_dbd_stmt_exec(*sth, errstr)) {
			pfs::vector<pfs::unitype> row;
			s3_dbd_stmt_fetch_row_array (*sth, row);
			PFS_ASSERT(row.size() == 1);
			r = (row[0]->toInteger() > 0);
		}
		s3_dbd_stmt_close(sth);
	}

	PFS_VERIFY_X(errstr.isEmpty(), errstr.c_str());

	return r;
}


bool s3_dbd_begin (cwt::debby::database_data & dbh, pfs::string & errstr)
{
	return s3_dbd_query(dbh, pfs::string("BEGIN"), errstr);
}

bool s3_dbd_commit (cwt::debby::database_data & dbh, pfs::string & errstr)
{
	return s3_dbd_query(dbh, pfs::string("COMMIT"), errstr);
}

bool s3_dbd_rollback (cwt::debby::database_data & dbh, pfs::string & errstr)
{
	return s3_dbd_query(dbh, pfs::string("ROLLBACK"), errstr);
}

// AUTOINCREMENT only applies to primary keys
//
bool s3_dbd_meta (cwt::debby::database_data & dbh
		, const pfs::string & table
		, pfs::vector<cwt::debby::column_meta> & meta
		, pfs::string & errstr)
{
	bool r = false;
	pfs::string sqlForMeta;
	pfs::string sqlForAutoinc;

	sqlForMeta << "PRAGMA table_info(" << table << ")";
	sqlForAutoinc << "SELECT 1 FROM sqlite_master WHERE tbl_name=\""
			<< table
			<< "\" AND sql LIKE \"%AUTOINCREMENT%\"";

	cwt::debby::statement_data * sthForMeta    = s3_dbd_prepare(dbh, sqlForMeta, errstr);
	cwt::debby::statement_data * sthForAutoinc = s3_dbd_prepare(dbh, sqlForAutoinc, errstr);

	if (sthForMeta && sthForAutoinc) {
		if (s3_dbd_stmt_exec(*sthForMeta, errstr) && s3_dbd_stmt_exec(*sthForAutoinc, errstr)) {

			pfs::vector<pfs::unitype> autoincRow;
			uint_t autoinc = 0;
			if (s3_dbd_stmt_fetch_row_array (*sthForAutoinc, autoincRow))
				autoinc = 1;

			pfs::map<pfs::string, pfs::unitype> row;
			while (s3_dbd_stmt_fetch_row_hash (*sthForMeta, row)) {
				cwt::debby::column_meta m;
				m.column_name       = row[_l1("name")].toString();
				m.native_type       = row[_l1("type")].toString();

				// ignore parentheses (length or scale/prec)
				pfs::string::const_iterator it = m.native_type.find("(", 0);
				if (it != m.native_type.cend()) {
					m.native_type = m.native_type.substr(m.native_type.cbegin(), it).trim();
				} else {
					m.native_type = m.native_type.trim();
				}
				m.column_type       = __mappings.columnType(m.native_type);

				m.has_pk.first      = true;
				m.has_pk.second     = row[_l1("pk")].toBoolean();

				m.has_autoinc.first  = true;
				m.has_autoinc.second = autoinc;

				m.has_not_null.first  = true;
				m.has_not_null.second =  row[_l1("notnull")].toBoolean();

				m.has_unique.first  = false;
				m.has_unique.second = false;

				m.has_default_value.first  = true;
				// dequote
				if (row[_l1("dflt_value")].type_id() == pfs::String) {
					pfs::string dequoted = row[_l1("dflt_value")].toString();
					if (dequoted.length() >= 2) {
						pfs::ucchar first = dequoted[0];
						pfs::ucchar last = dequoted[dequoted.length() - 1];
						if ((first == '"' && last == '"')
								|| (first == '\'' && last == '\'')
								|| (first == '`' && last == '`')) {

							dequoted = dequoted.substr(1, dequoted.length() - 2);
						}
					}
					m.has_default_value.second = dequoted;
				} else {
					m.has_default_value.second = row[_l1("dflt_value")];
				}


				meta.append(m);
				row.clear();
			}

			r = true;
		}
		s3_dbd_stmt_close(sthForMeta);
		s3_dbd_stmt_close(sthForAutoinc);
	}

	return r;
}


void s3_dbd_stmt_close (cwt::debby::statement_data * sth)
{
	if (sth) {
		Sqlite3DbStatement * s3_sth = static_cast<Sqlite3DbStatement*>(sth);
		sqlite3 * dbh_native = sqlite3_db_handle(s3_sth->_native_handler);

		int rc = sqlite3_finalize(s3_sth->_native_handler);

		if (rc != SQLITE_OK) {
			pfs::string errstr;
			errstr << _Tr("Failed to close statement: ") << pfs::string(sqlite3_errmsg(dbh_native));
			PFS_VERIFY_X(errstr.isEmpty(), errstr.c_str());
		}
		s3_sth->_native_handler = nullptr;
		s3_sth->_driver = nullptr;
		delete s3_sth;
	}
}

bool s3_dbd_stmt_exec (cwt::debby::statement_data & sth, pfs::string & errstr)
{
	Sqlite3DbStatement * s3_sth = static_cast<Sqlite3DbStatement*>(& sth);
	bool r = true;

	static pfs::mutex __local_mutex;
	pfs::auto_lock<> lock(& __local_mutex);

	s3_sth->reset();
	s3_sth->_columnCount = sqlite3_column_count(s3_sth->_native_handler);

	if (s3_sth->_columnCount > 0) { // SELECT statement
		int rc = sqlite3_reset(s3_sth->_native_handler);
		r = (rc == SQLITE_OK ? true : false);
	} else {
		int rc = sqlite3_step(s3_sth->_native_handler);
		r = (rc == SQLITE_OK || rc == SQLITE_DONE ? true : false);

		sqlite3 * dbh_native = sqlite3_db_handle(s3_sth->_native_handler);

		int nrows = sqlite3_changes(dbh_native);
		s3_sth->_nrows = nrows < 0 ? ulong_t(0) : ulong_t(nrows);
		s3_sth->_lastId = ulong_t(sqlite3_last_insert_rowid(dbh_native));

		if (rc == SQLITE_DONE) {
			sqlite3_reset(s3_sth->_native_handler);
			sqlite3_clear_bindings(s3_sth->_native_handler);
		}
	}

	if (!r) {
		errstr = __s3_stmt_errmsg(s3_sth);
	}

	return r;
}


ulong_t s3_dbd_stmt_affected_rows (cwt::debby::statement_data & sth)
{
	Sqlite3DbStatement * s3_sth = static_cast<Sqlite3DbStatement*>(& sth);
	return s3_sth->_nrows;
}

long_t s3_dbd_stmt_last_id (cwt::debby::statement_data & sth)
{
	Sqlite3DbStatement * s3_sth = static_cast<Sqlite3DbStatement*>(& sth);
	return s3_sth->_lastId;
}


static int __fetch_helper (Sqlite3DbStatement * s3_sth)
{
	int rc = SQLITE_OK;
	int nretries = MaxExecRetryCount;

	do {
		rc = sqlite3_step(s3_sth->_native_handler);

		if ( rc == SQLITE_BUSY ) {
			if (nretries++ > MaxExecRetryCount) {
				break;
			} else {
				sqlite3_sleep(MaxExecRetrySleep);
			}
		}
	} while (rc == SQLITE_BUSY);

	if (rc == SQLITE_DONE) {
		; // ok
	} else if (rc == SQLITE_ROW) {
		; // ok
	} else {
		PFS_VERIFY_X(false, __s3_stmt_errmsg(s3_sth).c_str());
	}

	return rc;
}

bool s3_dbd_stmt_fetch_row_array (cwt::debby::statement_data & sth, pfs::vector<pfs::unitype> & row)
{
	Sqlite3DbStatement * s3_sth = static_cast<Sqlite3DbStatement*>(& sth);
	int rc = __fetch_helper(s3_sth);

	if (rc == SQLITE_ROW) {
		int ncols = sqlite3_column_count(s3_sth->_native_handler);
		for (int i = 0; i < ncols; ++i) {
			//pfs::string column_name (sqlite3_column_name(s3_sth->sth_native, i));

			switch (sqlite3_column_type(s3_sth->_native_handler, i)) {
			case SQLITE_INTEGER:
				row.append(pfs::unitype(long_t(sqlite3_column_int64(s3_sth->_native_handler, i))));
				break;
			case SQLITE_FLOAT:
				row.append(pfs::unitype(sqlite3_column_double(s3_sth->_native_handler, i)));
				break;
			case SQLITE_TEXT: {
				const char * text = reinterpret_cast<const char*>(sqlite3_column_text(s3_sth->_native_handler, i));
				row.append(pfs::unitype(pfs::string(text)));
				}
				break;
			case SQLITE_BLOB: {
				const char * bytes = reinterpret_cast<const char*>(sqlite3_column_blob(s3_sth->_native_handler, i));
				int nbytes = sqlite3_column_bytes(s3_sth->_native_handler, i);
				PFS_ASSERT(nbytes >= 0);
				row.append(pfs::unitype(pfs::bytearray(bytes, size_t(nbytes))));
				}
				break;
			case SQLITE_NULL:
			default:
				row.append(pfs::unitype());
				break;
			}
		}
		return true;
	}

	return false;
}

bool s3_dbd_stmt_fetch_row_hash (cwt::debby::statement_data & sth, pfs::map<pfs::string, pfs::unitype> & row)
{
	Sqlite3DbStatement * s3_sth = static_cast<Sqlite3DbStatement*>(& sth);
	int rc = __fetch_helper(s3_sth);

	if (rc == SQLITE_ROW) {
		int ncols = sqlite3_column_count(s3_sth->_native_handler);
		for (int i = 0; i < ncols; ++i) {
			pfs::string column_name (sqlite3_column_name(s3_sth->_native_handler, i));

			switch (sqlite3_column_type(s3_sth->_native_handler, i)) {
			case SQLITE_INTEGER:
				row.insert(column_name, pfs::unitype(long_t(sqlite3_column_int64(s3_sth->_native_handler, i))));
				break;
			case SQLITE_FLOAT:
				row.insert(column_name, pfs::unitype(sqlite3_column_double(s3_sth->_native_handler, i)));
				break;
			case SQLITE_TEXT: {
				const char * text = reinterpret_cast<const char*>(sqlite3_column_text(s3_sth->_native_handler, i));
				row.insert(column_name, pfs::unitype(_u8(text)));
				break;
			}
			case SQLITE_BLOB: {
				const char * bytes = reinterpret_cast<const char*>(sqlite3_column_blob(s3_sth->_native_handler, i));
				int nbytes = sqlite3_column_bytes(s3_sth->_native_handler, i);
				PFS_ASSERT(nbytes >= 0);
				row.insert(column_name, pfs::unitype(pfs::bytearray(bytes, size_t(nbytes))));
				}
				break;
			case SQLITE_NULL:
			default:
				row.insert(column_name, pfs::unitype());
				break;
			}
		}
		return true;
	}

	return false;
}


/* The leftmost SQL parameter has an index of 1, but cwt-debby uses indices beginning from 0.
 */
bool s3_dbd_stmt_bind (cwt::debby::statement_data & sth, size_t index, const pfs::unitype & param)
{
	int rc;
	Sqlite3DbStatement * s3_sth = static_cast<Sqlite3DbStatement*>(& sth);

	/* 'sqlite3_bind_parameter_count' routine actually
	 * returns the index of the largest (rightmost) parameter */
	if (index >= size_t(sqlite3_bind_parameter_count(s3_sth->_native_handler)))
		return false;

	int s3_index = int(index) + 1;

	switch (param.type_id()) {
	case pfs::Null :
		rc = sqlite3_bind_null(s3_sth->_native_handler, s3_index);
		break;
	case pfs::Bool:
		rc = sqlite3_bind_int(s3_sth->_native_handler, s3_index, param.toBoolean());
		break;
	case pfs::Integer:
		rc = sqlite3_bind_int64(s3_sth->_native_handler, s3_index, param.toInteger());
		break;
	case pfs::Float:
		rc = sqlite3_bind_double(s3_sth->_native_handler, s3_index, param.toDouble());
		break;
	case pfs::String: {
		pfs::string text = param.toString();
		size_t sz = text.size();
		PFS_ASSERT(sz <= PFS_INT_MAX);
		// Fifth argument - SQLITE_TRANSIENT - SQLite makes its own private copy of the data immediately,
		// before the sqlite3_bind_*() routine returns.
		rc = sqlite3_bind_text(s3_sth->_native_handler, s3_index, text.c_str(), int(sz), SQLITE_TRANSIENT);
		}
		break;
	case pfs::Blob: {
		pfs::byte_string blob = param.toBlob();
		size_t sz = blob.size();
		PFS_ASSERT(sz <= PFS_INT_MAX);
		// Fifth argument - SQLITE_TRANSIENT - SQLite makes its own private copy of the data immediately,
		// before the sqlite3_bind_*() routine returns.
		rc = sqlite3_bind_blob(s3_sth->_native_handler, s3_index, blob.data(), int(sz), SQLITE_TRANSIENT);
		}
		break;
	default:
		PFS_VERIFY_X(false, _Tr("Unsupported bind parameter type"));
		return false;
	}

	PFS_VERIFY_X(rc == SQLITE_OK, __s3_stmt_errmsg(s3_sth).c_str());

	return rc == SQLITE_OK ? true : false;
}

size_t s3_dbd_stmt_column_count (cwt::debby::statement_data & sth)
{
	Sqlite3DbStatement * s3_sth = static_cast<Sqlite3DbStatement*>(& sth);
	return s3_sth->_columnCount;
}

pfs::string s3_dbd_stmt_column_name (cwt::debby::statement_data & sth, size_t index)
{
	PFS_ASSERT(index <= size_t(pfs::max_type<int>()));
	Sqlite3DbStatement * s3_sth = static_cast<Sqlite3DbStatement*>(& sth);
	return pfs::string(sqlite3_column_name(s3_sth->_native_handler, int(index)));
}

cwt::debby::column_type s3_dbd_stmt_column_type (cwt::debby::statement_data & sth, size_t index)
{
	PFS_ASSERT(index <= size_t(pfs::max_type<int>()));
	Sqlite3DbStatement * s3_sth = static_cast<Sqlite3DbStatement*>(& sth);
	return __mappings.columnType(sqlite3_column_type(s3_sth->_native_handler, int(index)));
}


PFS_PLUGIN_API pfs::pluggable * __plugin_ctor__ (void)
{
	return dynamic_cast<pfs::pluggable *>(new settings_plugin);
}

PFS_PLUGIN_API void __plugin_dtor__ (pfs::pluggable * p)
{
	if (p) {
		delete dynamic_cast<settings_plugin *>(p);
	}
}
