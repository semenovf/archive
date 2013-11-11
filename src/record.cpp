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

enum {
      SthForLoad
	, SthForCreate
	, SthForUpdate
	, SthForDelet
};

const cwt::String Record::PkName = "id";

Attribute::Attribute (const cwt::String & name, const cwt::UniType & value)
	: _name(name)
	, _value(value)
	, _meta()
{
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

Record::Record (const cwt::String & name, cwt::UniType::TypeEnum pkType)
	: _name(name)
	, _pk(pkType != cwt::UniType::NullValue ? PkName : cwt::String(), cwt::UniType())
{
	if (pkType != cwt::UniType::NullValue) {
		_pk._meta.insert(TypeKey, cwt::UniType::toStringType(pkType));
		_pk._meta.insert(PkKey, true);
		_pk._meta.insert(AutoincKey, 0);
	}
	_sthPool[0] = _sthPool[1] = _sthPool[2] = _sthPool[3] = nullptr;
}

Record::~Record ()
{
	AttributeMap::iterator it = _attrs.begin();
	AttributeMap::iterator itEnd = _attrs.end();
	for (; it != itEnd; ++it) {
		delete it->second;
		it->second = nullptr;
	}

	for (int i = 0; i < 4; ++i) {
		if (_sthPool[i]) {
			delete _sthPool[i];
			_sthPool[i] = nullptr;
		}
	}
}

Attribute & Record::addBoolean (const cwt::String & name)
{
	Attribute * attr = new Attribute(name, cwt::UniType());
	attr->_meta.insert(TypeKey, cwt::UniType::toStringType(cwt::UniType::BoolValue));
	_attrs.insert(name, attr);
	return *attr;
}

Attribute & Record::addInteger (const cwt::String & name, size_t size, bool isUnsigned)
{
	return addDecimal(name, size, 0, isUnsigned);
}

Attribute & Record::addDecimal (const cwt::String & name, size_t size, size_t decimals, bool isUnsigned)
{
	Attribute * attr = new Attribute(name, cwt::UniType());
	attr->_meta.insert(TypeKey, cwt::UniType::toStringType(cwt::UniType::IntegerValue));

	if (size)
		attr->_meta.insert(SizeKey, size);

	if (decimals)
		attr->_meta.insert(DecimalsKey, decimals);

	if (isUnsigned)
		attr->_meta.insert(UnsignedKey, isUnsigned);

	_attrs.insert(name, attr);
	return *attr;
}

Attribute & Record::addFloat (const cwt::String & name, bool isUnsigned)
{
	Attribute * attr = new Attribute(name, cwt::UniType());
	attr->_meta.insert(TypeKey, cwt::UniType::toStringType(cwt::UniType::FloatValue));
	if (isUnsigned)
		attr->_meta.insert(UnsignedKey, isUnsigned);
	_attrs.insert(name, attr);
	return *attr;
}

Attribute & Record::addDouble (const cwt::String & name, bool isUnsigned)
{
	Attribute * attr = new Attribute(name, cwt::UniType());
	attr->_meta.insert(TypeKey, cwt::UniType::toStringType(cwt::UniType::DoubleValue));
	if (isUnsigned)
		attr->_meta.insert(UnsignedKey, isUnsigned);
	_attrs.insert(name, attr);
	return *attr;
}

Attribute & Record::addString (const cwt::String & name, size_t size)
{
	Attribute * attr = new Attribute(name, cwt::UniType());
	attr->_meta.insert(TypeKey, cwt::UniType::toStringType(cwt::UniType::StringValue));
	if (size)
		attr->_meta.insert(SizeKey, size);
	_attrs.insert(name, attr);
	return *attr;
}

Attribute & Record::addBlob (const cwt::String & name)
{
	Attribute * attr = new Attribute(name, cwt::UniType());
	attr->_meta.insert(TypeKey, cwt::UniType::toStringType(cwt::UniType::BlobValue));
	_attrs.insert(name, attr);
	return *attr;
}

Attribute & Record::addDate (const cwt::String & name, bool isTimeStamp)
{
	Attribute * attr = new Attribute(name, cwt::UniType());
	attr->_meta.insert(TypeKey, cwt::UniType::toStringType(cwt::UniType::DateValue));
	if (isTimeStamp)
		attr->_meta.insert(Timestamp, true);
	_attrs.insert(name, attr);
	return *attr;
}

Attribute & Record::addTime (const cwt::String & name, bool isTimeStamp)
{
	Attribute * attr = new Attribute(name, cwt::UniType());
	attr->_meta.insert(TypeKey, cwt::UniType::toStringType(cwt::UniType::TimeValue));
	if (isTimeStamp)
		attr->_meta.insert(Timestamp, true);
	_attrs.insert(name, attr);
	return *attr;
}

Attribute & Record::addDateTime (const cwt::String & name, bool isTimeStamp)
{
	Attribute * attr = new Attribute(name, cwt::UniType());
	attr->_meta.insert(TypeKey, cwt::UniType::toStringType(cwt::UniType::DateTimeValue));
	if (isTimeStamp)
		attr->_meta.insert(Timestamp, true);
	_attrs.insert(name, attr);
	return *attr;
}

Attribute & Record::addRef (const cwt::String & name, const Record & refrecord)
{
	Attribute * attr = new Attribute(name, cwt::UniType());
	CWT_ASSERT(!refrecord.isInvalid());
	attr->_meta.insert(TypeKey, cwt::UniType::toStringType(refrecord.pk().type()));
	attr->_meta.insert(RefKey, refrecord.name());
	_attrs.insert(name, attr);
	return *attr;
}

void Record::setAutoinc (uint_t n)
{
	_pk._meta[AutoincKey] = n;
}

uint_t Record::autoinc () const
{
	return uint_t(_pk._meta[AutoincKey].integer());
}

const Attribute & Record::operator [] (const cwt::String & name) const
{
	if (name == PkName)
		return _pk;
	AttributeMap::const_iterator it = _attrs.find(name);
	return it == _attrs.cend() ? _dummy : *it->second;
}

Attribute & Record::operator [] (const cwt::String & name)
{
	if (name == PkName)
		return _pk;
	AttributeMap::const_iterator it = _attrs.find(name);
	return it == _attrs.cend() ? _dummy : *it->second;
}


/***************************************************************/

cwt::String Record::buildSqlForCreate () const
{
	cwt::String r("INSERT INTO");

	r << ' ' << _name;

	cwt::String columns;
	cwt::String values;
	bool needComma = false;

	if (!_pk.isAutoinc()) {
		columns << '`' << _pk.name() << '`';
		values << '?';
		needComma = true;
	}

	AttributeMap::const_iterator it = _attrs.cbegin();
	AttributeMap::const_iterator itEnd = _attrs.cend();
	for (; it != itEnd; ++it) {
		if (needComma) {
			columns << ", ";
			values << ", ";
		}
		columns << '`' << it->first << '`';
		values << '?';
		needComma = true;
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


	if (!_pk.isAutoinc()) {
		_sthPool[SthForCreate]->bind(_pk.value());
	}

	AttributeMap::const_iterator it = _attrs.cbegin();
	AttributeMap::const_iterator itEnd = _attrs.cend();

	for (; it != itEnd; ++it) {
		_sthPool[SthForCreate]->bind(it->second->value());
	}

	return _sthPool[SthForCreate]->exec();
}

} // namespace debby

CWT_NS_END


