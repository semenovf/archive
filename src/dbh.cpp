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
		cwt::log::error(_Fr("Invalid URI specified for DB driver: %s") % uri_str);
		return false;
	}

	pfs::string debby_name = uri.scheme();

	if (debby_name.isEmpty()) {
		cwt::log::error(_Tr("Invalid URI specified for DB driver: DB driver name is empty."));
		return false;
	}

	debby_name.prepend(pfs::string("cwt-debby-"));
	driver * drv = nullptr;
	cwt::dl dl;

	pfs::string dlpath = dl.buildDlFileName(debby_name);
	if (!dl.pluginOpen(debby_name, dlpath, & drv)) {
		cwt::log::error(_Fr("Fatal error while loading DB driver for %s from %s") % uri.scheme() % dlpath);
		return false;
	}

	pfs::vector<pfs::string> userinfo = uri.userinfo().split(_l1(":"));
	pfs::map<pfs::string, pfs::string> params;

	handler_data * driverData = drv->open (uri.path()
			, userinfo.size() > 0 ? userinfo[0] : pfs::string() // login
			, userinfo.size() > 1 ? userinfo[1] : pfs::string() // password
			, uri.queryItems());

	if (!driverData)
		return false;

	_pimpl.reset(driverData, impl_deleter());
	return true;
}

void handler::close ()
{
	if (_pimpl && _pimpl->_driver) {
		_pimpl->_driver->close(_pimpl.get());
		_pimpl.reset();
	}
}

statement handler::prepare (const pfs::string & sql)
{
	PFS_ASSERT(_pimpl && _pimpl->_driver);
	statement_data * d = _pimpl->_driver->prepare(*_pimpl, sql);

	if (d) {
		statement sth(d);
		sth._pimpl->_bindCursor = 0; // reset counter of bind parameters
		return sth;
	}

	return statement();
}

}} // cwt::debby


