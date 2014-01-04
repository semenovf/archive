/*
 * unitype.cpp
 *
 *  Created on: Jun 3, 2013
 *      Author: wladt
 */

#include "../include/pfs/unitype.hpp"
#include <pfs/map.hpp>
#include <string.h>

namespace pfs {

long_t unidata_blob::toLong (bool & ok) const
{
	const long_t * r = reinterpret_cast<const long_t *>(_value.constData());
	if (_value.size() == 0) {
		ok = true;
		return long_t(0);
	} else if (_value.size() <= sizeof(long_t)) {
		ok = true;
	} else {
		ok = false;
	}
	return *r;
}

double unidata_blob::toDouble (bool & ok) const
{
	const double * r = reinterpret_cast<const double *>(_value.constData());
	if (_value.size() == 0) {
		ok = true;
		return double(0);
	} else if (_value.size() <= sizeof(double)) {
		ok = true;
	} else {
		ok = false;
	}
	return *r;
}

string unidata_blob::toString (bool & ok) const
{
	string::ConvertState state;
	string r(string::fromUtf8 (_value, & state));

	ok = state.invalidChars == 0 && state.nremain == 0;
	return r;
}


bool unitype::toBool (bool * pok) const
{
	bool ok;
	bool r = _pimpl->toBool(ok);
	if (pok) *pok = ok;
	return ok ? r : false;
}

byte_t unitype::toByte (bool * pok) const
{
	bool ok;
	long_t r = _pimpl->toLong(ok);
	ok = (r >= 0 && r <= PFS_BYTE_MAX) ? ok : false;
	if (pok) *pok = ok;
	return ok ? byte_t(r) : 0;
}

sbyte_t unitype::toSByte (bool * pok) const
{
	bool ok;
	long_t r = _pimpl->toLong(ok);
	ok = (r >= PFS_SBYTE_MIN && r <= PFS_SBYTE_MAX) ? ok : false;
	if (pok) *pok = ok;
	return ok ? sbyte_t(r) : 0;
}

short_t unitype::toShort (bool * pok) const
{
	bool ok;
	long_t r = _pimpl->toLong(ok);
	ok = (r >= PFS_SHORT_MIN && r <= PFS_SHORT_MAX) ? ok : false;
	if (pok) *pok = ok;
	return ok ? short_t(r) : 0;
}

ushort_t unitype::toUShort (bool * pok) const
{
	bool ok;
	long_t r = _pimpl->toLong(ok);
	ok = (r >= 0 && r <= PFS_USHORT_MAX) ? ok : false;
	if (pok) *pok = ok;
	return ok ? ushort_t(r) : 0;
}

int_t unitype::toInt (bool * pok) const
{
	bool ok;
	long_t r = _pimpl->toLong(ok);
	ok = (r >= PFS_INT_MIN && r <= PFS_INT_MAX) ? ok : false;
	if (pok) *pok = ok;
	return ok ? int_t(r) : 0;
}

uint_t unitype::toUInt (bool * pok) const
{
	bool ok;
	long_t r = _pimpl->toLong(ok);
	ok = (r >= 0 && r <= PFS_UINT_MAX) ? ok : false;
	if (pok) *pok = ok;
	return ok ? uint_t(r) : 0;
}

long_t unitype::toLong (bool * pok) const
{
	bool ok;
	long_t r = _pimpl->toLong(ok);
	if (pok) *pok = ok;
	return r;
}

ulong_t unitype::toULong (bool * pok) const
{
	bool ok;
	long_t r = _pimpl->toLong(ok);
	if (pok) *pok = ok;
	return ulong_t(r);
}


float unitype::toFloat (bool * pok) const
{
	bool ok;
	double r = _pimpl->toDouble(ok);
	ok = (r >= double(PFS_FLOAT_MIN) && r <= double(PFS_FLOAT_MAX)) ? ok : false;
	if (pok) *pok = ok;
	return ok ? float(r) : float(0);
}

double unitype::toDouble (bool * pok) const
{
	bool ok;
	double r = _pimpl->toDouble(ok);
	if (pok) *pok = ok;
	return ok ? r : double(0);
}

ucchar unitype::toUCChar (bool * pok) const
{
	bool ok;
	long_t r = _pimpl->toLong(ok);
	ok = (r >= 0 && r <= ucchar::MaxCodePoint) ? ok : false;
	if (pok) *pok = ok;
	return ok ? ucchar(uint32_t(r)) : ucchar(ucchar::Null);
}


string unitype::toString(bool * pok) const
{
	bool ok;
	string r = _pimpl->toString(ok);
	if (pok) *pok = ok;
	return ok ? r : string();
}

bytearray unitype::toBlob(bool * pok) const
{
	bool ok;
	bytearray r = _pimpl->toBlob(ok);
	if (pok) *pok = ok;
	return ok ? r : bytearray();
}

/**
 * @brief Set unitype value according to value is string.
 * @details If string can be converted to long, unsigned long, float or double
 * 	then that value will be assigned to unitype,
 * 	otherwise original string will be assigned.
 *
 * @param s string value.
 */
void unitype::setFromString (const string & s)
{
	bool ok;
	// check if string is integer
	ulong_t ulong_val = s.toULong(& ok);
	if (ok) {
		setULong(ulong_val);
		return;
	}

	long_t long_val = s.toLong(& ok);
	if (ok) {
		setLong(long_val);
		return;
	}

	double double_val = s.toDouble(& ok);
	if (ok) {
		setDouble(double_val);
		return;
	}

	// TODO need to set from string Time, Date and DateTime instances

	setString(s);
}

/**
 * @brief Cast unitype to boolean value.
 *
 * @param ok always stores @c true if ok is not null.
 * @return @c false
 * 	if unitype is NullValue;
 * 	if unitype is StringValue and it is empty, equals to "no", "false" or "0";
 * 	if unitype is BlobValue and it is empty, equals to "no", "false" or "0";
 * 	if unitype is FloatValue and it equals to 0.0;
 * 	if unitype is DoubleValue and it equals to 0.0;
 * 	if unitype is integer value and it equals to 0;
 * 	if unitype is Time, Date or DateTime value and it is invalid;
 * 	if unitype is ObjectValue is null;
 * 	in other cases return @c true.
 */

#ifdef __COMMENT__
string unitype::typeToString(unitype::Type t)
{
	static const char * strings[] = {
		  "null"
		, "boolean"
		, "integer"
		, "float"
		, "double"
		, "string"
		, "blob"
		, "time"
		, "date"
		, "datetime"
		, "object"
	};
	return string::fromLatin1(strings[t]);
}


struct UniTypeStringAffinity: public map<string, unitype::Type>
{
	static UniTypeStringAffinity affinity;

	UniTypeStringAffinity() {
		affinity << std::make_pair(string("null")     , unidata::NullValue)
				 << std::make_pair(string("bool")     , unidata::BoolValue)
		         << std::make_pair(string("boolean")  , unidata::BoolValue)
		         << std::make_pair(string("int")      , unidata::IntegerValue)
		         << std::make_pair(string("integer")  , unidata::IntegerValue)
		         << std::make_pair(string("float")    , unidata::FloatValue)
		         << std::make_pair(string("double")   , unidata::DoubleValue)
		         << std::make_pair(string("string")   , unidata::StringValue)
		         << std::make_pair(string("blob")     , unidata::BlobValue)
		         << std::make_pair(string("bin")      , unidata::BlobValue)
		         << std::make_pair(string("binary")   , unidata::BlobValue)
		         << std::make_pair(string("time")     , unidata::TimeValue)
		         << std::make_pair(string("date")     , unidata::DateValue)
		         << std::make_pair(string("datetime") , unidata::DateTimeValue)
		         << std::make_pair(string("obj")      , unidata::ObjectValue)
		         << std::make_pair(string("object")   , unidata::ObjectValue);
	}
};

UniTypeStringAffinity UniTypeStringAffinity::affinity;

unidata::type unitype::typeFromString(const string & s)
{
	return UniTypeStringAffinity::affinity.value(s.toLower(), unidata::NullValue);
}
#endif

} // pfs
