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
	PFS_CHECK_SIZEOF_TYPES;
	PFS_ASSERT(self == nullptr);
	self = this;
	_program = (progname.isEmpty() ? string("<anonymous>") : progname);
}

app::app (int argc, char * argv[])
	: _program(argc > 0 ? _u8(argv[0]) : string("<anonymous>"))
{
	PFS_CHECK_SIZEOF_TYPES;
	PFS_ASSERT(self == nullptr);
	self = this;
}

} // pfs
