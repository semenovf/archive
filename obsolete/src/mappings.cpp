/*
 * mappings.cpp
 *
 *  Created on: Nov 18, 2013
 *      Author: wladt
 */


#include "../include/cwt/debby/schema.hpp"
//#include <utility>

CWT_NS_BEGIN

namespace debby
{

struct MappingEntry
{
	cwt::String  _sqlTypeAffinity;
	cwt::UniType _intialValue;

	MappingEntry (const cwt::String & sqlTypeAffinity
			, cwt::UniType intialValue)
		: _sqlTypeAffinity(sqlTypeAffinity)
		, _intialValue(intialValue)
	{}
};


struct Mapping : public cwt::Map<Attribute::Type, MappingEntry>
{
	static Mapping mapping;
	Mapping() {
		mapping << std::make_pair(cwt::UniType::NullValue     , MappingEntry(""        , cwt::UniType()))
				<< std::make_pair(cwt::UniType::BoolValue     , MappingEntry("BOOLEAN" , cwt::UniType(false)))
		        << std::make_pair(cwt::UniType::IntegerValue  , MappingEntry("INTEGER" , cwt::UniType(0)))
		        << std::make_pair(cwt::UniType::FloatValue    , MappingEntry("FLOAT"   , cwt::UniType(float(0))))
		        << std::make_pair(cwt::UniType::DoubleValue   , MappingEntry("DOUBLE"  , cwt::UniType(double(0))))
		        << std::make_pair(cwt::UniType::StringValue   , MappingEntry("TEXT"    , cwt::UniType(cwt::String())))
		        << std::make_pair(cwt::UniType::BlobValue     , MappingEntry("BLOB"    , cwt::UniType(cwt::ByteArray())))
		        << std::make_pair(cwt::UniType::TimeValue     , MappingEntry("TIME"    , cwt::UniType(cwt::Time())))
		        << std::make_pair(cwt::UniType::DateValue     , MappingEntry("DATE"    , cwt::UniType(cwt::Date())))
		        << std::make_pair(cwt::UniType::DateTimeValue , MappingEntry("DATETIME", cwt::UniType(cwt::DateTime())))
		        << std::make_pair(cwt::UniType::ObjectValue   , MappingEntry(""        , cwt::UniType()));
	}
};

Mapping Mapping::mapping;

cwt::String __sql_type_affinity(Attribute::Type type)
{
	return Mapping::mapping[type]._sqlTypeAffinity;
}

cwt::UniType __initial_value(Attribute::Type type)
{
	return Mapping::mapping[type]._intialValue;
}

} // namespace debby

CWT_NS_END
