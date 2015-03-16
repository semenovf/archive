/*
 * vector_impl.hpp
 *
 *  Created on: 14 авг. 2014 г.
 *      Author: wladt
 */

#ifndef __PFS_BITS_VECTOR_IMPL_HPP__
#define __PFS_BITS_VECTOR_IMPL_HPP__

#include <vector>
#include <memory>
#include <pfs/bits/allocator.hpp>
#include <pfs/bits/reference.hpp>
#include <pfs/bits/iterator.hpp>

namespace pfs {

template <typename T, typename Alloc = pfs::allocator<T> >
class vector_impl : protected std::vector<T,Alloc>
{
public:
	typedef vector_impl<T, Alloc> self_class;
	typedef typename std::vector<T,Alloc> base_class;
	typedef typename base_class::value_type value_type;

	typedef T *       pointer;
	typedef const T * const_pointer;
	typedef size_t    size_type;
	typedef ptrdiff_t difference_type;

public:
	vector_impl () : base_class() {}
	vector_impl (size_t n, const T & v = T()) : base_class(n, v) {}
	virtual ~vector_impl () {}

	bool isEmpty () const { return base_class::empty(); }
	const_pointer constData () const { return base_class::data(); }
	pointer data ()                  { return base_class::data(); }

	void clear ()
	{
		base_class::clear();
	}


	size_type capacity () const
	{
		return base_class::capacity();
	}

	size_type size () const
	{
		return base_class::size();
	}

	template <typename InputIt>
	void insert (size_type pos, InputIt first, InputIt last)
	{
#	if __cplusplus >= 201103L
		base_class::insert(base_class::cbegin() + pos, first, last);
#	else
		base_class::insert(base_class::begin() + pos, first, last);
#	endif
	}

	void insert (size_type pos, size_type count, const T & value)
	{
#	if __cplusplus >= 201103L
		base_class::insert(base_class::cbegin() + pos, count, value);
#	else
		base_class::insert(base_class::begin() + pos, count, value);
#	endif
	}

	void resize (size_type count, const T & value)
	{
		base_class::resize(count, value);
	}

	void erase (size_type pos)
	{
#	if __cplusplus >= 201103L
		base_class::erase(base_class::cbegin() + pos);
#	else
		base_class::erase(base_class::begin() + pos);
#endif
	}

	void erase (size_type from, size_type to)
	{
#	if __cplusplus >= 201103L
		base_class::erase(base_class::cbegin() + from, base_class::cbegin() + to);
#	else
		base_class::erase(base_class::begin() + from, base_class::begin() + to);
#endif
	}

};

} // pfs

#endif
