/*
 * stopwatch.hpp
 *
 *  Created on: Nov 30, 2013
 *      Author: wladt
 */

#ifndef __CWT_STOPWATCH_HPP__
#define __CWT_STOPWATCH_HPP__

#include <pfs/string.hpp>

namespace cwt {

class stopwatch
{
	double _sec;

public:
	stopwatch () : _sec(0) { start(); }
	void start ();
	double ellapsed () const;

	static std::string stringifyTime (double t);
};

} // cwt

#endif /* __CWT_STOPWATCH_HPP__ */
