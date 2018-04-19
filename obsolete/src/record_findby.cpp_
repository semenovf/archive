/*
 * record_findby.cpp
 *
 *  Created on: Nov 12, 2013
 *      Author: wladt
 */

#include "../include/cwt/debby/record.hpp"

CWT_NS_BEGIN

namespace debby
{
#ifdef __COMMENT__
cwt::String Record::buildSqlForLoad () const
{
	cwt::String r("SELECT ");

	cwt::String columns;
	cwt::String pkname;
	bool needComma = false;

	AttributeMap::const_iterator it = _attrs.cbegin();
	AttributeMap::const_iterator itEnd = _attrs.cend();

	for (; it != itEnd; ++it) {
		if (it->second->isPk())
			pkname = it->first;

		if (needComma) {
			columns << ", ";
		}

		columns << '`' << it->first << '`';
		needComma = true;
	}

	r << " FROM " << _name
	  << " WHERE " << pkname << " = ?";

	CWT_TRACE(r.c_str());

	return r;
}

bool Record::load (DbHandler & dbh, const cwt::String & pkvalue)
{
	if (!_sthPool[SthForLoad]) {

		if (!initAttributesFromMeta(dbh))
			return false;

		cwt::String sql(); // = buildSqlForLoad();
		sql << "SELECT * FROM " << _name
			<< " WHERE " << _pkname << " = " << pkvalue;
		_sthPool[SthForLoad] = dbh.prepare(sql);
	}

	if (!_sthPool[SthForLoad])
		return false;

	AttributeMap::const_iterator it = _attrs.cbegin();
	AttributeMap::const_iterator itEnd = _attrs.cend();

	for (; it != itEnd; ++it) {
		_sthPool[SthForLoad]->bind(it->second->value());
	}

	return _sthPool[SthForLoad]->exec();
}


RecordPtr Record::findBy (DbHandler & dbh
		, int argc, const std::pair<cwt::String, cwt::UniType> args[])
{
	RecordPtr r(new Record);
}

#endif

} // namespace debby

CWT_NS_END
