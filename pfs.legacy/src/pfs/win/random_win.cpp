/*
 * random_win.cpp
 *
 *  Created on: Sep 23, 2013
 *      Author: wladt
 */

#define _CRT_RAND_S
#include <stdlib.h>
#include "pfs/random.hpp"

namespace pfs {

class random_impl
{
public:
    random_impl () : _randomValue(0) {}
	uint32_t rand ();

private:
	uint32_t _randomValue;
};

inline uint32_t random_impl::rand ()
{
	PFS_VERIFY(rand_s(& _randomValue) == 0);
	return _randomValue;
}

random::random() : _d(new random_impl)
{}

random::random(uint32_t ) : _d(new random_impl)
{}

uint32_t random::srand (uint32_t )
{
	return 0;
}

uint32_t random::rand ()
{
	return _d.cast<random_impl>()->rand();
}

} // pfs
