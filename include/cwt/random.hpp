/*
 * random.hpp
 *
 *  Created on: Sep 23, 2013
 *      Author: wladt
 */

#ifndef __CWT_RANDOM_HPP__
#define __CWT_RANDOM_HPP__

#include <cwt/pimpl.hpp>

CWT_NS_BEGIN

class Random
{
	CWT_PIMPL_IMPL(Random);

public:
	Random();
	Random(uint_t seed);
	int srandom (uint_t seed);
	int random ();
};

CWT_NS_END

#endif /* __CWT_RANDOM_HPP__ */
