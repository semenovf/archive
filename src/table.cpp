/*
 * table.cpp
 *
 *  Created on: Sep 11, 2013
 *      Author: wladt
 */

#include "../include/cwt/debby/scheme.hpp"
#include <cwt/logger.hpp>

CWT_NS_BEGIN

DebbyField * DebbyTable::addField (const String & name, DebbyField * field)
{
	DebbyFields::iterator it = m_fields.insert(name, shared_ptr<DebbyField>(field));
	CWT_ASSERT(it != m_fields.end());
	DebbyField * r = it.value().get();
	CWT_ASSERT(r);
	return r;
}

DebbyField * DebbyTable::addBooleanField (const String & name)
{
	return addField(name, new DebbyField(DebbyBool));
}

DebbyField * DebbyTable::addNumberField (const String & name, ulong_t max)
{
	CWT_UNUSED(max);
	return addField(name, new DebbyField(DebbyInteger));
}

DebbyField * DebbyTable::addNumberField (const String & name, long_t min, long_t max)
{
	CWT_UNUSED2(min, max);
	return addField(name, new DebbyField(DebbyInteger));
}

DebbyField * DebbyTable::addNumberField (const String & name, double min, double max)
{
	DebbyField * r;

	if (min < CWT_FLOAT_MIN || max > CWT_FLOAT_MAX)
		r = addField(name, new DebbyField(DebbyDouble));
	else
		r = addField(name, new DebbyField(DebbyFloat));

	return r;
}

DebbyField * DebbyTable::addStringField (const String & name, size_t maxLength)
{
	CWT_UNUSED(maxLength);
	return addField(name, new DebbyField(DebbyString));
}

DebbyField * DebbyTable::addBlobField (const String & name)
{
	return addField(name, new DebbyField(DebbyBlob));
}

DebbyField * DebbyTable::addDateField (const String & name)
{
	return addField(name, new DebbyField(DebbyDate));
}

DebbyField * DebbyTable::addTimeField (const String & name)
{
	return addField(name, new DebbyField(DebbyTime));
}

DebbyField * DebbyTable::addDateTimeField (const String & name)
{
	return addField(name, new DebbyField(DebbyDateTime));
}

DebbyField * DebbyField::addTimeStampField (const String & name)
{
	return addField(name, new DebbyField(DebbyTimeStamp));
}

DebbyField * DebbyTable::addRefField (const String & name, const DebbyTable * reftable)
{
	DebbyField * r;
	r = addField(name, new DebbyField());
	r->setRefTable(reftable);
	return r;
}

CWT_NS_END





