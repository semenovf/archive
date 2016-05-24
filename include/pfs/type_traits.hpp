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

template <typename T, T v>
struct integral_constant 
{
    static T const value = v;
     
    typedef T value_type;
     
    typedef integral_constant<T,v> type;
    
    operator value_type () const { return value; }
};

//template <typename T, T v>
//T integral_constant<T, v>::value;
  
//template <bool v>
//struct integral_constant<bool, v> 
//{
//    static bool const value = v;
//
//    typedef bool value_type;
//    
//    typedef integral_constant<bool, v> type;
//
//    operator value_type () const { return value; }
//};

typedef integral_constant<bool, true> true_type;
   
typedef integral_constant<bool, false> false_type;

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

namespace details {

template <typename T1, typename T2>
struct and_op2
    : public conditional<T1::value, T2, T1>::type
{ };

template <typename T>
struct not_op
    : public integral_constant<bool, !T::value>
{ };

}

template<typename T>
struct remove_const
{
	typedef T type;
};

template<typename T>
struct remove_const<T const>
{
	typedef T type;
};

template<typename T>
struct remove_volatile
{
	typedef T type;
};

template<typename T>
struct remove_volatile<T volatile>
{
	typedef T type;
};

template<typename T>
struct remove_cv
{
	typedef typename remove_volatile<typename remove_const<T>::type>::type type;
};


template <typename T>
struct remove_reference
{
    typedef T type;
};

template <typename T>
struct remove_reference<T &>
{
    typedef T type;
};

template <typename T>
struct remove_pointer
{
    typedef T type;
};

template <typename T>
struct remove_pointer<T *>
{
    typedef T type;
};

template <typename T>
struct remove_pointer<T const *>
{
    typedef T type;
};

template <typename T>
struct remove_pointer<T volatile *>
{
    typedef T type;
};

template <typename T>
struct remove_pointer<T const volatile *>
{
    typedef T type;
};

//struct true_type
//{
//	enum { value = 1 };
//};
//
//struct false_type
//{
//	enum { value = 0 };
//};

// Compare for equality of types.
template <typename, typename>
struct is_same
    : public false_type 
{ };

template <typename T>
struct is_same<T, T>
    : public true_type 
{ };

// TODO OBSOLETE, use is_same
template<typename, typename>
struct are_same
{
	enum { value = 0 };
    typedef false_type type;
};

// TODO OBSOLETE, use is_same
template<typename T>
struct are_same<T, T>
{
	enum { value = 1 };
	typedef true_type type;
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

template <typename T>
struct is_void 
    : public is_same<void, typename remove_cv<T>::type> 
{};

template <typename>
struct __is_integral_helper
    : public false_type 
{ };

template <>
struct __is_integral_helper<bool>
    : public true_type 
{ };
  
template <>
struct __is_integral_helper<char>
    : public true_type 
{ };

template <>
struct __is_integral_helper<signed char>
    : public true_type 
{ };

template <>
struct __is_integral_helper<unsigned char>
    : public true_type 
{ };

template <>
struct __is_integral_helper<wchar_t>
    : public true_type { };


template <>
    struct __is_integral_helper<short>
    : public true_type 
{ };

template <>
struct __is_integral_helper<unsigned short>
    : public true_type 
{ };

template <>
struct __is_integral_helper<int>
    : public true_type
{ };

template <>
struct __is_integral_helper<unsigned int>
    : public true_type 
{ };

template <>
struct __is_integral_helper<long>
    : public true_type
{ };

template <>
struct __is_integral_helper<unsigned long>
    : public true_type 
{ };

#if PFS_HAVE_LONGLONG

template <>
struct __is_integral_helper<long long>
    : public true_type 
{ };

template <>
struct __is_integral_helper<unsigned long long>
    : public true_type 
{ };

#endif // PFS_HAVE_LONGLONG

template <typename T>
struct is_integral
    : public __is_integral_helper<typename remove_cv<T>::type>::type
{ };

template <typename>
struct __is_floating_point_helper
    : public false_type 
{ };

template <>
struct __is_floating_point_helper<float>
    : public true_type 
{ };

template <>
struct __is_floating_point_helper<double>
    : public true_type 
{ };

#if PFS_HAVE_LONG_DOUBLE

template <>
struct __is_floating_point_helper<long double>
    : public true_type 
{ };

#endif

template <typename T>
struct is_floating_point
    : public __is_floating_point_helper<typename remove_cv<T>::type>::type
{ };

/**
 * If T is an arithmetic type (that is, an integral type or a floating-point type), 
 * provides the member constant value equal @c true. For any other type, value is @c false. 
 */
template <typename T>
struct is_arithmetic 
    : public integral_constant<bool
        , is_integral<T>::value 
        || is_floating_point<T>::value> 
{ };

/**
 * If T is a fundamental type (that is, arithmetic type, void), 
 * provides the member constant value equal @c true. 
 * For any other type, value is @c false. 
 */
template <typename T>
struct is_fundamental
    : public integral_constant<bool
        , is_arithmetic<T>::value 
        || is_void<T>::value>
{};


namespace details {

template <typename>
struct is_unsigned
    : public false_type 
{ };

template <>
struct is_unsigned<bool>
    : public true_type 
{ };

template <>
struct is_unsigned<unsigned char>
    : public true_type 
{ };

template <>
struct is_unsigned<unsigned short>
    : public true_type 
{ };

template <>
struct is_unsigned<unsigned int>
    : public true_type 
{ };

template <>
struct is_unsigned<unsigned long>
    : public true_type 
{ };

#if PFS_HAVE_LONGLONG

template <>
struct is_unsigned<unsigned long long>
    : public true_type 
{ };

#endif

} // details

template <typename T>
struct is_unsigned
    : public details::is_unsigned<typename remove_cv<T>::type>::type
{ };

template <typename T>
struct is_signed
    : public details::and_op2<is_arithmetic<T>, details::not_op<is_unsigned<T> > >::type
{ };

#if __COMMENT__

template<class T> struct is_const          : false_type {};
template<class T> struct is_const<const T> : true_type {};

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

template <> struct is_signed<char>           { static const bool value = true; };
template <> struct is_signed<signed char>    { static const bool value = true; };
template <> struct is_signed<signed short>   { static const bool value = true; };
template <> struct is_signed<signed int>     { static const bool value = true; };
template <> struct is_signed<signed long>    { static const bool value = true; };

#ifdef PFS_HAVE_LONGLONG
template <> struct is_signed<signed long long> { static const bool value = true; };
#endif


template <typename T>
struct is_boolean { static const bool value = false; };

template <>
struct is_boolean<bool> { static const bool value = true; };


template <typename T>
struct is_number { static const bool value = is_integral<T>::value || is_floating_point<T>::value; };

// FIXME add `void` support and remove cv-qualifier (below implementations for too)
template <typename T>
struct is_fundamental { static const bool value = is_boolean<T>::value || is_number<T>::value; };
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
