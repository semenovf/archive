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

#if __cplusplus >= 201103L // C++11

//std::function;

#else // __cplusplus >= 201103L

#define PFS_FUNCTION_MAX_ARGS 8

#include <pfs/bits/function0.hpp>
#if PFS_FUNCTION_MAX_ARGS > 0
#	include <pfs/bits/function1.hpp>
#endif
#if PFS_FUNCTION_MAX_ARGS > 1
#	include <pfs/bits/function2.hpp>
#endif
/*
#if PFS_FUNCTION_MAX_ARGS > 2
#	include <pfs/bits/function3.hpp>
#endif
#if PFS_FUNCTION_MAX_ARGS > 3
#	include <pfs/bits/function4.hpp>
#endif
#if PFS_FUNCTION_MAX_ARGS > 4
#	include <pfs/bits/function5.hpp>
#endif
#if PFS_FUNCTION_MAX_ARGS > 5
#	include <pfs/bits/function6.hpp>
#endif
#if PFS_FUNCTION_MAX_ARGS > 6
#	include <pfs/bits/function7.hpp>
#endif
#if PFS_FUNCTION_MAX_ARGS > 7
#	include <pfs/bits/function8.hpp>
#endif
*/
#endif // __cplusplus < 201103L

#endif /* __PFS_FUNCTIONAL_HPP__ */
