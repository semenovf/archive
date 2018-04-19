/*
 * strftime.cpp
 *
 *  Created on: Jan 3, 2016
 *      Author: wladt
 */

#include <time.h>
#include <pfs/string.hpp>

namespace pfs { namespace platform {

string strftime (const string & format
		, const struct tm & tm
		, size_t max)
{
#if PFS_STRING_UTF16

#	error Need to implement

#elif PFS_STRING_UTF32

#	error Need to implement

#else

	char buffer[128];
	char * p = buffer;

	if (max > 128) {
		p = new char [max];
	}

	strftime(p, max, reinterpret_cast<const char *>(format.c_str()), & tm);

	string r(p);

	if (max > 128) {
		delete [] p;
	}

	return r;
#endif

}

}} // pfs::platform
