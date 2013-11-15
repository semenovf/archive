/*
 * unitype.cpp
 *
 *  Created on: Jun 3, 2013
 *      Author: wladt
 */

#include "../include/cwt/unitype.hpp"
#include "../include/cwt/map.hpp"
#include <string.h>

CWT_NS_BEGIN

UniType::SharedData::~SharedData()
{
	switch(type) {
	case StringValue:
		delete d.string_val;
		break;
	case BlobValue:
		delete d.blob_val;
		break;
	case TimeValue:
		delete d.time_val;
		break;
	case DateValue:
		delete d.date_val;
		break;
	case DateTimeValue:
		delete d.datetime_val;
		break;
	case ObjectValue:
		delete d.object_val;
		break;
	default:
		break;
	}
}


UniType::SharedData * UniType::clone()
{
	UniType::SharedData *d = NULL;
	d = new SharedData();
	d->type = m_d->type;

	switch(m_d->type) {
	case UniType::NullValue:
		break;

	case UniType::StringValue:
		*d->d.string_val = *m_d->d.string_val;
		break;

	case UniType::BlobValue:
		*d->d.blob_val = *m_d->d.blob_val;
		break;

	case UniType::FloatValue:
	case UniType::DoubleValue:
		d->d.double_val = m_d->d.double_val;
		break;

	case TimeValue:
		*d->d.time_val = *m_d->d.time_val;
		break;

	case DateValue:
		*d->d.date_val = *m_d->d.date_val;
		break;

	case DateTimeValue:
		*d->d.datetime_val = *m_d->d.datetime_val;
		break;

	case UniType::ObjectValue:
		*d->d.object_val = *m_d->d.object_val;
		break;

	case UniType::BoolValue:
	case UniType::LongValue:
	default:
		d->d.long_val = m_d->d.long_val;
		break;
	}

	return d;
}

/**
 * @brief Set unitype value according to value is string.
 * @details If string can be converted to long, unsigned long, float or double
 * 	then that value will be assigned to unitype,
 * 	otherwise original string will be assigned.
 *
 * @param s string value.
 */
void UniType::setFromString (const String & s)
{
	bool ok;
	// check if string is integer
	ulong_t ulong_val = s.toULong(&ok);
	if (ok) {
		setULong(ulong_val);
		return;
	}

	long_t long_val = s.toLong(&ok);
	if (ok) {
		setLong(long_val);
		return;
	}

	double double_val = s.toDouble(&ok);
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
bool UniType::toBool(bool * ok) const
{
	bool r = true;

	switch(m_d->type) {
	case UniType::NullValue:
		r = false;
		break;
	case UniType::StringValue:
		if (m_d->d.string_val->isEmpty()
				|| m_d->d.string_val->compare(String("no")) == 0
				|| m_d->d.string_val->compare(String("false")) == 0
				|| m_d->d.string_val->compare(String("0")) == 0) {
			r = false;
		}
		break;
	case UniType::BlobValue:
		if (m_d->d.blob_val->length() == 0
				|| strcmp(m_d->d.blob_val->data(), "no") == 0
				|| strcmp(m_d->d.blob_val->data(), "false") == 0
				|| strcmp(m_d->d.blob_val->data(), "0") == 0) {
			r = false;
		}
		break;

	case UniType::FloatValue:
	case UniType::DoubleValue:
		if (m_d->d.double_val == 0.0f)
			r = false;
		break;

	case UniType::BoolValue:
	case UniType::LongValue:
		if (m_d->d.long_val == 0L)
			r = false;
		break;

	case TimeValue:
		if (! m_d->d.time_val->isValid())
			r = false;
		break;

	case DateValue:
		if (! m_d->d.date_val->isValid())
			r = false;
		break;

	case DateTimeValue:
		if (! m_d->d.datetime_val->isValid())
			r = false;
		break;


	case UniType::ObjectValue:
	default:
		if (!(m_d->d.object_val))
			r = false;
		break;
	}

	if (ok)
		*ok = true;

	return r;
}

template <typename int_type>
int_type to_int_type(const UniType &ut, int_type min, int_type max, bool *ok)
{
	bool tmpOk;
	long_t result = ut.toLong(& tmpOk);

	if (tmpOk && result >= long_t(min) && result <= long_t(max)) {
		tmpOk = true;
	}

	if (ok)
		*ok = tmpOk;
	return int_type(result);
}

template <typename uint_type>
uint_type to_uint_type(const UniType & ut, uint_type max, bool *ok)
{
	bool tmpOk;
	ulong_t result = ut.toULong(& tmpOk);

	if (tmpOk && result <= ulong_t(max)) {
		tmpOk = true;
	}

	if (ok)
		*ok = tmpOk;
	return uint_type(result);
}

sbyte_t UniType::toSByte(bool * ok) const
{
	return to_int_type<sbyte_t>(*this, CWT_SBYTE_MIN, CWT_SBYTE_MAX, ok);
}

byte_t UniType::toByte(bool *ok) const
{
	return to_uint_type<byte_t>(*this, CWT_BYTE_MAX, ok);
}

short_t UniType::toShort(bool *ok) const
{
	return to_int_type<short_t>(*this, CWT_SHORT_MIN, CWT_SHORT_MAX, ok);
}

ushort_t UniType::toUShort(bool *ok) const
{
	return to_uint_type<ushort_t>(*this, CWT_USHORT_MAX, ok);
}

int_t UniType::toInt(bool *ok) const
{
	return to_int_type<int_t>(*this, CWT_INT_MIN, CWT_INT_MAX, ok);
}

uint_t UniType::toUInt(bool *ok) const
{
	return to_uint_type<uint_t>(*this, CWT_UINT_MAX, ok);
}

long_t UniType::toLong(bool * ok) const
{
	long_t r = long_t(0);
	bool tmpOk = false;

	switch(m_d->type) {
	case UniType::NullValue:
		break;
	case UniType::StringValue:
		r = m_d->d.string_val->toLong(& tmpOk, 0);
		break;
	case UniType::BlobValue:
		r = m_d->d.blob_val->toLong(& tmpOk, 0);
		break;
	case UniType::FloatValue:
	case UniType::DoubleValue:
		if (m_d->d.double_val >= double(CWT_LONG_MIN) && m_d->d.double_val <= double(CWT_LONG_MAX)) {
			r = long_t(m_d->d.double_val);
			tmpOk = true;
		}
		break;
	case UniType::BoolValue:
	case UniType::LongValue:
		r = m_d->d.long_val;
		tmpOk = true;
		break;

	case TimeValue:
		r = long_t(m_d->d.time_val->millis());
		break;

	case DateValue:
		r = long_t(m_d->d.date_val->julianDay());
		break;

	case DateTimeValue:
		r = long_t(m_d->d.datetime_val->millisSinceEpoch());
		break;

	default:
		break;
	}

	if (ok)
		*ok = tmpOk;

	return r;
}

ulong_t UniType::toULong(bool * ok) const
{
	ulong_t r = long_t(0);
	bool tmpOk = false;

	switch(m_d->type) {
	case UniType::NullValue:
		break;
	case UniType::StringValue:
		r = m_d->d.string_val->toULong(&tmpOk, 0);
		break;
	case UniType::BlobValue:
		r = m_d->d.blob_val->toULong(&tmpOk, 0);
		break;
	case UniType::FloatValue:
	case UniType::DoubleValue:
		if ((m_d->d.double_val >= double(CWT_LONG_MIN) && m_d->d.double_val <= double(CWT_LONG_MAX))
				|| (m_d->d.double_val >= 0.0f && m_d->d.double_val <= double(CWT_ULONG_MAX))) {
			r = ulong_t(m_d->d.double_val);
			tmpOk = true;
		}
		break;
	case UniType::BoolValue:
	case UniType::LongValue:
		r = ulong_t(m_d->d.long_val);
		tmpOk = true;
		break;

	case TimeValue:
		r = ulong_t(m_d->d.time_val->millis());
		break;

	case DateValue:
		r = ulong_t(m_d->d.date_val->julianDay());
		break;

	case DateTimeValue:
		r = ulong_t(m_d->d.datetime_val->millisSinceEpoch());
		break;

	default:
		break;
	}

	if (ok)
		*ok = tmpOk;

	return r;
}

float UniType::toFloat(bool * ok) const
{
	float r = 0.0f;
	bool tmpOk = false;

	switch(m_d->type) {
	case UniType::NullValue:
		break;
	case UniType::StringValue:
		r = m_d->d.string_val->toFloat(& tmpOk);
		break;
	case UniType::BlobValue:
		r = m_d->d.blob_val->toFloat(& tmpOk);
		break;
	case UniType::FloatValue:
	case UniType::DoubleValue:
		if ((m_d->d.double_val >= double(CWT_FLOAT_MIN) && m_d->d.double_val <= double(CWT_FLOAT_MAX))) {
			r = float(m_d->d.double_val);
			tmpOk = true;
		}
		break;
	case UniType::BoolValue:
	case UniType::LongValue:
	case UniType::TimeValue:
	case UniType::DateValue:
	case UniType::DateTimeValue: {
		tmpOk = true;
		double v = double(this->toLong(& tmpOk));

		if (tmpOk) {
			if (v >= double(CWT_FLOAT_MIN) && v <= double(CWT_FLOAT_MAX)) {
				r = float(v);
			} else {
				tmpOk = false;
			}
		}
		}
		break;

	default:
		break;
	}

	if (ok)
		*ok = tmpOk;

	return r;
}

double UniType::toDouble(bool *ok) const
{
	double r = 0.0f;
	bool tmpOk = false;

	switch(m_d->type) {
	case UniType::NullValue:
		break;
	case UniType::StringValue:
		r = m_d->d.string_val->toDouble(&tmpOk);
		break;
	case UniType::BlobValue:
		r = m_d->d.blob_val->toDouble(&tmpOk);
		break;
	case UniType::FloatValue:
	case UniType::DoubleValue:
		r = m_d->d.double_val;
		tmpOk = true;
		break;
	case UniType::BoolValue:
	case UniType::LongValue:
	case UniType::TimeValue:
	case UniType::DateValue:
	case UniType::DateTimeValue:
		r = double(this->toLong(&tmpOk));
		break;

	default:
		break;
	}

	if (ok)
		*ok = tmpOk;

	return r;
}

UChar UniType::toUChar(bool *ok) const
{
	bool tmpOk;
	UChar r(to_uint_type<uint_t>(*this, UChar::MaxCodePoint, &tmpOk));

	if (ok)
		*ok = tmpOk;

	return r;
}

String UniType::toString(bool *ok) const
{
	String r;

	if (ok)
		*ok = false;

	if (m_d->type == UniType::ObjectValue) {
		return String();
	}

	switch(m_d->type) {
	case UniType::NullValue:
		break;
	case UniType::BoolValue:
		r = m_d->d.long_val ? String("true") : String("false");
		break;
	case UniType::StringValue:
		r = *m_d->d.string_val;
		break;
	case UniType::BlobValue:
		r = String::fromUtf8(*m_d->d.blob_val);
		break;
	case UniType::FloatValue:
		r = String::number(float(m_d->d.double_val));
		break;
	case UniType::DoubleValue:
		r = String::number(m_d->d.double_val);
		break;

	case UniType::TimeValue:
		r = m_d->d.time_val->toString();
		break;

	case UniType::DateValue:
		r = m_d->d.date_val->toString();
		break;

	case UniType::DateTimeValue:
		r = m_d->d.datetime_val->toString();
		break;

	case UniType::LongValue:
	default:
		r = String::number(m_d->d.long_val);
		break;
	}

	if (ok)
		*ok = true;

	return r;
}

ByteArray UniType::toBlob(bool * ok) const
{
	ByteArray r;

	if (ok)
		*ok = false;

	if (m_d->type == UniType::ObjectValue) {
		return ByteArray();
	}

	switch(m_d->type) {
	case UniType::NullValue:
		break;
	case UniType::StringValue:
		CWT_ASSERT(m_d->d.string_val);
		r = ByteArray(m_d->d.string_val->data(), m_d->d.string_val->size());
		break;
	case UniType::BlobValue:
		r = *m_d->d.blob_val;
		break;
	case UniType::FloatValue:
		r = ByteArray::number(float(m_d->d.double_val));
		break;
	case UniType::DoubleValue:
		r = ByteArray::number(m_d->d.double_val);
		break;

	case UniType::TimeValue:
		r = ByteArray::number(m_d->d.time_val->millis());
		break;

	case UniType::DateValue:
		r = ByteArray::number(m_d->d.date_val->julianDay());
		break;

	case UniType::DateTimeValue:
		r = ByteArray::number(m_d->d.datetime_val->millisSinceEpoch());
		break;

	case UniType::BoolValue:
	case UniType::LongValue:
	default:
		r = ByteArray::number(m_d->d.long_val);
		break;
	}

	if (ok)
		*ok = true;

	return r;
}


String UniType::typeToString(UniType::TypeEnum t)
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
	return String(strings[t]);
}


struct UniTypeStringAffinity: public Map<String, UniType::TypeEnum>
{
	static UniTypeStringAffinity affinity;

	UniTypeStringAffinity() {
		affinity << std::make_pair(String("null")     , UniType::NullValue)
				 << std::make_pair(String("bool")     , UniType::BoolValue)
		         << std::make_pair(String("boolean")  , UniType::BoolValue)
		         << std::make_pair(String("int")      , UniType::IntegerValue)
		         << std::make_pair(String("integer")  , UniType::IntegerValue)
		         << std::make_pair(String("float")    , UniType::FloatValue)
		         << std::make_pair(String("double")   , UniType::DoubleValue)
		         << std::make_pair(String("string")   , UniType::StringValue)
		         << std::make_pair(String("blob")     , UniType::BlobValue)
		         << std::make_pair(String("bin")      , UniType::BlobValue)
		         << std::make_pair(String("binary")   , UniType::BlobValue)
		         << std::make_pair(String("time")     , UniType::TimeValue)
		         << std::make_pair(String("date")     , UniType::DateValue)
		         << std::make_pair(String("datetime") , UniType::DateTimeValue)
		         << std::make_pair(String("obj")      , UniType::ObjectValue)
		         << std::make_pair(String("object")   , UniType::ObjectValue);
	}
};

UniTypeStringAffinity UniTypeStringAffinity::affinity;

UniType::TypeEnum UniType::typeFromString(const String & s)
{
	return UniTypeStringAffinity::affinity.value(s.toLower(), NullValue);
}

CWT_NS_END
