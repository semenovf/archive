/**
 * @file   limits.h
 * @author wladt
 * @date   Jan 23, 2013 4:19:14 PM
 *
 * @brief
 */

#ifndef __PFS_LIMITS_H__
#define __PFS_LIMITS_H__

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
#define PFS_INT64_MIN  (-9223372036854775807LL-1LL)
#define PFS_INT64_MAX  9223372036854775807LL
#define PFS_UINT64_MAX 18446744073709551615ULL

#define PFS_BYTE_MAX   PFS_UINT8_MAX
#define PFS_SBYTE_MAX  PFS_INT8_MAX
#define PFS_SBYTE_MIN  PFS_INT8_MIN
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



#include <float.h>
#define PFS_FLOAT_MIN  FLT_MIN /* 1.175494351e-38F */
#define PFS_FLOAT_MAX  FLT_MAX /* 3.402823466e+38F */
#define PFS_DOUBLE_MIN DBL_MIN /* 2.2250738585072014e-308 */
#define PFS_DOUBLE_MAX DBL_MAX /* 1.7976931348623158e+308 */

#define PFS_SIZE_MAX   PFS_UINT_MAX
#define PFS_SSIZE_MAX  PFS_INT_MAX

#ifndef NDEBUG
#   define PFS_CHECK_SIZEOF_TYPE(type,sz) PFS_ASSERT(sizeof(type) == (sz))
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
	PFS_CHECK_SIZEOF_TYPE(ulong_t, 8);
#else
#	define PFS_CHECK_SIZEOF_TYPES
#endif

#endif /* __PFS_LIMITS_H__ */