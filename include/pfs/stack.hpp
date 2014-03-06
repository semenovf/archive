/*
 * vector.hpp
 *
 *  Created on: Jan 26, 2014
 *      Author: wladt
 */

#ifndef __PFS_STACK_HPP__
#define __PFS_STACK_HPP__

#include <stack>
#include <pfs/pimpl.hpp>

namespace pfs {

template <typename T>
class stack
{
	typedef std::stack<T> impl;
	PFS_PIMPL_INLINE(stack, protected, impl);

public:
	typedef T item_type;
	typedef typename impl::size_type size_type;
	typedef typename impl::value_type value_type;

public:
	stack () : _pimpl(new impl()) {}

	bool isEmpty() const { return _pimpl->empty(); }
	size_type size() const { return _pimpl->size(); }
	value_type & top() { return _pimpl->top(); }
	const value_type & top() const { return _pimpl->top(); }
	void push (const value_type & v) { _pimpl->push(v); }
	void pop () { _pimpl->pop(); }
};

} // cwt

#endif /* __PFS_STACK_HPP__ */
