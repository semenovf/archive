/*
 * sth.cpp
 *
 *  Created on: Aug 20, 2013
 *      Author: wladt
 */

#include "../include/cwt/debby/statement.hpp"

namespace cwt { namespace debby {

/**
 *
 */
void statement::close ()
{
	if (!isNull()) {
		_pimpl->_driver->closeStmt(_pimpl.get());
		_pimpl->_driver = nullptr;
		_pimpl.reset();
	}
}

bool statement::exec ()
{
	bool r = false;

	if (!isNull()) {
		pfs::string errstr;
		if (_pimpl && _pimpl->_driver) {
			_pimpl->_bindCursor = 0;
			r = _pimpl->_driver->execStmt(*_pimpl, errstr);
			if (!r)
				this->addError(errstr);
		}
	}
	return r;
}

pfs::vector<pfs::unitype> statement::fetchRowArray ()
{
	pfs::vector<pfs::unitype> r;
	if (!isNull())
		_pimpl->_driver->fetchRowArray(*_pimpl, r);
	return r;
}

pfs::map<pfs::string, pfs::unitype> statement::fetchRowHash ()
{
	pfs::map<pfs::string, pfs::unitype> r;
	if (!isNull())
		_pimpl->_driver->fetchRowHash(*_pimpl, r);
	return r;
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
	if (!isNull()) {
		_pimpl->_driver->bind(*_pimpl, index, param);
	}
	return *this;
}


/**
 *
 * @param param
 * @return
 */
statement & statement::bind (const pfs::unitype & param)
{
	if (!isNull()) {
		if (_pimpl->_driver->bind(*_pimpl, _pimpl->_bindCursor, param)) {
			++_pimpl->_bindCursor;
		}
	}
	return *this;
}

}} // cwt::debby

