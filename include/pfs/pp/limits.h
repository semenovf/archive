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

#if defined(HAVE_INT64) && ! defined(PFS_OS_64BITS)
#	define PFS_LONG_LITERAL(x) x##LL
#	define PFS_ULONG_LITERAL(x) x##ULL
#else
#	define PFS_LONG_LITERAL(x) x##L
#	define PFS_ULONG_LITERAL(x) x##UL
#endif

#define PFS_INT8_MAX   127
#define PFS_INT8_MIN   (-127-1)
#define PFS_UINT8_MAX  255
#define PFS_INT16_MAX  32767
#define	PFS_INT16_MIN  (-32768)
#define	PFS_UINT16_MAX 65535
#define	PFS_INT24_MAX  8388607
#define	PFS_INT24_MIN  (-8388608)
#define	PFS_UINT24_MAX 16777215
#define PFS_INT32_MAX  2147483647
#define	PFS_INT32_MIN  (-2147483647-1)
#define	PFS_UINT32_MAX 4294967295UL
#ifdef PFS_HAVE_INT64
#	define PFS_INT64_MAX  PFS_LONG_LITERAL(9223372036854775807)
#	define PFS_INT64_MIN (-PFS_INT64_MAX-PFS_LONG_LITERAL(1))
#	define PFS_UINT64_MAX PFS_ULONG_LITERAL(18446744073709551615)
#endif

#define PFS_BYTE_MAX   PFS_UINT8_MAX
#define PFS_SBYTE_MAX  PFS_INT8_MAX
#define PFS_SBYTE_MIN  PFS_INT8_MIN
#define PFS_CHAR_MAX   PFS_INT8_MAX
#define PFS_CHAR_MIN   PFS_INT8_MIN
#define PFS_USHORT_MAX PFS_UINT16_MAX
#define PFS_SHORT_MAX  PFS_INT16_MAX
#define PFS_SHORT_MIN  PFS_INT16_MIN
#define PFS_UINT_MAX   PFS_UINT32_MAX
#define PFS_INT_MAX    PFS_INT32_MAX
#define PFS_INT_MIN    PFS_INT32_MIN

#ifdef PFS_HAVE_INT64
#	define PFS_ULONG_MAX  PFS_UINT64_MAX
#	define PFS_LONG_MAX   PFS_INT64_MAX
#	define PFS_LONG_MIN   PFS_INT64_MIN
#else
#	define PFS_ULONG_MAX  PFS_UINT32_MAX
#	define PFS_LONG_MAX   PFS_INT32_MAX
#	define PFS_LONG_MIN   PFS_INT32_MIN
#endif

#define PFS_FLOAT_MIN  FLT_MIN /* 1.175494351e-38F */
#define PFS_FLOAT_MAX  FLT_MAX /* 3.402823466e+38F */

#ifdef DBL_MIN
#	define PFS_DOUBLE_MIN DBL_MIN /* 2.22507385850720138309e-308 */
#	define PFS_DOUBLE_MAX DBL_MAX /* 1.79769313486231570815e+308 */
#	define PFS_DOUBLE_MIN_10_EXP DBL_MIN_10_EXP /* -307 */
#	define PFS_DOUBLE_MAX_10_EXP DBL_MAX_10_EXP /* 308 */
#else
#	error DBL_MIN is not defined
#endif

#ifdef LDBL_MIN
#	if defined(PFS_CC_MSC_VERSION) && PFS_CC_MSC_VERSION <= 1600 /* <= VS 2010 */
	/* TODO Need to check in newer versions */
	/* LDBL_MIN is equal to DBL_MIN
	 * no differences between double and long double
	 * */
#	else
#		define PFS_HAVE_LONG_DOUBLE 1
#		define PFS_LONG_DOUBLE_MIN LDBL_MIN /* 3.36210314311209350626e-4932L */
#		define PFS_LONG_DOUBLE_MAX LDBL_MAX /* 1.18973149535723176502e+4932L */
#		define PFS_LONG_DOUBLE_MIN_10_EXP LDBL_MIN_10_EXP /* -4931 */
#		define PFS_LONG_DOUBLE_MAX_10_EXP LDBL_MAX_10_EXP /* 4932 */
#	endif
#else
#	error LDBL_MIN is not defined
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

#ifdef PFS_HAVE_LONG_DOUBLE
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
#		error INFINITY is undefined
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
#		error NAN is undefined
#	endif
#endif


/*
#define PFS_INFINITY (PFS_DOUBLE_MAX+PFS_DOUBLE_MAX)
#define PFS_NAN (PFS_INFINITY-PFS_INFINITY)
*/
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
#		define PFS_CHECK_SIZEOF_LONG              \
			PFS_CHECK_SIZEOF_TYPE(int64_t, 8);    \
			PFS_CHECK_SIZEOF_TYPE(uint64_t, 8);   \
			PFS_CHECK_SIZEOF_TYPE(long_t, 8);     \
			PFS_CHECK_SIZEOF_TYPE(ulong_t, 8);
#	else
#		define PFS_CHECK_SIZEOF_LONG              \
			PFS_CHECK_SIZEOF_TYPE(long_t, 4);     \
			PFS_CHECK_SIZEOF_TYPE(ulong_t, 4);
#	endif

#	define PFS_CHECK_SIZEOF_TYPES                 \
	PFS_CHECK_SIZEOF_TYPE(int8_t, 1);             \
	PFS_CHECK_SIZEOF_TYPE(uint8_t, 1);            \
	PFS_CHECK_SIZEOF_TYPE(int16_t, 2);            \
	PFS_CHECK_SIZEOF_TYPE(uint16_t, 2);           \
	PFS_CHECK_SIZEOF_TYPE(int32_t, 4);            \
	PFS_CHECK_SIZEOF_TYPE(uint32_t, 4);           \
	PFS_CHECK_SIZEOF_TYPE(sbyte_t, 1);            \
	PFS_CHECK_SIZEOF_TYPE(byte_t, 1);             \
	PFS_CHECK_SIZEOF_TYPE(short_t, 2);            \
	PFS_CHECK_SIZEOF_TYPE(ushort_t, 2);           \
	PFS_CHECK_SIZEOF_TYPE(int_t, 4);              \
	PFS_CHECK_SIZEOF_TYPE(uint_t, 4);             \
	PFS_CHECK_SIZEOF_LONG                         \
	PFS_CHECK_SIZEOF_WCHAR
#else
#	define PFS_CHECK_SIZEOF_TYPES
#endif


#if defined(__cplusplus) && ! defined(PFS_CC_BORLAND_REAL)
namespace pfs {

template <typename _number_type> _number_type max_type ();
template <typename _number_type> _number_type min_type ();

template<> inline int8_t   max_type<int8_t>   () { return int8_t(PFS_INT8_MAX); }
template<> inline uint8_t  max_type<uint8_t>  () { return uint8_t(PFS_UINT8_MAX); }
template<> inline int16_t  max_type<int16_t>  () { return int16_t(PFS_INT16_MAX); }
template<> inline uint16_t max_type<uint16_t> () { return uint16_t(PFS_UINT16_MAX); }
template<> inline int32_t  max_type<int32_t>  () { return int32_t(PFS_INT32_MAX); }
template<> inline uint32_t max_type<uint32_t> () { return uint32_t(PFS_UINT32_MAX); }
#ifdef PFS_HAVE_INT64
template<> inline int64_t  max_type<int64_t>  () { return int64_t(PFS_INT64_MAX); }
template<> inline uint64_t max_type<uint64_t> () { return uint64_t(PFS_UINT64_MAX); }
#endif
template<> inline float    max_type<float>    () { return float(PFS_FLOAT_MAX); }
template<> inline double   max_type<double>   () { return double(PFS_DOUBLE_MAX); }

#ifdef PFS_HAVE_LONG_DOUBLE
template<> inline long double   max_type<long double>   () { return PFS_LONG_DOUBLE_MAX; }
#endif

template<> inline int8_t   min_type<int8_t>   () { return int8_t(PFS_INT8_MIN); }
template<> inline uint8_t  min_type<uint8_t>  () { return uint8_t(0); }
template<> inline int16_t  min_type<int16_t>  () { return int16_t(PFS_INT16_MIN); }
template<> inline uint16_t min_type<uint16_t> () { return uint16_t(0); }
template<> inline int32_t  min_type<int32_t>  () { return int32_t(PFS_INT32_MIN); }
template<> inline uint32_t min_type<uint32_t> () { return uint32_t(0); }
#ifdef PFS_HAVE_INT64
template<> inline int64_t  min_type<int64_t>  () { return int64_t(PFS_INT64_MIN); }
template<> inline uint64_t min_type<uint64_t> () { return uint64_t(0); }
#endif
template<> inline float    min_type<float>    () { return float(PFS_FLOAT_MIN); }
template<> inline double   min_type<double>   () { return double(PFS_DOUBLE_MIN); }

#ifdef PFS_HAVE_LONG_DOUBLE
template<> inline long double   min_type<long double>   () { return PFS_LONG_DOUBLE_MIN; }
#endif

} /* namespace pfs */

#endif

#endif /* __PFS_PP_LIMITS_H__ */
