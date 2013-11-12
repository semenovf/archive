/*
 * s3_create_schema.cpp
 *
 *  Created on: Nov 6, 2013
 *      Author: wladt
 */

#include "sqlite3_dbd.hpp"
#include <cwt/logger.hpp>
#include <cwt/safeformat.hpp>
#include <cwt/debby/record.hpp>
#include <cwt/debby/schema.hpp>

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

static String __s3_column_def (const cwt::String & name, const Attribute & attr)
{
	cwt::String r;
	r << name
	  << " "
	  << SqlTypeAffinity::affinity.value(attr.type(), cwt::String());

	if (!attr.isNullable())
		r << " NOT NULL";
	if (attr.isPk())
		r << " PRIMARY KEY";
	if (attr.isAutoinc())
		r << " AUTOINCREMENT";
	if (attr.isUnique())
		r << " UNIQUE";

	if (attr.hasDefault()) {
		cwt::String defaultValue;

		switch(attr.type()) {
		case cwt::UniType::BoolValue:
			defaultValue << (attr.defaultValue().toBool() ? "1" : "0");
			break;
		case cwt::UniType::IntegerValue:
		case cwt::UniType::FloatValue:
		case cwt::UniType::DoubleValue:
			defaultValue << attr.defaultValue().toString();
			break;
		case cwt::UniType::StringValue:
		case cwt::UniType::TimeValue:
		case cwt::UniType::DateValue:
		case cwt::UniType::DateTimeValue:
			defaultValue << '"' << attr.defaultValue().toString() << '"';
			break;
		default:
			break;
		}

		CWT_ASSERT(!defaultValue.isEmpty());
		r << " DEFAULT " << defaultValue;
	}

	return r;
}


bool s3_create_schema (DbHandlerData & dbh, const Schema & schema)
{
	bool r = true;

	Vector<cwt::String> tableNames = schema.names();
	size_t ntables = tableNames.size();

	for (size_t t = 0; t < ntables; ++t) {
		const Table & table = schema[tableNames[t]];
		cwt::String sql("CREATE TABLE IF NOT EXISTS ");
		sql << tableNames[t] << " (";

		Vector<cwt::String> fieldNames = table.names();
		size_t nfields = fieldNames.size();
		bool needComma = false;

		for (size_t f = 0; f < nfields; ++f) {
			if (needComma)
				sql << ", ";
			sql << __s3_column_def(fieldNames[f], table[fieldNames[f]]);
			needComma = true;
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

	Vector<cwt::String> tableNames = schema.names();
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
