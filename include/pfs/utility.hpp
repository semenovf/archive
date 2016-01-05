/**
 * @file   utility.hpp
 * @author wladt
 * @date   Mar 26, 2013 2:43:49 PM
 *
 * @brief
 */

#ifndef __PFS_UTILITY_HPP__
#define __PFS_UTILITY_HPP__

#include <pfs.hpp>

// std::swap

#if __cplusplus >= 201103L // C++11
//#	include <algorithm> // until C++11
#	include <utility>   // since C++11
#	define PFS_HAVE_STD_SWAP 1

#elif __cplusplus < 201103L
#	include <algorithm>

#	define PFS_HAVE_STD_SWAP 1
#endif


namespace pfs {

template<typename T>
inline void swap (T & a, T & b)
{
#if PFS_HAVE_STD_SWAP
	std::swap(a, b);
#else
	T tmp = a;
	a = b;
	b = tmp;
#endif
}

} // pfs

#ifdef max
#	undef max
#endif

#ifdef min
#	undef min
#endif

namespace pfs {
//
// see https://sites.google.com/site/murmurhash
//
DLL_API uint32_t hash32 (const void * key, int len, unsigned int seed);

#ifdef PFS_HAVE_INT64
#	ifdef PFS_OS_64BITS
		DLL_API uint64_t hash64 (const void * key, int len, unsigned int seed);
#	else
		DLL_API uint64_t hash64 (const void * key, int len, unsigned int seed);
#	endif
#endif

template <typename T>
inline T min (T a, T b) { return a <= b ? a : b; }

template <typename T>
inline T max (T a, T b) { return a >= b ? a : b; }

template <typename T>
inline T abs (T x) { return x < 0 ? x * T(-1) : x; }

template <typename T, typename U, typename W>
inline bool approx_eq (const T a, const U b, const W diff)
{
    T d = a - b;
    return d < 0 ? d + diff >= 0 : d <= diff;
}

// FIXME
//template <typename T>
//inline bool approx_eq (const T a, const T b)
//{
//    return ! (a > b || a < b);
//}

} // pfs

#define PFS_AUTO_CTOR_FUNCTION(func)           \
    static const struct func ## _ctor_struct { \
        func ## _ctor_struct () { func(); }    \
    } func ## _ctor_struct;

#define PFS_AUTO_DTOR_FUNCTION(func)           \
    static const struct func ## _dtor_struct { \
        func ## _dtor_struct () { }            \
        ~ func ## _dtor_struct () { func(); }  \
    } func ## _dtor_instance;

#endif /* __PFS_UTILITY_HPP__ */
