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

#if __cplusplus >= 201103L // C++11

#	include <type_traits>

#else

namespace pfs {

struct true_type
{
	enum { value = 1 };
};

struct false_type
{
	enum { value = 0 };
};

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

template <bool, typename T1, typename>
struct conditional
{
	typedef T1 type;
};

template <typename T1, typename T2>
struct conditional<false, T1, T2>
{
	typedef T2 type;
};

// Define a nested type if some predicate holds.
// Same as conditional
template <bool, typename T1, typename>
struct if_else
{
	typedef T1 type;
};

template <typename T1, typename T2>
struct if_else<false, T1, T2>
{
	typedef T2 type;
};

template<typename T>
struct remove_const
{
	typedef T type;
};

template<typename T>
struct remove_const<const T>
{
	typedef T type;
};

template<typename T>
struct remove_volatile
{
	typedef T type;
};

template<typename T>
struct remove_volatile<volatile T>
{
	typedef T type;
};

template<typename T>
struct remove_cv
{
	typedef typename remove_volatile<typename remove_const<T>::type>::type type;
};

template <typename T, size_t Size = 0>
struct max_sizeof
{
	enum { result = sizeof(T) > Size ? sizeof(T) : Size };
};

/**
 * If T is an unsigned arithmetic type, provides the member constant value equal true.
 * For any other type, value is false.
 */
template <typename T>
struct is_unsigned { static const bool value = false; };

template <> struct is_unsigned<unsigned char>  { static const bool value = true; };
template <> struct is_unsigned<unsigned short> { static const bool value = true; };
template <> struct is_unsigned<unsigned int>   { static const bool value = true; };
template <> struct is_unsigned<unsigned long>  { static const bool value = true; };

#ifdef PFS_HAVE_LONGLONG
template <> struct is_unsigned<unsigned long long> { static const bool value = true; };
#endif

template <typename T>
struct is_signed { static const bool value = false; };

template <> struct is_unsigned<char>           { static const bool value = true; };
template <> struct is_unsigned<signed char>    { static const bool value = true; };
template <> struct is_unsigned<signed short>   { static const bool value = true; };
template <> struct is_unsigned<signed int>     { static const bool value = true; };
template <> struct is_unsigned<signed long>    { static const bool value = true; };

#ifdef PFS_HAVE_LONGLONG
template <> struct is_unsigned<signed long long> { static const bool value = true; };
#endif

template <typename T>
struct is_floating_point { static const bool value = false; };

template <> struct is_floating_point<float> { static const bool value = true; };
template <> struct is_floating_point<double> { static const bool value = true; };

#ifdef PFS_HAVE_LONG_DOUBLE
template <> struct is_floating_point<long double> { static const bool value = true; };
#endif

} // pfs

#endif

#if __DEPRECATED__
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

#endif

#endif /* __PFS_TYPE_TRAITS_HPP__ */
