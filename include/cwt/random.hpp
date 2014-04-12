/*
 * random.hpp
 *
 *  Created on: Sep 23, 2013
 *      Author: wladt
 */

#ifndef __CWT_RANDOM_HPP__
#define __CWT_RANDOM_HPP__

#include <pfs/pimpl.hpp>

namespace cwt {

class random
{
	class impl;
	pfs::pimpl _d;

public:
	random ();
	random (uint_t seed);       // TODO obsolete
	uint_t srand (uint_t seed); // TODO obsolete
	uint_t rand ();
};

} // cwt

#endif /* __CWT_RANDOM_HPP__ */
