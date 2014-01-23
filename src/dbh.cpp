/*
 * dbh.cpp
 *
 *  Created on: Aug 19, 2013
 *      Author: wladt
 */

#include "../include/cwt/debby/dbh.hpp"
#include "../include/cwt/debby/sth.hpp"
#include <cwt/dl.hpp>
#include <cwt/logger.hpp>
#include <cwt/safeformat.hpp>
#include <cwt/uri.hpp>

namespace cwt { namespace debby {

bool handler::open (const pfs::string & uri_str)
{
	cwt::uri uri;

	if (!uri.parse(uri_str)) {
		this->addError(_Fr("Invalid URI specified for DB driver: %s") % uri_str);
		return false;
	}

	pfs::string debby_name = uri.scheme();

	if (debby_name.isEmpty()) {
		this->addError(_u8(_Tr("Invalid URI specified for DB driver: DB driver name is empty")));
		return false;
	}

	debby_name.prepend(pfs::string("cwt-debby-"));
	driver * drv = nullptr;
	cwt::dl dl;

	pfs::string dlpath = dl.buildDlFileName(debby_name);
	if (!dl.pluginOpen(debby_name, dlpath, & drv)) {
		this->addError(_Fr("Fatal error while loading DB driver for %s from %s") % uri.scheme() % dlpath);
		return false;
	}

	pfs::vector<pfs::string> userinfo = uri.userinfo().split(_l1(":"));
	pfs::map<pfs::string, pfs::string> params;
	pfs::string errstr;

	handler_data * driverData = drv->open (uri.path()
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

void handler::close ()
{
	if (_pimpl && _pimpl->_driver) {
		_pimpl->_driver->close(_pimpl.get());
		_pimpl->_driver = nullptr;
		_pimpl.reset();
	}
}

bool handler::query (const pfs::string & sql)
{
	pfs::string errstr;
	bool r = _pimpl->_driver->query(*_pimpl, sql, errstr);
	if (!r)
		this->addError(errstr);
	return r;
}

statement handler::prepare (const pfs::string & sql)
{
	PFS_ASSERT(_pimpl && _pimpl->_driver);
	pfs::string errstr;
	statement_data * d = _pimpl->_driver->prepare(*_pimpl, sql, errstr);

	if (d) {
		statement sth(d);
		sth._pimpl->_bindCursor = 0; // reset counter of bind parameters
		return sth;
	}
	this->addError(errstr);
	return statement();
}

bool handler::begin ()
{
	pfs::string errstr;
	bool r = _pimpl->_driver->begin(*_pimpl, errstr);
	if (!r)
		this->addError(errstr);
	return r;
}

bool handler::commit ()
{
	pfs::string errstr;
	bool r = _pimpl->_driver->commit(*_pimpl, errstr);
	if (!r)
		this->addError(errstr);
	return r;
}

bool handler::rollback ()
{
	pfs::string errstr;
	bool r = _pimpl->_driver->rollback(*_pimpl, errstr);
	if (!r)
		this->addError(errstr);
	return r;
}

bool handler::meta (const pfs::string & table, pfs::vector<column_meta> & meta)
{
	pfs::string errstr;
	bool r = _pimpl->_driver->meta(*_pimpl, table, meta, errstr);
	if (!r)
		this->addError(errstr);
	return r;
}


}} // cwt::debby


