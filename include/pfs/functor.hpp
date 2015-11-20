/*
 * functor.hpp
 *
 *  Created on: Nov 16, 2015
 *      Author: wladt
 */

#ifndef __PFS_FUNCTOR_HPP__
#define __PFS_FUNCTOR_HPP__

namespace pfs {

template <typename T>
struct base_type_trait { typedef T type; };

template <typename T>
struct base_type_trait<const T &> { typedef T type; };

template <typename Return>
class functor_base
{
public:
	typedef Return return_type;
	typedef return_type (* funcptr_type) ();

protected:
	size_t _size;
	funcptr_type _f;

	functor_base (size_t size, funcptr_type f)
		: _size(size)
		, _f(f)
	{}

public:
	virtual ~functor_base () {}

	size_t size () const
	{
		return _size;
	}

	virtual return_type operator () () const = 0;
};

template <typename Return>
class functor0 : public functor_base<Return>
{
public:
	typedef functor_base<Return> base_class;
	typedef Return return_type;
	typedef return_type (* functptr_type) ();

	functor0 (functptr_type f)
		: base_class(sizeof(functor0<return_type>), f)
	{}

	virtual ~functor0 () {}

	virtual return_type operator () () const
	{
		return this->_f();
	};
};

template <typename Return, typename Arg1>
class functor1 : public functor_base<Return>
{
	typename base_type_trait<Arg1>::type _a1;

public:
	typedef functor_base<Return> base_class;
	typedef Return return_type;
	typedef return_type (* funcptr_type) (Arg1);

public:
	functor1 (funcptr_type f, Arg1 a1)
		: base_class(sizeof(functor1)
				, reinterpret_cast<typename base_class::funcptr_type>(f))
		, _a1(a1)
	{}

	virtual ~functor1 () {}

	virtual return_type operator () () const
	{
		funcptr_type f = reinterpret_cast<funcptr_type>(this->_f);
		return f(_a1);
	};
};


template <typename Return, typename Arg1, typename Arg2>
class functor2 : public functor_base<Return>
{
	typename base_type_trait<Arg1>::type _a1;
	typename base_type_trait<Arg2>::type _a2;

public:
	typedef functor_base<Return> base_class;
	typedef Return return_type;
	typedef return_type (* funcptr_type) (Arg1, Arg2);

public:
	functor2 (funcptr_type f, Arg1 a1, Arg2 a2)
		: base_class(sizeof(functor2)
				, reinterpret_cast<typename base_class::funcptr_type>(f))
		, _a1(a1)
		, _a2(a2)
	{}

	virtual ~functor2 () {}

	virtual return_type operator () () const
	{
		funcptr_type f = reinterpret_cast<funcptr_type>(this->_f);
		return f(_a1, _a2);
	};
};

} // pfs

#endif /* __PFS_FUNCTOR_HPP__ */
