/*
 * server.cpp
 *
 *  Created on: Jan 13, 2016
 *      Author: wladt
 */

#include "pfs/io/server.hpp"

namespace pfs { namespace io {

error_code server::close ()
{
	error_code ex;

	if (_d) {
		ex = _d->close();
		shared_ptr<bits::server> nil;
		_d.swap(nil);
	}

	return ex;
}

bool server::accept (device & peer, bool non_blocking, error_code * ex)
{
	PFS_ASSERT(_d);

	bits::device * p;

	bool rc = _d->accept(& p, non_blocking, ex);

	if (rc) {
	    shared_ptr<bits::device> d(p);
	    peer._d.swap(d);
	}

	return rc;
}


}} // pfs::io
