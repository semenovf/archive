/*
 * stopwatch.hpp
 *
 *  Created on: Nov 30, 2013
 *      Author: wladt
 */

#ifndef __PFS_TEST_STOPWATCH_HPP__
#define __PFS_TEST_STOPWATCH_HPP__

#include "dllapi.h"

namespace pfs { namespace test {

class DLL_API stopwatch
{
	double _sec;

public:
	stopwatch () : _sec(0) { start(); }
	void start ();
	double ellapsed () const;
};

}} // pfs::test

#endif /* __PFS_TEST_STOPWATCH_HPP__ */
