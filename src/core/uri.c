/*
 * uri.c
 *
 *  Created on: Oct 9, 2012
 *      Author: wladt
 */

#include <cwt/utils.h>
#include <string.h>
#include <cwt/bytearr.h>
#include <cwt/logger.h>

#define _PERCENT_CHAR '%'
/*
static const char *__hex_digits = "01234567890abcdefABCDEF";
static const char *__gen_delims = ":/?#[]@:";
static const char *__sub_delims = "!$&'()*+,;=";
static const char *__alphas     = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
static const char *__digits     = "0123456789";
*/

/*static CwtByteArrayNS *__baNS = NULL;*/

#define _IS_HEX_DIGIT(ch) (strchr(__hex_digits, ch) != NULL ? TRUE : FALSE)
#define _IS_GEN_DELIM(ch) (strchr(__gen_delims, ch) != NULL ? TRUE : FALSE)
#define _IS_SUB_DELIM(ch) (strchr(__sub_delims, ch) != NULL ? TRUE : FALSE)
#define _IS_RESERVED(ch)  (_IS_GEN_DELIM(ch) || _IS_SUB_DELIM(ch) ?  TRUE : FALSE)
#define _IS_ALPHA(ch)     (strstr(__alphas, ch) != NULL ? TRUE : FALSE)
#define _IS_DIGIT(ch)     (strstr(__digits, ch) != NULL ? TRUE : FALSE)
#define _IS_UNRESERVED    (_IS_ALPHA(ch)       \
		                    || _IS_DIGIT(ch)   \
		                    || (ch) == '-'     \
		                    || (ch) == '.'     \
		                    || (ch) == '_'     \
		                    || (ch) == '~'     \
							? TRUE : FALSE)

#define _IS_FIRST_SCHEME_CHAR(ch) _IS_ALPHA(ch)
#define _IS_SCHEME_CHAR(ch)  (_IS_ALPHA(ch)    \
                            || _IS_DIGIT(ch)   \
                            || (ch) == '+'     \
                            || (ch) == '-'     \
                            || (ch) == '.')


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
#define _ST_URI_SLASH        10

BOOL __utils_parseURI(const char *uri_string, CwtUri *uri)
{
	BOOL ok = FALSE;

	CWT_UNUSED(uri_string);
	CWT_UNUSED(uri);
#ifdef __COMMENT__
	CwtByteArray *buf;
	size_t len, i;
	int state;

	CWT_ASSERT(uri);

	if( !__baNS ) {
		__baNS = cwtByteArrayNS();
	}

	len = strlen(uri_string);
	if( !len ) {
		cwtLoggerNS()->warn(_Tr("empty URI string"));
		return FALSE;
	}

	buf = __baNS->create();
	i = 0;

	state = _ST_URI_BEGIN;

	while(i < len) {
		switch(state) {
		case _ST_URI_BEGIN:
			if( __is_first_scheme_char(uri_string[i]) ) {
				i++;
			} else {
				state = _ST_URI_ERROR;
			}
			break;

		case _ST_URI_ERROR:
			return FALSE;

		case _ST_URI_SCHEME:
			if(uri_string[i] == _T(':')) {
				state = _ST_URI_END_SCHEME;
				i++;
			} else if( __is_scheme_char(uri_string[i]) ) {
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

		case _ST_URI_SLASH:
			if( uri_string[i] == _T('/') ) {

			}
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

	__baNS->free(buf);
#endif
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

void __utils_initURI (CwtUri *uri)
{
	CWT_UNUSED(uri);
}

void __utils_destroyURI (CwtUri *uri)
{
	CWT_UNUSED(uri);
}

