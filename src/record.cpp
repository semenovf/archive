/*
 * record.cpp
 *
 *  Created on: Nov 11, 2013
 *      Author: wladt
 */

#include "../include/cwt/debby/dbh.hpp"
#include "../include/cwt/debby/record.hpp"
#include <cwt/logger.hpp>
#include <cwt/safeformat.hpp>

CWT_NS_BEGIN

namespace debby
{

static const cwt::String PkKey       ("pk");
static const cwt::String AutoincKey  ("autoinc");
static const cwt::String TypeKey     ("type");
static const cwt::String UniqueKey   ("unique");
static const cwt::String NullableKey ("nullable");
static const cwt::String UnsignedKey ("unsigned");
static const cwt::String SizeKey     ("size");
static const cwt::String Timestamp   ("timestamp");
static const cwt::String DecimalsKey ("decimals");
static const cwt::String RefKey      ("ref");
static const cwt::String DefaultKey  ("default");

Attribute & Attribute::setPk(bool b)
{
	_meta.insert(PkKey, b ? true : false);
	return *this;
}

Attribute & Attribute::setAutoinc (uint_t n)
{
	_meta.insert(AutoincKey, n);
	return * this;
}

Attribute & Attribute::setNullable(bool b)
{
	_meta.insert(NullableKey, b ? true : false);
	return *this;
}

Attribute & Attribute::setUnique(bool b)
{
	_meta.insert(UniqueKey, b ? true : false);
	return *this;
}

Attribute & Attribute::setDefault  (const cwt::UniType & defaultValue)
{
	bool ok;
	_meta.insert(DefaultKey, defaultValue.toString(&ok));
	CWT_ASSERT(ok);
	return *this;
}

inline bool Attribute::isBooleanSet(const cwt::String & optname) const
{
	MetaType::const_iterator it = _meta.find(optname);
	return it == _meta.cend() ? false : it->second.boolean();
}

inline bool Attribute::hasOpt (const cwt::String & optname) const
{
	MetaType::const_iterator it = _meta.find(optname);
	return it == _meta.cend() ? false : true;
}

bool Attribute::isPk () const
{
	return isBooleanSet(PkKey);
}

bool Attribute::isAutoinc  () const
{
	return autoinc() != 0;
}

uint_t Attribute::autoinc () const
{
	return hasOpt(AutoincKey)
			? _meta[AutoincKey].integer()
			: 0;
}

bool Attribute::isNullable() const
{
	return isBooleanSet(NullableKey);
}

bool Attribute::isUnique() const
{
	return isBooleanSet(UniqueKey);
}

bool Attribute::isUnsigned() const
{
	return isBooleanSet(UnsignedKey);
}

bool Attribute::hasDefault() const
{
	return hasOpt(DefaultKey);
}

cwt::UniType Attribute::defaultValue () const
{
	return hasDefault() ?  _meta[DefaultKey] : cwt::UniType();
}

cwt::UniType::TypeEnum Attribute::type () const
{
	return cwt::UniType::fromStringType(_meta[TypeKey].string());
}

cwt::String Attribute::stringifyValue(const Attribute & attr)
{
	String r;

	switch(attr.type()) {
	case cwt::UniType::StringValue:
		r << '"' << attr._value.toString() << '"';
		break;

	case cwt::UniType::BoolValue:
	case cwt::UniType::IntegerValue:
	case cwt::UniType::FloatValue:
	case cwt::UniType::DoubleValue:
		r << attr._value.toString();
		break;

	case cwt::UniType::TimeValue:
	case cwt::UniType::DateValue:
	case cwt::UniType::DateTimeValue:
		// TODO how to represent the date/time values - number or string?
		r << attr._value.toString();
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


Record::~Record ()
{
	AttributeMap::iterator it = _attrs.begin();
	AttributeMap::iterator itEnd = _attrs.end();
	for (; it != itEnd; ++it) {
		delete it->second;
		it->second = nullptr;
	}
}

inline Attribute & Record::add (const cwt::String & name)
{
	CWT_ASSERT(!_attrs.contains(name));
	Attribute * pattr = new Attribute;
	_attrs.insert(name, pattr);
	return *pattr;
}

Attribute & Record::addBoolean (const cwt::String & name)
{
	Attribute & attr = this->add(name);
	attr._meta.insert(TypeKey, cwt::UniType::toStringType(cwt::UniType::BoolValue));
	return attr;
}

Attribute & Record::addInteger (const cwt::String & name, size_t size, bool isUnsigned)
{
	return addDecimal(name, size, 0, isUnsigned);
}

Attribute & Record::addDecimal (const cwt::String & name, size_t size, size_t decimals, bool isUnsigned)
{
	Attribute & attr = this->add(name);
	attr._meta.insert(TypeKey, cwt::UniType::toStringType(cwt::UniType::IntegerValue));

	if (size)
		attr._meta.insert(SizeKey, size);

	if (decimals)
		attr._meta.insert(DecimalsKey, decimals);

	if (isUnsigned)
		attr._meta.insert(UnsignedKey, isUnsigned);

	return attr;
}

Attribute & Record::addFloat (const cwt::String & name, bool isUnsigned)
{
	Attribute & attr = this->add(name);
	attr._meta.insert(TypeKey, cwt::UniType::toStringType(cwt::UniType::FloatValue));
	if (isUnsigned)
		attr._meta.insert(UnsignedKey, isUnsigned);
	return attr;
}

Attribute & Record::addDouble (const cwt::String & name, bool isUnsigned)
{
	Attribute & attr = this->add(name);
	attr._meta.insert(TypeKey, cwt::UniType::toStringType(cwt::UniType::DoubleValue));
	if (isUnsigned)
		attr._meta.insert(UnsignedKey, isUnsigned);
	return attr;
}

Attribute & Record::addString (const cwt::String & name, size_t size)
{
	Attribute & attr = this->add(name);
	attr._meta.insert(TypeKey, cwt::UniType::toStringType(cwt::UniType::StringValue));
	if (size)
		attr._meta.insert(SizeKey, size);
	return attr;
}

Attribute & Record::addBlob (const cwt::String & name)
{
	Attribute & attr = this->add(name);
	attr._meta.insert(TypeKey, cwt::UniType::toStringType(cwt::UniType::BlobValue));
	return attr;
}

Attribute & Record::addDate (const cwt::String & name, bool isTimeStamp)
{
	Attribute & attr = this->add(name);
	attr._meta.insert(TypeKey, cwt::UniType::toStringType(cwt::UniType::DateValue));
	if (isTimeStamp)
		attr._meta.insert(Timestamp, true);
	return attr;
}

Attribute & Record::addTime (const cwt::String & name, bool isTimeStamp)
{
	Attribute & attr = this->add(name);
	attr._meta.insert(TypeKey, cwt::UniType::toStringType(cwt::UniType::TimeValue));
	if (isTimeStamp)
		attr._meta.insert(Timestamp, true);
	return attr;
}

Attribute & Record::addDateTime (const cwt::String & name, bool isTimeStamp)
{
	Attribute & attr = this->add(name);
	attr._meta.insert(TypeKey, cwt::UniType::toStringType(cwt::UniType::DateTimeValue));
	if (isTimeStamp)
		attr._meta.insert(Timestamp, true);
	return attr;
}

Attribute & Record::addFromAttr (const cwt::String & name, const Attribute & other)
{
	Attribute & attr = this->add(name);
	attr = other;
	attr.setPk(false);
	attr.setAutoinc(0);
	return attr;
}

const Attribute & Record::operator [] (const cwt::String & name) const
{
	AttributeMap::const_iterator it = _attrs.find(name);
	return it == _attrs.cend() ? _dummy : *it->second;
}

Attribute & Record::operator [] (const cwt::String & name)
{
	AttributeMap::const_iterator it = _attrs.find(name);
	return it == _attrs.cend() ? _dummy : *it->second;
}

} // namespace debby

CWT_NS_END


