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

#elif defined(PFS_CC_GNUC)

template <size_t Len, size_t Align>
struct aligned_storage {
	struct type {
		unsigned char data[Len] __attribute__((aligned(Align)));
	};
};

#elif defined(PFS_CC_MSC)

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

#else
#	error Do not know how to apply alignment!
#endif

struct true_type { enum {value = 1}; };
struct false_type { enum {value = 0}; };

// Define a nested type if some predicate holds.
template <bool, typename>
struct enable_if
{};

template <typename T>
struct enable_if<true, T>
{
	typedef T type;
};

// Compare for equality of types.
template<typename, typename>
struct are_same
{
	enum { value = 0 };
    typedef false_type type;
};

template<typename T>
struct are_same<T, T>
{
	enum { value = 1 };
	typedef true_type type;
};

template<class T> struct is_const          : false_type {};
template<class T> struct is_const<const T> : true_type {};

}

#endif /* __PFS_TYPE_TRAITS_HPP__ */
