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
protected:
	typedef std::stack<T> impl;

	pimpl _d; //PFS_PIMPL_INLINE(stack, protected, impl);

public:
	typedef T item_type;
	typedef typename impl::size_type size_type;
	typedef typename impl::value_type value_type;

public:
	stack () : _d(new impl()) {}

	bool isEmpty() const { return _d.cast<impl>()->empty(); }
	void clear() { stack s; swap(s); }
	void swap(stack & o) { _d.swap<impl>(o._d); }
	size_type size() const { return _d.cast<impl>()->size(); }
	value_type & top() { return _d.cast<impl>()->top(); }
	const value_type & top() const { return _d.cast<impl>()->top(); }
	void push (const value_type & v) { _d.cast<impl>()->push(v); }
	void pop () { _d.cast<impl>()->pop(); }
};

} // cwt

#endif /* __PFS_STACK_HPP__ */
