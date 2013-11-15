/*
 * record_load.cpp
 *
 *  Created on: Nov 12, 2013
 *      Author: wladt
 */


#include "../include/cwt/debby/record.hpp"

CWT_NS_BEGIN

namespace debby
{

Record * Record::load (DbHandler & dbh, const cwt::String & name)
{
	// read table's meta information
	cwt::Vector<DbColumnMeta> tableMeta;

	if (!dbh.meta (name, tableMeta) )
		return nullptr;

	cwt::Vector<DbColumnMeta>::const_iterator itMeta = tableMeta.cbegin();
	cwt::Vector<DbColumnMeta>::const_iterator itMetaEnd = tableMeta.cend();

	Record * r = new Record;

	for (; itMeta != itMetaEnd; ++itMeta) {
		Json jsonMeta;
		jsonMeta.insert("type"      , cwt::UniType::typeToString(itMeta->column_type));
		jsonMeta.insert("pk"        , itMeta->has_pk.first ? itMeta->has_pk.second : false);
		jsonMeta.insert("nullable"  , itMeta->has_not_null.first ? !itMeta->has_not_null.second : false);
		jsonMeta.insert("unique"    , itMeta->has_unique.first ? !itMeta->has_unique.second : false);
		jsonMeta.insert("unsigned"  , itMeta->has_unsigned.first ? itMeta->has_unsigned.second : false);
		jsonMeta.insert("timestamp" , itMeta->has_timestamp.first ? itMeta->has_timestamp.second : false);
		jsonMeta.insert("size"      , itMeta->has_size.first ? size_t(itMeta->has_size.second) : 0);
		jsonMeta.insert("autoinc"   , itMeta->has_autoinc.first ? itMeta->has_autoinc.second : 0);

		if (itMeta->has_default_value.first) {
			cwt::String v(itMeta->has_default_value.second.toString());

			if (itMeta->column_type == cwt::UniType::StringValue) {
				v.prepend(1, '"');
				v.append(1, '"');
			}
			jsonMeta.insert("default", v);
		}

		r->addFromJson (itMeta->column_name, jsonMeta);
	}

	return r;
}

} // namespace debby

CWT_NS_END

