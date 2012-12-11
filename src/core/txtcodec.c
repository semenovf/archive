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

static char*      txtcodec_to_latin1     (const CWT_CHAR *s, size_t n);
static CWT_CHAR*  txtcodec_from_latin1   (const char *s, size_t n);
static char*      txtcodec_to_utf8       (const CWT_CHAR *s, size_t n);
static CWT_CHAR*  txtcodec_from_utf8     (const char *utf8, size_t n);
static char*      txtcodec_to_mbcs       (const CWT_CHAR *s, const CWT_CHAR *csname, size_t n);
static CWT_CHAR*  txtcodec_from_mbcs     (const char *s, const CWT_CHAR *csname, size_t n);

static CwtTextCodecNS __cwtTextCodecNS = {
	  txtcodec_to_latin1
	, txtcodec_from_latin1
	, txtcodec_to_utf8
	, txtcodec_from_utf8
	, txtcodec_to_mbcs
	, txtcodec_from_mbcs
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
static char* txtcodec_to_latin1( const CWT_CHAR *s, size_t n )
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


static CWT_CHAR* txtcodec_from_latin1(const char *s, size_t n)
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

static char* txtcodec_to_utf8(const CWT_CHAR *s, size_t n)
{
	if (!s)
		return NULL;

#ifdef CWT_UNICODE
	return cwt_encode_utf8(s, n);
#else
		/* FIXME need to convert Local8Bit chars into UTF8 */
	return cwt_strndup(s, n);
#endif
}


/**
 *
 * @param utf8
 * @return
 *
 * Note: Return value must be deallocated.
 */
static CWT_CHAR* txtcodec_from_utf8(const char *utf8, size_t n)
{
	if( !utf8 )
		return NULL;

#ifdef CWT_UNICODE
	return cwt_decode_utf8(utf8, n);
#else
	/* FIXME need to convert UTF8 to Local8Bit chars */
	return cwt_strndup(utf8, n);
#endif
}


static char* txtcodec_to_mbcs(const CWT_CHAR *s, const CWT_CHAR *csname, size_t n)
{
	if( __cwtStrNS->strEq(_T("utf8"), csname))
		return txtcodec_to_utf8(s, n);

	if( __cwtStrNS->strEq(_T("latin1"), csname))
		return txtcodec_to_latin1(s, n);

	cwt_logger_ns()->warn(_Tr("CwtStrNS::toMBCS(): no text codec is attached, converting to latin1"));
	return txtcodec_to_latin1(s, n);
}


static CWT_CHAR* txtcodec_from_mbcs(const char *s, const CWT_CHAR *csname, size_t n)
{
	if( __cwtStrNS->strEq(_T("utf8"), csname))
		return txtcodec_from_utf8(s, n);
	if( __cwtStrNS->strEq(_T("latin1"), csname))
		return txtcodec_from_latin1(s, n);

	cwt_logger_ns()->warn(_Tr("CwtStrNS::fromMBCS(): no text codec is attached, converting from latin1"));
	return txtcodec_from_latin1(s, n);
}

