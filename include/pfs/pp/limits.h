/**
 * @file   limits.h
 * @author wladt
 * @date   Jan 23, 2013 4:19:14 PM
 *
 * @brief
 */

#ifndef __PFS_PP_LIMITS_H__
#define __PFS_PP_LIMITS_H__

#include <float.h>
#include <math.h> /* for NAN */

#if defined(PFS_HAVE_INT64) && ! defined(PFS_OS_64BITS)
#	define PFS_LONG_LITERAL(x) x##LL
#	define PFS_ULONG_LITERAL(x) x##ULL
#else
#	define PFS_LONG_LITERAL(x) x##L
#	define PFS_ULONG_LITERAL(x) x##UL
#endif

#define PFS_INT8_MAX   127
#define PFS_INT8_MIN   (-127-1)
#define PFS_UINT8_MAX  255
#define PFS_UINT8_MIN  0
#define PFS_INT16_MAX  32767
#define	PFS_INT16_MIN  (-32768)
#define	PFS_UINT16_MAX 65535
#define	PFS_UINT16_MIN 0
#define	PFS_INT24_MAX  8388607
#define	PFS_INT24_MIN  (-8388608)
#define	PFS_UINT24_MAX 16777215
#define	PFS_UINT24_MIN 0
#define PFS_INT32_MAX  2147483647
#define	PFS_INT32_MIN  (-2147483647-1)
#define	PFS_UINT32_MAX 4294967295UL
#define	PFS_UINT32_MIN 0
#ifdef PFS_HAVE_INT64
#	define PFS_INT64_MAX  PFS_LONG_LITERAL(9223372036854775807)
#	define PFS_INT64_MIN (-PFS_INT64_MAX-PFS_LONG_LITERAL(1))
#	define PFS_UINT64_MAX PFS_ULONG_LITERAL(18446744073709551615)
#	define PFS_UINT64_MIN 0
#endif

#define PFS_CHAR_MAX   CHAR_MAX
#define PFS_CHAR_MIN   CHAR_MIN
#define PFS_BYTE_MAX   UCHAR_MAX
#define PFS_BYTE_MIN   0
#define PFS_SBYTE_MAX  SCHAR_MAX
#define PFS_SBYTE_MIN  SCHAR_MIN
#define PFS_SCHAR_MAX  SCHAR_MAX
#define PFS_SCHAR_MIN  SCHAR_MIN
#define PFS_UCHAR_MAX  UCHAR_MAX
#define PFS_UCHAR_MIN  0
#define PFS_USHORT_MAX USHRT_MAX
#define PFS_USHORT_MIN 0
#define PFS_SHORT_MAX  SHRT_MAX
#define PFS_SHORT_MIN  SHRT_MIN
#define PFS_UINT_MAX   UINT_MAX
#define PFS_UINT_MIN   0
#define PFS_INT_MAX    INT_MAX
#define PFS_INT_MIN    INT_MIN
#define PFS_ULONG_MAX  ULONG_MAX
#define PFS_ULONG_MIN  0
#define PFS_LONG_MAX   LONG_MAX
#define PFS_LONG_MIN   LONG_MIN

#ifdef PFS_HAVE_LONGLONG
#	define PFS_LLONG_MAX   LLONG_MAX
#	define PFS_LLONG_MIN   LLONG_MIN
#	define PFS_ULLONG_MAX  ULLONG_MAX
#	define PFS_ULLONG_MIN  0
#endif

#if defined(PFS_HAVE_INT64) && defined(PFS_HAVE_LONGLONG)
#	define PFS_INTEGRAL_MAX  PFS_LLONG_MAX
#	define PFS_INTEGRAL_MIN  PFS_LLONG_MIN
#	define PFS_UINTEGRAL_MAX PFS_ULLONG_MAX
#	define PFS_UINTEGRAL_MIN 0
#else
#	define PFS_INTEGRAL_MAX  PFS_LONG_MAX
#	define PFS_INTEGRAL_MIN  PFS_LONG_MIN
#	define PFS_UINTEGRAL_MAX PFS_ULONG_MAX
#	define PFS_UINTEGRAL_MIN 0
#endif

#define PFS_FLOAT_MIN  FLT_MIN /* 1.175494351e-38F */
#define PFS_FLOAT_MAX  FLT_MAX /* 3.402823466e+38F */

#ifdef DBL_MIN
#	define PFS_DOUBLE_MIN DBL_MIN /* 2.22507385850720138309e-308 */
#	define PFS_DOUBLE_MAX DBL_MAX /* 1.79769313486231570815e+308 */
#	define PFS_DOUBLE_MIN_10_EXP DBL_MIN_10_EXP /* -307 */
#	define PFS_DOUBLE_MAX_10_EXP DBL_MAX_10_EXP /* 308 */
#else
#	error "DBL_MIN is not defined"
#endif

/*
 * sizeof(FLT) * 8 = FLT_MANT_DIG + lg(FLT_MAX_EXP) + 1
 * 32: FLT_MANT_DIG = 24, FLT_MAX_EXP = 128
 * 64: DBL_MANT_DIG = 53, DBL_MAX_EXP = 1024
 * 128: LDBL_MANT_DIG = 113, LDBL_MAX_EXP = 16384
 *
 * x86 Extended Precision Format:
 * @see http://en.wikipedia.org/wiki/Extended_precision
 * sizeof(FLT) * 8 = FLT_MANT_DIG + lg(FLT_MAX_EXP) + 1 + 1
 * 80: LDBL_MANT_DIG = 64, LDBL_MAX_EXP = 16384: 64 + 14 + 1 + 1 = 80
 */

#ifdef LDBL_MIN
#	if LDBL_MANT_DIG == 53
#		define PFS_HAVE_REAL64 1
#	elif LDBL_MANT_DIG == 64
#		define PFS_HAVE_REAL80 1
#	elif LDBL_MANT_DIG == 113
#		define PFS_HAVE_REAL128 1
#	else
#		error "Unsupported platform"
#	endif

#	define PFS_HAVE_LONG_DOUBLE 1
#	define PFS_LONG_DOUBLE_MIN LDBL_MIN
#	define PFS_LONG_DOUBLE_MAX LDBL_MAX
#	define PFS_LONG_DOUBLE_MIN_10_EXP LDBL_MIN_10_EXP
#	define PFS_LONG_DOUBLE_MAX_10_EXP LDBL_MAX_10_EXP
#endif

#ifdef PFS_HAVE_LONG_DOUBLE
#	define PFS_REAL_LITERAL(x) x##L
#	define PFS_REAL_MIN         PFS_LONG_DOUBLE_MIN
#	define PFS_REAL_MAX         PFS_LONG_DOUBLE_MAX
#	define PFS_REAL_MIN_10_EXP  PFS_LONG_DOUBLE_MIN_10_EXP
#	define PFS_REAL_MAX_10_EXP  PFS_LONG_DOUBLE_MAX_10_EXP
#else
#	define PFS_REAL_LITERAL(x) x
#	define PFS_REAL_MIN         PFS_DOUBLE_MIN
#	define PFS_REAL_MAX         PFS_DOUBLE_MAX
#	define PFS_REAL_MIN_10_EXP  PFS_DOUBLE_MIN_10_EXP
#	define PFS_REAL_MAX_10_EXP  PFS_DOUBLE_MAX_10_EXP
#endif

#ifdef HUGE_VALL
#	define PFS_HUGE_VAL HUGE_VALL
#else
#	define PFS_HUGE_VAL HUGE_VAL
#endif

#ifdef INFINITY
#	define PFS_INFINITY INFINITY
#else
#	ifdef PFS_CC_MSVC
#		define PFS_INFINITY (LDBL_MAX+LDBL_MAX)
#		define isinf(x) (!_finite(x))
#	elif (defined(__BORLANDC__) && __BORLANDC__ <= 0x410) || defined(__TURBOC__)
#		define PFS_INFINITY (DBL_MAX+DBL_MAX)
#	else
#		error "INFINITY is undefined"
#	endif
#endif

#ifdef NAN
#	define PFS_NAN NAN
#else
#	ifdef PFS_CC_MSVC
#		define PFS_NAN (PFS_INFINITY-PFS_INFINITY)
#		define isnan(x) _isnan(x)
#	elif (defined(__BORLANDC__) && __BORLANDC__ <= 0x410) || defined(__TURBOC__)
#		define PFS_NAN (PFS_INFINITY-PFS_INFINITY)
#	else
#		error "NAN is undefined"
#	endif
#endif

#define PFS_SIZE_MAX   PFS_UINT_MAX
#define PFS_SSIZE_MAX  PFS_INT_MAX

#ifndef NDEBUG
#   define PFS_CHECK_SIZEOF_TYPE(type,sz) PFS_ASSERT(sizeof(type) == (sz))

#	ifdef PFS_CC_MSVC
#		define PFS_CHECK_SIZEOF_WCHAR PFS_CHECK_SIZEOF_TYPE(wchar_t,2)
#	else
#		define PFS_CHECK_SIZEOF_WCHAR PFS_CHECK_SIZEOF_TYPE(wchar_t,4)
#	endif

#	ifdef PFS_HAVE_INT64
#		define PFS_CHECK_SIZEOF_LONG               \
			PFS_CHECK_SIZEOF_TYPE(integral_t, 8);  \
			PFS_CHECK_SIZEOF_TYPE(uintegral_t, 8)
#	else
#		define PFS_CHECK_SIZEOF_LONG               \
			PFS_CHECK_SIZEOF_TYPE(integral_t, 4);  \
			PFS_CHECK_SIZEOF_TYPE(uintegral_t, 4)
#	endif

#	if defined(PFS_HAVE_REAL128)
#		define PFS_CHECK_SIZEOF_REAL PFS_CHECK_SIZEOF_TYPE(real_t,16)
#	elif defined(PFS_HAVE_REAL80)
#		define PFS_CHECK_SIZEOF_REAL PFS_CHECK_SIZEOF_TYPE(real_t,12)
#	elif defined(PFS_HAVE_REAL64)
#		define PFS_CHECK_SIZEOF_REAL PFS_CHECK_SIZEOF_TYPE(real_t,8)
#	endif

#	define PFS_CHECK_SIZEOF_TYPES                 \
	PFS_CHECK_SIZEOF_TYPE(int8_t, 1);             \
	PFS_CHECK_SIZEOF_TYPE(uint8_t, 1);            \
	PFS_CHECK_SIZEOF_TYPE(int16_t, 2);            \
	PFS_CHECK_SIZEOF_TYPE(uint16_t, 2);           \
	PFS_CHECK_SIZEOF_TYPE(int32_t, 4);            \
	PFS_CHECK_SIZEOF_TYPE(uint32_t, 4);           \
	PFS_CHECK_SIZEOF_TYPE(float, 4);              \
	PFS_CHECK_SIZEOF_TYPE(double, 8);             \
	PFS_CHECK_SIZEOF_LONG;                        \
	PFS_CHECK_SIZEOF_WCHAR;                       \
	PFS_CHECK_SIZEOF_REAL;
#else
#	define PFS_CHECK_SIZEOF_TYPES
#endif


#if defined(__cplusplus) && ! defined(PFS_CC_BORLAND_REAL)
namespace pfs {

template <typename _number_type> _number_type max_type ();
template <typename _number_type> _number_type min_type ();

template<> inline char           max_type<char>           () { return static_cast<char>(PFS_CHAR_MAX); }
template<> inline signed char    max_type<signed char>    () { return static_cast<signed char>(PFS_SCHAR_MAX); }
template<> inline unsigned char  max_type<unsigned char>  () { return static_cast<unsigned char>(PFS_UCHAR_MAX); }
template<> inline short          max_type<short>          () { return static_cast<short>(PFS_SHORT_MAX); }
template<> inline unsigned short max_type<unsigned short> () { return static_cast<unsigned short>(PFS_USHORT_MAX); }
template<> inline int            max_type<int>            () { return static_cast<int>(PFS_INT_MAX); }
template<> inline unsigned int   max_type<unsigned int>   () { return static_cast<unsigned int>(PFS_UINT_MAX); }
template<> inline long           max_type<long>           () { return static_cast<long>(PFS_LONG_MAX); }
template<> inline unsigned long  max_type<unsigned long>  () { return static_cast<unsigned long>(PFS_ULONG_MAX); }

#ifdef PFS_HAVE_LONGLONG
template<> inline long long  max_type<long long>  () { return static_cast<long long>(PFS_LLONG_MAX); }
template<> inline unsigned long long max_type<unsigned long long> () { return static_cast<unsigned long long>(PFS_ULLONG_MAX); }
#endif

template<> inline float    max_type<float>    () { return static_cast<float>(PFS_FLOAT_MAX); }
template<> inline double   max_type<double>   () { return static_cast<double>(PFS_DOUBLE_MAX); }

#ifdef PFS_HAVE_LONG_DOUBLE
template<> inline long double   max_type<long double>   () { return static_cast<long double>(PFS_LONG_DOUBLE_MAX); }
#endif

template<> inline char           min_type<char>           () { return static_cast<char>(PFS_CHAR_MIN); }
template<> inline signed char    min_type<signed char>    () { return static_cast<signed char>(PFS_SCHAR_MIN); }
template<> inline unsigned char  min_type<unsigned char>  () { return static_cast<unsigned char>(PFS_UCHAR_MIN); }
template<> inline short          min_type<short>          () { return static_cast<short>(PFS_SHORT_MIN); }
template<> inline unsigned short min_type<unsigned short> () { return static_cast<unsigned short>(PFS_USHORT_MIN); }
template<> inline int            min_type<int>            () { return static_cast<int>(PFS_INT_MIN); }
template<> inline unsigned int   min_type<unsigned int>   () { return static_cast<unsigned int>(PFS_UINT_MIN); }
template<> inline long           min_type<long>           () { return static_cast<long>(PFS_LONG_MIN); }
template<> inline unsigned long  min_type<unsigned long>  () { return static_cast<unsigned long>(PFS_ULONG_MIN); }

#ifdef PFS_HAVE_LONGLONG
template<> inline long long  min_type<long long>  () { return static_cast<long long>(PFS_LLONG_MIN); }
template<> inline unsigned long long min_type<unsigned long long> () { return static_cast<unsigned long long>(PFS_ULLONG_MIN); }
#endif

template<> inline float    min_type<float>    () { return static_cast<float>(PFS_FLOAT_MIN); }
template<> inline double   min_type<double>   () { return static_cast<double>(PFS_DOUBLE_MIN); }

#ifdef PFS_HAVE_LONG_DOUBLE
template<> inline long double   min_type<long double>   () { return static_cast<long double>(PFS_LONG_DOUBLE_MIN); }
#endif


template <typename T1, typename T2>
inline T1 integral_cast_check (T2 x)
{
	PFS_ASSERT(x >= min_type<T1>() && x <= max_type<T1>());
	return static_cast<T1>(x);
}

} /* pfs */

#endif

#endif /* __PFS_PP_LIMITS_H__ */
