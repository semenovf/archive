/*
 * string.c
 *
 *  Created on: 18.06.2012
 *      Author: wladt
 */


#include <cwt/str.h>

const CWT_CHAR* CWT_CONST_EMPTYSTR = _T("");
const CWT_CHAR* CWT_CONST_NULLSTR  = _T("<null>");

static BOOL      __streq(const CWT_CHAR *s1, const CWT_CHAR *s2);
static BOOL      __strieq(const CWT_CHAR *s1, const CWT_CHAR *s2);
static void*     __bzero(void *block, size_t sz);
static CWT_CHAR* __strndup(const CWT_CHAR *s, size_t n);
static CWT_CHAR  __toupper(CWT_CHAR ch);
static CWT_CHAR  __tolower(CWT_CHAR ch);
static void      __toupperStr   (CWT_CHAR *dest, const CWT_CHAR *src, size_t n);
static void      __tolowerStr   (CWT_CHAR *dest, const CWT_CHAR *src, size_t n);
static CWT_CHAR* __strrstr(const CWT_CHAR *s, const CWT_CHAR *substr);
static void      __chomp(CWT_CHAR *s);
/*static CWT_CHAR  __charAt(const CWT_CHAR *s, size_t i);*/

EXTERN_C_BEGIN
extern LONGLONG   __toLONGLONG   (const CWT_CHAR *str, int radix, BOOL *ok);
extern ULONGLONG  __toULONGLONG  (const CWT_CHAR *str, int radix, BOOL *ok);
extern LONG       __toLONG       (const CWT_CHAR *str, int radix, BOOL *ok);
extern ULONG      __toULONG      (const CWT_CHAR *str, int radix, BOOL *ok);
extern INT        __toINT        (const CWT_CHAR *str, int radix, BOOL *ok);
extern UINT       __toUINT       (const CWT_CHAR *str, int radix, BOOL *ok);
extern SHORT      __toSHORT      (const CWT_CHAR *str, int radix, BOOL *ok);
extern USHORT     __toUSHORT     (const CWT_CHAR *str, int radix, BOOL *ok);
extern SBYTE      __toSBYTE      (const CWT_CHAR *str, int radix, BOOL *ok);
extern BYTE       __toBYTE       (const CWT_CHAR *str, int radix, BOOL *ok);
EXTERN_C_END

static const char* __toLatin1(const CWT_CHAR *s, CwtByteArray *latins1);

static CwtStrNS __cwtStrNS = {
#ifdef CWT_UNICODE
	  wcsftime
	, wcslen
	, wcscpy
	, wcsncpy
	, wcschr
	, wcsstr
	, __strrstr
	, __streq
	, __strieq
	, wcscmp
	, wcsncmp
	, CWT_ISO_CPP_NAME(wcsicmp)
	, CWT_ISO_CPP_NAME(wcsnicmp)
	, CWT_ISO_CPP_NAME(wcsdup)
	, __strndup
	, wcscat
	, wcsncat
	, wcstok
	, CWT_ISO_CPP_NAME(wcserror)
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
#else
	  strftime
	, strlen
	, strcpy
	, strncpy
	, strchr
	, strstr
	, __strrstr
	, __streq
	, __strieq
	, strcmp
	, strncmp
	, CWT_ISO_CPP_NAME(stricmp)
	, CWT_ISO_CPP_NAME(strnicmp)
	, CWT_ISO_CPP_NAME(strdup)
	, __strndup
	, strcat
	, strncat
	, strtok
	, strerror
	, strtol
	, strtoul
#	ifdef CWT_CC_MSC
	, _strtoi64
	, _strtoui64
#	else
	, strtoull
	, strtoll
#	endif
	, isalpha
#endif
	, memcpy
	, memmove
	, memset
	, __bzero

	, __toLONGLONG
	, __toULONGLONG
	, __toLONG
	, __toULONG
	, __toINT
	, __toUINT
	, __toSHORT
	, __toUSHORT
	, __toSBYTE
	, __toBYTE

	, __toSBYTE
	, __toBYTE
	, __toSHORT
	, __toUSHORT
	, __toINT
	, __toUINT
	, __toLONGLONG
	, __toULONGLONG

	, __toupper
	, __tolower
	, __toupperStr
	, __tolowerStr
	, __chomp
	, __toLatin1

};


DLL_API_EXPORT CwtStrNS* cwtStrNS(void)
{
	return &__cwtStrNS;
}


static BOOL __streq(const CWT_CHAR *s1, const CWT_CHAR *s2)
{
	return __cwtStrNS.strcmp(s1, s2) == 0 ? TRUE : FALSE;
}

static BOOL __strieq(const CWT_CHAR *s1, const CWT_CHAR *s2)
{
	return __cwtStrNS.stricmp(s1, s2) == 0 ? TRUE : FALSE;
}

static void* __bzero(void *block, size_t sz)
{
	return __cwtStrNS.memset(block, 0, sz);
}


static CWT_CHAR* __strndup(const CWT_CHAR *s, size_t n)
{
	CWT_CHAR *s0 = NULL;

	s0 = CWT_MALLOCA(CWT_CHAR, n+1);
	__cwtStrNS.strncpy(s0, s, n);
	s0[n] = (CWT_CHAR)0;

	return s0;
}

static CWT_CHAR  __toupper(CWT_CHAR ch)
{
#ifdef CWT_UNICODE
	return (CWT_CHAR)towupper(ch);
#else
	return (CWT_CHAR)toupper((int)ch);
#endif
}

static CWT_CHAR  __tolower(CWT_CHAR ch)
{
#ifdef CWT_UNICODE
	return (CWT_CHAR)towlower(ch);
#else
	return (CWT_CHAR)tolower(ch);
#endif
}


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
static CWT_CHAR* __strrstr(const CWT_CHAR *s1, const CWT_CHAR *s2)
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


static void __toupperStr(CWT_CHAR *dest, const CWT_CHAR *src, size_t n)
{
	size_t i;

	for( i = 0; i < n; i++ ) {
		dest[i] = __toupper(src[i]);
	}
}

static void __tolowerStr(CWT_CHAR *dest, const CWT_CHAR *src, size_t n)
{
	size_t i;

	for( i = 0; i < n; i++ ) {
		dest[i] = __tolower(src[i]);
	}
}


static void __chomp(CWT_CHAR *s)
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

/**
  @return converted string, or empty string if error occured or source string was empty
*/
static const char* __toLatin1( const CWT_CHAR *s, CwtByteArray *latins1 )
{
	CwtStrNS *strNS = cwtStrNS();
	CwtByteArrayNS *baNS = cwtByteArrayNS();
	size_t length = strNS->strlen(s);

	baNS->clear(latins1);

	if( length ) {
		const CWT_CHAR *src = s;
		BYTE *dst = latins1->m_buffer;

		baNS->resize(latins1, length);

        while (length--) {
            *dst++ = (*src > 0xff) ? '?' : (BYTE)*src;
            ++src;
        }
	}
	return baNS->cstr(latins1);
}
