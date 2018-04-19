/*
 * stack.hpp
 *
 *      Author: wladt
 */

#ifndef __PFS_STACK_HPP__
#define __PFS_STACK_HPP__

#include <stack>

namespace pfs {

template <typename T, typename Sequence = std::deque<T> >
class stack : public std::stack<T, Sequence>
{
	typedef std::stack<T, Sequence> base_class;

public:
	typedef typename base_class::value_type       value_type;
	typedef typename base_class::reference        reference;
	typedef typename base_class::const_reference  const_reference;
	typedef typename base_class::size_type        size_type;
	typedef typename base_class::container_type   container_type;

public:
	stack ()
		: base_class()
	{}

	explicit stack (const container_type & cont)
	  : base_class(cont)
	{}

	stack (const stack & other)
		: base_class(other)
	{}

#if __cplusplus >= 201103L
	explicit stack (container_type && cont = container_type())
		: base_class(cont)
	{}

	stack (stack && other)
		: base_class(other)
	{}

	template <typename Alloc>
	explicit stack (const Alloc & alloc)
		: base_class(alloc)
	{}

	template <typename Alloc>
	stack (const container_type & cont, const Alloc & alloc)
		: base_class(cont, alloc)
	{}

	template <typename Alloc>
	stack (container_type && cont, const Alloc & alloc)
		: base_class(cont, alloc)
	{}

	template <typename Alloc>
	stack (const stack & other, const Alloc & alloc)
		: base_class(other, alloc)
	{}

	template <typename Alloc>
	stack (stack && other, const Alloc & alloc);
		: base_class(other, alloc)
	{}
#endif
};

} // pfs

#endif /* __PFS_STACK_HPP__ */
