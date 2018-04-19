/**
 * @file   utility.hpp
 * @author wladt
 * @date   Mar 26, 2013 2:43:49 PM
 *
 * @brief
 */

#ifndef __CWT_UTILITY_HPP__
#define __CWT_UTILITY_HPP__

#include <cwt/cwt.hpp>

CWT_NS_BEGIN

#if 0 // TODO Need recognition of CXX11 standard
#	include <algorithm> // until C++11
//#	include <utility>   // since C++11
template<typename T>
inline void swap(T &a, T &b)
{
	std::swap(a, b);
}
#
#else
template<typename T>
inline void swap(T &a, T &b)
{
	T tmp = a;
	a = b;
	b = tmp;
}
#endif

CWT_NS_END

#endif /* __CWT_UTILITY_HPP__ */
