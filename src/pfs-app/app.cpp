/*
 * app.cpp
 *
 *  Created on: Aug 15, 2013
 *      Author: wladt
 */


#include <pfs/logger.hpp>
#include "pfs/app.hpp"

namespace pfs {

app * app::self = nullptr;

app::app (const string & progname)
	: _program()
{
	PFS_ASSERT(self == nullptr);
	self = this;
	_program = (progname.isEmpty() ? string("<anonymous>") : progname);
}

int app::exec (dispatcher & d)
{
	int r = EXIT_FAILURE;

    d.connectAll();

    pfs::notification & nx = d.get_notification();

    if (d.start() && nx.count_error_type() == 0) {
   		r = d.exec();
    } else {
		pfs::log::print(nx);
		nx.clear();
    }

	return r;
}


} // pfs
