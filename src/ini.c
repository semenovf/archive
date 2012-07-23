/*
 * ini.c
 *
 *  Created on: 05.12.2011
 *      Author: wladt
 */


#include <cwt/str.h>
#include <cwt/string.h>
#include <cwt/unistd.h>
#include <cwt/logger.h>
#include <cwt/bytearr.h>
#include <cwt/string.h>
#include <cwt/strlist.h>
#include <cwt/io/channel.h>
#include <cwt/ini.h>
#include <cwt/algo/hash_tab.h>
#include <cwt/algo/hash_str.h>
#include <cwt/algo/cmp_str.h>


typedef struct CwtIniHandlerImpl {
	CwtChannel *pchan;
	size_t      line;
	HashTable  *directives;
	void       (*on_error)(CwtIniHandler, const CWT_CHAR*);
} CwtIniHandlerImpl;

static CwtIniHandler __create  (void);
static void          __free    (CwtIniHandler);
static BOOL          __parse   (CwtIniHandler, CwtChannel*);
static void          __error   (CwtIniHandler, const CWT_CHAR *errstr);
static void          __onError (CwtIniHandler, void (*callback)(CwtIniHandler, const CWT_CHAR*));
static void          __addDirective (CwtIniHandler, const CWT_CHAR *directive, CwtIniCallback handler);
static size_t        __line    (CwtIniHandler);

static void          __init    (CwtIniHandler);
static void          __destroy (CwtIniHandler);

static CwtIniNS __cwtIniNS = {
	  __create
	, __free
	, __parse
	, __error
	, __onError
	, __addDirective
	, __line
};


DLL_API_EXPORT CwtIniNS* cwtIniNS(void)
{
	return &__cwtIniNS;
}

static CwtIniHandler __create(void)
{
	CwtIniHandlerImpl *h;

	h = CWT_MALLOC(CwtIniHandlerImpl);
	__init(h);
	return (CwtIniHandler)h;
}

static void __init(CwtIniHandler h)
{
	CwtIniHandlerImpl *ph = (CwtIniHandlerImpl*)h;

	CWT_ASSERT(h);
	ph->pchan = NULL;
	ph->line = 0;
	ph->on_error = NULL;

	ph->directives = hash_table_new(cwt_string_hash, cwt_string_equal);
	hash_table_register_free_functions(ph->directives, cwtFree, NULL);
}

static void __destroy(CwtIniHandler h)
{
	CwtIniHandlerImpl *ph = (CwtIniHandlerImpl*)h;
	CWT_UNUSED(h);
	if( ph->directives ) {
		hash_table_free(ph->directives);
		ph->directives = NULL;
	}

}

static void __free(CwtIniHandler h)
{
	if( h ) {
		__destroy(h);
		CWT_FREE(h);
	}
}

static BOOL __parse(CwtIniHandler h, CwtChannel *pchan)
{
	CwtIniHandlerImpl *ph = (CwtIniHandlerImpl*)h;
	CwtStrNS       *strNS    = cwtStrNS();
	CwtStringNS    *stringNS = cwtStringNS();
	CwtStrListNS   *slNS     = cwtStrListNS();
	CwtChannelNS   *chNS     = cwtChannelNS();
	CwtByteArrayNS *baNS     = cwtByteArrayNS();
	CwtByteArray   *ba;
	CwtStrList     *tokens;
	BOOL esc = FALSE;
	BOOL ok = TRUE;

	CWT_ASSERT(h);

	ba = baNS->create();
	tokens = slNS->create();

	while( !chNS->atEnd(pchan) ) {
		if( chNS->readLine(pchan, ba) ) {
			CWT_CHAR *str;

			if( baNS->last(ba) == '\\' ) {
				baNS->resize(ba, baNS->size(ba)-1); /* remove backslash */
				baNS->appendElem(ba, ' ');
				esc = TRUE;
				continue;
			} else {
				esc = FALSE;
			}


			baNS->trim(ba);
			str = strNS->fromUtf8(baNS->cstr(ba), baNS->size(ba)); /* TODO need apply text codec insteed of fromUtf8 call */

			if( str && strNS->strlen(str) > 0 ) {

				/* not a comment */
				if( str[0] != _T('#')) {
					int rc = slNS->splitAny(tokens, str, CWT_WHITESPACES, CWT_QUOTES_BOTH);
					if( rc > 0 ) {
						CwtIniCallback cb;
						CWT_CHAR **argv;
						size_t argc = 0;

						argc = slNS->size(tokens);

						if( argc <= CWT_INI_MAX_TOKENS ) {
							argv = CWT_MALLOCA(CWT_CHAR*, argc);

							slNS->toArray(tokens, argv, &argc);
							cb  = (CwtIniCallback)hash_table_lookup(ph->directives, argv[0]);

							if( cb ) {
								ok = cb(h, argv, argc);
							} else {
								__error(h, _Tr("unsupported directive"));
								ok = FALSE;
							}
						} else {
							__error(h, _Tr("maximum tokens in directive line are exceeded"));
							ok = FALSE;
						}

						CWT_FREE(argv);
					} else if( rc < 0 ) {
						__error(h, _Tr("quotes are unbalanced"));
						ok = FALSE;
					}
				}
			}
			CWT_FREE(str);

			ph->line++;
			baNS->clear(ba);
			slNS->clear(tokens);

			if( !ok )
				break;
		}
	}

	slNS->free(tokens);
	baNS->free(ba);

	if( esc ) {
		__cwtIniNS.error(h, _T("unexpected end-of-file"));
		ok = FALSE;
	}

	return ok;
}


static void __error(CwtIniHandler h, const CWT_CHAR *errstr)
{
	CwtIniHandlerImpl *ph = (CwtIniHandlerImpl*)h;

	CWT_ASSERT(h);

	if( ph->on_error )
		ph->on_error(h, errstr);
}


static void __addDirective (CwtIniHandler h, const CWT_CHAR *directive, CwtIniCallback handler)
{
	CwtIniHandlerImpl *ph = (CwtIniHandlerImpl*)h;

	CWT_ASSERT(h);

	if( directive && handler ) {
		CWT_CHAR *dir = cwtStrNS()->strdup(directive);
		CWT_ASSERT(hash_table_insert(ph->directives, dir, handler));
	}
}

static size_t __line(CwtIniHandler h)
{
	CWT_ASSERT(h);
	return ((CwtIniHandlerImpl*)h)->line;
}

static void __onError(CwtIniHandler h, void (*callback)(CwtIniHandler, const CWT_CHAR*))
{
	CwtIniHandlerImpl *ph = (CwtIniHandlerImpl*)h;

	CWT_ASSERT(h);
	ph->on_error = callback;
}
