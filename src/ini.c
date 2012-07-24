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
	size_t      max_tokens; /* maximum number of tokens in INI file instruction line */
	UINT        flags;
	CwtChannel *pchan;
	size_t      line;
	HashTable  *directives;
	void       (*on_error)(CwtIniHandler, const CWT_CHAR*);
} CwtIniHandlerImpl;

static CwtIniHandler __create  (void);
static CwtIniHandler __createWithFlags(UINT flags, size_t max_tokens);
static void          __free    (CwtIniHandler);
static BOOL          __parse   (CwtIniHandler, CwtChannel*);
static void          __error   (CwtIniHandler, const CWT_CHAR *errstr);
static void          __onError (CwtIniHandler, void (*callback)(CwtIniHandler, const CWT_CHAR*));
static void          __addDirective (CwtIniHandler, const CWT_CHAR *directive, CwtIniCallback handler);
static size_t        __line    (CwtIniHandler);

static CwtIniNS __cwtIniNS = {
	  __create
	, __createWithFlags
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
	return  __createWithFlags(0, 0);
}


static CwtIniHandler __createWithFlags(UINT flags, size_t max_tokens)
{
	CwtIniHandlerImpl *h;
	h = CWT_MALLOC(CwtIniHandlerImpl);

	h->max_tokens = max_tokens > 0 ? max_tokens : 128;
	h->flags      = flags;
	h->pchan      = NULL;
	h->line       = 0;
	h->on_error   = NULL;

	if( flags & Cwt_IniFlag_DirectiveIgnoreCase ) {
		h->directives = hash_table_new(cwt_string_nocase_hash, cwt_string_nocase_equal);
	} else {
		h->directives = hash_table_new(cwt_string_hash, cwt_string_equal);
	}
	hash_table_register_free_functions(h->directives, cwtFree, NULL);

	return (CwtIniHandler)h;
}


static void __free(CwtIniHandler h)
{
	CwtIniHandlerImpl *ph = (CwtIniHandlerImpl*)h;
	if( h ) {
		if( ph->directives ) {
			hash_table_free(ph->directives);
			ph->directives = NULL;
		}
		CWT_FREE(ph);
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
	ph->line = 0;

	while( !chNS->atEnd(pchan) && chNS->readLine(pchan, ba) ) {
		CWT_CHAR *str;

		ph->line++;

		/* empty line */
		if( baNS->size(ba) == 0 )
			continue;

		if( baNS->last(ba) == '\\' ) {
			baNS->resize(ba, baNS->size(ba)-1); /* remove backslash */
			baNS->appendElem(ba, ' ');
			esc = TRUE;
			continue;
		} else {
			esc = FALSE;
		}


		baNS->trim(ba);
		if( baNS->size(ba) > 0 ) {
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

						if( argc <= ph->max_tokens ) {
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
		}
		CWT_FREE(str);

		baNS->clear(ba);
		slNS->clear(tokens);

		if( !ok )
			break;
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
