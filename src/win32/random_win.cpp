/*
 * random_win.cpp
 *
 *  Created on: Sep 23, 2013
 *      Author: wladt
 */

#define _CRT_RAND_S
#include <stdlib.h>
#include "../../include/cwt/random.hpp"

namespace cwt {

class random::impl
{
public:
	impl () : _randomValue(0) {}
	uint_t rand ();

private:
	uint_t _randomValue;
};

PFS_PIMPL_DEF(random, impl);

inline uint_t random::impl::rand ()
{
	PFS_VERIFY(rand_s(& _randomValue) == 0);
	return _randomValue;
}

random::random() : _pimpl(new random::impl) {}
random::random(uint_t ) : _pimpl(new random::impl) {}
uint_t random::srand (uint_t ) { return 0; }
uint_t random::rand () { return _pimpl->rand(); }

} // cwt
