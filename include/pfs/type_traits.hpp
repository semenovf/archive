/*
 * type_traits.hpp
 *
 *  Created on: Jan 30, 2015
 *      Author: wladt
 */

#ifndef __PFS_TYPE_TRAITS_HPP__
#define __PFS_TYPE_TRAITS_HPP__

#include <pfs.hpp>

#if __cplusplus >= 201103L // C++11
#	include <type_traits>
#endif

namespace pfs {

#if __cplusplus >= 201103L // C++11
#	include <type_traits>
template <size_t Len, size_t Align>
using aligned_storage = std::aligned_storage<Len, Align>;

#else

template <size_t Len, size_t Align>
struct aligned_storage {
	struct type {
#if defined(PFS_CC_GNUC)
		unsigned char data[Len] __attribute__((aligned(Align)));
#elif defined(PFS_CC_MSC)
		__declspec((align(Align))) unsigned char data[Len];
#else
#	error Do not know how to apply alignment!
#endif
	};
};

#endif

}

#endif /* __PFS_TYPE_TRAITS_HPP__ */
