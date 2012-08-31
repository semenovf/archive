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

/**
 * @fn CwtStrNS::strtod(const CWT_CHAR *s, CWT_CHAR **endptr)
 *
 * @note The radix is defined by the program's locale, category LC_NUMERIC.
 * In a locale where the radix is not defined or in the POSIX locale,
 * the radix defaults to a period (.).
 */

const CWT_CHAR* __CONST_EMPTYSTR = _T("");
const CWT_CHAR* __CONST_NULLSTR  = _T("<null>");
CWT_CHAR*       __errorstr = NULL; /* TODO must be freed when program finishing */

#ifdef CWT_UNICODE

static size_t    __cwt_strftime(CWT_CHAR *buf, size_t sz, const CWT_CHAR *format, const struct tm* ptm) { return wcsftime(buf, sz, format, ptm); }
static size_t    __cwt_strlen  (const CWT_CHAR *s) { return wcslen(s); }
static CWT_CHAR* __cwt_strcpy  (CWT_CHAR *dest, const CWT_CHAR *src) { return wcscpy(dest, src); }
static CWT_CHAR* __cwt_strncpy (CWT_CHAR *dest, const CWT_CHAR *src, size_t n) { return wcsncpy(dest,src,n); }
static CWT_CHAR* __cwt_strchr  (CWT_CHAR *s, CWT_CHAR ch) { return wcschr(s, ch); }
static CWT_CHAR* __cwt_strstr  (const CWT_CHAR *s, const CWT_CHAR *substr) { return wcsstr(s, substr); }
static int       __cwt_strcmp  (const CWT_CHAR *s1, const CWT_CHAR *s2) { return wcscmp(s1, s2); }
static int       __cwt_strncmp (const CWT_CHAR *s1, const CWT_CHAR *s2, size_t n) { return wcsncmp(s1, s2, n); }
static CWT_CHAR* __cwt_strdup  (const CWT_CHAR *s) { return CWT_ISO_CPP_NAME(wcsdup)(s); }
static CWT_CHAR* __cwt_strcat  (CWT_CHAR *dest, const CWT_CHAR *src) { return wcscat(dest, src); }
static CWT_CHAR* __cwt_strncat (CWT_CHAR *dest, const CWT_CHAR *src, size_t n) { return wcsncat(dest, src, n); }
static CWT_CHAR* __cwt_strtok  (CWT_CHAR *tok, const CWT_CHAR *delim) { return wcstok(tok, delim); }
static LONG      __cwt_strtol  (const CWT_CHAR *s, CWT_CHAR **endptr, int radix) { return wcstol(s, endptr, radix); }
static ULONG     __cwt_strtoul (const CWT_CHAR *s, CWT_CHAR **endptr, int radix) { return wcstoul(s, endptr, radix); }
static double    __cwt_strtod  (const CWT_CHAR *s, CWT_CHAR **endptr) { return wcstod(s, endptr); }
static int       __cwt_isalpha (CWT_CHAR ch) { return iswalpha(ch); }
static int       __cwt_isdigit (CWT_CHAR ch) { return iswdigit(ch); }
static int       __cwt_isspace (CWT_CHAR ch) { return iswspace(ch); }
static CWT_CHAR  __cwt_toupper(CWT_CHAR ch) { return (CWT_CHAR)towupper(ch); }
static CWT_CHAR  __cwt_tolower(CWT_CHAR ch) { return (CWT_CHAR)towlower(ch); }

#else

static size_t    __cwt_strftime(CWT_CHAR *buf, size_t sz, const CWT_CHAR *format, const struct tm* ptm) { return strftime(buf, sz, format, ptm); }
static size_t    __cwt_strlen  (const CWT_CHAR *s) { return strlen(s); }
static CWT_CHAR* __cwt_strcpy  (CWT_CHAR *dest, const CWT_CHAR *src) { return strcpy(dest, src); }
static CWT_CHAR* __cwt_strncpy(CWT_CHAR *dest, const CWT_CHAR *src, size_t n) { return strncpy(dest,src,n); }
static CWT_CHAR* __cwt_strchr  (CWT_CHAR *s, CWT_CHAR ch) { return strchr(s, ch); }
static CWT_CHAR* __cwt_strstr  (const CWT_CHAR *s, const CWT_CHAR *substr) { return strstr(s, substr); }
static int       __cwt_strcmp  (const CWT_CHAR *s1, const CWT_CHAR *s2) { return strcmp(s1,s2); }
static int       __cwt_strncmp (const CWT_CHAR *s1, const CWT_CHAR *s2, size_t n) { return strncmp(s1,s2,n); }
static CWT_CHAR* __cwt_strdup  (const CWT_CHAR *s) { return CWT_ISO_CPP_NAME(strdup)(s); }
static CWT_CHAR* __cwt_strcat  (CWT_CHAR *dest, const CWT_CHAR *src) { return strcat(dest, src); }
static CWT_CHAR* __cwt_strncat (CWT_CHAR *dest, const CWT_CHAR *src, size_t n) { return strncat(dest, src, n); }
static CWT_CHAR* __cwt_strtok  (CWT_CHAR *tok, const CWT_CHAR *delim) { return strtok(tok, delim); }
static LONG      __cwt_strtol  (const CWT_CHAR *s, CWT_CHAR **endptr, int radix) { return strtol(s, endptr, radix); }
static ULONG     __cwt_strtoul (const CWT_CHAR *s, CWT_CHAR **endptr, int radix) { return strtoul(s, endptr, radix); }
static double    __cwt_strtod  (const CWT_CHAR *s, CWT_CHAR **endptr) { return strtod(s, endptr); }
static int       __cwt_isalpha (CWT_CHAR ch) { return isalpha(ch); }
static int       __cwt_isdigit (CWT_CHAR ch) { return isdigit(ch); }
static int       __cwt_isspace (CWT_CHAR ch) { return isspace(ch); }
static CWT_CHAR  __cwt_toupper(CWT_CHAR ch)  { return (CWT_CHAR)toupper((int)ch); }
static CWT_CHAR  __cwt_tolower(CWT_CHAR ch)  { return (CWT_CHAR)tolower(ch); }

#endif /* !CWT_UNICODE */

static const CWT_CHAR* __cwt_strerror(int errn);
extern CWT_CHAR* __cwt_strptime (const CWT_CHAR *buf, const CWT_CHAR *fmt, struct tm *tm);
static CWT_CHAR* __cwt_strrstr (const CWT_CHAR *s, const CWT_CHAR *substr);
static int		 __cwt_stricmp  (const CWT_CHAR *s1, const CWT_CHAR *s2);
static int       __cwt_strnicmp (const CWT_CHAR *s1, const CWT_CHAR *s2, size_t n);
static BOOL      __cwt_streq    (const CWT_CHAR *s1, const CWT_CHAR *s2) { return __cwt_strcmp(s1, s2) == 0 ? TRUE : FALSE; }
static BOOL      __cwt_strieq   (const CWT_CHAR *s1, const CWT_CHAR *s2) { return __cwt_stricmp(s1, s2) == 0 ? TRUE : FALSE; }
static CWT_CHAR* __cwt_strndup  (const CWT_CHAR *s, size_t n);
static LONGLONG  __cwt_strtoll  (const CWT_CHAR *s, CWT_CHAR **endptr, int radix);
static ULONGLONG __cwt_strtoull (const CWT_CHAR *s, CWT_CHAR **endptr, int radix);

static void*     __cwt_bzero    (void *block, size_t sz) { return memset(block, 0, sz); }

static void      __cwt_toupperStr   (CWT_CHAR *dest, const CWT_CHAR *src, size_t n);
static void      __cwt_tolowerStr   (CWT_CHAR *dest, const CWT_CHAR *src, size_t n);
static void      __cwt_chomp(CWT_CHAR *s);

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
extern double    __cwt_toDouble     (const CWT_CHAR *str, BOOL *ok);
extern float     __cwt_toFloat      (const CWT_CHAR *str, BOOL *ok);

static void      __cwt_toTime       (const CWT_CHAR *str, CWT_TIME *tm, const CWT_CHAR *format, BOOL *ok);
static void      __cwt_toDate       (const CWT_CHAR *str, CWT_TIME *tm, const CWT_CHAR *format, BOOL *ok);
static void      __cwt_toDateTime   (const CWT_CHAR *str, CWT_TIME *tm, const CWT_CHAR *format, BOOL *ok);

static const CWT_CHAR* __cwt_constEmptyStr(void);
static const CWT_CHAR* __cwt_constNullStr(void);

static const CwtQuotePair*   __singleQuotesPair(void) { static const CwtQuotePair qp[] = {{_T('\''), _T('\'')}, {0, 0}}; return qp; }
static const CwtQuotePair*   __doubleQuotesPair(void) { static const CwtQuotePair qp[] = {{_T('"'), _T('"')}, {0, 0}} ; return qp; }
static const CwtQuotePair*   __quotesPair(void) { static const CwtQuotePair qp[] = {{_T('\''), _T('\'')}, {_T('"'), _T('"')}, {0, 0}} ; return qp; }
static const CWT_CHAR*       __whitespaces(void) { static const CWT_CHAR *ws = _T(" \t\n\v\f\r"); return ws; }


static CwtStrNS __cwtStrNS = {
	  __cwt_strerror
	, __cwt_strptime
	, __cwt_strftime
	, __cwt_strlen
	, __cwt_strcpy
	, __cwt_strncpy
	, __cwt_strchr
	, __cwt_strstr
	, __cwt_strrstr
	, __cwt_strcmp
	, __cwt_strncmp
	, __cwt_stricmp
	, __cwt_strnicmp
	, __cwt_streq
	, __cwt_strieq
	, __cwt_strdup
	, __cwt_strndup
	, __cwt_strcat
	, __cwt_strncat
	, __cwt_strtok
	, __cwt_strtol
	, __cwt_strtoul
	, __cwt_strtoll
	, __cwt_strtoull
	, __cwt_strtod
	, __cwt_isalpha
	, __cwt_isdigit
	, __cwt_isspace

	, memcpy
	, memmove
	, memset
	, memcmp
	, __cwt_bzero

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
	, __cwt_toDouble
	, __cwt_toFloat
	, __cwt_toTime
	, __cwt_toDate
	, __cwt_toDateTime

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

	, __singleQuotesPair
	, __doubleQuotesPair
	, __quotesPair
	, __whitespaces
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


static int __cwt_stricmp (const CWT_CHAR *s1, const CWT_CHAR *s2)
{
#ifdef CWT_UNICODE
	return CWT_ISO_CPP_NAME(wcsicmp)(s1, s2);
#else
#	ifdef CWT_CC_MSC
	return CWT_ISO_CPP_NAME(stricmp)(s1, s2);
#	else
	return strcasecmp(s1, s2);
#	endif
#endif
}

static int __cwt_strnicmp(const CWT_CHAR *s1, const CWT_CHAR *s2, size_t n)
{
#ifdef CWT_UNICODE
	return CWT_ISO_CPP_NAME(wcsnicmp)(s1, s2, n);
#else
#	ifdef CWT_CC_MSC
	return CWT_ISO_CPP_NAME(strnicmp)(s1, s2, n);
#	else
	return strncasecmp(s1, s2, n);
#	endif
#endif
}

static CWT_CHAR* __cwt_strndup(const CWT_CHAR *s, size_t n)
{
	CWT_CHAR *s0 = NULL;

	s0 = CWT_MALLOCA(CWT_CHAR, n+1);
	__cwt_strncpy(s0, s, n);
	s0[n] = (CWT_CHAR)0;

	return s0;
}


static LONGLONG  __cwt_strtoll (const CWT_CHAR *s, CWT_CHAR **endptr, int radix)
{
#ifdef CWT_UNICODE
#	ifdef CWT_CC_MSC
	return _wcstoi64(s, endptr, radix);
#	else
	return wcstoll(s, endptr, radix);
#	endif
#else
#	ifdef CWT_CC_MSC
	return _strtoi64(s, endptr, radix);
#	else
	return strtoll(s, endptr, radix);
#	endif
#endif
}

static ULONGLONG __cwt_strtoull(const CWT_CHAR *s, CWT_CHAR **endptr, int radix)
{
#ifdef CWT_UNICODE
#	ifdef CWT_CC_MSC
	return _wcstoui64(s, endptr, radix);
#	else
	return wcstoull(s, endptr, radix);
#	endif
#else
#	ifdef CWT_CC_MSC
	return _strtoui64(s, endptr, radix);
#	else
	return strtoull(s, endptr, radix);
#	endif
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
		if( __cwtStrNS.strNCmp(ptr, s2, s2_len) == 0 )
			return ptr;
		ptr--;
	}

	return NULL;
}


static void __cwt_toupperStr(CWT_CHAR *dest, const CWT_CHAR *src, size_t n)
{
	size_t i;

	for( i = 0; i < n; i++ ) {
		dest[i] = __cwtStrNS.toUpper(src[i]);
	}
}

static void __cwt_tolowerStr(CWT_CHAR *dest, const CWT_CHAR *src, size_t n)
{
	size_t i;

	for( i = 0; i < n; i++ ) {
		dest[i] = __cwtStrNS.toLower(src[i]);
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
 * @fn CwtStrNS::toTime(const CWT_CHAR *str, CWT_TIME *tm, const CWT_CHAR *format, BOOL *ok)
 *
 * @brief Converts string representation of time into CWT_TIME structure.
 *
 * @param str String representation of time
 * @param tm Time structure to store result.
 * @param format Format of string that specifies time.
 * @param ok Pointer to store result of conversion.
 *
 * @note There is restriction in this function: only predefined formats are supported:
 * @arg [H]H:[M]M[:[S]S[.sss]]
 * @arg HHMMSS
 * @arg HHMMSSsss
 *
 */
static void __cwt_toTime(const CWT_CHAR *str, CWT_TIME *tm, const CWT_CHAR *format, BOOL *ok)
{
	struct tm tm_;
	CWT_CHAR *ptr;

	if( !format )
		format = _T("%H:%M:%S");

	ptr = __cwtStrNS.strptime(str, format, &tm_);

	if( ptr || *ptr == _T('\0') ) {
		if( ok )
			*ok = TRUE;

		tm->hour = tm_.tm_hour;
		tm->min  = tm_.tm_min;
		tm->sec  = tm_.tm_sec;
	} else {
		if( ok )
			*ok = FALSE;
	}
}

/**
 * @fn CwtStrNS::toDate(const CWT_CHAR *str, CWT_TIME *tm, const CWT_CHAR *format, BOOL *ok)
 *
 * @param str
 * @param tm
 * @param format
 * @param ok
 *
 * @note There is restriction in this function: only predefined formats are supported:
 * @arg [d]d.[m]m[.[yy]yy]
 * @arg [d]d/[m]m[/[yy]yy]
 * @arg yyyy.[m]m.[d]d
 * @arg yyyy/[m]m/[d]d
 * @arg yyyymmdd
 *
 */
static void __cwt_toDate(const CWT_CHAR *str, CWT_TIME *tm, const CWT_CHAR *format, BOOL *ok)
{
	struct tm tm_;
	CWT_CHAR *ptr;

	if( !format )
		format = _T("%d.%m.%Y");

	ptr = __cwtStrNS.strptime(str, format, &tm_);

	if( ptr || *ptr == _T('\0') ) {
		if( ok )
			*ok = TRUE;

		tm->year = tm_.tm_year + 1900;
		tm->mon  = tm_.tm_mon + 1;
		tm->day  = tm_.tm_mday;
	} else {
		if( ok )
			*ok = FALSE;
	}
}

/**
 * @fn CwtStrNS::toDateTime(const CWT_CHAR *str, CWT_TIME *tm, const CWT_CHAR *format, BOOL *ok)
 *
 * @param str
 * @param tm
 * @param format
 * @param ok
 *
 * @note There is restriction in this function: only predefined formats are supported
 * 	     ( for date-format and time-format see @ref CwtStrNS::toDate and @ref CwtStrNS::toTime)
 * @arg date-format time-format
 * @arg time-format date-format
 *
 * TODO format support must be implemented
 */
static void __cwt_toDateTime(const CWT_CHAR *str, CWT_TIME *tm, const CWT_CHAR *format, BOOL *ok)
{
	struct tm tm_;
	CWT_CHAR *ptr;

	if( !format )
		format = _T("%H:%M:%S %d.%m.%Y");

	ptr = __cwtStrNS.strptime(str, format, &tm_);

	if( ptr || *ptr == _T('\0') ) {
		if( ok )
			*ok = TRUE;

		tm->hour = tm_.tm_hour;
		tm->min  = tm_.tm_min;
		tm->sec  = tm_.tm_sec;
		tm->year = tm_.tm_year + 1900;
		tm->mon  = tm_.tm_mon + 1;
		tm->day  = tm_.tm_mday;

	} else {
		if( ok )
			*ok = FALSE;
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

#ifdef CWT_CC_MSC
DLL_API_EXPORT char* strndup(const char *s, size_t n)
{
	char *s0 = NULL;

	s0 = CWT_MALLOCA(char, n+1);
	strncpy(s0, s, n);
	s0[n] = (char)0;

	return s0;
}
#endif
