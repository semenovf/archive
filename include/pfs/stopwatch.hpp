/*
 * stopwatch.hpp
 *
 *  Created on: Nov 30, 2013
 *      Author: wladt
 */

#ifndef __PFS_STOPWATCH_HPP__
#define __PFS_STOPWATCH_HPP__

#include <pfs/string.hpp>

namespace pfs {

class stopwatch
{
	double _sec;

public:
	stopwatch () : _sec(0) { start(); }
	void start ();
	double ellapsed () const;

	static std::string stringifyTime (double t);
};

} // pfs

#endif /* __PFS_STOPWATCH_HPP__ */
