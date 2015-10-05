/*
 * record.cpp
 *
 *  Created on: Nov 11, 2013
 *      Author: wladt
 */

#include "../include/cwt/debby/schema.hpp"
#include "../include/cwt/debby/record.hpp"
#include <cwt/logger.hpp>
#include <cwt/safeformat.hpp>

CWT_NS_BEGIN

namespace debby
{

Record::Record (Table & parent)
	: _parent(& parent)
	, _record()
	, _dummy()
{
	Table::AttributeMap::const_iterator it = _parent->_columns.cbegin();
	Table::AttributeMap::const_iterator itEnd = _parent->_columns.cend();

	for (; it != itEnd; ++it) {
		_record.insert(it->first, it->second->defaultValue());
	}
}

Record::Record (const Record & other)
	: _parent(other._parent)
	, _record(other._record)
	, _dummy()
{
}

cwt::UniType & Record::operator [] (const cwt::String & fname)
{
	FieldMap::iterator it = _record.find(fname);
	if (it != _record.end())
		return it->second;

	return _dummy;
}

const cwt::UniType & Record::operator [] (const cwt::String & fname) const
{
	FieldMap::const_iterator it = _record.find(fname);
	if (it != _record.cend())
		return it->second;

	return _dummy;
}

} // namespace debby

CWT_NS_END


