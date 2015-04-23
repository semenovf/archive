/*
* random_unix.cpp
*
* Created on: Sep 23, 2013
* Author: wladt
*/

#include "../random_p.hpp"
#include <cstdlib>

namespace pfs {

#if defined(_SVID_SOURCE) || defined(_BSD_SOURCE)
#	define PFS_HAVE_RANDOM_R
#endif

class random_impl
{
public:
    random_impl();
    random_impl(uint32_t seed);
	bool srand (uint32_t seed);
	int32_t rand ();

private:

#ifdef PFS_HAVE_RANDOM_R
	void init_random_r_version (uint32_t seed) {
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

inline random_impl::random_impl ()
{
#if defined PFS_HAVE_RANDOM_R
	init_random_r_version(1);
#endif
}

inline random_impl::random_impl (uint32_t seed)
{
#if defined PFS_HAVE_RANDOM_R
	init_random_r_version(seed);
#endif
	srandom(seed);
}

inline bool random_impl::srand (uint32_t seed)
{
#if defined PFS_HAVE_RANDOM_R
	return 0 == srandom_r(seed, & _rdata);
#endif
}

inline int32_t random_impl::rand ()
{
	int32_t r = 0;

#if defined PFS_HAVE_RANDOM_R
	PFS_VERIFY(random_r(& _rdata, & r) == 0);
#endif

	return r;
}

random::random() : _d(new random_impl)
{}

random::random(uint32_t seed) : _d(new random_impl(seed))
{}

uint32_t random::srand (uint32_t seed)
{
	return uint32_t(_d.cast<random_impl>()->srand(seed));
}

uint32_t random::rand ()
{
	return uint32_t(_d.cast<random_impl>()->rand());
}

} // pfs
