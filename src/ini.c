/*
 * loadini.c
 *
 *  Created on: 05.12.2011
 *      Author: user
 */


#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cwt/string.h>
#include <cwt/unistd.h>
#include <cwt/logger.h>
#include <cwt/strbuf.h>
#include <cwt/io/channel.h>
#include <cwt/io/filedev.h>
#include <cwt/ini.h>

#ifndef _NASSYNC
#	include <cwt/event/event.h>
#endif

#define _BUFSZ 256


typedef struct CwtIniHandler {
	CwtIniHandlerBase base;
	BOOL first_token;
	BOOL comment_line;
	int nitems;
	CHAR *instruction[CWT_INI_MAX_TOKENS];
	CwtIniRule *rules;
} CwtIniHandler;

static BOOL __cwtIniParseLine(CwtStringBufferPtr sb, size_t len, CwtIniHandlerBase* handlers);

static void __cwtIniOnDebugStartDocument(CwtIniHandlerBase* h);
static void __cwtIniOnDebugEndDocument(CwtIniHandlerBase* h);
static void __cwtIniOnDebugStartLine(CwtIniHandlerBase* h);
static void __cwtIniOnDebugEndLine(CwtIniHandlerBase* h);
static void __cwtIniOnDebugToken(CwtIniHandlerBase* h, const char* token);
static void __cwtIniOnDebugError(CwtIniHandlerBase* h, const char* errstr);

static void __cwtIniOnNullStartDocument(CwtIniHandlerBase* h) {CWT_UNUSED(h);}
static void __cwtIniOnNullEndDocument(CwtIniHandlerBase* h) {CWT_UNUSED(h);}
static void __cwtIniOnNullStartLine(CwtIniHandlerBase* h) {CWT_UNUSED(h);}
static void __cwtIniOnNullEndLine(CwtIniHandlerBase* h) {CWT_UNUSED(h);}
static void __cwtIniOnNullToken(CwtIniHandlerBase* h, const char* token) {CWT_UNUSED(h); CWT_UNUSED(token);}
static void __cwtIniOnNullError(CwtIniHandlerBase* h, const char* errstr) {CWT_UNUSED(h); CWT_UNUSED(errstr);}

static void __cwtIniOnRuleStartDocument(CwtIniHandlerBase* h);
static void __cwtIniOnRuleEndDocument(CwtIniHandlerBase* h);
static void __cwtIniOnRuleStartLine(CwtIniHandlerBase* h);
static void __cwtIniOnRuleEndLine(CwtIniHandlerBase* h);
static void __cwtIniOnRuleToken(CwtIniHandlerBase* h, const char* token);
static void __cwtIniOnRuleError(CwtIniHandlerBase* h, const char* errstr);


static CwtIniHandlerBase __cwtDebugHandler = {
	  NULL
	, 0
	, FALSE
	, __cwtIniOnDebugStartDocument
	, __cwtIniOnDebugEndDocument
	, __cwtIniOnDebugStartLine
	, __cwtIniOnDebugEndLine
	, __cwtIniOnDebugToken
	, __cwtIniOnDebugError
};

static CwtIniHandlerBase __cwtNullHandler = {
	  NULL
	, 0
	, FALSE
	, __cwtIniOnNullStartDocument
	, __cwtIniOnNullEndDocument
	, __cwtIniOnNullStartLine
	, __cwtIniOnNullEndLine
	, __cwtIniOnNullToken
	, __cwtIniOnNullError
};

void __cwtIniOnDebugStartDocument(CwtIniHandlerBase* h)
{
	CWT_UNUSED(h);
	print_trace("--START DOCUMENT");
}

void __cwtIniOnDebugEndDocument(CwtIniHandlerBase* h)
{
	CWT_UNUSED(h);
	print_trace("--END DOCUMENT");
}

void __cwtIniOnDebugStartLine(CwtIniHandlerBase* h)
{
	CWT_UNUSED(h);
	print_trace("--START LINE");
}

void __cwtIniOnDebugEndLine(CwtIniHandlerBase* h)
{
	CWT_UNUSED(h);
	print_trace("--END LINE");
}

void __cwtIniOnDebugToken(CwtIniHandlerBase* h, const char* token)
{
	CWT_UNUSED(h);
	CWT_UNUSED(token);
	printf_debug("token: %s", token);
}

void __cwtIniOnDebugError(CwtIniHandlerBase* h, const char* errstr)
{
	h->error = TRUE;
	printf_error("%s at %s line %u", errstr, h->file, h->line);
}


BOOL cwtLoadIniStd(const char* path, CWT_INI_STD_HANDLER h)
{
	CwtIniHandlerBase* handler = &__cwtNullHandler;

	switch(h) {
	case INI_HANDLER_NULL:
		handler = &__cwtNullHandler;
		break;
	case INI_HANDLER_DEBUG:
		handler = &__cwtDebugHandler;
		break;
	default:
		break;
	}
	return cwtLoadIni(path, handler);
}



BOOL cwtLoadIni(const char* path, CwtIniHandlerBase* handler)
{
	int ini;
	CwtIODevicePtr file;
	CwtStringBufferPtr sb;
/*	RingBufferPtr rb;*/
	ssize_t bw;

	file = cwtFileDeviceOpen(path, NULL, TRUE);

	/*ini = cwtOpen(path, O_RDONLY | O_TEXT);*/

	if( ini < 0 ) {
		printf_error(_Tr("%s: unable to open INI file: %s"), path, strerror(errno));
		return FALSE;
	}

	sb = cwtStringBuffer
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



static void __cwtIniOnRuleStartDocument(CwtIniHandlerBase* h)
{
	CWT_UNUSED(h);
}

static void __cwtIniOnRuleEndDocument(CwtIniHandlerBase* h)
{
	CWT_UNUSED(h);
}

static void __cwtIniOnRuleStartLine(CwtIniHandlerBase* h)
{
	((CwtIniHandler*)h)->first_token  = TRUE;
	((CwtIniHandler*)h)->comment_line = FALSE;
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
