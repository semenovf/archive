/*
 * type_traits.hpp
 *
 *  Created on: Oct 23, 2016
 *      Author: wladt
 */

#ifndef __PFS_CXX98_TYPE_TRAITS_HPP__
#define __PFS_CXX98_TYPE_TRAITS_HPP__

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

template <typename T>
struct is_bool
    : public is_same<bool, typename remove_cv<T>::type>
{};

namespace details {

template <typename>
struct is_integral
    : public false_type
{ };

template <>
struct is_integral<bool>
    : public true_type
{ };

template <>
struct is_integral<char>
    : public true_type
{ };

template <>
struct is_integral<signed char>
    : public true_type
{ };

template <>
struct is_integral<unsigned char>
    : public true_type
{ };

template <>
struct is_integral<wchar_t>
    : public true_type { };


template <>
struct is_integral<short>
    : public true_type
{ };

template <>
struct is_integral<unsigned short>
    : public true_type
{ };

template <>
struct is_integral<int>
    : public true_type
{ };

template <>
struct is_integral<unsigned int>
    : public true_type
{ };

template <>
struct is_integral<long>
    : public true_type
{ };

template <>
struct is_integral<unsigned long>
    : public true_type
{ };

#if PFS_HAVE_LONGLONG

template <>
struct is_integral<long long>
    : public true_type
{ };

template <>
struct is_integral<unsigned long long>
    : public true_type
{ };

#endif // PFS_HAVE_LONGLONG

} // details

template <typename T>
struct is_integral
    : public details::is_integral<typename remove_cv<T>::type>::type
{ };

namespace details {

template <typename>
struct is_floating_point
    : public false_type
{ };

template <>
struct is_floating_point<float>
    : public true_type
{ };

template <>
struct is_floating_point<double>
    : public true_type
{ };

#if PFS_HAVE_LONG_DOUBLE

template <>
struct is_floating_point<long double>
    : public true_type
{ };

#endif

} // details

template <typename T>
struct is_floating_point
    : public details::is_floating_point<typename remove_cv<T>::type>::type
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


template <typename T>
struct is_const
    : public false_type
{};

template <typename T>
struct is_const<const T>
    : public true_type
{};

template <typename>
struct is_volatile
    : public false_type
{};

template <typename T>
struct is_volatile<T volatile>
    : public true_type
{};

} // pfs

#endif /* __PFS_CXX98_TYPE_TRAITS_HPP__ */
