/*
 * schema_save.cpp
 *
 *  Created on: Nov 18, 2013
 *      Author: wladt
 */


#include "../include/cwt/debby/record.hpp"
#include "../include/cwt/debby/schema.hpp"

CWT_NS_BEGIN

namespace debby
{
extern cwt::String __sql_type_affinity(Attribute::Type type);

// TODO Need support for index

static String __column_def (const cwt::String & name, const Attribute & attr)
{
	cwt::String r;
	r << name
	  << " "
	  << __sql_type_affinity(attr.type());

	if (!attr.isNullable())
		r << " NOT NULL";
	if (attr.isPk())
		r << " PRIMARY KEY";
	if (attr.isAutoinc())
		r << " AUTOINCREMENT";
	if (attr.isUnique())
		r << " UNIQUE";

	if (attr.hasDefault()) {
		r << " DEFAULT " << attr.stringify();
	}

	return r;
}


bool Schema::save ()
{
	bool r = true;

	Vector<cwt::String> tnames = this->names();
	size_t ntables = tnames.size();

	_dbh->begin();

	for (size_t t = 0; t < ntables; ++t) {
		const Table & table = (*this)[tnames[t]];
		cwt::String sql("CREATE TABLE IF NOT EXISTS ");
		sql << tnames[t] << " (";

		Vector<cwt::String> fieldNames = table.names();
		size_t nfields = fieldNames.size();
		bool needComma = false;

		for (size_t f = 0; f < nfields; ++f) {
			if (needComma)
				sql << ", ";
			sql << __column_def(fieldNames[f], table[fieldNames[f]]);
			needComma = true;
		}

		sql << ")";

		CWT_TRACE(sql.c_str());

		r = _dbh->query(sql);
		if (!r) {
			r = false;
			break;
		}
	}

	if (!r) {
		_dbh->commit();
	} else {
		_dbh->rollback();
	}

	return r;
}


} // namespace debby

CWT_NS_END
