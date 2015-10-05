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

cwt::String Record::buildSqlForDestroy ()
{
	cwt::String r;
	cwt::String condition;

	r << "DELETE FROM " << _name;

	if (_cachedPk.second) {
		condition << _cachedPk.first << " = ?";
	} else {
		bool needAND = false;

		AttributeMap::const_iterator it = _attrs.cbegin();
		AttributeMap::const_iterator itEnd = _attrs.cend();

		for (; it != itEnd; ++it) {
			if (it->second->isPk()) {
				_cachedPk.first = it->first;
				_cachedPk.second = it->second;
				condition << it->first << " = ?";
				break;
			}

			if (it->second->isAutoinc() == 0) {
				if (needAND) {
					condition << " AND ";
				}

				condition << it->first << " = ? ";
				needAND = true;
			}
		}
	}

	r << " WHERE " << condition;

	CWT_TRACE(r.c_str());

	return r;
}

bool Record::destroy (DbHandler & dbh)
{
	if (!_sthPool[SthForDestroy]) {
		cwt::String sql = buildSqlForDestroy();
		_sthPool[SthForDestroy] = dbh.prepare(sql);

		if (!_sthPool[SthForDestroy])
			return false;
	}

	if (_cachedPk.second) {
		_sthPool[SthForDestroy]->bind(_cachedPk.second->value());
	} else {
		AttributeMap::const_iterator it = _attrs.cbegin();
		AttributeMap::const_iterator itEnd = _attrs.cend();

		for (; it != itEnd; ++it) {
			_sthPool[SthForDestroy]->bind(it->second->value());
		}
	}

	return _sthPool[SthForDestroy]->exec();
}

} // namespace debby

CWT_NS_END



