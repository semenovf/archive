/*
 * record.cpp
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

extern cwt::UniType __initial_value(Attribute::Type type);

Attribute::Attribute (Attribute::Type type)
	: _value(__initial_value(type))
{
}

inline uint32_t __toggle_bit(bool b, uint32_t bits, uint32_t bit)
{
	if (b) bits |= bit;
	else   bits &= ~uint32_t(bit);
	return bits;
}

Attribute & Attribute::setPk (bool b)
{
	__toggle_bit(b, _meta._bits, Pk);
	return *this;
}

Attribute & Attribute::setIndexable (bool b)
{
	__toggle_bit(b, _meta._bits, Indexable);
	return *this;
}

Attribute & Attribute::setNullable (bool b)
{
	__toggle_bit(b, _meta._bits, Nullable);
	return *this;
}

Attribute & Attribute::setUnique (bool b)
{
	__toggle_bit(b, _meta._bits, Unique);
	return *this;
}

Attribute & Attribute::setUnsigned (bool b)
{
	__toggle_bit(b, _meta._bits, Unsigned);
	return *this;
}

Attribute & Attribute::setTimestamp (bool b)
{
	__toggle_bit(b, _meta._bits, Timestamp);
	return *this;
}

Attribute & Attribute::setAutoinc (uint_t n)
{
	_meta._autoinc = n;
	return *this;
}

Attribute & Attribute::setSize (size_t size)
{
	_meta._size = size;
	return *this;
}

Attribute & Attribute::setDefault (const cwt::UniType & def)
{
	_meta._default = def;
	return *this;
}


cwt::String Attribute::stringify() const
{
	String r;

	switch(_value.type()) {
	case cwt::UniType::BoolValue:
		r << (_value.toBool() ? "1" : "0");
		break;
	case cwt::UniType::StringValue:
		r << '"' << _value.toString() << '"';
		break;
	case cwt::UniType::IntegerValue:
	case cwt::UniType::FloatValue:
	case cwt::UniType::DoubleValue:
		r << _value.toString();
		break;

	case cwt::UniType::TimeValue:
	case cwt::UniType::DateValue:
	case cwt::UniType::DateTimeValue:
		// TODO how to represent the date/time values - number or string?
		r << _value.toString();
		break;

	case cwt::UniType::BlobValue:
	case cwt::UniType::ObjectValue:
	case cwt::UniType::NullValue:
	default:
		// assert later
		break;
	}

	return r;
}

} // namespace debby

CWT_NS_END


