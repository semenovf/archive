/*
 * sth.cpp
 *
 *  Created on: Aug 20, 2013
 *      Author: wladt
 */

#include "../include/cwt/debby/sth.hpp"

namespace cwt { namespace debby {

/**
 *
 */
void statement::close ()
{
	if (_pimpl) {
		PFS_ASSERT(_pimpl->_driver);
		_pimpl->_driver->closeStmt(_pimpl.get());
		_pimpl.reset();
	}
}


/**
 * @fn pfs::vector<pfs::unitype> statement::fetchRow()
 *
 * @param sth
 * @return
 *
 * @note On error and at end of data returns empty vector, so need to call DbHandler::errno method to distinguishes this situation
 */


/**
 *
 * @param param
 * @return
 */
statement & statement::bind (const pfs::unitype & param)
{
	PFS_ASSERT(_pimpl);
	PFS_ASSERT(_pimpl->_driver);

	if (_pimpl->_driver->bind(*_pimpl, _pimpl->_bindCursor, param)) {
		++_pimpl->_bindCursor;
	}
	return *this;
}

}} // cwt::debby

