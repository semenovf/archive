/**
 * @file function_template.hpp
 * @author wladt
 * @date Dec 5, 2013
 */

#ifndef __PFS_BITS_FUNCTION_TEMPLATE_HPP__
#define __PFS_BITS_FUNCTION_TEMPLATE_HPP__

#define PFS_FUNCTION_COMMON                                                                    \
	private:                                                                                   \
		class function_basic;                                                                  \
		typedef pfs::shared_ptr<function_basic> invoker_type;                                  \
		invoker_type _invoker;                                                                 \
	public:                                                                                    \
		function () : _invoker() {}                                                            \
	                                                                                           \
		template <typename _Func>                                                              \
		function (_Func f) : _invoker(new function_holder<_Func>(f)) {}                        \
		function (const function & other) : _invoker(other._invoker->clone()) {}               \
		function & operator = (const function & other) { _invoker = other._invoker->clone(); return *this;}


#define PFS_FUNCTION_BASIC_COMMON                                     \
	private:                                                          \
		class function_basic                                          \
		{                                                             \
		public:                                                       \
			function_basic () {}                                      \
			virtual ~function_basic () {}                             \
			virtual pfs::shared_ptr<function_basic> clone () = 0;     \
		                                                              \
		private:                                                      \
			function_basic(const function_basic & );                  \
			void operator = (const function_basic &);

#define PFS_FUNCTION_HOLDER_COMMON                                    \
	private:                                                          \
		template <typename _Func>                                     \
		class function_holder : public function_basic                 \
		{                                                             \
		public:                                                       \
			function_holder(_Func func)                               \
				: function_basic()                                    \
				, _function(func)                                     \
			{}                                                        \
                                                                      \
			virtual invoker_type clone ()                             \
			{                                                         \
				return invoker_type(new function_holder(_function));  \
			}                                                         \
                                                                      \
		private:                                                      \
			_Func _function;


#endif /* __PFS_BITS_FUNCTION_TEMPLATE_HPP__ */
