/*
 * string.c
 *
 *  Created on: 18.06.2012
 *      Author: wladt
 */


#include <cwt/str.h>
#include <cwt/logger.h>

#ifdef CWT_CC_GNUC
#	include <stdlib.h>
#endif
#include <string.h>
#include <stdarg.h>


#ifdef strdup
#	define __strdup_builtin strdup
#	undef strdup
#else
#	define __strdup_builtin CWT_ISO_CPP_NAME(strdup)
#endif

#ifdef strcmp
#	define __strcmp_builtin strcmp
#	undef strcmp
#else
#	define __strcmp_builtin strcmp
#endif

#ifdef strncmp
#	define __strncmp_builtin strncmp
#	undef strncmp
#else
#	define __strncmp_builtin strncmp
#endif


#ifdef strncpy
#	define __strncpy_builtin strncpy
#	undef strncpy
#else
#	define __strncpy_builtin strncpy
#endif


#ifdef toupper
#	define __toupper_builtin toupper
#	undef toupper
#else
#	define __toupper_builtin toupper
#endif

#ifdef tolower
#	define __tolower_builtin tolower
#	undef tolower
#else
#	define __tolower_builtin tolower
#endif


#ifndef CWT_UNICODE
static CWT_CHAR* __cwt_strdup(const CWT_CHAR *s) { return __strdup_builtin(s); }
static int       __cwt_strcmp(const CWT_CHAR *s1, const CWT_CHAR *s2) { return __strcmp_builtin(s1,s2); }
static int       __cwt_strncmp(const CWT_CHAR *s1, const CWT_CHAR *s2, size_t n) { return __strncmp_builtin(s1,s2,n); }
static CWT_CHAR* __cwt_strncpy(CWT_CHAR *dest, const CWT_CHAR *src, size_t n) { return __strncpy_builtin(dest,src,n); }
static CWT_CHAR* __cwt_strchr       (CWT_CHAR *s, CWT_CHAR ch);
static CWT_CHAR* __cwt_strstr       (const CWT_CHAR *s, const CWT_CHAR *substr);
#endif

const CWT_CHAR* __CONST_EMPTYSTR = _T("");
const CWT_CHAR* __CONST_NULLSTR  = _T("<null>");
CWT_CHAR*       __errorstr = NULL; /* TODO must be freed when program finishing */

static const CWT_CHAR* __cwt_strerror(int errn);
static BOOL      __cwt_streq        (const CWT_CHAR *s1, const CWT_CHAR *s2);
static BOOL      __cwt_strieq       (const CWT_CHAR *s1, const CWT_CHAR *s2);
static void*     __cwt_bzero        (void *block, size_t sz);
static CWT_CHAR* __cwt_strndup      (const CWT_CHAR *s, size_t n);
#ifndef CWT_UNICODE
static int       __cwt_isalpha      (CWT_CHAR);
#endif
static CWT_CHAR  __cwt_toupper      (CWT_CHAR ch);
static CWT_CHAR  __cwt_tolower      (CWT_CHAR ch);
static void      __cwt_toupperStr   (CWT_CHAR *dest, const CWT_CHAR *src, size_t n);
static void      __cwt_tolowerStr   (CWT_CHAR *dest, const CWT_CHAR *src, size_t n);
static CWT_CHAR* __cwt_strrstr(const CWT_CHAR *s, const CWT_CHAR *substr);
static void      __cwt_chomp(CWT_CHAR *s);

EXTERN_C_BEGIN
extern LONGLONG  __cwt_toLONGLONG   (const CWT_CHAR *str, int radix, BOOL *ok);
extern ULONGLONG __cwt_toULONGLONG  (const CWT_CHAR *str, int radix, BOOL *ok);
extern LONG      __cwt_toLONG       (const CWT_CHAR *str, int radix, BOOL *ok);
extern ULONG     __cwt_toULONG      (const CWT_CHAR *str, int radix, BOOL *ok);
extern INT       __cwt_toINT        (const CWT_CHAR *str, int radix, BOOL *ok);
extern UINT      __cwt_toUINT       (const CWT_CHAR *str, int radix, BOOL *ok);
extern SHORT     __cwt_toSHORT      (const CWT_CHAR *str, int radix, BOOL *ok);
extern USHORT    __cwt_toUSHORT     (const CWT_CHAR *str, int radix, BOOL *ok);
extern SBYTE     __cwt_toSBYTE      (const CWT_CHAR *str, int radix, BOOL *ok);
extern BYTE      __cwt_toBYTE       (const CWT_CHAR *str, int radix, BOOL *ok);
EXTERN_C_END

static const CWT_CHAR* __cwt_constEmptyStr(void);
static const CWT_CHAR* __cwt_constNullStr(void);


static CwtStrNS __cwtStrNS = {
	__cwt_strerror
#ifdef CWT_UNICODE
	, wcsftime
	, wcslen
	, wcscpy
	, wcsncpy
	, wcschr
	, wcsstr
	, __cwt_strrstr
	, __cwt_streq
	, __cwt_strieq
	, wcscmp
	, wcsncmp
	, CWT_ISO_CPP_NAME(wcsicmp)
	, CWT_ISO_CPP_NAME(wcsnicmp)
	, CWT_ISO_CPP_NAME(wcsdup)
	, __cwt_strndup
	, wcscat
	, wcsncat
	, wcstok
	, wcstol
	, wcstoul
#	ifdef CWT_CC_MSC
	, _wcstoi64
	, _wcstoui64
#	else
	, wcstoll
	, wcstoull
#	endif
	, iswalpha
#else /*CWT_UNICODE*/
	, strftime
	, strlen
	, strcpy
	, __cwt_strncpy
	, __cwt_strchr
	, __cwt_strstr
	, __cwt_strrstr
	, __cwt_streq
	, __cwt_strieq
	, __cwt_strcmp
	, __cwt_strncmp
#	ifdef CWT_CC_MSC
	, CWT_ISO_CPP_NAME(stricmp)
	, CWT_ISO_CPP_NAME(strnicmp)
#	else
	, strcasecmp
	, strncasecmp
#	endif
	, __cwt_strdup
	, __cwt_strndup
	, strcat
	, strncat
	, strtok
	, strtol
	, strtoul
#	ifdef CWT_CC_MSC
	, _strtoi64
	, _strtoui64
#	else
	, strtoll
	, strtoull
#	endif
	, __cwt_isalpha
#endif /*!CWT_UNICODE*/
	, memcpy
	, memmove
	, memset
	, __cwt_bzero
	, memcmp

	, __cwt_toLONGLONG
	, __cwt_toULONGLONG
	, __cwt_toLONG
	, __cwt_toULONG
	, __cwt_toINT
	, __cwt_toUINT
	, __cwt_toSHORT
	, __cwt_toUSHORT
	, __cwt_toSBYTE
	, __cwt_toBYTE

	, __cwt_toSBYTE
	, __cwt_toBYTE
	, __cwt_toSHORT
	, __cwt_toUSHORT
	, __cwt_toINT
	, __cwt_toUINT
	, __cwt_toLONGLONG
	, __cwt_toULONGLONG

	, __cwt_toupper
	, __cwt_tolower
	, __cwt_toupperStr
	, __cwt_tolowerStr
	, __cwt_chomp

	, __cwt_constEmptyStr
	, __cwt_constNullStr
};


DLL_API_EXPORT CwtStrNS* cwtStrNS(void)
{
	return &__cwtStrNS;
}


static const CWT_CHAR* __cwt_strerror(int errn)
{
	if( __errorstr ) {
		CWT_FREE(__errorstr);
		__errorstr = NULL;
	}

#ifdef CWT_UNICODE
#	ifdef CWT_CC_MSC
	{
    LPVOID lpMsgBuf;

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        (DWORD)errn,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    __errorstr = __cwtStrNS.strdup((CWT_CHAR*)lpMsgBuf);
    LocalFree(lpMsgBuf);
    __cwtStrNS.chomp(__errorstr);
	}
#	else
	__errorstr = __cwtStrNS.strdup(CWT_ISO_CPP_NAME(wcserror)(errn));
#	endif
#else
	__errorstr = __cwtStrNS.strdup(strerror(errn));
#endif

	return __errorstr;
}


static BOOL __cwt_streq(const CWT_CHAR *s1, const CWT_CHAR *s2)
{
	return __cwtStrNS.strcmp(s1, s2) == 0 ? TRUE : FALSE;
}

static BOOL __cwt_strieq(const CWT_CHAR *s1, const CWT_CHAR *s2)
{
	return __cwtStrNS.stricmp(s1, s2) == 0 ? TRUE : FALSE;
}

static void* __cwt_bzero(void *block, size_t sz)
{
	return __cwtStrNS.memset(block, 0, sz);
}


static CWT_CHAR* __cwt_strndup(const CWT_CHAR *s, size_t n)
{
	CWT_CHAR *s0 = NULL;

	s0 = CWT_MALLOCA(CWT_CHAR, n+1);
	__cwtStrNS.strncpy(s0, s, n);
	s0[n] = (CWT_CHAR)0;

	return s0;
}

#ifndef CWT_UNICODE
static int __cwt_isalpha(CWT_CHAR ch)
{
	return isalpha(ch);
}
#endif


static CWT_CHAR  __cwt_toupper(CWT_CHAR ch)
{
#ifdef CWT_UNICODE
	return (CWT_CHAR)towupper(ch);
#else
	return (CWT_CHAR)__toupper_builtin((int)ch);
#endif
}

static CWT_CHAR  __cwt_tolower(CWT_CHAR ch)
{
#ifdef CWT_UNICODE
	return (CWT_CHAR)towlower(ch);
#else
	return (CWT_CHAR)__tolower_builtin(ch);
#endif
}

#ifndef CWT_UNICODE
static CWT_CHAR* __cwt_strchr(CWT_CHAR *s, CWT_CHAR ch)
{
	return strchr(s, ch);
}

static CWT_CHAR* __cwt_strstr (const CWT_CHAR *s, const CWT_CHAR *substr)
{
	return strstr(s, substr);
}
#endif

/**
 * @brief Finds the last occurrence of a substring in another string
 *
 * @c rstrstr scan @c s1 for the last occurrence of a substring @c s2
 *
 * @param s1
 * @param s2
 * @return pointer to the last element in s1 where s2 begins
 * 		   or @c null if @c s2 does not occur in @c s1
 */
static CWT_CHAR* __cwt_strrstr(const CWT_CHAR *s1, const CWT_CHAR *s2)
{
	CWT_CHAR *ptr;
	size_t s2_len;

	if( !s1 )
		return NULL;

	if( !s2 )
		return NULL;

	s2_len = __cwtStrNS.strlen(s2);
	ptr = (CWT_CHAR*)(s1 + __cwtStrNS.strlen(s1));

	while( ptr >= s1 ) {
		if( __cwtStrNS.strncmp(ptr, s2, s2_len) == 0 )
			return ptr;
		ptr--;
	}

	return NULL;
}


static void __cwt_toupperStr(CWT_CHAR *dest, const CWT_CHAR *src, size_t n)
{
	size_t i;

	for( i = 0; i < n; i++ ) {
		dest[i] = __cwtStrNS.toupper(src[i]);
	}
}

static void __cwt_tolowerStr(CWT_CHAR *dest, const CWT_CHAR *src, size_t n)
{
	size_t i;

	for( i = 0; i < n; i++ ) {
		dest[i] = __cwtStrNS.tolower(src[i]);
	}
}


static void __cwt_chomp(CWT_CHAR *s)
{
	CWT_CHAR *ptr;

	ptr = s + __cwtStrNS.strlen(s) - 1;
	while( ptr >= s && *ptr == _T('\n') ) {
		*ptr = (CWT_CHAR)0;
		ptr--;
	}
}

/**
 * Return character from @c s at position @c i.
 *
 * @param s string.
 * @param i character position.
 * @return (CHAR)0 if @c i is out of string bounds or character from string @c at position @c i.
 */
/*
static CWT_CHAR __charAt(const CWT_CHAR *s, size_t i)
{
	if( i >= cwtStrLen(s) )
		return (CWT_CHAR)0;
	return s[i];
}
*/

static const CWT_CHAR* __cwt_constEmptyStr(void)
{
	return __CONST_EMPTYSTR;
}

static const CWT_CHAR* __cwt_constNullStr(void)
{
	return __CONST_NULLSTR;
}
