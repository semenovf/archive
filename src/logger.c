#include <stdio.h>
#include <stdlib.h>
#include <cwt/string.h>
#include <cwt/strbuf.h>
#include <cwt/logger.h>

struct _LOGGER_CONTEXT {
	CHAR* prefix;
	StringBufferPtr sbuf;
	void (*printer)(const CHAR* msg);
} LOGGER_CONTEXT[LOGGER_COUNT] = {
	  {NULL, NULL, NULL}
	, {NULL, NULL, NULL}
	, {NULL, NULL, NULL}
	, {NULL, NULL, NULL}
	, {NULL, NULL, NULL}
};

static CHAR* __default_prefix[LOGGER_COUNT] = {
	  "Trace: "
	, "Debug: "
	, "Info : "
	, "Warn : "
	, "Error: "
};

void set_printer(LOGGER_TYPE type
	, void (*printer)(const CHAR* msg)
	, const CHAR* prefix)
{
	CWT_ASSERT(printer);

	if( LOGGER_CONTEXT[type].prefix ) {
		CWT_FREE(LOGGER_CONTEXT[type].prefix);
		LOGGER_CONTEXT[type].prefix = NULL;
	}

	if( prefix )
		LOGGER_CONTEXT[type].prefix = cwtStrDup(prefix);

	if( !LOGGER_CONTEXT[type].sbuf ) {
		LOGGER_CONTEXT[type].sbuf = strbuf_new_defaults();
	}

	if( printer )
		LOGGER_CONTEXT[type].printer = printer;
}


static void _print_trace(const CHAR* msg)
{ fprintf(stdout, "%s\n", msg); }

static void _print_debug(const CHAR* msg)
{ fprintf(stdout, "%s\n", msg); }

static void _print_info(const CHAR* msg)
{ fprintf(stdout, "%s\n", msg); }

static void _print_warn(const CHAR* msg)
{ fprintf(stderr, "%s\n", msg); }

static void _print_error(const CHAR* msg)
{ fprintf(stderr, "%s\n", msg); }


/*
void _printf_fatal(int exit_code, const CHAR* format, ...)
{
	va_list args;
	va_start(args, format);
	_vprintf_prefixed(stderr, __pfx_fatal, format, args);
	va_end(args);
	exit(exit_code);
}
*/

void set_default_printers(void)
{
	set_printer(LOGGER_TRACE, _print_trace, __default_prefix[LOGGER_TRACE]);
	set_printer(LOGGER_DEBUG, _print_debug, __default_prefix[LOGGER_DEBUG]);
	set_printer(LOGGER_INFO,  _print_info,  __default_prefix[LOGGER_INFO]);
	set_printer(LOGGER_WARN,  _print_warn,  __default_prefix[LOGGER_WARN]);
	set_printer(LOGGER_ERROR, _print_error, __default_prefix[LOGGER_ERROR]);
}

void init_loggers(void)
{
	set_default_printers();
}

static void _print(LOGGER_TYPE type, const CHAR* msg)
{
	if( ! LOGGER_CONTEXT[type].printer )
		init_loggers();

	strbuf_clear(LOGGER_CONTEXT[type].sbuf);
	if( LOGGER_CONTEXT[type].prefix )
		strbuf_append(LOGGER_CONTEXT[type].sbuf, LOGGER_CONTEXT[type].prefix);
	if( msg )
		strbuf_append(LOGGER_CONTEXT[type].sbuf, msg);
	/*strbuf_append(LOGGER_CONTEXT[type].sbuf, "\n");*/
	LOGGER_CONTEXT[type].printer(strbuf_cstr(LOGGER_CONTEXT[type].sbuf));
}


void print_trace(const CHAR* msg) { _print(LOGGER_TRACE, msg); }
void print_debug(const CHAR* msg) { _print(LOGGER_DEBUG, msg); }
void print_info(const CHAR* msg)  { _print(LOGGER_INFO, msg); }
void print_warn(const CHAR* msg)  { _print(LOGGER_WARN, msg); }
void print_error(const CHAR* msg) { _print(LOGGER_ERROR, msg); }


static void _printf(LOGGER_TYPE type, const CHAR* format, va_list args)
{
	CHAR msg[512];
	vsprintf(msg, format, args);
	_print(type, msg);
}

void printf_trace(const CHAR* format, ...)
{
	va_list args;
	va_start(args, format);
	_printf(LOGGER_TRACE, format, args);
	va_end(args);
}

void printf_debug(const CHAR* format, ...)
{
	va_list args;
	va_start(args, format);
	_printf(LOGGER_DEBUG, format, args);
	va_end(args);
}

void printf_info(const CHAR* format, ...)
{
	va_list args;
	va_start(args, format);
	_printf(LOGGER_INFO, format, args);
	va_end(args);
}

void printf_warn(const CHAR* format, ...)
{
	va_list args;
	va_start(args, format);
	_printf(LOGGER_WARN, format, args);
	va_end(args);
}

void printf_error(const CHAR* format, ...)
{
	va_list args;
	va_start(args, format);
	_printf(LOGGER_ERROR, format, args);
	va_end(args);
}
