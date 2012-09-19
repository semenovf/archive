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

static CwtIniHandler* __ini_create  (void);
static CwtIniHandler* __ini_createWithFlags(UINT flags, size_t max_tokens);
static void           __ini_free    (CwtIniHandler*);
static BOOL           __ini_parse   (CwtIniHandler*, CwtChannel*);
static void           __ini_error   (CwtIniHandler*, const CWT_CHAR *errstr);
static void           __ini_addDirective (CwtIniHandler*, const CWT_CHAR *directive, CwtIniCallback handler);
static void           __ini_setDefaultDirective (CwtIniHandler*, CwtIniCallback handler);
static size_t         __ini_line    (CwtIniHandler*);

static CwtIniNS __cwtIniNS = {
	  __ini_create
	, __ini_createWithFlags
	, __ini_free
	, __ini_parse
	, __ini_error
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

static CwtIniHandler* __ini_create(void)
{
	return  __ini_createWithFlags(0, 0);
}

static CwtIniHandler* __ini_createWithFlags(UINT flags, size_t max_tokens)
{
	CwtIniHandler *h;
	h = CWT_MALLOC(CwtIniHandler);

	h->max_tokens = max_tokens > 0 ? max_tokens : 128;
	h->flags      = flags;
	h->pchan      = NULL;
	h->defaultCallback = NULL;
	h->line       = 0;
	h->onError    = NULL;

	if( flags & Cwt_IniFlag_DirectiveIgnoreCase ) {
		h->directives = __htNS->create(__htNS->striHash, __htNS->strieq, cwtFree, NULL);
	} else {
		h->directives = __htNS->create(__htNS->strHash, __htNS->streq, cwtFree, NULL);
	}

	return h;
}


static void __ini_free(CwtIniHandler* h)
{
	if( h ) {
		if( h->directives ) {
			__htNS->free(h->directives);
			h->directives = NULL;
		}
		CWT_FREE(h);
	}
}

static BOOL __ini_parse(CwtIniHandler *h, CwtChannel *pchan)
{
	CwtStrNS       *strNS    = cwtStrNS();
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
	h->line = 0;

	while( !chNS->atEnd(pchan) && chNS->readLine(pchan, ba) ) {
		CWT_CHAR *str;

		h->line++;

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

			if( str && strNS->strLen(str) > 0 ) {

				/* not a comment */
				if( str[0] != _T('#')) {
					int rc = slNS->splitAny(tokens, str, CWT_WHITESPACES, CWT_QUOTES_BOTH, 0);
					if( rc > 0 ) {
						CwtIniCallback callback;
						CWT_CHAR **argv;
						size_t argc = 0;

						argc = slNS->size(tokens);

						if( argc <= h->max_tokens ) {
							argv = CWT_MALLOCA(CWT_CHAR*, argc);

							slNS->toArray(tokens, argv, &argc);
							callback  = (CwtIniCallback)__htNS->lookup(h->directives, argv[0]);

							if( callback ) {
								ok = callback(h, (const CWT_CHAR**)argv, argc);
							} else {
								if( h->defaultCallback ) {
									ok = h->defaultCallback(h, (const CWT_CHAR**)argv, argc);
								} else {
									if( !(h->flags & Cwt_IniFlag_DirectiveIgnoreUnknown) ) {
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


static void __ini_error(CwtIniHandler *h, const CWT_CHAR *errstr)
{
	CWT_ASSERT(h);

	if (h->onError)
		h->onError(h, errstr);
}


static void __ini_addDirective (CwtIniHandler *h, const CWT_CHAR *directive, CwtIniCallback handler)
{
	CWT_ASSERT(h);

	if( directive && handler ) {
		CWT_CHAR *dir = cwtStrNS()->strDup(directive);
		CWT_ASSERT(__htNS->insert(h->directives, dir, handler));
	}
}


static void __ini_setDefaultDirective (CwtIniHandler *h, CwtIniCallback handler)
{
	CWT_ASSERT(h);

	if( handler ) {
		h->defaultCallback = handler;
	}
}

static size_t __ini_line(CwtIniHandler *h)
{
	CWT_ASSERT(h);
	return h->line;
}

