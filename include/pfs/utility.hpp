/**
 * @file   utility.hpp
 * @author wladt
 * @date   Mar 26, 2013 2:43:49 PM
 *
 * @brief
 */

#ifndef __CWT_UTILITY_HPP__
#define __CWT_UTILITY_HPP__

namespace pfs {

#if 0 // TODO Need recognition of C++11 standard
#	include <algorithm> // until C++11
//#	include <utility>   // since C++11
template<typename T>
inline void swap(T & a, T & b)
{
	std::swap(a, b);
}

#else

template<typename T>
inline void swap(T & a, T & b)
{
	T tmp = a;
	a = b;
	b = tmp;
}
#endif

template<typename T>
inline T & max(T & a, T & b)
{
	return a >= b ? a : b;
}

template<typename T>
inline T & min(T & a, T & b)
{
	return a <= b ? a : b;
}

} // pfs

#endif /* __CWT_UTILITY_HPP__ */
