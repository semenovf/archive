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

app::app (int argc
#if (defined(PFS_OS_WIN32) || defined(PFS_OS_WIN64)) && defined(PFS_UNICODE)
			, wchar_t * argv[])
		: _program(argc > 0 ? string::fromUtf16(argv[0], wcslen(argv[0])) : string("<anonymous>"))

#else
			, char * argv[])
		: _program(argc > 0 ? string::fromUtf8(argv[0]) : string("<anonymous>")) // FIXME for string::fromLocal8Bit()
#endif
{
	PFS_ASSERT(self == nullptr);
	self = this;
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
