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
	PFS_PIMPL_DECL(random, impl);

public:
	random ();
	random (uint_t seed);
	int srand (uint_t seed);
	int rand ();
};

} // cwt

#endif /* __CWT_RANDOM_HPP__ */
