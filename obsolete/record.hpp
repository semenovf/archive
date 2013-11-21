/*
 * record.hpp
 *
 *  Created on: Nov 11, 2013
 *      Author: wladt
 */

#ifndef __CWT_DEBBY_RECORD_HPP__
#define __CWT_DEBBY_RECORD_HPP__

#include <cwt/string.hpp>
#include <cwt/unitype.hpp>
#include <cwt/map.hpp>

CWT_NS_BEGIN

namespace debby
{

class Table;

class Record
{
	typedef cwt::Map<cwt::String, cwt::UniType> FieldMap;

	Record () : _parent(nullptr) {}

public:
	Record (Table & parent);
	Record (const Record & other);
	cwt::UniType & operator [] (const cwt::String & fname);
	const cwt::UniType & operator [] (const cwt::String & fname) const;

	bool save ();

private:
	Table *      _parent;
	FieldMap     _record;
	cwt::UniType _dummy;
};

} // namespace debby

CWT_NS_END

#endif /* __CWT_DEBBY_RECORD_HPP__ */
