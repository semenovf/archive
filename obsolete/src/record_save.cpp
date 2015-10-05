/*
 * record_create.cpp
 *
 *  Created on: Nov 12, 2013
 *      Author: wladt
 */

#include "../include/cwt/debby/record.hpp"
#include "../include/cwt/debby/schema.hpp"
#include <cwt/safeformat.hpp>

CWT_NS_BEGIN

namespace debby
{



bool Record::save ()
{
	CWT_ASSERT(_parent);

	if (!_sthPool[SthForCreate]) {

		if (!dbh.tableExists(_name)) {
			if (!dbh.createTable(*this))
				return false;
		}

		cwt::String sql = __build_sql (_parent->_name, _parent->_attrs);

		_sthPool[SthForCreate] = dbh.prepare(sql);

		if (!_sthPool[SthForCreate])
			return false;
	}

}

} // namespace debby

CWT_NS_END



