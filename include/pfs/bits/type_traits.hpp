/*
 * type_traits.hpp
 *
 *  Created on: 05 апр. 2015 г.
 *      Author: wladt
 */

#ifndef __PFS_BITS_TYPE_TRAITS_HPP__
#define __PFS_BITS_TYPE_TRAITS_HPP__

namespace pfs {

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

} // pfs


#endif /* __PFS_BITS_TYPE_TRAITS_HPP__ */
