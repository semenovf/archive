/*
 * pointer.hpp
 *
 *  Created on: Sep 25, 2014
 *      Author: wladt
 */

#ifndef __PFS_BITS_REFERENCE_HPP__
#define __PFS_BITS_REFERENCE_HPP__

#include <pfs/bits/constness.hpp>

namespace pfs {

template <typename Holder>
class reference
{
	typedef reference<Holder> self_class;
	typedef typename constness<Holder>::pointer pointer;

public:
	typedef typename Holder::value_type value_type;

protected:
	Holder & _holder;
	pointer  _p;

public:
	reference (Holder & holder, pointer p)
		: _holder(holder), _p(p) {}

	reference (const reference & r)
		: _holder(r._holder), _p(r._p) {}

	operator value_type () const
	{
		return value_type(*_p);
	}

	pointer operator & () const { return _p; }

	reference & operator = (const reference & o)
	{
		return operator = (value_type(o));
	}

	reference & operator = (const value_type & value)
	{
		this->_holder.detach_and_assign(_p, value);
		return *this;
	}

    bool operator == (const self_class & o) const  { return value_type(*this) == value_type(o); }
    bool operator == (const value_type & o) const  { return value_type(*this) == o; }
    bool operator != (const self_class & o) const  { return value_type(*this) != value_type(o); }
    bool operator != (const value_type & o) const  { return value_type(*this) != o; }
    bool operator >  (const self_class & o) const  { return value_type(*this) > value_type(o); }
    bool operator >  (const value_type & o) const  { return value_type(*this) > o; }
    bool operator >= (const self_class & o) const  { return value_type(*this) >= value_type(o); }
    bool operator >= (const value_type & o) const  { return value_type(*this) >= o; }
    bool operator <  (const self_class & o) const  { return value_type(*this) < value_type(o); }
    bool operator <  (const value_type & o) const  { return value_type(*this) < o; }
    bool operator <= (const self_class & o) const  { return value_type(*this) <= value_type(o); }
    bool operator <= (const value_type & o) const  { return value_type(*this) <= o; }
};

} // pfs

#endif /* __PFS_BITS_REFERENCE_HPP__ */
