/*
 * table.cpp
 *
 *  Created on: Nov 11, 2013
 *      Author: wladt
 */

#include "../include/cwt/debby/schema.hpp"
#include <cwt/logger.hpp>
#include <cwt/safeformat.hpp>

CWT_NS_BEGIN

namespace debby
{

cwt::String Table::buildSqlForCreateRecord ()
{
	cwt::String columns;
	cwt::String values;
	bool needComma = false;

	AttributeMap::const_iterator it = _columns.cbegin();
	AttributeMap::const_iterator itEnd = _columns.cend();

	for (; it != itEnd; ++it) {
		if (it->second->isAutoinc() == 0) {
			if (needComma) {
				columns << ", ";
				values << ", ";
			}

			columns << '`' << it->first << '`';
			values << '?';
			needComma = true;
		}
	}

	cwt::String r;
	r << "INSERT INTO " << _name
	r << " (" << columns << ")";
	r << " VALUES (" << values << ")";

	CWT_TRACE(r.c_str());

	return r;
}

bool Table::createRecord (const Record & record)
{
	CWT_ASSERT(_pschema);

	if (!_prepared[SthForCreate].get()) {

		cwt::String sql = buildSqlForCreateRecord();

		_prepared[SthForCreate] = _pschema->_dbh->prepare(sql);

		if (!_prepared[SthForCreate].get())
			return false;
	}
	AttributeMap::const_iterator it = _columns.cbegin();
	AttributeMap::const_iterator itEnd = _columns.cend();

	for (; it != itEnd; ++it) {
		if (it->second->isAutoinc() == 0) {
			_prepared[SthForCreate]->bind(record[it->first]);
		}
	}

	return _prepared[SthForCreate]->exec();
}


cwt::String Table::buildSqlForUpdateRecord ()
{

}

bool Table::updateRecord (const Record & record)
{

}

bool Table::deleteRecord (const Record & record)
{

}



Attribute & Table::newAttribute (const cwt::String & attrname, Attribute::Type attrtype)
{
	CWT_ASSERT(!_columns.contains(attrname));
	AttributePtr attr(new Attribute(attrtype));
	AttributeMap::iterator it = _columns.insert(attrname, attr);
	return *it->second;
}

const Attribute & Table::operator [] (const cwt::String & name) const
{
	AttributeMap::const_iterator it = _columns.find(name);
	CWT_ASSERT(it != _columns.cend());
	return *it->second;
}

Attribute & Table::operator [] (const cwt::String & name)
{
	AttributeMap::iterator it = _columns.find(name);
	CWT_ASSERT(it != _columns.end());
	return *it->second;
}

} // namespace debby

CWT_NS_END


