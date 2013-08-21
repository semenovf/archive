/**
 * @file   sqlite3_dbd.cpp
 * @author wladt
 * @date   Aug 19, 2013 14:45
 *
 * @brief
 */

//#include <ctime> /* strftime */
#include <cstdlib>
#include <cwt/string.hpp>
#include <cwt/safeformat.hpp>
#include <cwt/hash.hpp>
#include <cwt/dbd.hpp>
#include <cwt/dbh.hpp>
#include <cwt/unitype.hpp>
#include <cwt/vector.hpp>
#include <cwt/bytearray.hpp>
#include <cwt/logger.hpp>

#include "../sqlite3/sqlite3.h"

CWT_NS_BEGIN

static int __refs = 0;
static DbDriver * __dbd = nullptr;

const int __MAX_SQL_TIMEOUT      = 10000; /* 10 seconds */
const int __MAX_EXEC_RETRY_COUNT = 10;
const int __MAX_EXEC_RETRY_SLEEP = 200;

struct Sqlite3DbHandler : public DbHandlerData
{
	sqlite3 * dbh_native;
};

struct Sqlite3DbStatement : public DbStatementData
{
	sqlite3_stmt * sth_native;
};


/* DBI API functions implementations */
DbHandlerData *         s3_dbd_open  (const String & driver_uri
		, const String & username
		, const String & password
		, const Hash<String, String> & params);
static void             s3_dbd_close           (DbHandlerData * dbh);
static bool             s3_dbd_set_auto_commit (DbHandlerData & dbh, bool on);
static bool             s3_dbd_auto_commit     (DbHandlerData & dbh);
static long_t           s3_dbd_errno           (DbHandlerData & dbh);

static bool             s3_dbd_query           (DbHandlerData & dbh, const String & sql);
static DbStatementData * s3_dbd_prepare        (DbHandlerData & dbh, const String & sql);
static ulong_t          s3_dbd_affected_rows   (DbHandlerData & dbh);
static ulong_t 			s3_dbd_last_id         (DbHandlerData & dbh);
static Vector<String>   s3_dbd_tables          (DbHandlerData & dbh);
static bool             s3_dbd_table_exists    (DbHandlerData & dbh, const String & name);
static bool             s3_dbd_begin           (DbHandlerData & dbh);
static bool             s3_dbd_commit          (DbHandlerData & dbh);
static bool             s3_dbd_rollback        (DbHandlerData & dbh);

static void             s3_dbd_stmt_close      (DbStatementData * sth);
static bool             s3_dbd_stmt_exec       (DbStatementData & sth);
static bool             s3_dbd_stmt_fetch_row_array (DbStatementData & sth, Vector<UniType> & row);
static bool             s3_dbd_stmt_fetch_row_hash (DbStatementData & sth, Hash<String, UniType> & row);
static bool             s3_dbd_stmt_bind       (DbStatementData & sth, size_t index, const UniType & param);


inline String __s3_stmt_errmsg(Sqlite3DbStatement * s3_sth)
{
	sqlite3 * dbh_native = sqlite3_db_handle(s3_sth->sth_native);
	return String(sqlite3_errmsg(dbh_native));
}


extern "C" DbDriver * __open__()
{
	if (!__dbd) {
		__dbd = new DbDriver;

		__dbd->open          = s3_dbd_open;
		__dbd->close         = s3_dbd_close;
		__dbd->autoCommit    = s3_dbd_auto_commit;
		__dbd->setAutoCommit = s3_dbd_set_auto_commit;
		__dbd->errno         = s3_dbd_errno;
		__dbd->tables        = s3_dbd_tables;
		__dbd->tableExists   = s3_dbd_table_exists;
		__dbd->query         = s3_dbd_query;
		__dbd->prepare       = s3_dbd_prepare;
		__dbd->rows          = s3_dbd_affected_rows;
		__dbd->lastId        = s3_dbd_last_id;
		__dbd->begin         = s3_dbd_begin;
		__dbd->commit        = s3_dbd_commit;
		__dbd->rollback      = s3_dbd_rollback;
		__dbd->closeStmt     = s3_dbd_stmt_close;
		__dbd->execStmt      = s3_dbd_stmt_exec;
		__dbd->fetchRowArray = s3_dbd_stmt_fetch_row_array;
		__dbd->fetchRowHash  = s3_dbd_stmt_fetch_row_hash;
		__dbd->bind          = s3_dbd_stmt_bind;
	}

	return __dbd;
}

/**
 *
 * @brief Connects to sqlite3 databases.
 *
 * @param driver_dsn Details see SQLite docs on sqlite3_open_v2 function.
 *				Examples:
 *              	file:data.db
 *                 		Open the file "data.db" in the current directory.
 *
 *                  file:/home/fred/data.db
 *                  file:///home/fred/data.db
 *                  file://localhost/home/fred/data.db
 *                  	Open the database file "/home/fred/data.db".
 *
 *                  file:///C:/Documents%20and%20Settings/fred/Desktop/data.db
 *                  	Windows only: Open the file "data.db" on fred's desktop on drive C:.
 *                  	Note that the %20 escaping in this example is not
 *                  	strictly necessary - space characters can be used literally
 *                  	in URI filenames.
 *
 *                  file:data.db?mode=ro&cache=private
 *                  	Open file "data.db" in the current directory for read-only access.
 *                  	Regardless of whether or not shared-cache mode is enabled
 *                  	by default, use a private cache.
 *
 *                  file:/home/fred/data.db?vfs=unix-nolock
 *                  	Open file "/home/fred/data.db". Use the special VFS "unix-nolock".
 *
 *                  file:data.db?mode=readonly
 *                  	An error. "readonly" is not a valid option for the "mode" parameter.
 *
 *				Mode values:
 *					mode=ro
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
 * //@param csname     Character set name (Sqlite supports only UTF-16, UTF-16le, UTF-16be, UTF-8).
 * //                  Only UTF-8 is supported now by DBI driver.
 * @return DBI connection to specified sqlite3 databases.
 *
 * @note  Autocommit mode is on by default.
 */
DbHandlerData * s3_dbd_open(const String & path
		, const String & username
		, const String & password
		, const Hash<String, String> & params)
{
	Sqlite3DbHandler * dbh = nullptr;
	sqlite3 *  dbh_native = nullptr;
	int        rc = SQLITE_OK;
	int        s3_flags;
	int        s3_flag_mode;

	CWT_UNUSED2(username, password);

    s3_flags = SQLITE_OPEN_URI;
	s3_flag_mode = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;

	Hash<String, String>::const_iterator mode = params.find(String("mode"));

	if (mode != params.cend()) {
		if (*mode == "ro")
			s3_flag_mode = SQLITE_OPEN_READONLY;
		else if (*mode == "rw")
			s3_flag_mode = SQLITE_OPEN_READWRITE;
		else if (*mode == "rwc")
			s3_flag_mode = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
		else if (*mode == "memory")
			s3_flag_mode = SQLITE_OPEN_MEMORY;
	}

	s3_flags |= s3_flag_mode;

    rc = sqlite3_open_v2(path.c_str(), & dbh_native, s3_flags, NULL);

	if (rc != SQLITE_OK) {
		if (!dbh_native) {
			Logger::error(_Tr("Unable to allocate memory for database handler."));
		} else {
			switch( rc ) {
				case SQLITE_CANTOPEN:
					Logger::error(_Fr("Unable to open the database file. Try to check path '%s'") % path);
					break;
				default: break;
			}
			sqlite3_close(dbh_native);
			dbh_native = nullptr;
		}
	} else {
		CWT_ASSERT(__dbd);
		dbh = new Sqlite3DbHandler;

		// TODO for what this call ?
		sqlite3_busy_timeout(dbh_native, __MAX_SQL_TIMEOUT);

		// Enable extended result codes
		sqlite3_extended_result_codes(dbh_native, 1);

		dbh->driver = __dbd;
		dbh->dbh_native = dbh_native;
	}

	++__refs;

    return reinterpret_cast<DbHandlerData*>(dbh);
}


static void s3_dbd_close (DbHandlerData * dbh)
{
	Sqlite3DbHandler * s3_dbh = reinterpret_cast<Sqlite3DbHandler*>(dbh);

	if (!dbh )
		return;

/*
	while ((stmt = sqlite3_next_stmt(s3_dbh->dbh_native, 0)) != 0)
		sqlite3_finalize(stmt);
*/

	sqlite3_close_v2(s3_dbh->dbh_native);
	s3_dbh->driver = nullptr;
	s3_dbh->dbh_native = nullptr;
	delete s3_dbh;

	--__refs;

	CWT_ASSERT(__dbd);
	CWT_ASSERT(__refs >= 0);
	if (!__refs) {
		delete __dbd;
		__dbd = nullptr;
		__refs = 0;
	}
}

bool s3_dbd_set_auto_commit (DbHandlerData & , bool)
{
	return true;
}

bool s3_dbd_auto_commit(DbHandlerData & dbh)
{
	Sqlite3DbHandler * s3_dbh = reinterpret_cast<Sqlite3DbHandler *>(& dbh);
	return 0 == sqlite3_get_autocommit(s3_dbh->dbh_native)
			? false
			: true;
}

long_t s3_dbd_errno (DbHandlerData & dbh)
{
	Sqlite3DbHandler * s3_dbh = reinterpret_cast<Sqlite3DbHandler *>(& dbh);
	int rc = sqlite3_errcode(s3_dbh->dbh_native);
	if (rc == SQLITE_OK
			|| rc == SQLITE_DONE
			|| rc == SQLITE_ROW)
		rc = 0;
	return long_t(rc);
}

/*
String s3_dbd_strerror (DbHandlerData & dbh)
{
	Sqlite3DbHandler * s3_dbh = reinterpret_cast<Sqlite3DbHandler *>(& dbh);
	return String(sqlite3_errmsg(s3_dbh->dbh_native));
}
*/

bool s3_dbd_query (DbHandlerData & dbh, const String & sql)
{
	Sqlite3DbHandler * s3_dbh = reinterpret_cast<Sqlite3DbHandler *>(& dbh);
	char * errmsg;

	int rc = sqlite3_exec(s3_dbh->dbh_native, sql.c_str(), NULL, NULL, & errmsg);

	if (SQLITE_OK != rc) {
		if (errmsg) {
			Logger::error(String(errmsg));
			sqlite3_free(errmsg);
		}
		return false;
	}

	return true;
}


DbStatementData * s3_dbd_prepare(DbHandlerData & dbh, const String & sql)
{
	Sqlite3DbHandler * s3_dbh = reinterpret_cast<Sqlite3DbHandler *>(& dbh);
	sqlite3_stmt * sth_native = nullptr;

	int rc = sqlite3_prepare_v2(
			s3_dbh->dbh_native
			, sql.c_str()      /* SQL statement, UTF-8 encoded */
			, -1               /* Maximum length of zSql in bytes. */
			, & sth_native     /* OUT: Statement handle */
			, NULL );          /* OUT: Pointer to unused portion of zSql */

	if (rc != SQLITE_OK) {
		Logger::error(String(sqlite3_errmsg(s3_dbh->dbh_native)));
		return nullptr;
	}

	Sqlite3DbStatement * sth = new Sqlite3DbStatement;
	CWT_ASSERT(__dbd);
	sth->driver = __dbd;
	sth->sth_native = sth_native;

//	int column_count = sqlite3_column_count(sth->sth_native);
//	if (column_count > 0) { /* SELECT statement */
//	}

	return reinterpret_cast<DbStatementData*>(sth);
}

ulong_t s3_dbd_affected_rows (DbHandlerData & dbh)
{
	Sqlite3DbHandler * s3_dbh = reinterpret_cast<Sqlite3DbHandler *>(& dbh);
	int nrows = sqlite3_changes(s3_dbh->dbh_native);
	return nrows < 0 ? ulong_t(0) : ulong_t(nrows);
}

ulong_t s3_dbd_last_id (DbHandlerData & dbh)
{
	Sqlite3DbHandler * s3_dbh = reinterpret_cast<Sqlite3DbHandler *>(& dbh);
	return ulong_t(sqlite3_last_insert_rowid(s3_dbh->dbh_native));
}

Vector<String> s3_dbd_tables (DbHandlerData & dbh)
{
	Vector<String> r;

	DbStatementData * sth = s3_dbd_prepare(dbh, "SELECT name FROM sqlite_master WHERE type='table' ORDER BY name");

	if (sth) {
		if (s3_dbd_stmt_exec(*sth)) {
			Vector<UniType> row;
			while (s3_dbd_stmt_fetch_row_array (*sth, row)) {
				r.append(row[0].toString());
				row.clear();
			}
		}
		s3_dbd_stmt_close(sth);
	}
	return r;
}

bool s3_dbd_table_exists (DbHandlerData & dbh, const String & name)
{
	bool r = false;
	String sql(cwt::SafeFormat("SELECT COUNT(*) as table_count FROM sqlite_master WHERE type = 'table' AND name = '%s'") % name);
	DbStatementData * sth = s3_dbd_prepare(dbh, sql);

	if (sth) {
		if (s3_dbd_stmt_exec(*sth)) {
			Vector<UniType> row;
			s3_dbd_stmt_fetch_row_array (*sth, row);
			CWT_ASSERT(row.size() == 1);
			r = (row[0].toInt() > 0);
		}
		s3_dbd_stmt_close(sth);
	}
	return r;
}

bool s3_dbd_begin (DbHandlerData & dbh)
{
	return s3_dbd_query(dbh, "BEGIN");
}

bool s3_dbd_commit (DbHandlerData & dbh)
{
	return s3_dbd_query(dbh, "COMMIT");
}

bool s3_dbd_rollback (DbHandlerData & dbh)
{
	return s3_dbd_query(dbh, "ROLLBACK");
}

void s3_dbd_stmt_close (DbStatementData * sth)
{
	if (sth) {
		Sqlite3DbStatement * s3_sth = reinterpret_cast<Sqlite3DbStatement*>(sth);

		int rc = sqlite3_finalize(s3_sth->sth_native);
		if (rc != SQLITE_OK) {
			Logger::error(_Fr("Failed to close statement: %s") % __s3_stmt_errmsg(s3_sth));
		}
		s3_sth->sth_native = nullptr;
		s3_sth->driver = nullptr;
		delete s3_sth;
	}
}

bool s3_dbd_stmt_exec (DbStatementData & sth)
{
	Sqlite3DbStatement * s3_sth = reinterpret_cast<Sqlite3DbStatement*>(& sth);
	bool r = true;

	if (sqlite3_column_count(s3_sth->sth_native) > 0) { // SELECT statement
		int rc = sqlite3_reset(s3_sth->sth_native);
		r = (rc == SQLITE_OK ? true : false);
	} else {
		int rc = sqlite3_step(s3_sth->sth_native);
		r = (rc == SQLITE_OK || rc == SQLITE_DONE ? true : false);

		if (rc == SQLITE_DONE) {
			sqlite3_reset(s3_sth->sth_native);
			sqlite3_clear_bindings(s3_sth->sth_native);
		}
	}

	if (!r) {
		Logger::error(__s3_stmt_errmsg(s3_sth));
	}

	return r;
}

static int __fetch_helper (Sqlite3DbStatement * s3_sth)
{
	int rc = SQLITE_OK;
	int nretries = __MAX_EXEC_RETRY_COUNT;

	do {
		rc = sqlite3_step(s3_sth->sth_native);

		if ( rc == SQLITE_BUSY ) {
			if (nretries++ > __MAX_EXEC_RETRY_COUNT) {
				break;
			} else {
				sqlite3_sleep(__MAX_EXEC_RETRY_SLEEP);
			}
		}
	} while (rc == SQLITE_BUSY);

	if (rc == SQLITE_DONE) {
		; // ok
	} else if (rc == SQLITE_ROW) {
		; // ok
	} else {
		Logger::error(__s3_stmt_errmsg(s3_sth));
	}

	return rc;
}

bool s3_dbd_stmt_fetch_row_array (DbStatementData & sth, Vector<UniType> & row)
{
	Sqlite3DbStatement * s3_sth = reinterpret_cast<Sqlite3DbStatement*>(& sth);
	int rc = __fetch_helper(s3_sth);

	if (rc == SQLITE_ROW) {
		int ncols = sqlite3_column_count(s3_sth->sth_native);
		for (int i = 0; i < ncols; ++i) {
			//String column_name (sqlite3_column_name(s3_sth->sth_native, i));

			switch (sqlite3_column_type(s3_sth->sth_native, i)) {
			case SQLITE_INTEGER:
				row.append(UniType(sqlite3_column_int64(s3_sth->sth_native, i)));
				break;
			case SQLITE_FLOAT:
				row.append(UniType(sqlite3_column_double(s3_sth->sth_native, i)));
				break;
			case SQLITE_TEXT: {
				const char * text = reinterpret_cast<const char*>(sqlite3_column_text(s3_sth->sth_native, i));
				row.append(UniType(String(text)));
				}
				break;
			case SQLITE_BLOB: {
				const char * bytes = (const char *)sqlite3_column_blob(s3_sth->sth_native, i);
				int nbytes = sqlite3_column_bytes(s3_sth->sth_native, i);
				CWT_ASSERT(nbytes >= 0);
				row.append(UniType(ByteArray(bytes, size_t(nbytes))));
				}
				break;
			case SQLITE_NULL:
			default:
				row.append(UniType());
				break;
			}
		}
		return true;
	}

	return false;
}

bool s3_dbd_stmt_fetch_row_hash (DbStatementData & sth, Hash<String, UniType> & row)
{
	Sqlite3DbStatement * s3_sth = reinterpret_cast<Sqlite3DbStatement*>(& sth);
	int rc = __fetch_helper(s3_sth);

	if (rc == SQLITE_ROW) {
		int ncols = sqlite3_column_count(s3_sth->sth_native);
		for (int i = 0; i < ncols; ++i) {
			String column_name (sqlite3_column_name(s3_sth->sth_native, i));

			switch (sqlite3_column_type(s3_sth->sth_native, i)) {
			case SQLITE_INTEGER:
				row.insert(column_name, UniType(sqlite3_column_int64(s3_sth->sth_native, i)));
				break;
			case SQLITE_FLOAT:
				row.insert(column_name, UniType(sqlite3_column_double(s3_sth->sth_native, i)));
				break;
			case SQLITE_TEXT:
				row.insert(column_name, UniType(sqlite3_column_text(s3_sth->sth_native, i)));
				break;
			case SQLITE_BLOB: {
				const char * bytes = (const char *)sqlite3_column_blob(s3_sth->sth_native, i);
				int nbytes = sqlite3_column_bytes(s3_sth->sth_native, i);
				CWT_ASSERT(nbytes >= 0);
				row.insert(column_name, UniType(ByteArray(bytes, size_t(nbytes))));
				}
				break;
			case SQLITE_NULL:
			default:
				row.insert(column_name, UniType());
				break;
			}
		}
		return true;
	}

	return false;
}


/* The leftmost SQL parameter has an index of 1, but cwt-debby uses indices beginning from 0.
 */
bool s3_dbd_stmt_bind (DbStatementData & sth, size_t index, const UniType & param)
{
	int rc;
	Sqlite3DbStatement * s3_sth = reinterpret_cast<Sqlite3DbStatement*>(& sth);

	/* 'sqlite3_bind_parameter_count' routine actually
	 * returns the index of the largest (rightmost) parameter */
	if (index >= size_t(sqlite3_bind_parameter_count(s3_sth->sth_native)))
		return false;

	int s3_index = int(index) + 1;

	switch (param.type()) {
	case UniType::NullValue :
		rc = sqlite3_bind_null(s3_sth->sth_native, s3_index);
		break;
	case UniType::BoolValue:
		rc = sqlite3_bind_int(s3_sth->sth_native, s3_index, param.toBool());
		break;
	case UniType::LongValue:
		rc = sqlite3_bind_int64(s3_sth->sth_native, s3_index, param.toLong());
		break;
	case UniType::FloatValue:
	case UniType::DoubleValue:
		rc = sqlite3_bind_double(s3_sth->sth_native, s3_index, param.toDouble());
		break;
	case UniType::StringValue: {
		String text = param.toString();
		size_t sz = text.size();
		CWT_ASSERT(sz <= CWT_INT_MAX);
		// Fifth argument - SQLITE_TRANSIENT - SQLite makes its own private copy of the data immediately,
		// before the sqlite3_bind_*() routine returns.
		rc = sqlite3_bind_text(s3_sth->sth_native, s3_index, text.c_str(), int(sz), SQLITE_TRANSIENT);
		}
		break;
	case UniType::BlobValue: {
		ByteArray blob = param.toBlob();
		size_t sz = blob.size();
		CWT_ASSERT(sz <= CWT_INT_MAX);
		// Fifth argument - SQLITE_TRANSIENT - SQLite makes its own private copy of the data immediately,
		// before the sqlite3_bind_*() routine returns.
		rc = sqlite3_bind_blob(s3_sth->sth_native, s3_index, blob.data(), int(sz), SQLITE_TRANSIENT);
		}
		break;
	case UniType::ObjectValue:
	default:
		Logger::error(_Tr("Unsupported bind parameter type"));
		return false;
	}

	if (rc != SQLITE_OK) {
		Logger::error(__s3_stmt_errmsg(s3_sth));
	}
	return rc == SQLITE_OK ? true : false;
}

CWT_NS_END
