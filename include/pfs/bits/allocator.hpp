/*
 * allocator.hpp
 *
 *  Created on: Sep 17, 2014
 *      Author: wladt
 */

#ifndef __PFS_BITS_ALLOCATOR_HPP__
#define __PFS_BITS_ALLOCATOR_HPP__

namespace pfs {

template <class T>
struct allocator : public std::allocator<T>
{
	typedef std::allocator<T> base_class;
	typedef typename std::allocator<T>::size_type size_type;
	typedef T value_type;

	allocator () : base_class() {}
	allocator (const std::allocator<T> & a) : base_class(a) {}

	template <class U>
	allocator (const allocator<U> &) : base_class() /*noexcept*/ {}

	template <class U>
	struct rebind
	{
	   typedef allocator<U> other;
	};

//
//	value_type * allocate (std::size_t n, const void * = 0)
//	{
//		return base_class::allocate(n+1);
//	}
//	void deallocate (value_type * p, std::size_t /*n*/)
//	{
//		base_class::deallocate(p, 0);
//	}
};

}

#endif /* __PFS_BITS_ALLOCATOR_HPP__ */
