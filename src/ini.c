/*
 * ini.c
 *
 *  Created on: 05.12.2011
 *      Author: wladt
 */


/*
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
*/
#include <cwt/str.h>
#include <cwt/string.h>
#include <cwt/unistd.h>
#include <cwt/logger.h>
#include <cwt/bytearr.h>
#include <cwt/io/channel.h>
#include <cwt/ini.h>

typedef struct CwtIniHandlerImpl {
	CwtChannel *pchan;
	size_t      line;
	void (*on_error)(CwtIniHandler, const CWT_CHAR*);

/*
	BOOL error;
	void (*on_start_document)(struct CwtIniHandlerBase* h);
	void (*on_end_document)(struct CwtIniHandlerBase* h);
	void (*on_start_line)(struct CwtIniHandlerBase* h);
	void (*on_end_line)(struct CwtIniHandlerBase* h);
	void (*on_token)(struct CwtIniHandlerBase* h, const CWT_CHAR* token);
*/
} CwtIniHandlerImpl;

/*
#ifndef _NASSYNC
#	include <cwt/event/event.h>
#endif

#define _BUFSZ 256
*/
static CwtIniHandler __create  (void);
static void          __init    (CwtIniHandler);
static void          __destroy (CwtIniHandler);
static void          __free    (CwtIniHandler);
static BOOL          __parse   (CwtIniHandler, CwtChannel*);
static void          __onError (CwtIniHandler, void (*callback)(CwtIniHandler, const CWT_CHAR*));
static void          __addRule (CwtIniHandler, CwtIniCallback resolver, CwtIniCallback handler);


static CwtIniNS __cwtIniNS = {
	  __create
	, __init
	, __destroy
	, __free
	, __parse
	, __onError
	, __addRule
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
}

static void __destroy(CwtIniHandler h)
{
	CWT_UNUSED(h);
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
	CwtChannelNS   *chanNS   = cwtChannelNS();
	CwtByteArrayNS *baNS     = cwtByteArrayNS();
	CwtByteArray ba;
	size_t sz;

	CWT_ASSERT(h);

	baNS->init(&ba);

	baNS->destroy(&ba);

	while( (sz = chanNS->bytesAvailable(pchan)) > 0 ) {
		size_t bsz = baNS->size(&ba);
		size_t off, off_cr, off_lf;

		baNS->resize(&ba, sz + bsz);
		chanNS->read(pchan, ba.m_buffer + bsz, sz);

		off_cr = 0;
		off_lf = 0;
		if( baNS->find(&ba, (BYTE)'\r', &off_cr) || baNS->find(&ba, (BYTE)'\n', &off_lf) ) {
			CWT_CHAR *str;

			off = CWT_MIN(off_cr, off_lf);
			str = strNS->fromUtf8(ba.m_buffer, off);
			if( str && strNS->strlen(str) > 0 ) {

			}
			CWT_FREE(str);
			ph->line++;
		}

#ifdef __COMMENT__
		while( baNS->find(&ba, (BYTE)'\n') )

		while( rb_find_byte(rb, (BYTE)'\n', nl_pos, &nl_pos) && !handler->error ) {

			handler->on_start_line(handler);
			handler->line++;

			if( __cwtIniParseLine(rb, nl_pos, handler) ) {
				handler->on_end_line(handler);
				rb_pop_front(rb, nl_pos+1);
				nl_pos = 0;
			}
		}

		if( handler->error )
			break;
	}

	if( !handler->error ) {
		if( bw < 0 ) {
			handler->error = TRUE;
		} else {
			if( rb_size(rb) > 0 ) {
				__cwtIniParseLine(rb, rb_size(rb), handler); /* last line*/
			}
		}
	}

	handler->on_end_document(handler);

	rb_delete(rb);
	cwtClose(ini);

	return handler->error ? FALSE : TRUE;
#endif

	return FALSE;
}


#ifdef __COMMENT__

typedef struct CwtIniHandler {
	CwtIniHandlerBase base;
	BOOL first_token;
	BOOL comment_line;
	int nitems;
	CHAR *instruction[CWT_INI_MAX_TOKENS];
	CwtIniRule *rules;
} CwtIniHandler;


BOOL cwtLoadIni(const char* path, CwtIniHandlerBase* handler)
{
	int ini;
	CwtIODevicePtr file;
	CwtStringBufferPtr sb;
/*	RingBufferPtr rb;*/
	ssize_t bw;

	file = cwtFileDeviceOpen(path, O_RDONLY);

	/*ini = cwtOpen(path, O_RDONLY | O_TEXT);*/

	if( ini < 0 ) {
		printf_error(_Tr("%s: unable to open INI file: %s"), path, strerror(errno));
		return FALSE;
	}

	rb = rb_new_defaults();
	CWT_ASSERT(rb);

	if( !handler )
		handler = &__cwtNullHandler;

	handler->file = path;
	handler->on_start_document(handler);
	handler->line = 0;
	handler->error = FALSE;

	while( (bw = rb_write_from_file(rb, ini, _BUFSZ)) > 0 ) {
		size_t nl_pos = 0;

		if( bw < _BUFSZ && rb_size(rb) > 0 ) { /* end-of-file */
			rb_put(rb, (BYTE)'\n');
		}

		while( rb_find_byte(rb, (BYTE)'\n', nl_pos, &nl_pos) && !handler->error ) {

			handler->on_start_line(handler);
			handler->line++;

			if( __cwtIniParseLine(rb, nl_pos, handler) ) {
				handler->on_end_line(handler);
				rb_pop_front(rb, nl_pos+1);
				nl_pos = 0;
			}

/*
#ifndef _NASSYNC
			process_events();
#endif
*/
		}

		if( handler->error )
			break;
	}

	if( !handler->error ) {
		if( bw < 0 ) {
			handler->error = TRUE;
		} else {
			if( rb_size(rb) > 0 ) {
				__cwtIniParseLine(rb, rb_size(rb), handler); /* last line*/
			}
		}
	}

	handler->on_end_document(handler);

	rb_delete(rb);
	cwtClose(ini);

	return handler->error ? FALSE : TRUE;
}




static void __on_token(RingBufferPtr rb, void* extra)
{

	CwtIniHandlerBase* h = (CwtIniHandlerBase*)extra;

	if( rb_size(rb) > 0 ) {
		CHAR* token;
		size_t len = rb_size(rb);

		token = CWT_MALLOCA(CHAR, len + 1);
		rb_read(rb, (BYTE*)token, len);
		token[len] = '\0';
		h->on_token(h, token);
		CWT_FREE(token);

	} else {
		h->on_token(h, "");
	}
}

BOOL __cwtIniParseLine(CwtStringBufferPtr sb, size_t len, CwtIniHandlerBase* handler)
{
	int rc = rb_split((BYTE)' ', rb, len, 0, __on_token, handler);

	if( rc < 0 ) {
		if( rc == RBE_QUOTE_CHAR_UNBALANCED ) {
			handler->on_error(handler, _Tr("unbalanced quote char"));
		} else {
			handler->on_error(handler, _Tr("line is incorrect"));
		}
	}

	return handler->error == TRUE ? FALSE : TRUE;
}



static void __cwtIniOnRuleEndLine(CwtIniHandlerBase* h)
{
	int i;
	CwtIniHandler *dh = (CwtIniHandler*)h;

	if( dh->nitems > 0 ) {
		CwtIniRule *rule_ptr = dh->rules;
		BOOL rule_accepted = FALSE;

		while( rule_ptr->cmd ) {
			if( cwtStrEq(dh->instruction[0], rule_ptr->cmd)
					&& dh->nitems >= rule_ptr->min_argc) {
				rule_accepted = TRUE;
				rule_ptr->callback(h, dh->nitems, dh->instruction);
				break;
			}
			rule_ptr++;
		}

		if( !rule_accepted ) {
			dh->base.on_error((CwtIniHandlerBase*)dh, _Tr("bad instruction or incomplete arguments"));
		}
	}

	for( i = 0; i < dh->nitems; i++ ) {
		if( dh->instruction[i] )
			CWT_FREE(dh->instruction[i]);
		dh->instruction[i] = NULL;
	}
	dh->nitems = 0;
}

static void __cwtIniOnRuleToken(CwtIniHandlerBase* h, const char* token)
{
	CwtIniHandler *dh = (CwtIniHandler*)h;

	if( cwtStrLen(token) == 0 )
		return;

	if( dh->first_token && token[0] == '#' ) { /* comment */
		dh->comment_line = TRUE;
	}

	if( dh->comment_line != TRUE ) {
		CWT_ASSERT(dh->nitems < CWT_INI_MAX_TOKENS);
		dh->instruction[dh->nitems++] = cwtStrDup(token);
	}

	dh->first_token = FALSE;
}

/*
static void __cwtIniOnRuleError(CwtIniHandlerBase* h, const char* errstr)
{
	h->error = TRUE;
	printf_error("%s at %s line %u", errstr, h->file, h->line);
}
*/


DLL_API_EXPORT BOOL cwtLoadIniByRules(const CHAR* path, CwtIniRule *rules, void (*on_error)(CwtIniHandlerBase*, const char*))
{
	CwtIniHandler cwtIniHandler = {
		{
			   NULL /* file */
			,  0
			, FALSE
			, __cwtIniOnRuleStartDocument
			, __cwtIniOnRuleEndDocument
			, __cwtIniOnRuleStartLine
			, __cwtIniOnRuleEndLine
			, __cwtIniOnRuleToken
			, on_error
		}     /* base */
		, TRUE  /* first_token */
		, FALSE /* comment_line */
		, 0 /* nitems */
		, {NULL} /*instruction*/
		, rules
	};

	if( cwtLoadIni(path, (CwtIniHandlerBase*)&cwtIniHandler) )
		return cwtIniHandler.base.error == TRUE ? FALSE : TRUE;
	return FALSE;
}

#endif
