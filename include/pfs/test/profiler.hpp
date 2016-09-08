/*
 * stopwatch.hpp
 *
 *  Created on: Nov 30, 2013
 *      Author: wladt
 */

#ifndef __PFS_TEST_PROFILER_HPP__
#define __PFS_TEST_PROFILER_HPP__

#include "dllapi.h"

namespace pfs { namespace test {

class DLL_API profiler
{
	double _sec;

public:
	profiler () : _sec(0) { start(); }
	void start ();
	double ellapsed () const;
};

}} // pfs::test

#endif /* __PFS_TEST_PROFILER_HPP__ */
