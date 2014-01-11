/*
* random_unix.cpp
*
* Created on: Sep 23, 2013
* Author: wladt
*/

#include "../random_p.hpp"
#include <cstdlib>

namespace cwt {

#if defined(_SVID_SOURCE) || defined(_BSD_SOURCE)
#	define CWT_HAVE_RANDOM_R
#endif

class random::impl
{
public:
	impl();
	impl(uint_t seed);
	bool srand (uint_t seed);
	int rand ();

private:

#ifdef CWT_HAVE_RANDOM_R
	void init_random_r_version (uint_t seed) {
		memset(& _rdata, 0, sizeof(struct random_data));
		PFS_VERIFY(initstate_r(seed, _stateBuf, StateSize, & _rdata) == 0);
		PFS_VERIFY(setstate_r(_stateBuf, & _rdata) == 0);
	}

	static const size_t StateSize = 64;
	char _stateBuf[StateSize];
	struct random_data _rdata;
#else
#	error "random does not implemented yet"
#endif
};

PFS_PIMPL_DEF(random, impl);

inline random::impl::impl ()
{
#if defined CWT_HAVE_RANDOM_R
	init_random_r_version(1);
#endif
}

inline random::impl::impl (uint_t seed)
{
#if defined CWT_HAVE_RANDOM_R
	init_random_r_version(seed);
#endif
	srandom(seed);
}

inline bool random::impl::srand (uint_t seed)
{
#if defined CWT_HAVE_RANDOM_R
	return 0 == srandom_r(seed, & _rdata);
#endif
}

inline int random::impl::rand ()
{
	int r = 0;

#if defined CWT_HAVE_RANDOM_R
	PFS_VERIFY(random_r(& _rdata, & r) == 0);
#endif

	return r;
}

random::random() : _pimpl(new random::impl) {}
random::random(uint_t seed) : _pimpl(new random::impl(seed)) {}
uint_t random::srand (uint_t seed) { return uint_t(_pimpl->srand(seed)); }
uint_t random::rand () { return uint_t(_pimpl->rand()); }

} // cwt
