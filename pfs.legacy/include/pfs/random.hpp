/*
 * random.hpp
 *
 *  Created on: Sep 23, 2013
 *      Author: wladt
 */

#ifndef __PFS_RANDOM_HPP__
#define __PFS_RANDOM_HPP__

#include <pfs/pimpl.hpp>

#ifdef PFS_CC_MSVC
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif

namespace pfs {

class DLL_API random
{
	pimpl _d;

public:
	random ();
	random (uint32_t seed);         // TODO obsolete
	uint32_t srand (uint32_t seed); // TODO obsolete
	uint32_t rand ();
};

} // pfs

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

#endif /* __PFS_RANDOM_HPP__ */
