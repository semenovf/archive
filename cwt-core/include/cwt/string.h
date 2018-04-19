/*
 * string.h
 *
 *  Created on: 12 июля 2013 г.
 *      Author: wladt
 */

#ifndef __CWT_STRING_H__
#define __CWT_STRING_H__

#include <cwt/cwt.h>
#include <string.h>

#ifdef CWT_CC_MSVC
#	define strcasecmp _stricmp
#endif

EXTERN_C_BEGIN

DLL_API char *   cwt_strerror      (int errn, char *buf, size_t buflen);
DLL_API double	 cwt_str_to_double (const char * s, bool *ok);
DLL_API float	 cwt_str_to_float  (const char * s, bool *ok);
DLL_API long_t   cwt_str_to_long   (const char * s, bool *ok, int base);
DLL_API ulong_t	 cwt_str_to_ulong  (const char * s, bool *ok, int base);
DLL_API int_t	 cwt_str_to_int    (const char * s, bool *ok, int base);
DLL_API uint_t	 cwt_str_to_uint   (const char * s, bool *ok, int base);
DLL_API short_t	 cwt_str_to_short  (const char * s, bool *ok, int base);
DLL_API ushort_t cwt_str_to_ushort (const char * s, bool *ok, int base);
DLL_API sbyte_t  cwt_str_to_sbyte  (const char * s, bool *ok, int base);
DLL_API byte_t	 cwt_str_to_byte   (const char * s, bool *ok, int base);

EXTERN_C_END


#endif /* __CWT_STRING_H__ */
