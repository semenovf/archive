/**
 * @file functional.hpp
 * @author wladt
 * @date Dec 5, 2013
 */

#ifndef __PFS_FUNCTIONAL_HPP__
#define __PFS_FUNCTIONAL_HPP__

#if __cplusplus >= 201103L
#	include <functional>
#endif

#include <pfs/shared_ptr.hpp>
#include <pfs/bits/function_template.hpp>
#include <pfs/bits/function_template0.hpp>
#include <pfs/bits/function_template1.hpp>

namespace pfs {

#if __cplusplus >= 201103L // C++11

//std::function;

#else

// Base on code from https://github.com/prograholic/blog/blob/master/cxx_function/main.cpp
//

#define PFS_FUNCTION_TEMPLATE_PARMS
#define PFS_FUNCTION_SIG
#define PFS_FUNCTION_PARMS
#define PFS_FUNCTION_ARGS

template <typename>
class funktion;

template <typename _Ret PFS_FUNCTION_TEMPLATE_PARMS>
class funktion <_Ret (PFS_FUNCTION_SIG)>
{
public:
	class function_basic;
	typedef pfs::shared_ptr<function_basic> invoker_type;
	typedef _Ret return_type;
	typedef return_type (* pointer) (PFS_FUNCTION_PARMS);
	//typedef return_type signature_type ();

private:
	invoker_type _invoker;

public:
	funktion () : _invoker() {}

	template <typename _Func>
	funktion (_Func f) : _invoker(new function_holder<_Func>(f)) {}
	funktion (const funktion & other) : _invoker(other._invoker->clone()) {}
	funktion & operator = (const funktion & other) { _invoker = other._invoker->clone(); return *this;}

	return_type operator () (PFS_FUNCTION_PARMS) { return _invoker->invoke(PFS_FUNCTION_ARGS); }

//private:
	class function_basic
	{
	public:
		function_basic () {}
		virtual ~function_basic () {}
		virtual pfs::shared_ptr<function_basic> clone () = 0;

	private:
		function_basic(const function_basic & );
		void operator = (const function_basic &);
	public:
		virtual return_type invoke (PFS_FUNCTION_SIG) = 0;
	};

private:
	template <typename _Func>
	class function_holder : public function_basic
	{
	public:
		function_holder(_Func func)
			: function_basic()
			, _function(func)
		{}

		virtual invoker_type clone ()
		{
			return invoker_type(new function_holder(_function));
		}

	private:
		_Func _function;
	private:
		virtual return_type invoke (PFS_FUNCTION_PARMS) { return _function(PFS_FUNCTION_ARGS); }
	};
};


template <typename>
class function;

template <typename _Ret>
class function <_Ret (void)>
{
	PFS_FUNCTION_COMMON
	PFS_FUNCTION_BASIC_COMMON
	PFS_FUNCTION_BASIC_SPECIFIC0(_Ret)
	PFS_FUNCTION_HOLDER_COMMON
	PFS_FUNCTION_HOLDER_SPECIFIC0(_Ret)
	PFS_FUNCTION_SPECIFIC0(_Ret)
};


template <typename _Ret, typename _Arg1>
class function<_Ret (_Arg1)>
{
	PFS_FUNCTION_COMMON
	PFS_FUNCTION_BASIC_COMMON
	PFS_FUNCTION_BASIC_SPECIFIC1(_Ret, _Arg1)
	PFS_FUNCTION_HOLDER_COMMON
	PFS_FUNCTION_HOLDER_SPECIFIC1(_Ret, _Arg1)
	PFS_FUNCTION_SPECIFIC1(_Ret, _Arg1)

#ifdef __COMMENT__
/*
public:
	template <typename FunctionType, typename ClassType>
	function(FunctionType ClassType::* f)
			: _invoker(new member_function_holder<FunctionType, _Arg1>(f))
	{}
*/

/*
	template <typename FunctionType, typename ClassType, typename RestArgumentTypes>
	class member_function_holder : public function_holder_base
	{
	public:

			typedef FunctionType ClassType::* member_function_signature_t;

			member_function_holder(member_function_signature_t f)
					: _function(f)
			{}

			virtual _Ret invoke(ClassType obj, RestArgumentTypes restArgs)
			{
					return (obj.*_function)(restArgs);
			}

			virtual invoker_t clone()
			{
					return invoker_t(new member_function_holder(_function));
			}

	private:
			member_function_signature_t _function;
	};
*/
#endif
};

#endif

} // pfs

#endif /* __PFS_FUNCTIONAL_HPP__ */
