/*
 * string.c
 *
 *  Created on: 18.06.2012
 *      Author: wladt
 */


#include <cwt/str.h>
#include <cwt/logger.h>

const CWT_CHAR* __CONST_EMPTYSTR = _T("");
const CWT_CHAR* __CONST_NULLSTR  = _T("<null>");
CWT_CHAR*       __errorstr = NULL; /* TODO must be freed when program finishing */

static const CWT_CHAR* __strerror(int errn);
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

static char*      __toLatin1     (const CWT_CHAR *s);
static CWT_CHAR*  __fromLatin1   (const char *s);
static char*      __toUtf8       (const CWT_CHAR *s);
static CWT_CHAR*  __fromUtf8     (const char *utf8);
static char*      __toMBCS       (const CWT_CHAR *s, const CWT_CHAR *csname);
static CWT_CHAR*  __fromMBCS     (const char *s, const CWT_CHAR *csname);

static const CWT_CHAR* __constEmptyStr(void);
static const CWT_CHAR* __constNullStr(void);


static CwtStrNS __cwtStrNS = {
	__strerror
#ifdef CWT_UNICODE
	, wcsftime
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
	, strftime
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
	, __fromLatin1
	, __toUtf8
	, __fromUtf8
	, __toMBCS
	, __fromMBCS

	, __constEmptyStr
	, __constNullStr
};


DLL_API_EXPORT CwtStrNS* cwtStrNS(void)
{
	return &__cwtStrNS;
}


static const CWT_CHAR* __strerror(int errn)
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
	__chomp(__errorstr);
	}
#	else
	__errorstr = __cwtStrNS.strdup(CWT_ISO_CPP_NAME(wcserror)(errn));
#	endif
#else
	__errorstr = __cwtStrNS.strdup(strerror(errn));
#endif

	return __errorstr;
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
static char* __toLatin1( const CWT_CHAR *s )
{
	char *latins1 = NULL;

	if( s ) {
#ifdef CWT_UNICODE
		CwtStrNS *strNS = cwtStrNS();
		size_t length = strNS->strlen(s);

		if( length ) {
			const CWT_CHAR *src = s;
			char *ptr;

			latins1 = CWT_MALLOCA(char, length+1);
			ptr = latins1;
			latins1[length] = '\0';

			while( length-- ) {
				*ptr++ = (*src > 0xff) ? '?' : (char)*src;
				++src;
			}
		}
#else
		latins1 = strdup(s);
#endif
	} /* if s */
	return latins1;
}


static CWT_CHAR* __fromLatin1(const char *s)
{
	CWT_CHAR *str;
#ifdef CWT_UNICODE
	/*CwtStrNS *strNS = cwtStrNS();*/
	size_t length = strlen(s);

	if( length ) {
		const char *src = s;
		CWT_CHAR *ptr;

		str = CWT_MALLOCA(CWT_CHAR, length+1);
		ptr = str;
        str[length] = _T('\0');

        while( length-- ) {
            *ptr++ = (CWT_CHAR)(*src++);
        }
	}
#else
	return str = strdup(s);
#endif
	return str;
}

static char* __toUtf8(const CWT_CHAR *s)
{
	char *utf8 = NULL;

	if( s ) {

#ifdef CWT_UNICODE
#	ifdef CWT_CC_MSC
		int length = __cwtStrNS.strlen(s);
		/*Get length (in chars) of resulting UTF-8 string*/
		const int utf8_length = WideCharToMultiByte(
			CP_UTF8,               /*convert to UTF-8*/
			0,                     /*default flags*/
			s,                     /*source UTF-16 string*/
			length,                /*source string length, in wchar_t's */
			NULL,                  /*unused - no conversion required in this step*/
			0,                     /*request buffer size*/
			NULL, NULL);           /*unused*/

		if( utf8_length == 0 ) {
			printf_error(_Tr("getting length of result utf8 string failed: %s"), __cwtStrNS.strerror(GetLastError()));
			return NULL;
		}

		/*Allocate destination buffer for UTF-8 string*/
		utf8 = CWT_MALLOCA(char, utf8_length+1);

		/*Do the conversion from UTF-16 to UTF-8*/
		if ( ! WideCharToMultiByte(
				CP_UTF8,
				0,
				s,
				length,
				&utf8[0],
				utf8_length,
				NULL, NULL )) {
			printf_error(_Tr("converting string to utf8 failed: %s"), __cwtStrNS.strerror(GetLastError()));
			CWT_FREE(utf8);
			return NULL;
		}
		utf8[utf8_length] = '\0';
#	else
#		error __toUtf8 is not implemented yet
#	endif
#else
	utf8 = strdup(s);
#endif

	} /* s */

	return utf8;
}


/**
 *
 * @param utf8
 * @return
 *
 * Note: Return value must be deallocated.
 */
static CWT_CHAR* __fromUtf8(const char *utf8)
{
	CwtStrNS *strNS = cwtStrNS();
	CWT_CHAR *str = NULL;
	int utf8_length;

	if( !utf8 ) {
		return NULL;
	}

	utf8_length = strlen(utf8);

#ifdef CWT_UNICODE
#	ifdef CWT_CC_MSC

	if( utf8_length ) {
		/*Get length (in wchar_t's) of resulting UTF-16 string*/
		const int length = MultiByteToWideChar(
			CP_UTF8,           /*convert from UTF-8*/
			0,                 /*default flags*/
			utf8,              /*source UTF-8 string*/
			utf8_length,       /*length (in chars) of source UTF-8 string*/
			NULL,              /*unused - no conversion done in this step*/
			0);                /*request size of destination buffer, in wchar_t's*/

		if( length == 0 ) {
			printf_error(_Tr("getting length of result string failed %s"), __cwtStrNS.strerror(GetLastError()));
			return NULL;
		}

		str = CWT_MALLOCA(CWT_CHAR, length+1);

		/*Do the conversion from UTF-8 to UTF-16*/
		if( ! MultiByteToWideChar(
				CP_UTF8,
				0,
				utf8,
				utf8_length,
				&str[0],
				length )) {
			printf_error(_Tr("converting string from_utf8_failed: %s"), __cwtStrNS.strerror(GetLastError()));
			CWT_FREE(str);
			return NULL;
		}
	}
#	else
#		error __toUtf8 is not implemented yet
#	endif
#else
#endif

	return str;
}


static char* __toMBCS(const CWT_CHAR *s, const CWT_CHAR *csname)
{
	if( __cwtStrNS.streq(_T("utf8"), csname))
		return __cwtStrNS.toUtf8(s);
	if( __cwtStrNS.streq(_T("latin1"), csname))
		return __cwtStrNS.toLatin1(s);

	printf_warn(_Tr("CwtStrNS::toMBCS(): no text codec is attached, converting to MBCS"));
	return __cwtStrNS.toLatin1(s);
}


static CWT_CHAR* __fromMBCS(const char *s, const CWT_CHAR *csname)
{
	if( __cwtStrNS.streq(_T("utf8"), csname))
		return __cwtStrNS.fromUtf8(s);
	if( __cwtStrNS.streq(_T("latin1"), csname))
		return __cwtStrNS.fromLatin1(s);

	printf_warn(_Tr("CwtStrNS::fromMBCS(): no text codec is attached, converting from MBCS"));
	return __cwtStrNS.fromLatin1(s);
}


static const CWT_CHAR* __constEmptyStr(void)
{
	return __CONST_EMPTYSTR;
}

static const CWT_CHAR* __constNullStr(void)
{
	return __CONST_NULLSTR;
}
