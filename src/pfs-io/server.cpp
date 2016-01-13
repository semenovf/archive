/*
 * server.cpp
 *
 *  Created on: Jan 13, 2016
 *      Author: wladt
 */

#include "pfs/io/server.hpp"

namespace pfs { namespace io {

bool server::close (error_code * ex)
{
	bool r = false;

	if (_d) {
		if (_d->close(ex))
			r = true;

		delete _d;
        _d = 0;
	}

	return r;
}

bool server::accept (device & peer, bool non_blocking, error_code * ex)
{
	PFS_ASSERT(_d);

	device d;
	bool rc = _d->accept(& d._d, non_blocking, ex);

	if (rc) {
		peer.swap(d);
	}

	return rc;
}


}} // pfs::io
