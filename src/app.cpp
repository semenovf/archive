/*
 * app.cpp
 *
 *  Created on: Aug 15, 2013
 *      Author: wladt
 */


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

int app::exec (sepaloid & sepaloid)
{
	int r = EXIT_FAILURE;

    sepaloid.connectAll();
    if (sepaloid.start() && sepaloid.isGood()) {
   		r = sepaloid.exec();
    }
    sepaloid.logErrors();

	return r;
}


} // pfs
