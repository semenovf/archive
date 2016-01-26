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
//		shared_ptr<bits::server> nil;
//		_d.swap(nil);
	}

	return ex;
}

error_code server::accept (device & peer, bool non_blocking)
{
	PFS_ASSERT(_d);

	bits::device * p;

	error_code ex = _d->accept(& p, non_blocking);

	if (!ex) {
	    shared_ptr<bits::device> d(p);
	    peer._d.swap(d);
	}

	return ex;
}


}} // pfs::io
