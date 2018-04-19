/*
 * database.cpp
 *
 *  Created on: Aug 19, 2013
 *      Author: wladt
 */

#include "pfs/debby/database.hpp"
#include "pfs/debby/statement.hpp"
#include <pfs/dl.hpp>
#include <pfs/logger.hpp>
#include <pfs/safeformat.hpp>

/**
 * #include <pfs/debby/debby.hpp
 *
 * using pfs::debby::database;
 *
 * database db;
 * db.connect("sqlite3:///tmp/test.db?mode=rwc");
 *
 * if (!db.connected())
 * 		return;
 *
 * ...
 *
 * db.close(); // optional, automatically closed in destructor
 *
 */

namespace pfs { namespace debby {

static const pfs::string DefaultPluginPrefix("pfs-debby-");

database::database ()
	: _d(0)
	, _pluginPrefix(DefaultPluginPrefix)
{}

bool database::connect (const pfs::string & uristr)
{
	pfs::uri uri;

	if (!uri.parse(uristr)) {
		this->addError(_u8("Invalid URI specified for DB driver: ") << uristr);
		return false;
	}

	pfs::string plugin_name = uri.scheme();

	if (plugin_name.isEmpty()) {
		this->addError(_u8("Invalid URI specified for DB driver: DB driver name is empty"));
		return false;
	}

	plugin_name.prepend(_pluginPrefix);

	pfs::dl & dl = pfs::dl::getPluginLoader();

	_d = dynamic_cast<database_impl *>(dl.openPlugin(plugin_name));

	if (!_d) {
		this->addError(dl.lastErrorText());
		return false;
	}

	pfs::stringlist userinfo = uri.userinfo().split(_l1(":"));

	if (!_d->open(uri.path()
			, userinfo.size() > 0 ? userinfo[0] : pfs::string() // login
			, userinfo.size() > 1 ? userinfo[1] : pfs::string() // password
			, uri.queryItems()
			, this)) {
		dl.closePlugin(plugin_name, _d);
		_d = 0;
		return false;
	}

	return true;
}

void database::close ()
{
	if (_d) {
		_d->close();
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


