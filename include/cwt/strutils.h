/*
 * strutils.h
 *
 *  Created on: 10.01.2012
 *      Author: wladt
 */

#ifndef __CWT_STRUTILS_H__
#define __CWT_STRUTILS_H__

#include <cwt/global.h>
#include <cwt/types.h>
#include <time.h>


EXTERN_C_BEGIN

/*TODO remove this block */
#ifdef __COMMENT__
DLL_API_EXPORT LONG      cwtStrToLONG_DEBUG(const CHAR *str, int radix, BOOL *ok);
DLL_API_EXPORT LONGLONG  cwtStrToLONGLONG_DEBUG(const CHAR *str, int radix, BOOL *ok);
#endif

DLL_API_EXPORT LONGLONG  cwtStrToLONGLONG(const CHAR *str, int radix, BOOL *ok);
DLL_API_EXPORT ULONGLONG cwtStrToULONGLONG(const CHAR *str, int radix, BOOL *ok);
DLL_API_EXPORT LONG      cwtStrToLONG(const CHAR *str, int radix, BOOL *ok);
DLL_API_EXPORT ULONG     cwtStrToULONG(const CHAR *str, int radix, BOOL *ok);
DLL_API_EXPORT INT       cwtStrToINT(const CHAR *str, int radix, BOOL *ok);
DLL_API_EXPORT UINT      cwtStrToUINT(const CHAR *str, int radix, BOOL *ok);
DLL_API_EXPORT SHORT     cwtStrToSHORT(const CHAR *str, int radix, BOOL *ok);
DLL_API_EXPORT USHORT    cwtStrToUSHORT(const CHAR *str, int radix, BOOL *ok);
DLL_API_EXPORT SBYTE     cwtStrToSBYTE(const CHAR *str, int radix, BOOL *ok);
DLL_API_EXPORT BYTE      cwtStrToBYTE(const CHAR *str, int radix, BOOL *ok);

#define cwtStrToINT8(str,radix,ok)   cwtStrToSBYTE(str,radix,ok)
#define cwtStrToUINT8(str,radix,ok)  cwtStrToBYTE(str,radix,ok)
#define cwtStrToINT16(str,radix,ok)  cwtStrToSHORT(str,radix,ok)
#define cwtStrToUINT16(str,radix,ok) cwtStrToUINT(str,radix,ok)
#define cwtStrToINT32(str,radix,ok)  cwtStrToINT(str,radix,ok)
#define cwtStrToUINT32(str,radix,ok) cwtStrToUINT(str,radix,ok)
#define cwtStrToINT64(str,radix,ok)  cwtStrToLONGLONG(str,radix,ok)
#define cwtStrToUINT64(str,radix,ok) cwtStrToULONGLONG(str,radix,ok)

DLL_API_EXPORT CHAR* rstrstr(const CHAR *s1, const CHAR *s2);
DLL_API_EXPORT void chomp( CHAR *s );
DLL_API_EXPORT CHAR str_char_at(const CHAR *s, size_t i);
DLL_API_EXPORT void str_toupper(CHAR *dest, const CHAR *src, size_t n);
DLL_API_EXPORT void str_tolower(CHAR *dest, const CHAR *src, size_t n);

#define cwtRStrstr    rstrstr
#define cwtChomp      chomp
#define cwtStrCharAt  str_char_at
#define cwtStrToUpper str_toupper
#define cwtStrToLower str_tolower

EXTERN_C_END

#endif /* #define __CWT_STRUTILS_H__ */
