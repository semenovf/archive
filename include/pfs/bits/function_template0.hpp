/**
 * @file function_template.hpp
 * @author wladt
 * @date Dec 5, 2013
 */

#ifndef __PFS_BITS_FUNCTION_TEMPLATE0_HPP__
#define __PFS_BITS_FUNCTION_TEMPLATE0_HPP__

#define PFS_FUNCTION_BASIC_SPECIFIC0(_Ret)                            \
		public:                                                       \
			virtual _Ret invoke () = 0;                               \
		};

#define PFS_FUNCTION_HOLDER_SPECIFIC0(_Ret)                           \
	private:                                                          \
		virtual _Ret invoke () { return _function(); }                \
	};

#define PFS_FUNCTION_SPECIFIC0(_Ret)                                  \
	public:                                                           \
		typedef _Ret signature_type ();                               \
		typedef _Ret (* pointer) ();                                  \
		_Ret operator () () { return _invoker->invoke(); }


#endif /* __PFS_BITS_FUNCTION_TEMPLATE0_HPP__ */
