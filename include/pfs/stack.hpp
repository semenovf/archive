/*
 * stack.hpp
 *
 *      Author: wladt
 */

#ifndef __PFS_STACK_HPP__
#define __PFS_STACK_HPP__

#include <pfs/pimpl.hpp>
#include <pfs/bits/stack_impl.hpp>
#include <pfs/bits/reference.hpp>

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


template <typename T>
class stack : public nullable<stack_impl<T> >
{
protected:
	typedef stack_impl<T> impl_class;
	typedef nullable<impl_class>     base_class;
	typedef stack                    self_class;

public:
	typedef T          value_type;
	typedef size_t     size_type;
	typedef value_type char_type;
	typedef T *        pointer;
	typedef const T *  const_pointer;
	typedef pfs::reference<self_class>         reference;
	typedef pfs::reference<const self_class>   const_reference;

public:
	stack () : base_class() {}
	virtual ~stack () {}

	bool isEmpty () const { return base_class::isNull() || size() == 0; }
	bool empty () const { return isEmpty(); }
	void clear ()  { base_class::detach(); swap(self_class()); }

	size_type size () const { return base_class::cast()->size(); }

	T & topValueRef () { return base_class::cast()->topRef(); }
	const T & topValueRef () const { return base_class::cast()->topRef(); }

	value_type topValue () const { return topValueRef(); }
	reference top () const
	{
		stack * self = const_cast<stack *>(this);
		return reference(*self, self->cast()->topPtr());
	}

	void push (const value_type & v) { base_class::detach(); base_class::cast()->push(v); }
	void pop () { base_class::detach(); base_class::cast()->pop(); }

	stack & operator << (const value_type & v) { push(v); return *this; }

	void detach_and_assign (pointer & p, const value_type & value); // pfs::reference class requirement
};

template <typename T>
void stack<T>::detach_and_assign (pointer & p, const value_type & value)
{
	PFS_ASSERT(base_class::cast()->topPtr() == p);
	base_class::detach();
	*base_class::cast()->topPtr() = value;
}



} // pfs

#endif /* __PFS_STACK_HPP__ */
