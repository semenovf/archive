/**
 * @file function_template.hpp
 * @author wladt
 * @date Dec 5, 2013
 */

/*
#ifndef __PFS_FUNCTION_TEMPLATE_HPP__
#define __PFS_FUNCTION_TEMPLATE_HPP__
*/
// Base on code from https://github.com/prograholic/blog/blob/master/cxx_function/main.cpp
//
#include <pfs/pp/repeat.h>

#undef PFS_FUNCTION_TEMPLATE_PARMS
#undef PFS_FUNCTION_TEMPLATE_ARGS
#undef PFS_FUNCTION_PARMS
#undef PFS_FUNCTION_ARGS
#undef PFS_FUNCTION_MEMBER_TEMPLATE_PARMS
#undef PFS_FUNCTION_MEMBER_PARMS
#undef PFS_FUNCTION_MEMBER_ARGS

#if PFS_FUNCTION_ARGS_NUM > 0
#	define PFS_FUNCTION_TEMPLATE_PARMS  PFS_JOIN_TEMPLATE_ARGS(PFS_FUNCTION_ARGS_NUM,_A)
#	define PFS_FUNCTION_TEMPLATE_ARGS   PFS_JOIN_ARGS(PFS_FUNCTION_ARGS_NUM,_A)
#	define PFS_FUNCTION_PARMS           PFS_JOIN_PARMS(PFS_FUNCTION_ARGS_NUM,_A,a)
#	define PFS_FUNCTION_ARGS            PFS_JOIN_ARGS(PFS_FUNCTION_ARGS_NUM,a)
#	define PFS_FUNCTION_MEMBER_TEMPLATE_PARMS PFS_JOIN_TEMPLATE_ARGS(PFS_FUNCTION_ARGS_NUM,_B)
#	define PFS_FUNCTION_MEMBER_PARMS          PFS_JOIN_PARMS(PFS_FUNCTION_ARGS_NUM,_B,a)
#	define PFS_FUNCTION_MEMBER_ARGS           PFS_JOIN_ARGS_SHIFT1(PFS_FUNCTION_ARGS_NUM,a)
#else
#	define PFS_FUNCTION_TEMPLATE_PARMS
#	define PFS_FUNCTION_TEMPLATE_ARGS
#	define PFS_FUNCTION_PARMS
#	define PFS_FUNCTION_ARGS
#	define PFS_FUNCTION_MEMBER_TEMPLATE_PARMS
#	define PFS_FUNCTION_MEMBER_PARMS
#	define PFS_FUNCTION_MEMBER_ARGS
#endif

#undef PFS_FUNCTION_COMMA
#if PFS_FUNCTION_ARGS_NUM > 0
#	define 	PFS_FUNCTION_COMMA ,
#else
#	define 	PFS_FUNCTION_COMMA
#endif

#include <pfs/shared_ptr.hpp>

namespace pfs {

template <typename>
class function;

template <typename _Ret PFS_FUNCTION_COMMA PFS_FUNCTION_TEMPLATE_PARMS>
class function <_Ret (PFS_FUNCTION_PARMS)>
{
public:
	class function_basic;
	typedef pfs::shared_ptr<function_basic> invoker_type;
	typedef _Ret return_type;

private:
	invoker_type _invoker;

public:
	function () : _invoker() {}

	template <typename _Func>
	function (_Func f) : _invoker(new function_holder<_Func>(f)) {}

#if PFS_FUNCTION_ARGS_NUM > 0
	template <typename _Func, typename _Class>
	function (_Func _Class::* f)
			: _invoker(new member_function_holder<_Func PFS_FUNCTION_COMMA PFS_FUNCTION_TEMPLATE_ARGS>(f))	{}
#endif

	function (const function & other) : _invoker(other._invoker->clone()) {}
	function & operator = (const function & other) { _invoker = other._invoker->clone(); return *this;}

	return_type operator () (PFS_FUNCTION_PARMS) { return _invoker->invoke(PFS_FUNCTION_ARGS); }

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
		virtual return_type invoke (PFS_FUNCTION_PARMS) = 0;
	};

	template <typename _Func>
	class function_holder : public function_basic
	{
		typedef _Func value_type;
	public:
		function_holder(value_type func)
			: function_basic()
			, _function(func)
		{}

		virtual invoker_type clone ()
			{ return invoker_type(new function_holder(_function)); }

	private:
		virtual return_type invoke (PFS_FUNCTION_PARMS)
			{ return _function(PFS_FUNCTION_ARGS); }

		value_type _function;
	};

#if PFS_FUNCTION_ARGS_NUM > 0
	template <typename _Func PFS_FUNCTION_COMMA PFS_FUNCTION_MEMBER_TEMPLATE_PARMS>
	class member_function_holder : public function_basic
	{
		typedef _Func _B1::* value_type;
	public:
		member_function_holder(value_type f) : _function(f)	{}

		virtual return_type invoke (PFS_FUNCTION_MEMBER_PARMS)
			{ return (a1.*_function)(PFS_FUNCTION_MEMBER_ARGS); }

		virtual invoker_type clone ()
			{ return invoker_type(new member_function_holder(_function)); }

	private:
		value_type _function;
	};
#endif
};

} // pfs

//#endif /* __PFS_FUNCTIONAL_HPP__ */
