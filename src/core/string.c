/*
 * strbuf.c
 *
 *  Created on: 13.01.2012
 *      Author: wladt
 */


#include <stdlib.h>
#include <cwt/string.h>
#include <cwt/str.h>
#include <cwt/stdio.h>

static CwtStringNS __CwtStringNS;

static void string_append   (CwtString *s, const CWT_CHAR *str);
static const CWT_CHAR* string_cstr(CwtString *s);
static int  string_sprintf  (CwtString *s, const CWT_CHAR *format, ...);
static int  string_vsprintf (CwtString *s, const CWT_CHAR *format, va_list args);

CWT_BEGIN_DEF_VECTOR_NS(CwtStringNS, CwtString, CWT_CHAR)
	, __appendElem
	, __appendElems
	, __prependElem
	, __insertElem
	, __removeElem
    , string_append
	, string_cstr
	, string_sprintf
	, string_vsprintf
CWT_END_DEF_VECTOR_NS(CwtStringNS)


DLL_API_EXPORT CwtStringNS* cwt_string_ns(void)
{
	return &__CwtStringNS;
}

CWT_VECTOR_METHODS(CwtString,CWT_CHAR)


static void string_append(CwtString *s, const CWT_CHAR *str)
{
	__appendElems(s, str, cwt_str_ns()->strLen(str));
}


static const CWT_CHAR* string_cstr(CwtString *sb)
{
	__appendElem(sb, _T('\x0'));
	sb->m_count--;
	return sb->m_buffer;
}


static int  string_sprintf(CwtString *s, const CWT_CHAR *format, ...)
{
	int n;
	va_list args;
	va_start(args, format);

	n = string_vsprintf(s, format, args);

	va_end(args);
	return n;
}


static int  string_vsprintf (CwtString *s, const CWT_CHAR *format, va_list args)
{
	CwtStrNS   *str_ns   = cwt_str_ns();
	CwtStdioNS *stdio_ns = cwt_stdio_ns();
	size_t sz;
	int n;

	CWT_ASSERT(s);

	sz = str_ns->strLen(format);

	do {
		CWT_ASSERT(sz < CWT_SIZE_T_MAX/2 );

		sz *= 2;

		__clear(s);
		__resize(s, sz+1);
		n = stdio_ns->vsnprintf(s->m_buffer, sz, format, args);
	} while( n < 0 || (size_t)n == sz );

	__resize(s, (size_t)n);

	return (int)__size(s);
}
