/*
 * uri.c
 *
 *  Created on: Oct 9, 2012
 *      Author: wladt
 */

#include <cwt/utils.h>
#include <cwt/fsm.h>
#include <cwt/str.h>

#include "../src/core/uri-rfc3986.h"

static CwtUtilsNS *__utilsNS = NULL;

/**
 * @fn void CwtUtilsNS::parseURI(const CWT_CHAR *uri_string, CwtUri *uri)
 *
 * @brief Parses URI (Uniform Resource Identifier) represented by a string
 *        and decompose it to components.
 *
 * @details Implementation of RFC 3986 (http://tools.ietf.org/html/rfc3986)
 *
 * @param uri_string URI represented by a string
 * @param uri URI represented by it's components
 */
BOOL __utils_parseURI(const CWT_CHAR *uri_string, CwtUri *uri)
{
	CwtFsm fsm;

	if( !__utilsNS ) {
		__utilsNS = cwtUtilsNS();
		__strNS = cwtStrNS();
	}

	if( !uri_string || __strNS->strLen(uri_string) == 0 )
		return FALSE;

	FSM_INIT(fsm, CWT_CHAR, uri_reference_fsm, uri, cwtBelongCwtChar, cwtExactCwtChar);
	return cwtFsmNS()->exec(&fsm, 0, uri_string, __strNS->strLen(uri_string)) >= 0
			? TRUE
			: FALSE;
}

void __utils_initURI (CwtUri *uri)
{
	if( uri )
		cwtStrNS()->bzero(uri, sizeof(*uri));
}

void __utils_destroyURI (CwtUri *uri)
{
	if( uri ) {
		CWT_FREE(uri->scheme);
		CWT_FREE(uri->path);
		CWT_FREE(uri->host);
		CWT_FREE(uri->query);
		__utils_initURI(uri);
	}
}

