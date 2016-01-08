/*
 * app.cpp
 *
 *  Created on: Aug 15, 2013
 *      Author: wladt
 */


#include <pfs/logger.hpp>
#include "pfs/app.hpp"

namespace pfs {

app * app::self = 0;

app::app (const string & progname)
	: _program()
{
	PFS_ASSERT(self == 0);
	self = this;
	_program = (progname.empty() ? string("<anonymous>") : progname);
}

int app::exec (dispatcher & d)
{
	int r = EXIT_FAILURE;

    d.connect_all();

    pfs::notification & nx = d.get_notification();

    if (d.start() && nx.count_error_type() == 0) {
    	if (nx.count()) {
    		pfs::log::print(nx);
    		nx.clear();
    	}
   		r = d.exec();
    }

    d.finalize();

	return r;
}


} // pfs
