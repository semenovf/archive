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
//namespace pfs {

#if __cplusplus >= 201103L // C++11
#	include <algorithm> // until C++11
//#	include <utility>   // since C++11
template<typename T>
inline void pfs_swap (T & a, T & b)
{
	std::swap(a, b);
}

//#elif defined(_XUTILITY_) // MSVC
//
//template<typename T>
//inline void swap (T & a, T & b)
//{
//	std::swap(a, b);
//}
//
#else
template<typename T>
inline void pfs_swap (T & a, T & b)
{
	T tmp = a;
	a = b;
	b = tmp;
}
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

} // pfs

#endif /* __PFS_UTILITY_HPP__ */
