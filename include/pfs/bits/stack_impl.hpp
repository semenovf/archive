/*
 * stack_impl.hpp
 *
 *  Created on: 09 Jan 2015
 *      Author: wladt
 */

#ifndef __PFS_BITS_STACK_IMPL_HPP__
#define __PFS_BITS_STACK_IMPL_HPP__

#include <stack>

namespace pfs {

template <typename T>
class stack_impl : protected std::stack<T, std::deque<T> >
{
public:
	typedef std::stack<T, std::deque<T> > base_class;
	typedef stack_impl     self_class;
	typedef size_t         size_type;
	typedef T              value_type;
//	typedef typename base_class::reference       reference;
//	typedef typename base_class::const_reference const_reference;

public:
	stack_impl () : base_class() {}
	virtual ~stack_impl () {}

	size_type size () const { return base_class::size(); }

	void push ( const value_type & value ) { base_class::push(value); }
	void pop () { base_class::pop(); }

	T * topPtr () { return & base_class::top(); }
	T & topRef () { return base_class::top(); }
	const T & topRef () const { return base_class::top(); }
};

} // pfs

#endif // __PFS_BITS_STACK_IMPL_HPP__
