/*
 * stack.hpp
 *
 *      Author: wladt
 */

#ifndef __PFS_STACK_HPP__
#define __PFS_STACK_HPP__

#include <pfs/pimpl.hpp>
#include <pfs/bits/stack_impl.hpp>

namespace pfs {

//template <typename Class>
//struct emptyable
//{
//	bool isEmpty () const { return C::base_class::isNull() || C::size() == 0; }
//	bool empty () const { return isEmpty(); }
//
//};
//
//template <typename Class>
//struct clearable
//{
//
//};
//
//template <typename Impl, typename ValueT, typename RawT>
//struct container: public Impl
//{
//	typedef ValueT     value_type;
//	typedef RawT       raw_type;
//	typedef size_t     size_type;
//	typedef value_type char_type;
//};


template <typename T, typename Container = std::deque<T> >
class stack : public nullable<stack_impl<T, Container> >
{
protected:
	typedef stack_impl<T, Container> impl_class;
	typedef nullable<impl_class>     base_class;
	typedef stack                    self_class;

public:
	typedef T          value_type;
	typedef size_t     size_type;
	typedef value_type char_type;

public:
	stack () : base_class() {}
	virtual ~stack () {}

	bool isEmpty () const { return base_class::isNull() || size() == 0; }
	bool empty () const { return isEmpty(); }
	void clear ()  { base_class::detach(); swap(self_class()); }

	size_type size () const { return base_class::cast()->size(); }

	value_type top () const { return base_class::cast()->top(); }
	void push (const value_type & v) { base_class::detach(); base_class::cast()->push(v); }
	void pop () { base_class::detach(); base_class::cast()->pop(); }

	stack & operator << (const value_type & v) { push(v); return *this; }
};

} // pfs

#endif /* __PFS_STACK_HPP__ */
