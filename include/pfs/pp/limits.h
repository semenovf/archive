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
#ifdef HAVE_INT64
#	ifdef PFS_OS_64BITS
#		define PFS_INT64_MAX  9223372036854775807L
#		define PFS_INT64_MIN (-PFS_INT64_MAX-1L)
#		define PFS_UINT64_MAX 18446744073709551615UL
#	else /* !PFS_OS_64BITS */
#		define PFS_INT64_MAX  9223372036854775807LL
#		define PFS_INT64_MIN (-PFS_INT64_MAX-1LL)
#		define PFS_UINT64_MAX 18446744073709551615ULL
#	endif /* PFS_OS_64BITS */
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

#ifdef HAVE_INT64
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
#ifdef HAVE_INT64
#	define PFS_DOUBLE_MIN LDBL_MIN /* 3.36210314311209350626e-4932L */
#	define PFS_DOUBLE_MAX LDBL_MAX /* 1.18973149535723176502e+4932L */
#	define PFS_DOUBLE_MIN_10_EXP LDBL_MIN_10_EXP /* -4931 */
#	define PFS_DOUBLE_MAX_10_EXP LDBL_MAX_10_EXP /* 4932 */
#else
#	define PFS_DOUBLE_MIN DBL_MIN /* 2.22507385850720138309e-308 */
#	define PFS_DOUBLE_MAX DBL_MAX /* 1.79769313486231570815e+308) */
#	define PFS_DOUBLE_MIN_10_EXP DBL_MIN_10_EXP /* -307 */
#	define PFS_DOUBLE_MAX_10_EXP DBL_MAX_10_EXP /* 308 */
#endif

#ifdef HAVE_INT64
#	define PFS_DOUBLE_LITERAL(x) x##L
#else
#	define PFS_DOUBLE_LITERAL(x) x
#endif


#ifdef INFINITY
#	define PFS_INFINITY INFINITY
#else
#	error "INFINITY is undefined"
#endif

#ifdef NAN
#	define PFS_NAN NAN
#else
#	error "NAN is undefined"
#endif

/*
#define PFS_INFINITY (PFS_DOUBLE_MAX+PFS_DOUBLE_MAX)
#define PFS_NAN (PFS_INFINITY-PFS_INFINITY)

#define PFS_SIZE_MAX   PFS_UINT_MAX
#define PFS_SSIZE_MAX  PFS_INT_MAX
*/

#ifndef NDEBUG
#   define PFS_CHECK_SIZEOF_TYPE(type,sz) PFS_ASSERT(sizeof(type) == (sz))

#	ifdef PFS_CC_MSVC
#		define PFS_CHECK_SIZEOF_WCHAR PFS_CHECK_SIZEOF_TYPE(wchar_t,2)
#	else
#		define PFS_CHECK_SIZEOF_WCHAR
#	endif

#	define PFS_CHECK_SIZEOF_TYPES                       \
	PFS_CHECK_SIZEOF_TYPE(int8_t, 1);                   \
	PFS_CHECK_SIZEOF_TYPE(uint8_t, 1);                  \
	PFS_CHECK_SIZEOF_TYPE(int16_t, 2);                  \
	PFS_CHECK_SIZEOF_TYPE(uint16_t, 2);                 \
	PFS_CHECK_SIZEOF_TYPE(int32_t, 4);                  \
	PFS_CHECK_SIZEOF_TYPE(uint32_t, 4);                 \
	PFS_CHECK_SIZEOF_TYPE(int64_t, 8);                  \
	PFS_CHECK_SIZEOF_TYPE(uint64_t, 8);                 \
	PFS_CHECK_SIZEOF_TYPE(sbyte_t, 1);                  \
	PFS_CHECK_SIZEOF_TYPE(byte_t, 1);                   \
	PFS_CHECK_SIZEOF_TYPE(short_t, 2);                  \
	PFS_CHECK_SIZEOF_TYPE(ushort_t, 2);                 \
	PFS_CHECK_SIZEOF_TYPE(int_t, 4);                    \
	PFS_CHECK_SIZEOF_TYPE(uint_t, 4);                   \
	PFS_CHECK_SIZEOF_TYPE(long_t, 8);                   \
	PFS_CHECK_SIZEOF_TYPE(ulong_t, 8);                  \
	PFS_CHECK_SIZEOF_WCHAR
#else
#	define PFS_CHECK_SIZEOF_TYPES
#endif

#ifdef __cplusplus
namespace pfs {

template <typename _number_type> _number_type max_type ();
template <typename _number_type> _number_type min_type ();

template<> inline int8_t   max_type<int8_t>   () { return int8_t(PFS_INT8_MAX); }
template<> inline uint8_t  max_type<uint8_t>  () { return uint8_t(PFS_UINT8_MAX); }
template<> inline int16_t  max_type<int16_t>  () { return int16_t(PFS_INT16_MAX); }
template<> inline uint16_t max_type<uint16_t> () { return uint16_t(PFS_UINT16_MAX); }
template<> inline int32_t  max_type<int32_t>  () { return int32_t(PFS_INT32_MAX); }
template<> inline uint32_t max_type<uint32_t> () { return uint32_t(PFS_UINT32_MAX); }
template<> inline int64_t  max_type<int64_t>  () { return int64_t(PFS_INT64_MAX); }
template<> inline uint64_t max_type<uint64_t> () { return uint64_t(PFS_UINT64_MAX); }
template<> inline float    max_type<float>    () { return float(PFS_FLOAT_MAX); }
template<> inline double   max_type<double>   () { return double(PFS_DOUBLE_MAX); }

template<> inline int8_t   min_type<int8_t>   () { return int8_t(PFS_INT8_MIN); }
template<> inline uint8_t  min_type<uint8_t>  () { return uint8_t(0); }
template<> inline int16_t  min_type<int16_t>  () { return int16_t(PFS_INT16_MIN); }
template<> inline uint16_t min_type<uint16_t> () { return uint16_t(0); }
template<> inline int32_t  min_type<int32_t>  () { return int32_t(PFS_INT32_MIN); }
template<> inline uint32_t min_type<uint32_t> () { return uint32_t(0); }
template<> inline int64_t  min_type<int64_t>  () { return int64_t(PFS_INT64_MIN); }
template<> inline uint64_t min_type<uint64_t> () { return uint64_t(0); }
template<> inline float    min_type<float>    () { return float(PFS_FLOAT_MIN); }
template<> inline double   min_type<double>   () { return double(PFS_DOUBLE_MIN); }

}

#endif

#endif /* __PFS_PP_LIMITS_H__ */
