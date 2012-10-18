/*
 * uri.c
 *
 *  Created on: Oct 9, 2012
 *      Author: wladt
 */

#include <cwt/uri.h>
#include <cwt/fsm.h>
#include <cwt/str.h>

static CwtStrNS *__strNS = NULL; /* declare before header file 'uri-rfc3986.h' will be included */

#include "../src/core/uri-rfc3986.h"

static CwtUri*  __uri_create  (void);
static void     __uri_free    (CwtUri *uri);
static void     __uri_init    (CwtUri *uri);
static void     __uri_destroy (CwtUri *uri);
static BOOL     __uri_parse   (const CWT_CHAR *uri_string, CwtUri *uri);

static CwtUriNS __cwtUriNS = {
	  __uri_create
	, __uri_free
	, __uri_init
	, __uri_destroy
	, __uri_parse
};

DLL_API_EXPORT CwtUriNS* cwtUriNS(void)
{
	if( __strNS ) {
		__strNS = cwtStrNS();
	}
	return &__cwtUriNS;
}

static CwtUri*  __uri_create (void)
{
	CwtUri *uri;
	uri = CWT_MALLOC(CwtUri);
	__uri_init(uri);
	return uri;
}

static void __uri_free (CwtUri *uri)
{
	if( uri ) {
		__uri_destroy(uri);
		CWT_FREE(uri);
	}
}

static inline void __uri_init (CwtUri *uri)
{
	if( uri )
		cwtStrNS()->bzero(uri, sizeof(*uri));
}

static void __uri_destroy (CwtUri *uri)
{
	if( uri ) {
		CWT_FREE(uri->scheme);
		CWT_FREE(uri->path);
		CWT_FREE(uri->host);
		CWT_FREE(uri->query);
	}
}


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
static BOOL __uri_parse(const CWT_CHAR *uri_string, CwtUri *uri)
{
	CwtFsm fsm;

	fsm_common_unused();

	if( !uri_string || cwtStrNS()->strLen(uri_string) == 0 )
		return FALSE;


	/* TODO remove this after successful testing URI namespace */
	CWT_UNUSED(unreserved_fsm);
	CWT_UNUSED(segment_nz_fsm);
	CWT_UNUSED(segment_nz_nc_fsm);
	CWT_UNUSED(segment_fsm);
	CWT_UNUSED(ipv6address_fsm);
	CWT_UNUSED(path_absolute_fsm);
	CWT_UNUSED(path_noscheme_fsm);

	FSM_INIT(fsm, CWT_CHAR, NULL/* uri_reference_fsm*/, uri, cwtBelongCwtChar, cwtExactCwtChar);
	return cwtFsmNS()->exec(&fsm, 0, uri_string, cwtStrNS()->strLen(uri_string)) >= 0
			? TRUE
			: FALSE;
}


