/*
 * txtcodec.c
 *
 *  Created on: 02.07.2012
 *      Author: wladt
 */


#include <string.h>
#include <cwt/txtcodec.h>
#include <cwt/str.h>
#include <cwt/logger.h>

/*
#ifdef CWT_CC_GNUC
#	include <stdlib.h>
#endif
#include <string.h>
#include <stdarg.h>
*/

char* cwt_strndup (const char *s, size_t n);

static char*      __cwt_toLatin1     (const CWT_CHAR *s, size_t n);
static CWT_CHAR*  __cwt_fromLatin1   (const char *s, size_t n);
static char*      __cwt_toUtf8       (const CWT_CHAR *s, size_t n);
static CWT_CHAR*  __cwt_fromUtf8     (const char *utf8, size_t n);
static char*      __cwt_toMBCS       (const CWT_CHAR *s, const CWT_CHAR *csname, size_t n);
static CWT_CHAR*  __cwt_fromMBCS     (const char *s, const CWT_CHAR *csname, size_t n);

static CwtTextCodecNS __cwtTextCodecNS = {
	  __cwt_toLatin1
	, __cwt_fromLatin1
	, __cwt_toUtf8
	, __cwt_fromUtf8
	, __cwt_toMBCS
	, __cwt_fromMBCS
};

static CwtStrNS *__cwtStrNS = NULL;

DLL_API_EXPORT CwtTextCodecNS* cwt_textcodec_ns(void)
{
	if( !__cwtStrNS ) {
		__cwtStrNS = cwt_str_ns();
	}
	return &__cwtTextCodecNS;
}


/**
  @return converted string, or empty string if error occured or source string was empty
*/
static char* __cwt_toLatin1( const CWT_CHAR *s, size_t n )
{
	char *str = NULL;

	if( s ) {
#ifdef CWT_UNICODE
		size_t length = n;/*__cwtStrNS->strlen(s)*/

		if( length ) {
			const CWT_CHAR *src = s;
			char *ptr;

			str = CWT_MALLOCA(char, length+1);
			ptr = str;
			str[length] = '\0';

			while( length-- ) {
				*ptr++ = (*src > 0xff) ? '?' : (char)*src;
				++src;
			}
		}
#else
		str = cwt_strndup(s, n);
#endif
	} /* if s */
	return str;
}


static CWT_CHAR* __cwt_fromLatin1(const char *s, size_t n)
{
	CWT_CHAR *str = NULL;

#ifdef CWT_UNICODE
	size_t length = n; /*strlen(s);*/

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
	str = cwt_strndup(s, n);
#endif

	return str;
}

static char* __cwt_toUtf8(const CWT_CHAR *s, size_t n)
{
	char *utf8 = NULL;

	if( s ) {

#ifdef CWT_UNICODE
#	ifdef CWT_CC_MSC
		/*size_t length = n; *//*(size_t)__cwtStrNS.strlen(s)*/
		/*Get length (in chars) of resulting UTF-8 string*/
		const int utf8_length = WideCharToMultiByte(
			CP_UTF8,               /*convert to UTF-8*/
			0,                     /*default flags*/
			s,                     /*source UTF-16 string*/
			(int)n,                /*source string length, in wchar_t's */ /*FIXME need to check*/
			NULL,                  /*unused - no conversion required in this step*/
			0,                     /*request buffer size*/
			NULL, NULL);           /*unused*/

		if( utf8_length == 0 ) {
			printf_error(_Tr("getting length of result utf8 string failed: %s"), __cwtStrNS->strError(GetLastError()));
			return NULL;
		}

		/*Allocate destination buffer for UTF-8 string*/
		utf8 = CWT_MALLOCA(char, utf8_length+1);

		/*Do the conversion from UTF-16 to UTF-8*/
		if ( ! WideCharToMultiByte(
				CP_UTF8,
				0,
				s,
				(int)n, /*FIXME need to check*/
				&utf8[0],
				utf8_length,
				NULL, NULL )) {
			printf_error(_Tr("converting string to utf8 failed: %s"), __cwtStrNS->strError(GetLastError()));
			CWT_FREE(utf8);
			return NULL;
		}
		utf8[utf8_length] = '\0';
#	else
#		error __toUtf8 is not implemented yet
#	endif
#else
		/* FIXME need to convert Local8Bit chars into UTF8 */
		utf8 = cwt_strndup(s, n);
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
static CWT_CHAR* __cwt_fromUtf8(const char *utf8, size_t n)
{
	CWT_CHAR *str = NULL;
	size_t utf8_length;

	if( !utf8 ) {
		return NULL;
	}

	utf8_length = n;

#ifdef CWT_UNICODE
#	ifdef CWT_CC_MSC

	if( utf8_length ) {
		/*Get length (in wchar_t's) of resulting UTF-16 string*/
		int length = MultiByteToWideChar(
			CP_UTF8,           /*convert from UTF-8*/
			0,                 /*default flags*/
			utf8,              /*source UTF-8 string*/
			(int)utf8_length,  /*length (in chars) of source UTF-8 string*/ /*FIXME need to check*/
			NULL,              /*unused - no conversion done in this step*/
			0);                /*request size of destination buffer, in wchar_t's*/

		if( length == 0 ) {
			printf_error(_Tr("getting length of result string failed %s"), __cwtStrNS->strError(GetLastError()));
			return NULL;
		}

		str = CWT_MALLOCA(CWT_CHAR, length+1);

		/*Do the conversion from UTF-8 to UTF-16*/
		if( ! MultiByteToWideChar(
				CP_UTF8,
				0,
				utf8,
				(int)utf8_length, /*FIXME need to check*/
				&str[0],
				length )) {
			printf_error(_Tr("converting string from_utf8_failed: %s"), __cwtStrNS->strError(GetLastError()));
			CWT_FREE(str);
			return NULL;
		}

		str[length] = _T('\0');
	}
#	else
#		error __toUtf8 is not implemented yet
#	endif
#else
	CWT_UNUSED(utf8_length);
	/* FIXME need to convert UTF8 to Local8Bit chars */
	str = cwt_strndup(utf8, n);
#endif

	return str;
}


static char* __cwt_toMBCS(const CWT_CHAR *s, const CWT_CHAR *csname, size_t n)
{
	if( __cwtStrNS->strEq(_T("utf8"), csname))
		return __cwt_toUtf8(s, n);

	if( __cwtStrNS->strEq(_T("latin1"), csname))
		return __cwt_toLatin1(s, n);

	cwt_logger_ns()->warn(_Tr("CwtStrNS::toMBCS(): no text codec is attached, converting to MBCS"));
	return __cwt_toLatin1(s, n);
}


static CWT_CHAR* __cwt_fromMBCS(const char *s, const CWT_CHAR *csname, size_t n)
{
	if( __cwtStrNS->strEq(_T("utf8"), csname))
		return __cwt_fromUtf8(s, n);
	if( __cwtStrNS->strEq(_T("latin1"), csname))
		return __cwt_fromLatin1(s, n);

	cwt_logger_ns()->warn(_Tr("CwtStrNS::fromMBCS(): no text codec is attached, converting from MBCS"));
	return __cwt_fromLatin1(s, n);
}

