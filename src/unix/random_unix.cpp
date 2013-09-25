/*
 * random_unix.cpp
 *
 *  Created on: Sep 23, 2013
 *      Author: wladt
 */

#include "../random_p.hpp"
#include <cstdlib>

CWT_NS_BEGIN

#if defined(_SVID_SOURCE) || defined(_BSD_SOURCE)
#	define CWT_HAVE_RANDOM_R
#endif

class Random::Impl
{
public:
	Impl() {}
	Impl(uint_t seed) { srandom(seed); }
	bool srandom (uint_t seed);
	int  random ();

private:
#ifdef CWT_HAVE_RANDOM_R
	struct random_data m_rdata;
#else
#	error "Random does not implemented yet"
#endif
};


inline bool Random::Impl::srandom (uint_t seed)
{
#if	defined CWT_HAVE_RANDOM_R
	return 0 == srandom_r(seed, & m_rdata);
#endif
}

inline int Random::Impl::random ()
{
	int r = 0;

#if	defined CWT_HAVE_RANDOM_R
	return 0 == random_r(& m_rdata, & r);
#endif

	return r;
}

Random::Random() : pimpl(new Random::Impl) {}
Random::Random(uint_t seed) : pimpl(new Random::Impl(seed)) {}
int Random::srandom (uint_t seed) { return pimpl->srandom(seed); }
int Random::random () { return pimpl->random(); }

CWT_NS_END



