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

class random::impl
{
public:
	impl () : _randomValue(0) {}
	uint_t rand ();

private:
	uint_t _randomValue;
};

inline uint_t random::impl::rand ()
{
	PFS_VERIFY(rand_s(& _randomValue) == 0);
	return _randomValue;
}

random::random() : _d(new random::impl)
{}

random::random(uint_t ) : _d(new random::impl)
{}

uint_t random::srand (uint_t )
{
	return 0;
}

uint_t random::rand ()
{
	return _d.cast<impl>()->rand();
}

} // pfs
