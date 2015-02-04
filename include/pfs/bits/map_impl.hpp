/*
 * map_impl.hpp
 *
 *  Created on: Feb 4, 2015
 *      Author: wladt
 */

#ifndef __PFS_BITS_MAP_IMPL_HPP__
#define __PFS_BITS_MAP_IMPL_HPP__

#include <map>
//#include <memory>
#include <pfs/bits/allocator.hpp>
#include <pfs/bits/reference.hpp>
//#include <pfs/bits/iterator.hpp>

namespace pfs {

template <typename Key, typename T, typename Compare = std::less<Key>, typename Allocator = pfs::allocator<std::pair<const Key, T> > >
class map_impl : protected std::map<Key, T, Compare, Allocator>
{
public:
	typedef map_impl<Key, T, Compare, Allocator> self_class;
	typedef typename std::map<Key, T, Compare, Allocator> base_class;

	typedef typename base_class::value_type value_type;
//	typedef typename base_class::pointer pointer;
//	typedef typename base_class::const_pointer const_pointer;
	typedef typename base_class::iterator iterator;
	typedef size_t    size_type;
	typedef ptrdiff_t difference_type;

public:
	map_impl () : base_class() {}
	virtual ~map_impl () {}

	bool isEmpty () const
	{
		return base_class::empty();
	}

	void clear ()
	{
		base_class::clear();
	}

	size_type size () const
	{
		return base_class::size();
	}
};

template <typename Key, typename T, typename Compare, typename Allocator, typename Holder>
class map_ptr
{
//	typedef typename constness<Holder>::pointer pointer;

public:
	typedef typename Holder::difference_type difference_type;
	typedef typename Holder::size_type       size_type;
	typedef typename map_impl<Key, T, Compare, Allocator>::iterator offset_type;
//	typedef byte_t   value_type;

protected:
	Holder *    _holder;
	offset_type _off; // actually it is an iterator

public:
	map_ptr (Holder * holder, offset_type off)
		: _holder(holder), _off(off)
	{}

	reference<Holder> ref () const;

//	value_type operator * () const;
//
	map_ptr & operator ++ ()
	{
		++_off;
		return *this;
	}

	map_ptr operator ++ (int)
    {
		map_ptr r(*this);
        this->operator ++ ();
        return r;
    }

	map_ptr & operator -- ()
	{
		--_off;
		return *this;
	}

	map_ptr operator -- (int)
    {
		map_ptr r(*this);
        this->operator -- ();
        return r;
    }

//	const byte_t * base () const { return _holder->constData() + _off; }
};

} // pfs


#endif /* __PFS_BITS_MAP_IMPL_HPP__ */
