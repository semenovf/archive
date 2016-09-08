/*
 * stopwatch.cpp
 *
 *  Created on: Nov 30, 2013
 *      Author: wladt
 */

#include "pfs/test/profiler.hpp"
#include <sys/time.h>

#if defined(NDEBUG) || defined(_NDEBUG)
#	define PFS_ASSERT(p) p
#else
#	include <cassert>
#	define PFS_ASSERT(p) assert(p)
#endif

namespace pfs { namespace test {


//static uint64_t microseconds()
//{
//	timeval tv;
//	gettimeofday(&tv, NULL);
//	return (uint64_t)tv.tv_sec * 1000000 + tv.tv_usec;
//}

void profiler::start ()
{
	struct timeval now = {0, 0};
	PFS_ASSERT(gettimeofday(& now, 0) == 0);
	_sec = static_cast<double>(now.tv_sec) + static_cast<double>(now.tv_usec)/1000000;
}

double profiler::ellapsed () const
{
	struct timeval now = {0, 0};
	PFS_ASSERT(gettimeofday(& now, 0) == 0);
	return (static_cast<double>(now.tv_sec) + static_cast<double>(now.tv_usec)/1000000) - _sec;
}

}} // pfs::test
