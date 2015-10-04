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
#include <pfs/uri.hpp>
#include <pfs/safeformat.hpp>

namespace pfs { namespace debby {

bool database::open (const pfs::string & uri_str)
{
	pfs::uri uri;

	if (!uri.parse(uri_str)) {
		this->addError(safeformat("Invalid URI specified for DB driver: %s")(uri_str)());
		return false;
	}

	pfs::string debby_name = uri.scheme();

	if (debby_name.isEmpty()) {
		this->addError(_u8("Invalid URI specified for DB driver: DB driver name is empty"));
		return false;
	}

	debby_name.prepend(pfs::string("pfs-debby-"));
	driver * drv = nullptr;
	pfs::dl & dl = pfs::dl::getDL();

	string dlpath = dl.buildDlFileName(debby_name);
	pfs::pluggable *
	if (!dl.openPlugin(debby_name, dlpath, & drv)) {
		this->addError(string(safeformat("Fatal error while loading DB driver for %s from %s") % uri.scheme() % dlpath));
		return false;
	}

	pfs::vector<pfs::string> userinfo = uri.userinfo().split(_l1(":"));
	pfs::map<pfs::string, pfs::string> params;
	pfs::string errstr;

	database_data * driverData = drv->open (uri.path()
			, userinfo.size() > 0 ? userinfo[0] : pfs::string() // login
			, userinfo.size() > 1 ? userinfo[1] : pfs::string() // password
			, uri.queryItems()
			, errstr);

	if (!driverData) {
		this->addError(errstr);
		return false;
	}

	_pimpl.reset(driverData, impl_deleter());
	return true;
}

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


