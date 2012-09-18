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

static void __append   (CwtString *s, const CWT_CHAR *str);
static const CWT_CHAR* __cstr(CwtString *s);
static int  __sprintf  (CwtString *s, const CWT_CHAR *format, ...);
static int  __vsprintf (CwtString *s, const CWT_CHAR *format, va_list args);

/*static append_string_f __append_ptr = __append;*/

CWT_BEGIN_DEF_VECTOR_NS(CwtStringNS, CwtString, CWT_CHAR)
	, __appendElem
	, __appendElems
	, __prependElem
	, __insertElem
	, __removeElem
    , __append
	, __cstr
	, __sprintf
	, __vsprintf
CWT_END_DEF_VECTOR_NS(CwtStringNS)


DLL_API_EXPORT CwtStringNS* cwtStringNS(void)
{
	return &__CwtStringNS;
}

CWT_VECTOR_METHODS(CwtString,CWT_CHAR)


static void __append(CwtString *s, const CWT_CHAR *str)
{
	__appendElems(s, str, cwtStrNS()->strLen(str));
}


static const CWT_CHAR* __cstr(CwtString *sb)
{
	__appendElem(sb, _T('\x0'));
	sb->m_count--;
	return sb->m_buffer;
}


static int  __sprintf(CwtString *s, const CWT_CHAR *format, ...)
{
	int n;
	va_list args;
	va_start(args, format);

	n = __vsprintf(s, format, args);

	va_end(args);
	return n;
}


static int  __vsprintf (CwtString *s, const CWT_CHAR *format, va_list args)
{
	CwtStrNS   *strNS   = cwtStrNS();
	CwtStdioNS *stdioNS = cwtStdioNS();
	size_t sz;
	int n;

	CWT_ASSERT(s);

	sz = strNS->strLen(format);

	do {
		CWT_ASSERT(sz < CWT_SIZE_T_MAX/2 );

		sz *= 2;

		__clear(s);
		__resize(s, sz+1);
		n = stdioNS->vsnprintf(s->m_buffer, sz, format, args);
	} while( n < 0 || (size_t)n == sz );

	__resize(s, (size_t)n);

	return (int)__size(s);
}
