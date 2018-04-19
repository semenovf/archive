/*
 * limits.hpp
 *
 *  Created on: Nov 24, 2015
 *      Author: wladt
 */

#ifndef __PFS_LIMITS_HPP__
#define __PFS_LIMITS_HPP__

#include <limits>
#include <pfs/bits/assert.h>

#ifndef PFS_HAVE_LONG_DOUBLE
#	if defined(LDBL_MIN) \
		|| defined(__LDBL_MIN__)      // valid for gcc

#		define PFS_HAVE_LONG_DOUBLE 1
#	endif
#endif

namespace pfs {

template <typename T> T inf_value ();
template <> inline float inf_value<float> () { return std::numeric_limits<float>::infinity(); }
template <> inline double inf_value<double> () { return std::numeric_limits<double>::infinity(); }

#if PFS_HAVE_LONG_DOUBLE

template <> inline long double inf_value<long double> () { return std::numeric_limits<long double>::infinity(); }

#endif

template <typename T> T nan_value ();
template <> inline float nan_value<float> () { return std::numeric_limits<float>::quiet_NaN(); }
template <> inline double nan_value<double> () { return std::numeric_limits<double>::quiet_NaN(); }

#if PFS_HAVE_LONG_DOUBLE

template <> inline long double nan_value<long double> () { return std::numeric_limits<long double>::quiet_NaN(); }

#endif

template <typename T>
inline bool is_nan (T x) { return false; }

template <> inline bool is_nan<float> (float x) { return x != x; }
template <> inline bool is_nan<double> (double x) { return x != x; }

#if PFS_HAVE_LONG_DOUBLE

template <> inline bool is_nan<long double> (long double x) { return x != x; }

#endif


template <typename T>
inline bool is_inf (T x) { return false; }

template <> inline bool is_inf<float> (float x) { return x == inf_value<float>() || x == - inf_value<float>(); }
template <> inline bool is_inf<double> (double x) { return x == inf_value<double>() || x == - inf_value<double>(); }

#if PFS_HAVE_LONG_DOUBLE

template <> inline bool is_inf<long double> (long double x) { return x == inf_value<long double>() || x == - inf_value<long double>();}

#endif


template <typename T> int min_exponent ();
template <typename T> int min_exponent10 ();
template <typename T> int max_exponent ();
template <typename T> int max_exponent10 ();

template <> inline int min_exponent<float>   () { return std::numeric_limits<float>::min_exponent; }
template <> inline int min_exponent10<float> () { return std::numeric_limits<float>::min_exponent10; }
template <> inline int max_exponent<float>   () { return std::numeric_limits<float>::max_exponent; }
template <> inline int max_exponent10<float> () { return std::numeric_limits<float>::max_exponent10; }

template <> inline int min_exponent<double>   () { return std::numeric_limits<double>::min_exponent; }
template <> inline int min_exponent10<double> () { return std::numeric_limits<double>::min_exponent10; }
template <> inline int max_exponent<double>   () { return std::numeric_limits<double>::max_exponent; }
template <> inline int max_exponent10<double> () { return std::numeric_limits<double>::max_exponent10; }

#if PFS_HAVE_LONG_DOUBLE

template <> inline int min_exponent<long double>   () { return std::numeric_limits<long double>::min_exponent; }
template <> inline int min_exponent10<long double> () { return std::numeric_limits<long double>::min_exponent10; }
template <> inline int max_exponent<long double>   () { return std::numeric_limits<long double>::max_exponent; }
template <> inline int max_exponent10<long double> () { return std::numeric_limits<long double>::max_exponent10; }

#endif

template <typename T> inline T max_value () { return std::numeric_limits<T>::max(); }
template <typename T> inline T min_value () { return std::numeric_limits<T>::min(); }

template <typename T> inline T max_value (const T &) { return std::numeric_limits<T>::max(); }
template <typename T> inline T min_value (const T &) { return std::numeric_limits<T>::min(); }


template <typename T1, typename T2>
inline T1 integral_cast_check (T2 x)
{
    PFS_ASSERT_RANGE(static_cast<T1>(x) >= min_value<T1>()
            && static_cast<T1>(x) <= max_value<T1>());
    return static_cast<T1>(x);
}

} // pfs

#endif /* __PFS_LIMITS_HPP__ */
