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

static CwtIniContext* __ini_create  (void);
static CwtIniContext* __ini_createWithFlags(UINT flags, size_t max_tokens);
static void           __ini_free    (CwtIniContext*);
static BOOL           __ini_parse   (CwtIniContext*, CwtChannel*);
static void           __ini_error   (CwtIniContext*, const CWT_CHAR *errstr);
static void           __ini_addDirective (CwtIniContext*, const CWT_CHAR *directive, CwtIniCallback handler);
static void           __ini_setDefaultDirective (CwtIniContext*, CwtIniCallback handler);
static size_t         __ini_line    (CwtIniContext*);

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

DLL_API_EXPORT CwtIniNS* cwt_ini_ns(void)
{
	if( !__htNS ) {
		__htNS = cwt_hashtable_ns();
	}
	return &__cwtIniNS;
}

static CwtIniContext* __ini_create(void)
{
	return  __ini_createWithFlags(0, 0);
}

static CwtIniContext* __ini_createWithFlags(UINT flags, size_t max_tokens)
{
	CwtIniContext *h;
	h = CWT_MALLOCT(CwtIniContext);

	h->max_tokens = max_tokens > 0 ? max_tokens : 128;
	h->flags      = flags;
	h->pchan      = NULL;
	h->defaultCallback = NULL;
	h->line       = 0;
	h->onError    = NULL;

	if( flags & Cwt_IniFlag_DirectiveIgnoreCase ) {
		h->directives = __htNS->create(__htNS->striHash, __htNS->strieq, cwt_free, NULL);
	} else {
		h->directives = __htNS->create(__htNS->strHash, __htNS->streq, cwt_free, NULL);
	}

	return h;
}


static void __ini_free(CwtIniContext* h)
{
	if( h ) {
		if( h->directives ) {
			__htNS->free(h->directives);
			h->directives = NULL;
		}
		CWT_FREE(h);
	}
}

static BOOL __ini_parse(CwtIniContext *h, CwtChannel *pchan)
{
	CwtStrNS       *str_ns    = cwt_str_ns();
	CwtStrListNS   *sl_ns     = cwt_strlist_ns();
	CwtChannelNS   *ch_ns     = cwt_channel_ns();
	CwtByteArrayNS *ba_ns     = cwt_bytearray_ns();
	CwtTextCodecNS *codec_ns  = cwt_textcodec_ns();
	CwtByteArray   *ba;
	CwtStrList     *tokens;
	BOOL esc = FALSE;
	BOOL ok = TRUE;

	CWT_ASSERT(h);

	ba = ba_ns->create();
	tokens = sl_ns->create();
	h->line = 0;

	while( !ch_ns->atEnd(pchan) && ch_ns->readLine(pchan, ba) ) {
		CWT_CHAR *str;

		h->line++;

		/* empty line */
		if( ba_ns->size(ba) == 0 )
			continue;

		if( ba_ns->last(ba) == '\\' ) {
			ba_ns->resize(ba, ba_ns->size(ba)-1); /* remove backslash */
			ba_ns->append(ba, ' ');
			esc = TRUE;
			continue;
		} else {
			esc = FALSE;
		}


		ba_ns->trim(ba);
		if( ba_ns->size(ba) > 0 ) {
			str = codec_ns->fromUtf8_n(ba_ns->cstr(ba), ba_ns->size(ba));

			if( str && str_ns->strLen(str) > 0 ) {

				/* not a comment */
				if( str[0] != _T('#')) {
					int rc = sl_ns->splitAny(tokens, str, CWT_WHITESPACES, CWT_QUOTES_BOTH, 0);
					if( rc > 0 ) {
						CwtIniCallback callback;
						CWT_CHAR **argv;
						size_t argc = 0;

						argc = sl_ns->size(tokens);

						if( argc <= h->max_tokens ) {
							argv = CWT_MALLOCA(CWT_CHAR*, argc);

							sl_ns->toArray(tokens, argv, &argc);
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

		ba_ns->clear(ba);
		sl_ns->clear(tokens);

		if( !ok )
			break;
	}

	sl_ns->free(tokens);
	ba_ns->free(ba);

	if( esc ) {
		__cwtIniNS.error(h, _T("unexpected end-of-file"));
		ok = FALSE;
	}

	return ok;
}


static void __ini_error(CwtIniContext *h, const CWT_CHAR *errstr)
{
	CWT_ASSERT(h);

	if (h->onError)
		h->onError(h, errstr);
}


static void __ini_addDirective (CwtIniContext *h, const CWT_CHAR *directive, CwtIniCallback handler)
{
	CWT_ASSERT(h);

	if( directive && handler ) {
		CWT_CHAR *dir = cwt_str_ns()->strDup(directive);
		CWT_ASSERT(__htNS->insert(h->directives, dir, handler));
	}
}


static void __ini_setDefaultDirective (CwtIniContext *h, CwtIniCallback handler)
{
	CWT_ASSERT(h);

	if( handler ) {
		h->defaultCallback = handler;
	}
}

static size_t __ini_line(CwtIniContext *h)
{
	CWT_ASSERT(h);
	return h->line;
}

