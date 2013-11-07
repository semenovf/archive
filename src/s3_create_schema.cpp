/*
 * s3_create_schema.cpp
 *
 *  Created on: Nov 6, 2013
 *      Author: wladt
 */

#include "sqlite3_dbd.hpp"
#include <cwt/logger.hpp>
#include <cwt/safeformat.hpp>
#include <cwt/debby/schema.hpp>

static const uint_t __version[] = { 1000, 1000 };


struct SqlTypeAffinity : public cwt::Map<cwt::UniType::TypeEnum, cwt::String>
{
	static SqlTypeAffinity affinity;
	SqlTypeAffinity() {
		affinity << std::make_pair(cwt::UniType::NullValue     , "")
				 << std::make_pair(cwt::UniType::BoolValue     , "NUMERIC")
		         << std::make_pair(cwt::UniType::IntegerValue  , "INTEGER")
		         << std::make_pair(cwt::UniType::FloatValue    , "REAL")
		         << std::make_pair(cwt::UniType::DoubleValue   , "REAL")
		         << std::make_pair(cwt::UniType::StringValue   , "TEXT")
		         << std::make_pair(cwt::UniType::BlobValue     , "BLOB")
		         << std::make_pair(cwt::UniType::TimeValue     , "NUMERIC")
		         << std::make_pair(cwt::UniType::DateValue     , "NUMERIC")
		         << std::make_pair(cwt::UniType::DateTimeValue , "NUMERIC")
		         << std::make_pair(cwt::UniType::ObjectValue   , "");
	}
};

SqlTypeAffinity SqlTypeAffinity::affinity;

static bool __check_version (const Schema & schema)
{
	ushort_t major, minor;
	uint_t v = schema.version(& major, & minor);
	if (v >= __version[0] && v <= __version[1]) {
		return true;
	}

	Logger::error(_Fr("Unsupported version: %u.%u") % major % minor);
	return false;
}


static String __s3_column_def (const Field & field)
{
	return SqlTypeAffinity::affinity.value(
			field.isRef() ? field.deref().type() : field.type()
			, cwt::String());
}

bool s3_create_schema (DbHandlerData & dbh, const Schema & schema)
{
	bool r = true;

	if (!__check_version(schema))
		return false;

	Vector<cwt::String> tableNames = schema.tableNames();
	size_t ntables = tableNames.size();

	for (size_t i = 0; i < ntables; ++i) {
		Table table = schema.table(tableNames[i]);
		cwt::String sql("CREATE TABLE IF NOT EXISTS ");
		sql << tableNames[i] << " (";

		Vector<cwt::String> fieldNames = table.fieldNames();
		size_t nfields = fieldNames.size();
		cwt::String comma;

		for (size_t f = 0; f < nfields; ++f) {
			Field field = table.field(fieldNames[f]);
			sql << comma << fieldNames[f] << " " << __s3_column_def(field);
			if (field.isPk())
				sql << " PRIMARY KEY";
			if (field.isAutoinc())
				sql << " AUTOINCREMENT";
			comma = ", ";
		}

		// Constraints

		// Primary Key
		Vector<cwt::String> pknames = table.primaryKeyNames();
		size_t npk = pknames.size();
		comma.clear();

		if (npk > 1) { // Compound primary key
			sql << ", CONSTRAINT __" << tableNames[i] << "_pk__ PRIMARY KEY (";
			for (size_t ipk = 0; ipk < npk; ++ipk) {
				sql << comma << pknames[ipk];
				comma = ", ";
			}
			sql << ")";
		}

		sql << ")";

		CWT_TRACE(sql.c_str());

		r = s3_dbd_query (dbh, sql);
		if (!r) {
			s3_drop_schema (dbh, schema);
			break;
		}
	}

	return r;
}


bool s3_drop_schema (DbHandlerData & dbh, const Schema & schema)
{
	bool r = true;

	if (!__check_version(schema))
		return false;

	Vector<cwt::String> tableNames = schema.tableNames();
	size_t ntables = tableNames.size();

	for (size_t i = 0; i < ntables; ++i) {
		cwt::String sql("DROP TABLE IF EXISTS ");
		sql << tableNames[i];

		CWT_TRACE(sql.c_str());
		r = s3_dbd_query (dbh, sql);
		if (!r) {
			break;
		}
	}

	return r;
/*
	Sqlite3DbHandler * s3_dbh = reinterpret_cast<Sqlite3DbHandler *>(dbh);
	const char * filename = sqlite3_db_filename(s3_dbh->dbh_native, "main");
	if (!filename) {
		Logger::error(_Tr("Bad scheme name or invalid sqlite3 connection"));
		return false;
	}

	s3_dbd_close(dbh);
	return FileSystem::unlink(filename);
*/
}
