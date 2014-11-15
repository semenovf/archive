/*
 * allocator.hpp
 *
 *  Created on: Sep 17, 2014
 *      Author: wladt
 */

#ifndef __PFS_BITS_ALLOCATOR_HPP__
#define __PFS_BITS_ALLOCATOR_HPP__

namespace pfs {

template <class T, std::size_t nextra = 0>
struct allocator : public std::allocator<T>
{
	typedef std::allocator<T> base_class;
	typedef typename std::allocator<T>::size_type size_type;
	typedef T value_type;

	allocator () : base_class() {}
	allocator (const std::allocator<T> & a) : base_class(a) {}

	template <class U>
	allocator (const allocator<U, nextra> &) : base_class() /*noexcept*/ {}

	template <class U>
	struct rebind
	{
	   typedef allocator<U, nextra> other;
	};

	value_type * allocate (std::size_t n, const void * = 0)
	{
		return base_class::allocate(n + nextra);
	}

	void deallocate (value_type * p, std::size_t /*n*/)
	{
		base_class::deallocate(p, 0);
	}
};

} // pfs


#ifdef __COMMENT__
// see http://en.cppreference.com/w/cpp/concept/Allocator
struct allocator
{
	typedef char value_type;

	//template<typename _Tp1>
	struct rebind
	{ typedef allocator other; };

	allocator(/*ctor args*/) {}
	allocator (const bytearray_allocator &) {}
	char * allocate (std::size_t n) { return static_cast<char *>(::operator new((n+1) * sizeof(char))) ; }
	void deallocate (char * p, std::size_t) { ::operator delete(p); }
};

// Returns true only if the storage allocated by the allocator a1
// can be deallocated through a2. Establishes reflexive, symmetric,
// and transitive relationship. Does not throw exceptions.
bool operator == (const allocator & /*a1*/, const bytearray_allocator & /*a2*/) { return true; }
// same as !(a1==a2)
bool operator != (const allocator & a1, const bytearray_allocator & a2) { return !(a1==a2); }
#endif

#endif /* __PFS_BITS_ALLOCATOR_HPP__ */
