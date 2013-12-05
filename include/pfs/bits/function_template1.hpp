/**
 * @file function_template.hpp
 * @author wladt
 * @date Dec 5, 2013
 */

#ifndef __PFS_BITS_FUNCTION_TEMPLATE1_HPP__
#define __PFS_BITS_FUNCTION_TEMPLATE1_HPP__

#define PFS_FUNCTION_BASIC_SPECIFIC1(_Ret,_Arg1)                      \
		public:                                                       \
			virtual _Ret invoke (_Arg1) = 0;                          \
		};

#define PFS_FUNCTION_HOLDER_SPECIFIC1(_Ret,_Arg1)                     \
	private:                                                          \
		virtual _Ret invoke (_Arg1 arg1) { return _function(arg1); }  \
	};

#define PFS_FUNCTION_SPECIFIC1(_Ret,_Arg1)                            \
	public:                                                           \
		typedef _Ret signature_type (_Arg1);                          \
		typedef _Ret (* pointer) (_Arg1);                             \
		_Ret operator () (_Arg1 arg1) { return _invoker->invoke(arg1); }


#endif /* __PFS_BITS_FUNCTION_TEMPLATE1_HPP__ */
