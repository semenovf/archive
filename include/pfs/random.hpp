/*
 * random.hpp
 *
 *  Created on: Sep 23, 2013
 *      Author: wladt
 */

#ifndef __PFS_RANDOM_HPP__
#define __PFS_RANDOM_HPP__

#include <pfs/pimpl.hpp>

namespace pfs {

class random
{
	pimpl _d;

public:
	random ();
	random (uint32_t seed);         // TODO obsolete
	uint32_t srand (uint32_t seed); // TODO obsolete
	uint32_t rand ();
};

} // pfs

#endif /* __PFS_RANDOM_HPP__ */
