/*
 * type_traits.hpp
 *
 *  Created on: Jan 30, 2015
 *      Author: wladt
 *
 *  @note Usage by MSC see below notes
 */

#ifndef __PFS_TYPE_TRAITS_HPP__
#define __PFS_TYPE_TRAITS_HPP__

#include <pfs.hpp>
#include <pfs/bits/type_traits.hpp>

#if __cplusplus >= 201103L // C++11
#	include <type_traits>
#endif

#if __cplusplus >= 201103L // C++11

namespace pfs {
template <size_t Len, size_t Align>
using aligned_storage = std::aligned_storage<Len, Align>;
} //pfs

#elif defined(PFS_CC_GNUC)

namespace pfs {

template <size_t Len, size_t Align>
struct aligned_storage {
	struct type {
		unsigned char data[Len] __attribute__((aligned(Align)));
	};
};
} // pfs

#elif defined(PFS_CC_MSC)
// In MSC version <= 1600 std::vector avoid of use aligned items:
//		error C2719: '_Val': formal parameter with __declspec(align('8')) won't be aligned.
// This case compile source with PFS_USE_DEFAULT_ALIGN_STORAGE macro set.
//

namespace pfs {

template <size_t Len, size_t Align>
struct aligned_storage;

template <size_t Len>
struct aligned_storage<Len,1> {
	struct type {
		__declspec(align(1)) unsigned char data[Len];
	};
};

template <size_t Len>
struct aligned_storage<Len,2> {
	struct type {
		__declspec(align(2)) unsigned char data[Len];
	};
};

template <size_t Len>
struct aligned_storage<Len,4> {
	struct type {
		__declspec(align(4)) unsigned char data[Len];
	};
};

template <size_t Len>
struct aligned_storage<Len,8> {
	struct type {
		__declspec(align(8)) unsigned char data[Len];
	};
};

template <size_t Len>
struct aligned_storage<Len,16> {
	struct type {
		__declspec(align(16)) unsigned char data[Len];
	};
};

template <size_t Len>
struct aligned_storage<Len,32> {
	struct type {
		__declspec(align(32)) unsigned char data[Len];
	};
};

template <size_t Len>
struct aligned_storage<Len,64> {
	struct type {
		__declspec(align(64)) unsigned char data[Len];
	};
};

template <size_t Len>
struct aligned_storage<Len,128> {
	struct type {
		__declspec(align(128)) unsigned char data[Len];
	};
};

template <size_t Len>
struct aligned_storage<Len,256> {
	struct type {
		__declspec(align(256)) unsigned char data[Len];
	};
};

template <size_t Len>
struct aligned_storage<Len,512> {
	struct type {
		__declspec(align(512)) unsigned char data[Len];
	};
};

template <size_t Len>
struct aligned_storage<Len,1024> {
	struct type {
		__declspec(align(1024)) unsigned char data[Len];
	};
};
} // pfs

#else
#	error Need to implement aligned_storage
#endif

#endif /* __PFS_TYPE_TRAITS_HPP__ */
