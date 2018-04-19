/**
 * @file schema.cpp
 * @author wladt
 * @date Feb 10, 2014
 */

#include "pfs/debby/schema.hpp"

namespace pfs { namespace debby {

bool table_basic::deploy (pfs::debby::database & db)
{
	pfs::string sql;

	sql << "CREATE TABLE IF NOT EXISTS " << _name << "(";

	pfs::string comma;

	for (fields_map_type::const_iterator it = _fields.cbegin()
			; it != _fields.cend()
			; ++it) {
		sql << comma << it->second->name();
		if (it->second->isPk())
			sql << " INTEGER PRIMARY KEY NOT NULL";
		else
			sql << " INTEGER NOT NULL";
		comma = _l1(",");
	}
	sql << ")";

	pfs::debby::statement st = db.prepare(sql);
	return st.exec();
}

}} // pfs::debby


