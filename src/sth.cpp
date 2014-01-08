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
	if (_pimpl && _pimpl->_driver) {
		_pimpl->_driver->closeStmt(_pimpl.get());
		_pimpl.reset();
	}
}

bool statement::exec  ()
{
	_pimpl->_bindCursor = 0;
	return _pimpl->_driver->execStmt(*_pimpl);
}

bool statement::fetchRowArray (pfs::vector<pfs::unitype> & row)
{
	return _pimpl->_driver->fetchRowArray(*_pimpl, row);
}

bool statement::fetchRowHash (pfs::map<pfs::string, pfs::unitype> & row)
{
	return _pimpl->_driver->fetchRowHash(*_pimpl, row);
}


/**
 * @fn pfs::vector<pfs::unitype> statement::fetchRow()
 *
 * @param sth
 * @return
 *
 * @note On error and at end of data returns empty vector, so need to call DbHandler::errno method to distinguishes this situation
 */


statement & statement::bind (size_t index, const pfs::unitype & param)
{
	PFS_ASSERT(_pimpl);
	PFS_ASSERT(_pimpl->_driver);
	_pimpl->_driver->bind(*_pimpl, index, param);
	return *this;
}


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

