/**
 * @file unitype.hpp
 * @author wladt
 * @date Jul 1, 2014
 */

#ifndef __PFS_UNITYPE1_HPP__
#define __PFS_UNITYPE1_HPP__

#include "pimpl.hpp"
#include "bits/cast_traits.hpp"

namespace pfs { namespace unitype1 {

template <typename T>
class unitype
{
	pfs::pimpl _d;

public:
	unitype () : _d() {}
	unitype (const T & d) : _d(new T(d)) {}

	bool isNull () { return _d.isNull(); }

	template <typename R>
	R get () const { return cast_trait<R,T>(*_d.cast<T>()); }
};

template <typename T>
unitype<T> make_unitype (const T & x) { return unitype<T>(x); }

template <typename T>
unitype<T> make_unitype () { return unitype<T>(); }

}} // pfs::unitype1

#endif /* __PFS_DETAIL_UNITYPE1_HPP__ */
