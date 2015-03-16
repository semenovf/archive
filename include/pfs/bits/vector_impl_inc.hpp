/*
 * vector_impl_inc.hpp
 *
 *  Created on: Nov 14, 2014
 *      Author: wladt
 */

#ifndef __PFS_BITS_VECTOR_IMPL_INC_HPP__
#define __PFS_BITS_VECTOR_IMPL_INC_HPP__

namespace pfs {

template <typename T, typename Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::insert (
		typename vector<T, Alloc>::const_iterator pos, typename vector<T, Alloc>::size_type count
		, const T & value)
{
	base_class::detach();
	PFS_ASSERT(pos >= cbegin() && pos <= cend());
	difference_type offset = pos - cbegin();

	base_class::cast()->insert(offset, count, value);
//#	if __cplusplus >= 201103L
//	base_class::cast()->insert(base_class::cast()->cbegin() + offset, count, value);
//#	else
//	base_class::cast()->insert(base_class::cast()->begin() + offset, count, value);
//#	endif
	return vector<T, Alloc>::iterator(this, data() + offset);
}

template <typename T, typename Alloc>
template <typename InputIterator>
typename vector<T, Alloc>::iterator vector<T, Alloc>::insert (
		typename vector<T, Alloc>::const_iterator pos
		, InputIterator first, InputIterator last)
{
	base_class::detach();
	PFS_ASSERT(pos >= cbegin() && pos <= cend());
	difference_type offset = pos - cbegin();

	base_class::cast()->insert(offset, first, last);
//#	if __cplusplus >= 201103L
//	base_class::cast()->insert(base_class::cast()->cbegin() + offset, first, last);
//#	else
//	base_class::cast()->insert(base_class::cast()->begin() + offset, first, last);
//#	endif

	return vector<T, Alloc>::iterator(this, data() + offset);
}

template <typename T, typename Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::erase (typename vector<T, Alloc>::const_iterator pos)
{
	base_class::detach();
	PFS_ASSERT(pos >= cbegin() && pos < cend());
	difference_type offset = pos - cbegin();

	base_class::cast()->erase(offset);
//#	if __cplusplus >= 201103L
//	base_class::cast()->erase(base_class::cast()->cbegin() + offset);
//#	else
//	base_class::cast()->erase(base_class::cast()->begin() + offset);
//#	endif

	return vector<T, Alloc>::iterator(this, data() + offset);
}

template <typename T, typename Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::erase (
		  typename vector<T, Alloc>::const_iterator first
		, typename vector<T, Alloc>::const_iterator last)
{
	base_class::detach();
	PFS_ASSERT(first >= cbegin() && first <= cend());

	if (last > cend())
		last = cend();

	PFS_ASSERT(last > first);
	difference_type from = first - cbegin();
	difference_type to = last - cbegin();

	base_class::cast()->erase(from, to);
//#	if __cplusplus >= 201103L
//	base_class::cast()->erase(base_class::cast()->cbegin() + from, base_class::cast()->cbegin() + to);
//#	else
//	base_class::cast()->erase(base_class::cast()->begin() + from, base_class::cast()->begin() + to);
//#	endif

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

#endif
