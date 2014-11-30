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

#endif
