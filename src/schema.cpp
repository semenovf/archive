/**
 * @file schema.cpp
 * @author wladt
 * @date Feb 10, 2014
 */

#include "../include/cwt/debby/schema.hpp"

namespace cwt { namespace debby {

const pfs::string schema::PrimaryKeyColumn("__debby_id__");

table::table ()
{
	add (column(schema::PrimaryKeyColumn, HIBERLITE_PRIMARY_KEY_STORAGE_TYPE));
}

void table::add (const column & col)
{
	_columns.insert(col.name(), col);
}

bool table::contains (const pfs::string & colname) const
{
	return _columns.find(colname) != _columns.cend();
}

void schema::add (table t)
{
	if (find(t.name()) != cend()) {
		insert(t.name(), t);
	} else {
		pfs::string errstr(t.name());
		errstr << _Tr(": table already exists");
		this->addError(errstr);
	}
}


}} // cwt::debby


