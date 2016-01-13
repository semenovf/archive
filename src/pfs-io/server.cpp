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

		shared_ptr<bits::server> nil;
		_d.swap(nil);
	}

	return r;
}

bool server::accept (device & peer, bool non_blocking, error_code * ex)
{
	PFS_ASSERT(_d);

	bits::device * pd;

	bool rc = _d->accept(& pd, non_blocking, ex);

	if (rc) {
		device dd(pd);
		peer.swap(dd);
	}

	return rc;
}


}} // pfs::io
