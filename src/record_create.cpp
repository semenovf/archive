/*
 * record_create.cpp
 *
 *  Created on: Nov 12, 2013
 *      Author: wladt
 */

#include "../include/cwt/debby/record.hpp"

CWT_NS_BEGIN

namespace debby
{

cwt::String Record::buildSqlForCreate () const
{
	cwt::String r("INSERT INTO");

	r << ' ' << _name;

	cwt::String columns;
	cwt::String values;
	bool needComma = false;

	AttributeMap::const_iterator it = _attrs.cbegin();
	AttributeMap::const_iterator itEnd = _attrs.cend();

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

	r << " (" << columns << ")";
	r << " VALUES (" << values << ")";

	CWT_TRACE(r.c_str());

	return r;
}

bool Record::create (DbHandler & dbh)
{
	if (!_sthPool[SthForCreate]) {
		cwt::String sql = buildSqlForCreate();
		_sthPool[SthForCreate] = dbh.prepare(sql);
	}

	if (!_sthPool[SthForCreate])
		return false;


	AttributeMap::const_iterator it = _attrs.cbegin();
	AttributeMap::const_iterator itEnd = _attrs.cend();

	for (; it != itEnd; ++it) {
		if (it->second->isAutoinc() == 0) {
			_sthPool[SthForCreate]->bind(it->second->value());
		}
	}

	return _sthPool[SthForCreate]->exec();
}

} // namespace debby

CWT_NS_END



