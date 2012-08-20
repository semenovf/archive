/*
 * ini.c
 *
 *  Created on: 05.12.2011
 *      Author: wladt
 */


#include <cwt/str.h>
#include <cwt/string.h>
#include <cwt/txtcodec.h>
#include <cwt/unistd.h>
#include <cwt/logger.h>
#include <cwt/bytearr.h>
#include <cwt/strlist.h>
#include <cwt/io/channel.h>
#include <cwt/ini.h>
#include <cwt/hashtab.h>


typedef struct CwtIniHandlerImpl {
	size_t      max_tokens; /* maximum number of tokens in INI file instruction line */
	UINT        flags;
	CwtChannel *pchan;
	size_t      line;
	CwtHashTable  *directives;
	CwtIniCallback defaultCallback;
	void       (*on_error)(CwtIniHandler, const CWT_CHAR*);
} CwtIniHandlerImpl;

static CwtIniHandler __ini_create  (void);
static CwtIniHandler __ini_createWithFlags(UINT flags, size_t max_tokens);
static void          __ini_free    (CwtIniHandler);
static BOOL          __ini_parse   (CwtIniHandler, CwtChannel*);
static void          __ini_error   (CwtIniHandler, const CWT_CHAR *errstr);
static void          __ini_onError (CwtIniHandler, void (*callback)(CwtIniHandler, const CWT_CHAR*));
static void          __ini_addDirective (CwtIniHandler, const CWT_CHAR *directive, CwtIniCallback handler);
static void          __ini_setDefaultDirective (CwtIniHandler, CwtIniCallback handler);
static size_t        __ini_line    (CwtIniHandler);

static CwtIniNS __cwtIniNS = {
	  __ini_create
	, __ini_createWithFlags
	, __ini_free
	, __ini_parse
	, __ini_error
	, __ini_onError
	, __ini_addDirective
	, __ini_setDefaultDirective
	, __ini_line
};

CwtHashTableNS *__htNS = NULL;

DLL_API_EXPORT CwtIniNS* cwtIniNS(void)
{
	if( !__htNS ) {
		__htNS = cwtHashTableNS();
	}
	return &__cwtIniNS;
}

static CwtIniHandler __ini_create(void)
{
	return  __ini_createWithFlags(0, 0);
}


static CwtIniHandler __ini_createWithFlags(UINT flags, size_t max_tokens)
{
	CwtIniHandlerImpl *h;
	h = CWT_MALLOC(CwtIniHandlerImpl);

	h->max_tokens = max_tokens > 0 ? max_tokens : 128;
	h->flags      = flags;
	h->pchan      = NULL;
	h->defaultCallback = NULL;
	h->line       = 0;
	h->on_error   = NULL;

	if( flags & Cwt_IniFlag_DirectiveIgnoreCase ) {
		h->directives = __htNS->create(__htNS->striHash, __htNS->strieq, cwtFree, NULL);
	} else {
		h->directives = __htNS->create(__htNS->strHash, __htNS->streq, cwtFree, NULL);
	}

	return (CwtIniHandler)h;
}


static void __ini_free(CwtIniHandler h)
{
	CwtIniHandlerImpl *ph = (CwtIniHandlerImpl*)h;
	if( h ) {
		if( ph->directives ) {
			__htNS->free(ph->directives);
			ph->directives = NULL;
		}
		CWT_FREE(ph);
	}
}

static BOOL __ini_parse(CwtIniHandler h, CwtChannel *pchan)
{
	CwtIniHandlerImpl *ph = (CwtIniHandlerImpl*)h;
	CwtStrNS       *strNS    = cwtStrNS();
	CwtStringNS    *stringNS = cwtStringNS();
	CwtStrListNS   *slNS     = cwtStrListNS();
	CwtChannelNS   *chNS     = cwtChannelNS();
	CwtByteArrayNS *baNS     = cwtByteArrayNS();
	CwtTextCodecNS *codecNS  = cwtTextCodecNS();
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
			str = codecNS->fromUtf8(baNS->cstr(ba), baNS->size(ba));

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
							cb  = (CwtIniCallback)__htNS->lookup(ph->directives, argv[0]);

							if( cb ) {
								ok = cb(h, argv, argc);
							} else {
								if( ph->defaultCallback ) {
									ok = ph->defaultCallback(h, argv, argc);
								} else {
									if( !(ph->flags & Cwt_IniFlag_DirectiveIgnoreUnknown) ) {
										__ini_error(h, _Tr("unsupported directive"));
										ok = FALSE;
									}
								}
							}
						} else {
							__ini_error(h, _Tr("maximum tokens in directive line are exceeded"));
							ok = FALSE;
						}

						CWT_FREE(argv);
					} else if( rc < 0 ) {
						__ini_error(h, _Tr("quotes are unbalanced"));
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


static void __ini_error(CwtIniHandler h, const CWT_CHAR *errstr)
{
	CwtIniHandlerImpl *ph = (CwtIniHandlerImpl*)h;

	CWT_ASSERT(h);

	if( ph->on_error )
		ph->on_error(h, errstr);
}


static void __ini_addDirective (CwtIniHandler h, const CWT_CHAR *directive, CwtIniCallback handler)
{
	CwtIniHandlerImpl *ph = (CwtIniHandlerImpl*)h;

	CWT_ASSERT(h);

	if( directive && handler ) {
		CWT_CHAR *dir = cwtStrNS()->strdup(directive);
		CWT_ASSERT(__htNS->insert(ph->directives, dir, handler));
	}
}


static void __ini_setDefaultDirective (CwtIniHandler h, CwtIniCallback handler)
{
	CwtIniHandlerImpl *ph = (CwtIniHandlerImpl*)h;

	CWT_ASSERT(h);

	if( handler ) {
		ph->defaultCallback = handler;
	}
}

static size_t __ini_line(CwtIniHandler h)
{
	CWT_ASSERT(h);
	return ((CwtIniHandlerImpl*)h)->line;
}

static void __ini_onError(CwtIniHandler h, void (*callback)(CwtIniHandler, const CWT_CHAR*))
{
	CwtIniHandlerImpl *ph = (CwtIniHandlerImpl*)h;

	CWT_ASSERT(h);
	ph->on_error = callback;
}
