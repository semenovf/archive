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
#include "allocator.hpp"
#include "reference.hpp"
#include "iterator.hpp"

namespace pfs {

template <typename T, typename Alloc = pfs::allocator<T> >
class vector_impl : public std::vector<T,Alloc>
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
};

} // pfs

#	define __PFS_BITS_VECTOR_IMPL_HPP__POSTINCLUDE__

#else /* __PFS_BITS_VECTOR_IMPL_HPP__ */
#	ifdef __PFS_BITS_VECTOR_IMPL_HPP__POSTINCLUDE__
namespace pfs {

template <typename T, typename Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::insert (
		vector<T, Alloc>::const_iterator pos, vector<T, Alloc>::size_type count
		, const T & value)
{
	base_class::detach();
	PFS_ASSERT(pos >= cbegin() && pos <= cend());
	difference_type offset = pos - cbegin();

#	if __cplusplus >= 201103L
	base_class::cast()->insert(base_class::cast()->cbegin() + offset), count, val);
#	else
	base_class::cast()->insert(base_class::cast()->begin() + offset, count, value);
#	endif
	return vector<T, Alloc>::iterator(this, data() + offset);
}

template <typename T, typename Alloc>
template <typename InputIterator>
typename vector<T, Alloc>::iterator vector<T, Alloc>::insert (
		vector<T, Alloc>::const_iterator pos
		, InputIterator first, InputIterator last)
{
	base_class::detach();
	PFS_ASSERT(pos >= cbegin() && pos <= cend());
	difference_type offset = pos - cbegin();

#	if __cplusplus >= 201103L
	base_class::cast()->insert(base_class::cast()->cbegin() + offset, first, last);
#	else
	base_class::cast()->insert(base_class::cast()->begin() + offset, first, last);
#	endif

	return vector<T, Alloc>::iterator(this, data() + offset);
}

template <typename T, typename Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::erase (vector<T, Alloc>::const_iterator pos)
{
	base_class::detach();
	PFS_ASSERT(pos >= cbegin() && pos < cend());
	difference_type offset = pos - cbegin();

#	if __cplusplus >= 201103L
	base_class::cast()->erase(base_class::cast()->cbegin() + offset);
#	else
	base_class::cast()->erase(base_class::cast()->begin() + offset);
#	endif

	return vector<T, Alloc>::iterator(this, data() + offset);
}

template <typename T, typename Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::erase (
		vector<T, Alloc>::const_iterator first
		, vector<T, Alloc>::const_iterator last)
{
	base_class::detach();
	PFS_ASSERT(first >= cbegin() && first <= cend());

	if (last > cend())
		last = cend();

	PFS_ASSERT(last > first);
	difference_type from = first - cbegin();
	difference_type to = last - cbegin();

#	if __cplusplus >= 201103L
	base_class::cast()->erase(base_class::cast()->cbegin() + from, base_class::cast()->cbegin() + to);
#	else
	base_class::cast()->erase(base_class::cast()->begin() + from, base_class::cast()->begin() + to);
#	endif

	return vector<T, Alloc>::iterator(this, data() + from);
}


template <typename T, typename Alloc>
void vector<T, Alloc>::shrink_to_fit ()
{
	base_class::detach();
#	if __cplusplus >= 201103L
	base_class::cast()->shrink_to_fit();
#	else
	resize(size());
#	endif
}

template <typename T, typename Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::find (const T & value) const
{
	const_iterator it = pfs::find(cbegin(), cend(), value);
	vector<T, Alloc> * self = const_cast<vector<T, Alloc> *>(this);
	pointer p = const_cast<pointer>(data());
	difference_type index = it - cbegin();

	return (it != cend())
			? iterator(self, p + index)
			: iterator(self, p + size()); // end()
}

} // pfs
#	endif
#endif
