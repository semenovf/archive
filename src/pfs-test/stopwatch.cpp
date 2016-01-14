/*
 * stopwatch.cpp
 *
 *  Created on: Nov 30, 2013
 *      Author: wladt
 */

#include "pfs/test/stopwatch.hpp"
#include "test_p.hpp"

#define HAVE_GETTIMEOFDAY 1

#if defined(_MSC_VER)
#	undef HAVE_GETTIMEOFDAY
#endif

#ifdef HAVE_GETTIMEOFDAY
#	include <sys/time.h> // for gettimeofday

#else
#	include <windows.h>
// [http://support.microsoft.com/kb/815668](http://support.microsoft.com/kb/815668)
// [QueryPerformanceCounter - бомба замедленного действия](http://devdoc.web-ide.ru/index.php/content/view/queryPerformanceCounter.htm)
#endif

namespace pfs { namespace test {

void stopwatch::start ()
{
#ifdef HAVE_GETTIMEOFDAY
	struct timeval now;
	PFS_ASSERT(gettimeofday(& now, nullptr) == 0);
	_sec = double(now.tv_sec) + double(now.tv_usec)/1000000;
#else
	LARGE_INTEGER c;
	PFS_ASSERT(QueryPerformanceCounter(& c));
	_sec = double(c.QuadPart);
#endif
}

double stopwatch::ellapsed () const
{
#ifdef HAVE_GETTIMEOFDAY
	struct timeval now;
	PFS_ASSERT(gettimeofday(& now, nullptr) == 0);
	return (double(now.tv_sec) + double(now.tv_usec)/1000000) - _sec;
#else
	LARGE_INTEGER c, freq;
	PFS_ASSERT(QueryPerformanceCounter(& c));
	PFS_ASSERT(QueryPerformanceFrequency(& freq));
	return (double(c.QuadPart) - _sec)/double(freq.QuadPart);
#endif
}

}} // pfs::test
