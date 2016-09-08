/*
 * stopwatch.cpp
 *
 *  Created on: Nov 30, 2013
 *      Author: wladt
 */

#include "pfs/test/profiler.hpp"
#include <windows.h>

#if defined(NDEBUG) || defined(_NDEBUG)
#	define PFS_ASSERT(p) p
#else
#	include <cassert>
#	define PFS_ASSERT(p) assert(p)
#endif

// [http://support.microsoft.com/kb/815668](http://support.microsoft.com/kb/815668)
// [QueryPerformanceCounter - бомба замедленного действия](http://devdoc.web-ide.ru/index.php/content/view/queryPerformanceCounter.htm)

namespace pfs { namespace test {

//static uint64_t microseconds()
//{
//	LARGE_INTEGER now;
//	QueryPerformanceCounter(&now);
//	LARGE_INTEGER freq;
//	QueryPerformanceFrequency(&freq);
//	return now.QuadPart * 1000 / (freq.QuadPart / 1000);
//}

void profiler::start ()
{
	LARGE_INTEGER c;
	PFS_ASSERT(QueryPerformanceCounter(& c));
	_sec = static_cast<double>(c.QuadPart);
}

double profiler::ellapsed () const
{
	LARGE_INTEGER c, freq;
	PFS_ASSERT(QueryPerformanceCounter(& c));
	PFS_ASSERT(QueryPerformanceFrequency(& freq));
	return (static_cast<double>(c.QuadPart) - _sec)/static_cast<double>(freq.QuadPart);
}

}} // pfs::test
