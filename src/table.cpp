/*
 * table.cpp
 *
 *  Created on: Sep 11, 2013
 *      Author: wladt
 */

#include "../include/cwt/debby/scheme.hpp"
#include <cwt/logger.hpp>

CWT_NS_BEGIN

namespace debby {

Field * Table::addField (const String & name)
{
	Fields::iterator it = m_fields.insert(name, shared_ptr<Field>(new Field()));
	CWT_ASSERT(it != m_fields.end());
	Field * r = it.value().get();
	CWT_ASSERT(r);
	return r;
}

} // namespace debby

CWT_NS_END





