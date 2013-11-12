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
		Attribute * pattr = nullptr;

		switch (itMeta->column_type) {
			case cwt::UniType::BoolValue:
				pattr = & r->addBoolean(itMeta->column_name);
				break;
			case cwt::UniType::IntegerValue:
				pattr = & r->addInteger(itMeta->column_name
						, itMeta->has_size.first ? itMeta->has_size.second : 0
						, itMeta->has_unsigned.first ? itMeta->has_unsigned.second : false);
				break;
			case cwt::UniType::FloatValue:
				pattr = & r->addFloat(itMeta->column_name
						, itMeta->has_unsigned.first ? itMeta->has_unsigned.second : false);
				break;
			case cwt::UniType::DoubleValue:
				pattr = & r->addDouble(itMeta->column_name
						, itMeta->has_unsigned.first ? itMeta->has_unsigned.second : false);
				break;
			case cwt::UniType::StringValue:
				pattr = & r->addString(itMeta->column_name
						, itMeta->has_size.first ? itMeta->has_size.second : 0);
				break;
			case cwt::UniType::BlobValue:
				pattr = & r->addBlob(itMeta->column_name);
				break;
			case cwt::UniType::TimeValue:
				pattr = & r->addTime(itMeta->column_name
						, itMeta->has_timestamp.first ? itMeta->has_timestamp.second : false);
				break;
			case cwt::UniType::DateValue:
				pattr = & r->addDate(itMeta->column_name
						, itMeta->has_timestamp.first ? itMeta->has_timestamp.second : false);
				break;
			case cwt::UniType::DateTimeValue:
				pattr = & r->addDateTime(itMeta->column_name
						, itMeta->has_timestamp.first ? itMeta->has_timestamp.second : false);
				break;
			case cwt::UniType::ObjectValue:
			case cwt::UniType::NullValue:
			default:
				// assert later
				break;
		}

		CWT_ASSERT(pattr);

		if (itMeta->has_pk.first)
			pattr->setPk(itMeta->has_pk.second);

		if (itMeta->has_autoinc.first)
			pattr->setAutoinc(itMeta->has_autoinc.second);

		if (itMeta->has_not_null.first)
			pattr->setNullable(!itMeta->has_not_null.second);

		if (itMeta->has_default_value.first && !pattr->isNullable())
			pattr->setDefault(itMeta->has_default_value.second);
	}

	return r;
}

} // namespace debby

CWT_NS_END

