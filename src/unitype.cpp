/*
 * unitype.cpp
 *
 *  Created on: Jun 3, 2013
 *      Author: wladt
 */

#include "../include/cwt/unitype.hpp"
#include <cstring>

CWT_NS_BEGIN


UniType::Data::~Data()
{
	switch(type) {
	case StringValue:
		delete d.string_val;
		break;
	case BlobValue:
		delete d.blob_val;
		break;
	case ObjectValue:
		delete d.object_val;
		break;
	default:
		break;
	}
}


UniType::Data* UniType::clone()
{
	UniType::Data *d = NULL;
	d = new Data();
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
		d->d.float_val = m_d->d.float_val;
		break;
	case UniType::DoubleValue:
		d->d.double_val = m_d->d.double_val;
		break;
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
void UniType::setFromString (const String &s)
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
 * 	in other cases return @c true.
 */
bool UniType::toBool(bool *ok) const
{
	bool result = true;

	switch(m_d->type) {
	case UniType::NullValue:
		result = false;
		break;
	case UniType::StringValue:
		if (m_d->d.string_val->isEmpty()
				|| m_d->d.string_val->compare(_U("no")) == 0
				|| m_d->d.string_val->compare(_U("false")) == 0
				|| m_d->d.string_val->compare(_U("0")) == 0) {
			result = false;
		}
		break;
	case UniType::BlobValue:
		if (m_d->d.blob_val->length() == 0
				|| ::strcmp(m_d->d.blob_val->data(), "no") == 0
				|| ::strcmp(m_d->d.blob_val->data(), "false") == 0
				|| ::strcmp(m_d->d.blob_val->data(), "0") == 0) {
			result = false;
		}
		break;
	case UniType::FloatValue:
		if (m_d->d.float_val == 0.0f)
			result = false;
		break;
	case UniType::DoubleValue:
		if (m_d->d.double_val == 0.0f)
			result = false;
		break;
	case UniType::LongValue:
		if (m_d->d.long_val == 0L)
			result = false;
		break;
	case UniType::ObjectValue:
	default:
		if (!(m_d->d.object_val && m_d->d.object_val->ptr))
			result = false;
		break;
	}

	if (ok)
		*ok = true;

	return result;
}

template <typename int_type>
int_type to_int_type(const UniType &ut, int_type min, int_type max, bool *ok)
{
	bool tmpOk;
	long_t result = ut.toLong(&tmpOk);

	if (tmpOk && result >= long_t(min) && result <= long_t(max)) {
		tmpOk = true;
	}

	if (ok)
		*ok = tmpOk;
	return int_type(result);
}

template <typename uint_type>
uint_type to_uint_type(const UniType &ut, uint_type max, bool *ok)
{
	bool tmpOk;
	ulong_t result = ut.toULong(&tmpOk);

	if (tmpOk && result <= ulong_t(max)) {
		tmpOk = true;
	}

	if (ok)
		*ok = tmpOk;
	return uint_type(result);
}

sbyte_t UniType::toSByte(bool *ok) const
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

long_t UniType::toLong(bool *ok) const
{
	long_t result = long_t(0);
	bool tmpOk = false;

	switch(m_d->type) {
	case UniType::NullValue:
		break;
	case UniType::StringValue:
		result = m_d->d.string_val->toLong(&tmpOk, 0);
		break;
	case UniType::BlobValue:
		result = m_d->d.blob_val->toLong(&tmpOk, 0);
		break;
	case UniType::FloatValue:
		result = long_t(m_d->d.float_val);
		tmpOk = true;
		break;
	case UniType::DoubleValue:
		if (m_d->d.double_val >= double(CWT_LONG_MIN) && m_d->d.double_val <= double(CWT_LONG_MAX)) {
			result = long_t(m_d->d.double_val);
			tmpOk = true;
		}
		break;
	case UniType::LongValue:
		result = m_d->d.long_val;
		tmpOk = true;
		break;
	case UniType::ObjectValue:
	default:
		break;
	}

	if (ok)
		*ok = tmpOk;

	return result;
}

ulong_t UniType::toULong(bool *ok) const
{
	ulong_t result = long_t(0);
	bool tmpOk = false;

	switch(m_d->type) {
	case UniType::NullValue:
		break;
	case UniType::StringValue:
		result = m_d->d.string_val->toULong(&tmpOk, 0);
		break;
	case UniType::BlobValue:
		result = m_d->d.blob_val->toULong(&tmpOk, 0);
		break;
	case UniType::FloatValue:
		result = long_t(m_d->d.float_val);
		tmpOk = true;
		break;
	case UniType::DoubleValue:
		if ((m_d->d.double_val >= double(CWT_LONG_MIN) && m_d->d.double_val <= double(CWT_LONG_MAX))
				|| (m_d->d.double_val >= 0.0f && m_d->d.double_val <= double(CWT_ULONG_MAX))) {
			result = ulong_t(m_d->d.double_val);
			tmpOk = true;
		}
		break;
	case UniType::LongValue:
		result = ulong_t(m_d->d.long_val);
		tmpOk = true;
		break;
	case UniType::ObjectValue:
	default:
		break;

	}

	if (ok)
		*ok = tmpOk;

	return result;
}

float UniType::toFloat(bool *ok) const
{
	float result = 0.0f;
	bool tmpOk = false;

	switch(m_d->type) {
	case UniType::NullValue:
		break;
	case UniType::StringValue:
		result = m_d->d.string_val->toFloat(&tmpOk);
		break;
	case UniType::BlobValue:
		result = m_d->d.blob_val->toFloat(&tmpOk);
		break;
	case UniType::FloatValue:
		result = m_d->d.float_val;
		tmpOk = true;
		break;
	case UniType::DoubleValue:
		if ((m_d->d.double_val >= double(CWT_FLOAT_MIN) && m_d->d.double_val <= double(CWT_FLOAT_MAX))) {
			result = float(m_d->d.double_val);
			tmpOk = true;
		}
		break;
	case UniType::LongValue:
		if ((m_d->d.long_val >= long_t(CWT_FLOAT_MIN) && m_d->d.double_val <= long_t(CWT_FLOAT_MAX))) {
			result = float(m_d->d.long_val);
			tmpOk = true;
		}
		break;
	case UniType::ObjectValue:
	default:
		break;
	}

	if (ok)
		*ok = tmpOk;

	return result;
}

double UniType::toDouble(bool *ok) const
{
	double result = 0.0f;
	bool tmpOk = false;

	switch(m_d->type) {
	case UniType::NullValue:
		break;
	case UniType::StringValue:
		result = m_d->d.string_val->toDouble(&tmpOk);
		break;
	case UniType::BlobValue:
		result = m_d->d.blob_val->toDouble(&tmpOk);
		break;
	case UniType::FloatValue:
		result = double(m_d->d.float_val);
		tmpOk = true;
		break;
	case UniType::DoubleValue:
		result = m_d->d.double_val;
		tmpOk = true;
		break;
	case UniType::LongValue:
		result = double(m_d->d.long_val);
		tmpOk = true;
		break;
	case UniType::ObjectValue:
	default:
		break;
	}

	if (ok)
		*ok = tmpOk;

	return result;
}

Char UniType::toChar(bool *ok) const
{
	bool tmpOk;
	Char result(to_uint_type<uint_t>(*this, Char::MaxCodePoint, &tmpOk));

	if (ok)
		*ok = tmpOk;

	return result;
}

String UniType::toString(bool *ok) const
{
	String result;

	if (ok)
		*ok = false;

	if (m_d->type == UniType::ObjectValue) {
		return String();
	}

	switch(m_d->type) {
	case UniType::NullValue:
		break;
	case UniType::StringValue:
		result = *m_d->d.string_val;
		break;
	case UniType::BlobValue:
		result = String::fromUtf8(*m_d->d.blob_val);
		break;
	case UniType::FloatValue:
		result = String::number(m_d->d.float_val);
		break;
	case UniType::DoubleValue:
		result = String::number(m_d->d.double_val);
		break;
	case UniType::LongValue:
	default:
		result = String::number(m_d->d.long_val);
		break;
	}

	if (ok)
		*ok = true;

	return result;
}

ByteArray UniType::toBlob(bool *ok) const
{
	ByteArray result;

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
		result = m_d->d.string_val->toUtf8();
		break;
	case UniType::BlobValue:
		result = *m_d->d.blob_val;
		break;
	case UniType::FloatValue:
		result = ByteArray::number(m_d->d.float_val);
		break;
	case UniType::DoubleValue:
		result = ByteArray::number(m_d->d.double_val);
		break;
	case UniType::LongValue:
	default:
		result = ByteArray::number(m_d->d.long_val);
		break;
	}

	if (ok)
		*ok = true;

	return result;
}

template <typename T>
T* UniType::toObject(bool *ok) const
{
	if (ok) *ok = false;

	if (m_d->type == ObjectValue) {
		if (ok) *ok = true;
		return *reinterpret_cast<T*>(m_d->d.object_val);
	}
	return NULL;
}



CWT_NS_END
