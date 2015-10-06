/*
 * dbh.cpp
 *
 *  Created on: Aug 19, 2013
 *      Author: wladt
 */

#include "pfs/debby/database.hpp"
#include "pfs/debby/statement.hpp"
#include <pfs/dl.hpp>
#include <pfs/logger.hpp>
#include <pfs/safeformat.hpp>

namespace pfs { namespace debby {

void database::close ()
{
	if (!isNull()) {
		_pimpl->_driver->close(_pimpl.get());
		_pimpl->_driver = nullptr;
		_pimpl.reset();
	}
}

bool database::query (const pfs::string & sql)
{
	bool r = false;

	if (!isNull()) {
		pfs::string errstr;
		r = _pimpl->_driver->query(*_pimpl, sql, errstr);
		if (!r)
			this->addError(errstr);
	}
	return r;
}

statement database::prepare (const pfs::string & sql)
{
	if (!isNull()) {
		pfs::string errstr;
		statement_data * d = _pimpl->_driver->prepare(*_pimpl, sql, errstr);

		if (d) {
			statement sth(d);
			sth._pimpl->_bindCursor = 0; // reset counter of bind parameters
			return sth;
		}
		this->addError(errstr);
	}

	return statement();
}

bool database::begin ()
{
	bool r = false;
	if (!isNull()) {
		pfs::string errstr;
		r = _pimpl->_driver->begin(*_pimpl, errstr);
		if (!r)
			this->addError(errstr);
	}
	return r;
}

bool database::commit ()
{
	bool r = false;
	if (!isNull()) {
		pfs::string errstr;
		r = _pimpl->_driver->commit(*_pimpl, errstr);
		if (!r)
			this->addError(errstr);
	}
	return r;
}

bool database::rollback ()
{
	bool r = false;
	if (!isNull()) {
		pfs::string errstr;
		r = _pimpl->_driver->rollback(*_pimpl, errstr);
		if (!r)
			this->addError(errstr);
	}
	return r;
}

bool database::meta (const pfs::string & table, pfs::vector<column_meta> & meta)
{
	bool r = false;
	if (!isNull()) {
		pfs::string errstr;
		r = _pimpl->_driver->meta(*_pimpl, table, meta, errstr);
		if (!r)
			this->addError(errstr);
	}
	return r;
}

}} // pfs::debby


