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

template <typename Holder, typename Key, typename T, typename Compare, typename Alloc>
class map_pointer;

template <typename Key, typename T, typename Compare, typename Alloc>
class map_impl : protected std::map<Key, T, Compare, Alloc>
{
public:
	typedef map_impl<Key, T, Compare, Alloc> self_class;
	typedef typename std::map<Key, T, Compare, Alloc> base_class;

	typedef size_t    size_type;
	typedef ptrdiff_t difference_type;
	typedef typename base_class::key_type   key_type;
	typedef typename base_class::value_type value_type;
	typedef typename base_class::iterator   iterator_type;

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

	size_type remove (const key_type & key) { return base_class::erase(key); }
	template <typename Holder>
	map_pointer<Key, T, Compare, Alloc, Holder>
	remove (const map_pointer<Key, T, Compare, Alloc, Holder> & ptr);

	template <typename Holder>
	std::pair<map_pointer<Key, T, Compare, Alloc, Holder>, bool>
	insert (Holder & holder, const value_type & value);

	template <typename Holder>
	map_pointer<Key, T, Compare, Alloc, Holder>
	find (Holder & holder, const key_type & key) const;

	template <typename Holder>
	map_pointer<Key, T, Compare, Alloc, Holder>
	beginPtr (Holder & holder);

	template <typename Holder>
	map_pointer<Key, T, Compare, Alloc, Holder>
	cbeginPtr (Holder & holder) const;

	template <typename Holder>
	map_pointer<Key, T, Compare, Alloc, Holder>
	endPtr (Holder & holder);

	template <typename Holder>
	map_pointer<Key, T, Compare, Alloc, Holder>
	cendPtr (Holder & holder) const;
};


template <typename Key, typename T, typename Compare, typename Alloc, typename Holder>
class map_pointer
{
public:
	typedef typename Holder::difference_type difference_type;
	typedef typename Holder::size_type       size_type;
	typedef typename Holder::key_type        key_type;
	typedef typename Holder::value_type      value_type;
	typedef typename map_impl<Key, T, Compare, Alloc>::iterator_type iterator_type;

	Holder *      _holder;
	iterator_type _ptr;

	map_pointer ()
		: _holder(nullptr), _ptr()
	{}

	map_pointer (Holder * holder, iterator_type ptr)
		: _holder(holder), _ptr(ptr)
	{}

	reference<Holder> ref () const
	{
		PFS_ASSERT(_holder);
		return reference<Holder>(*_holder, *this);
	}

	key_type key () const
	{
		return _ptr->first;
	}

	value_type value () const
	{
		return _ptr->second;
	}

	value_type operator * () const
	{
		return _ptr->second;
	}

	map_pointer & operator ++ ()
	{
		++_ptr;
		return *this;
	}

	map_pointer operator ++ (int)
	{
		map_pointer r(*this);
		this->operator ++ ();
		return r;
	}

	map_pointer & operator -- ()
	{
		--_ptr;
		return *this;
	}

	map_pointer operator -- (int)
	{
		map_pointer r(*this);
		this->operator -- ();
		return r;
	}

	const iterator_type base () const { return _ptr; }
};

template <typename Key, typename T, typename Compare, typename Alloc>
template <typename Holder>
map_pointer<Key, T, Compare, Alloc, Holder>
map_impl<Key, T, Compare, Alloc>::remove (const map_pointer<Key, T, Compare, Alloc, Holder> & ptr)
{
	iterator_type it = base_class::remove(ptr.base());
	return map_pointer<Key, T, Compare, Alloc, Holder>(ptr._holder, it);
}

template <typename Key, typename T, typename Compare, typename Alloc>
template <typename Holder>
std::pair<map_pointer<Key, T, Compare, Alloc, Holder>, bool>
map_impl<Key, T, Compare, Alloc>::insert (
	  Holder & holder
	, const value_type & value)
{
	std::pair<iterator_type, bool> r = base_class::insert(value);
	return std::pair<map_pointer<Key, T, Compare, Alloc, Holder>, bool>(
		map_pointer<Key, T, Compare, Alloc, Holder>(& holder, r.first)
		, r.second);
}

template <typename Key, typename T, typename Compare, typename Alloc>
template <typename Holder>
map_pointer<Key, T, Compare, Alloc, Holder>
map_impl<Key, T, Compare, Alloc>::find (Holder & holder, const key_type & key) const
{
	map_impl * self = const_cast<map_impl *>(this);
	iterator_type r = self->base_class::find(key);
	return map_pointer<Key, T, Compare, Alloc, Holder>(& holder, r);
}

template <typename Key, typename T, typename Compare, typename Alloc>
template <typename Holder>
inline map_pointer<Key, T, Compare, Alloc, Holder>
map_impl<Key, T, Compare, Alloc>::beginPtr (Holder & holder)
{
	return map_pointer<Key, T, Compare, Alloc, Holder>(& holder, this->base_class::begin());
}

template <typename Key, typename T, typename Compare, typename Alloc>
template <typename Holder>
inline map_pointer<Key, T, Compare, Alloc, Holder>
map_impl<Key, T, Compare, Alloc>::cbeginPtr (Holder & holder) const
{
	map_impl * self = const_cast<map_impl *>(this);
	return map_pointer<Key, T, Compare, Alloc, Holder>(& holder, self->base_class::begin());
}

template <typename Key, typename T, typename Compare, typename Alloc>
template <typename Holder>
inline map_pointer<Key, T, Compare, Alloc, Holder>
map_impl<Key, T, Compare, Alloc>::endPtr (Holder & holder)
{
	return map_pointer<Key, T, Compare, Alloc, Holder>(& holder, this->base_class::end());
}

template <typename Key, typename T, typename Compare, typename Alloc>
template <typename Holder>
inline map_pointer<Key, T, Compare, Alloc, Holder>
map_impl<Key, T, Compare, Alloc>::cendPtr (Holder & holder) const
{
	map_impl * self = const_cast<map_impl *>(this);
	return map_pointer<Key, T, Compare, Alloc, Holder>(& holder, self->base_class::end());
}

template <typename Key, typename T, typename Compare, typename Alloc, typename Holder1, typename Holder2>
inline bool operator != (
		  const map_pointer<Key, T, Compare, Alloc, Holder1> & p1
		, const map_pointer<Key, T, Compare, Alloc, Holder2> & p2)
{
	return p1.base() != p2.base();
}

template <typename Key, typename T, typename Compare, typename Alloc, typename Holder1, typename Holder2>
inline bool operator == (
		  const map_pointer<Key, T, Compare, Alloc, Holder1> & p1
		, const map_pointer<Key, T, Compare, Alloc, Holder2> & p2)
{
	return p1.base() == p2.base();
}

template <typename Key, typename T, typename Compare, typename Alloc, typename Holder1, typename Holder2>
inline bool operator > (
		  const map_pointer<Key, T, Compare, Alloc, Holder1> & p1
		, const map_pointer<Key, T, Compare, Alloc, Holder2> & p2)
{
	return p1.base() > p2.base();
}

template <typename Key, typename T, typename Compare, typename Alloc, typename Holder1, typename Holder2>
inline bool operator >= (
		  const map_pointer<Key, T, Compare, Alloc, Holder1> & p1
		, const map_pointer<Key, T, Compare, Alloc, Holder2> & p2)
{
	return p1.base() >= p2.base();
}

template <typename Key, typename T, typename Compare, typename Alloc, typename Holder1, typename Holder2>
inline bool operator < (
		  const map_pointer<Key, T, Compare, Alloc, Holder1> & p1
		, const map_pointer<Key, T, Compare, Alloc, Holder2> & p2)
{
	return p1.base() < p2.base();
}

template <typename Key, typename T, typename Compare, typename Alloc, typename Holder1, typename Holder2>
inline bool operator <= (
		  const map_pointer<Key, T, Compare, Alloc, Holder1> & p1
		, const map_pointer<Key, T, Compare, Alloc, Holder2> & p2)
{
	return p1.base() <= p2.base();
}

} // pfs


#endif /* __PFS_BITS_MAP_IMPL_HPP__ */
