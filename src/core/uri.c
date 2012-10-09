/*
 * uri.c
 *
 *  Created on: Oct 9, 2012
 *      Author: wladt
 */

#include <cwt/utils.h>
#include <cwt/str.h>
#include <cwt/string.h>
#include <cwt/logger.h>

#define _PERCENT_CHAR _T('%')
static CWT_CHAR *__hex_digits = _T("01234567890abcdefABCDEF");
static CWT_CHAR *__gen_delims = _T(":/?#[]@:");
static CWT_CHAR *__sub_delims = _T("!$&'()*+,;=");
static CWT_CHAR *__alphas     = _T("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
static CWT_CHAR *__digits     = _T("0123456789");

static CwtStrNS *__strNS = NULL;
static CwtStringNS *__stringNS = NULL;

#define __is_hex_digit(ch) (__strNS->strChr(__hex_digits, ch) != NULL ? TRUE : FALSE)
#define __is_gen_delim(ch) (__strNS->strChr(__gen_delims, ch) != NULL ? TRUE : FALSE)
#define __is_sub_delim(ch) (__strNS->strChr(__sub_delims, ch) != NULL ? TRUE : FALSE)
#define __is_reserved(ch)  (__is_gen_delim(ch) || __is_sub_delim(ch) ?  TRUE : FALSE)
#define __is_alpha(ch)     (__strNS->strChr(__alphas, ch) != NULL ? TRUE : FALSE)
#define __is_digit(ch)     (__strNS->strChr(__digits, ch) != NULL ? TRUE : FALSE)
#define __is_unreserved    (__is_alpha(ch)     \
		                    || __is_digit(ch)  \
		                    || (ch) == _T('-') \
		                    || (ch) == _T('.') \
		                    || (ch) == _T('_') \
		                    || (ch) == _T('~') \
							? TRUE : FALSE)

#define __is_first_scheme_char(ch) __is_alpha(ch)
#define __is_scheme_char(ch)  (__is_alpha(ch)  \
                            || __is_digit(ch)  \
                            || (ch) == _T('+') \
                            || (ch) == _T('-') \
                            || (ch) == _T('.'))


/**
 * @fn void CwtUtilsNS::parseURI(const CWT_CHAR *uri_string, CwtUri *uri)
 *
 * @brief Parses URI (Uniform Resource Identifier) represented by a string
 *        and decompose it to components.
 *
 * @details Implementation of RFC 3986 (http://tools.ietf.org/html/rfc3986)
 *
 *
 * @param uri_string URI represented by a string
 * @param uri URI represented by it's components
 *
 * @note ^(([^:/?#]+):)?(//([^/?#]*))?([^?#]*)(\?([^#]*))?(#(.*))?
 */

#define _ST_URI_BEGIN         0
#define _ST_URI_ERROR         1
#define _ST_URI_SCHEME        2
#define _ST_URI_END_SCHEME    3
#define _ST_URI_HIER_PART     4
#define _ST_URI_HIER_PART_END 5
#define _ST_URI_QUERY         6
#define _ST_URI_QUERY_END     7
#define _ST_URI_FRAGMENT      8
#define _ST_URI_FRAGMENT_END  9

BOOL __utils_parseURI(const CWT_CHAR *uri_string, CwtUri *uri)
{
	BOOL ok = FALSE;
	CwtString *buf;
	const CWT_CHAR *ptr;
	size_t len, i;
	int state;

	CWT_ASSERT(uri);

	if( !__strNS ) {
		__strNS = cwtStrNS();
		__stringNS = cwtStringNS();
	}

	len = __strNS->strLen(uri_string);
	if( !len ) {
		cwtLoggerNS()->warn(_Tr("empty URI string"));
		return FALSE;
	}

	buf = __stringNS->create();
	i = 0;


	state = _ST_URI_BEGIN;

	while(i < len) {
		switch(state) {
		case _ST_URI_BEGIN:
			if( __is_first_scheme_char(ptr[i]) ) {
				i++;
			} else {
				state = _ST_URI_ERROR;
			}
			break;

		case _ST_URI_ERROR:
			return FALSE;

		case _ST_URI_SCHEME:
			if(ptr[i] == _T(':')) {
				state = _ST_URI_END_SCHEME;
				i++;
			} else if( __is_scheme_char(ptr[i]) ) {
				i++;
			} else {
				state = _ST_URI_ERROR;
			}
			break;

		case _ST_URI_END_SCHEME:
			state = _ST_URI_HIER_PART;
			break;

		case _ST_URI_HIER_PART:
			break;
		case _ST_URI_HIER_PART_END:
			break;

		case _ST_URI_QUERY:
			break;
		case _ST_URI_QUERY_END:
			break;

		case _ST_URI_FRAGMENT:
			break;
		case _ST_URI_FRAGMENT_END:
			break;

		}
	};
/*
	do {
		__stringNS->clear(buf);
		if( !__read_scheme(&ptr, len, buf) )
			break;
		uri->scheme = __strNS->strDup(__stringNS->cstr(buf));

		__stringNS->clear(buf);

		ok = TRUE;
	} while(TRUE);
*/

	__stringNS->free(buf);

	return ok;
}

/**
 *
 *
 * @brief Parses data source name (DSN).
 *
 * @details Driver is a dynamically loadable module.
 *
 * @param dsn [in] Driver source name.
 * @param pscheme [out] Scheme name.
 * @param pdriver [out] driver name.
 * @param params [out] Parameters to driver.
 */
/*static void __parseDSN(const CWT_CHAR *dsn, CWT_CHAR **scheme, CWT_CHAR **driver, CWT_CHAR **params)
{
	CwtStrNS *strNS = cwtStrNS();
	CwtStrListNS *slNS = cwtStrListNS();
	CwtStrList *opts;
	const CWT_CHAR *opt;

    opts = slNS->create();
    slNS->splitAny(opts, dsn, _T(":"), CWT_QUOTES_BOTH, 0);

    opt = slNS->at(opts, 0);
    if( scheme && opt )
    	*scheme = strNS->strDup(opt);

    opt = slNS->at(opts, 1);
    if( driver && opt )
    	*driver = strNS->strDup(opt);

    opt = slNS->at(opts, 2);
    if( params && opt )
    	*params = strNS->strDup(opt);

    slNS->free(opts);
}*/
