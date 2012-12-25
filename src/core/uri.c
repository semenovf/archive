/*
 * uri.c
 *
 *  Created on: Oct 9, 2012
 *      Author: wladt
 */

#include <cwt/uri.h>
#include <cwt/fsm.h>
#include <cwt/str.h>
#include <cwt/string.h>
#include <cwt/stdio.h>

#include "uri-rfc3986.h"

static CwtUri*   __uri_create  (void);
static void      __uri_free    (CwtUri *uri);
static void      __uri_init    (CwtUri *uri);
static void      __uri_destroy (CwtUri *uri);
static ssize_t   __uri_parse   (const CWT_CHAR *uri_string, CwtUri *uri);
static CWT_CHAR* __uri_compose (CwtUri *uri);

static CwtUriNS __cwtUriNS = {
	  __uri_create
	, __uri_free
	, __uri_init
	, __uri_destroy
	, __uri_parse
	, __uri_compose
};

DLL_API_EXPORT CwtUriNS* cwt_uri_ns(void)
{
	return &__cwtUriNS;
}

static CwtUri*  __uri_create (void)
{
	CwtUri *uri;
	uri = CWT_MALLOCT(CwtUri);
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
		cwt_bzero(uri, sizeof(*uri));
}

static void __uri_destroy (CwtUri *uri)
{
	if( uri ) {
		CWT_FREE(uri->scheme);
		CWT_FREE(uri->userinfo);
		CWT_FREE(uri->host);
		CWT_FREE(uri->path);
		CWT_FREE(uri->query);
		CWT_FREE(uri->fragment);
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
static ssize_t __uri_parse(const CWT_CHAR *uri_string, CwtUri *uri)
{
	CwtFsm fsm;
	ssize_t rc;

	CWT_UNUSED(uri_reference_fsm);

	if( !uri_string || cwt_str_ns()->strLen(uri_string) == 0 )
		return FALSE;

	cwt_fsm_ns()->init(&fsm, sizeof(CWT_CHAR), uri_fsm, uri, cwt_fsm_belong_cwtchar, cwt_fsm_exact_cwtchar, cwt_fsm_range_cwtchar);
	rc = cwt_fsm_ns()->exec(&fsm, 0, uri_string, cwt_str_ns()->strLen(uri_string));
	cwt_fsm_ns()->destroy(&fsm);
	return rc;
}

/**
 *
 * @param uri
 * @return
 */
static CWT_CHAR* __uri_compose (CwtUri *uri)
{
	CwtStrNS *strNS = cwt_str_ns();
	CwtStringNS *stringNS = cwt_string_ns();
	CwtString *uri_string;
	CWT_CHAR *result;

	if( !uri )
		return NULL;

	/* userinfo without host is an error */
	if( !strNS->isEmpty(uri->userinfo)
			&& strNS->isEmpty(uri->host) ) {
		return NULL;
	}

	uri_string = stringNS->create();

	if( !strNS->isEmpty(uri->scheme) ) {
		stringNS->append(uri_string, uri->scheme);
		stringNS->appendChar(uri_string, _T(':'));
	}

	if( !strNS->isEmpty(uri->userinfo)
			|| !strNS->isEmpty(uri->host) ) {
		stringNS->append(uri_string, _T("//"));

		if( !strNS->isEmpty(uri->userinfo) ) {
			stringNS->append(uri_string, uri->userinfo);
			stringNS->appendChar(uri_string, _T('@'));
		}

		if( !strNS->isEmpty(uri->host) ) {
			stringNS->append(uri_string, uri->host);
		}

		if( uri->port > 0 ) {
			CWT_CHAR port_str[32];
			cwt_stdio_ns()->snprintf(port_str, 31, _T(":%u"), uri->port );
			stringNS->append(uri_string, port_str);
		}

		if( uri->path ) {
			stringNS->append(uri_string, uri->path);
		}

		if( !strNS->isEmpty(uri->query) ) {
			stringNS->appendChar(uri_string, _T('?'));
			stringNS->append(uri_string, uri->query);
		}

		if( !strNS->isEmpty(uri->fragment) ) {
			stringNS->appendChar(uri_string, _T('#'));
			stringNS->append(uri_string, uri->fragment);
		}
	}

	result = (CWT_CHAR*)stringNS->cstr(uri_string);
	stringNS->disrobe(uri_string);
	return result;
}
