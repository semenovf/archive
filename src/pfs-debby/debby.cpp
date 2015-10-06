/*
 * debby.cpp
 *
 *  Created on: Oct 6, 2015
 *      Author: wladt
 */

#include <pfs/dl.hpp>
#include <pfs/uri.hpp>
#include <pfs/safeformat.hpp>
#include <pfs/stringlist.hpp>
#include "debby/debby.hpp"
#include "debby/database.hpp"

namespace pfs { namespace debby {

static const pfs::string PluginPrefix("pfs-debby-");

database debby::connect (const pfs::string & uristr, pfs::string * perrorstr)
{
	pfs::uri uri;
	pfs::string errorstr;
	database result;

	do {
		if (!uri.parse(uristr)) {
			errorstr << _u8("Invalid URI specified for DB driver: ") << uristr;
			break;
		}

		pfs::string debby_name = uri.scheme();

		if (debby_name.isEmpty()) {
			errorstr = _u8("Invalid URI specified for DB driver: DB driver name is empty");
			break;
		}

		debby_name.prepend(PluginPrefix);

		pfs::dl & dl = pfs::dl::getPluginLoader();

		database_impl * impl = dynamic_cast<database_impl *>(dl.openPlugin(debby_name));

		if (!impl) {
			errorstr = dl.lastErrorText();
			break;
		}

		pfs::stringlist userinfo = uri.userinfo().split(_l1(":"));
		pfs::map<pfs::string, pfs::string> params;

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

		ok = true;
	} while (false);

	if (!ok && perrorstr)
		*perrorstr = errorstr;

	return database();
}

}} // pfs::debby

