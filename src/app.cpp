/*
 * app.cpp
 *
 *  Created on: Aug 15, 2013
 *      Author: wladt
 */


#include "../include/cwt/app.hpp"

namespace cwt {

app * app::self = nullptr;

app::app (const pfs::string & progname)
	: _program()
{
	PFS_CHECK_SIZEOF_TYPES;
	PFS_ASSERT(self == nullptr);
	self = this;
	_program = (progname.isEmpty() ? pfs::string("<anonymous>") : progname);
}

app::app (int argc, char * argv[])
	: _program(argc > 0 ? _u8(argv[0]) : pfs::string("<anonymous>"))
{
	PFS_CHECK_SIZEOF_TYPES;
	PFS_ASSERT(self == nullptr);
	self = this;
}

} // cwt
