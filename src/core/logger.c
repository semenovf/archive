#include <cwt/logger.h>
#include <stdlib.h>
#include <cwt/stdio.h>
#include <cwt/str.h>
#include <cwt/string.h>

static CwtLoggerNS __cwtLoggerNS = {
	  printf_trace
	, printf_debug
	, printf_info
	, printf_warn
	, printf_error
	, set_printer
	, set_default_printers
};

struct _LOGGER_CONTEXT {
	CWT_CHAR* prefix;
	CwtString *sbuf;
	void (*printer)(const CWT_CHAR* msg);
} LOGGER_CONTEXT[LOGGER_COUNT] = {
	  {NULL, NULL, NULL}
	, {NULL, NULL, NULL}
	, {NULL, NULL, NULL}
	, {NULL, NULL, NULL}
	, {NULL, NULL, NULL}
};

static CWT_CHAR* __default_prefix[LOGGER_COUNT] = {
	  _Tr("Trace: ")
	, _Tr("Debug: ")
	, _Tr("Info : ")
	, _Tr("Warn : ")
	, _Tr("Error: ")
};

static BOOL __is_logger_init = FALSE;


DLL_API_EXPORT CwtLoggerNS* cwtLoggerNS(void)
{
	if( !__is_logger_init ) {
		init_loggers();
		__is_logger_init = TRUE;
	}
	return &__cwtLoggerNS;
}


void set_printer(LOGGER_TYPE type
	, void (*printer)(const CWT_CHAR* msg)
	, const CWT_CHAR* prefix)
{
	CWT_ASSERT(printer);

	if( LOGGER_CONTEXT[type].prefix ) {
		CWT_FREE(LOGGER_CONTEXT[type].prefix);
		LOGGER_CONTEXT[type].prefix = NULL;
	}

	if( prefix ) {
		LOGGER_CONTEXT[type].prefix = cwtStrNS()->strdup(prefix);
	}

	if( !LOGGER_CONTEXT[type].sbuf ) {
		LOGGER_CONTEXT[type].sbuf = cwtStringNS()->create();
	}

	if( printer )
		LOGGER_CONTEXT[type].printer = printer;
}


static void _print_trace(const CWT_CHAR* msg)
{ cwtStdioNS()->fprintf(stdout, _T("%s\n"), msg); }

static void _print_debug(const CWT_CHAR* msg)
{ cwtStdioNS()->fprintf(stdout, _T("%s\n"), msg); }

static void _print_info(const CWT_CHAR* msg)
{ cwtStdioNS()->fprintf(stdout, _T("%s\n"), msg); }

static void _print_warn(const CWT_CHAR* msg)
{ cwtStdioNS()->fprintf(stderr, _T("%s\n"), msg); }

static void _print_error(const CWT_CHAR* msg)
{ cwtStdioNS()->fprintf(stderr, _T("%s\n"), msg); }


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

static void _print(LOGGER_TYPE type, const CWT_CHAR* msg)
{
	CwtStringNS *sbns = cwtStringNS();

	if( ! LOGGER_CONTEXT[type].printer )
		init_loggers();

	sbns->clear(LOGGER_CONTEXT[type].sbuf);
	if( LOGGER_CONTEXT[type].prefix )
		sbns->append(LOGGER_CONTEXT[type].sbuf, LOGGER_CONTEXT[type].prefix);
	if( msg )
		sbns->append(LOGGER_CONTEXT[type].sbuf, msg);
	/*strbuf_append(LOGGER_CONTEXT[type].sbuf, "\n");*/
	LOGGER_CONTEXT[type].printer(sbns->cstr(LOGGER_CONTEXT[type].sbuf));
}


void print_trace(const CWT_CHAR* msg) { _print(LOGGER_TRACE, msg); }
void print_debug(const CWT_CHAR* msg) { _print(LOGGER_DEBUG, msg); }
void print_info(const CWT_CHAR* msg)  { _print(LOGGER_INFO, msg); }
void print_warn(const CWT_CHAR* msg)  { _print(LOGGER_WARN, msg); }
void print_error(const CWT_CHAR* msg) { _print(LOGGER_ERROR, msg); }


#define __MSG_SZ 512
#define __MSG_MAX_SZ 1024
#define __MSG_INC_SZ 256
static void _printf(LOGGER_TYPE type, const CWT_CHAR* format, va_list args)
{
	int n;
	size_t sz = __MSG_SZ;
	CWT_CHAR msg[__MSG_SZ+1];
	CWT_CHAR *amsg = NULL;

	n = cwtStdioNS()->vsnprintf(msg, __MSG_SZ, format, args);

	while( n < 0 ) {
		sz += __MSG_INC_SZ;

		if( sz > __MSG_MAX_SZ ) {
			printf_error(_T("log string to0 big"));
			break;
		}

		if( amsg )
			CWT_FREE(amsg);

		amsg = CWT_MALLOCA(CWT_CHAR, sz+1);
		n = cwtStdioNS()->vsnprintf(amsg, sz, format, args);
	}

	if( sz <= __MSG_MAX_SZ) {
		if( amsg ) {
			_print(type, amsg);
			CWT_FREE(amsg);
		} else {
			_print(type, msg);
		}
	}
}

void printf_trace(const CWT_CHAR* format, ...)
{
	va_list args;
	va_start(args, format);
	_printf(LOGGER_TRACE, format, args);
	va_end(args);
}

void printf_debug(const CWT_CHAR* format, ...)
{
	va_list args;
	va_start(args, format);
	_printf(LOGGER_DEBUG, format, args);
	va_end(args);
}

void printf_info(const CWT_CHAR* format, ...)
{
	va_list args;
	va_start(args, format);
	_printf(LOGGER_INFO, format, args);
	va_end(args);
}

void printf_warn(const CWT_CHAR* format, ...)
{
	va_list args;
	va_start(args, format);
	_printf(LOGGER_WARN, format, args);
	va_end(args);
}

void printf_error(const CWT_CHAR* format, ...)
{
	va_list args;
	va_start(args, format);
	_printf(LOGGER_ERROR, format, args);
	va_end(args);
}
