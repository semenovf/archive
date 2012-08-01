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

static void __append  (CwtString *s, const CWT_CHAR *str);
static const CWT_CHAR* __cstr(CwtString *s);
static int  __sprintf (CwtString *s, const CWT_CHAR *format, ...);

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
CWT_END_DEF_VECTOR_NS(CwtStringNS)


DLL_API_EXPORT CwtStringNS* cwtStringNS(void)
{
	return &__CwtStringNS;
}

CWT_VECTOR_METHODS(CwtString,CWT_CHAR)


static void __append(CwtString *s, const CWT_CHAR *str)
{
	__appendElems(s, str, cwtStrNS()->strlen(str));
}


static const CWT_CHAR* __cstr(CwtString *sb)
{
	__appendElem(sb, _T('\x0'));
	sb->m_count--;
	return sb->m_buffer;
}


static int  __sprintf(CwtString *s, const CWT_CHAR *format, ...)
{
	CwtStrNS   *strNS   = cwtStrNS();
	CwtStdioNS *stdioNS = cwtStdioNS();
	va_list args;
	size_t sz;
	int n;

	CWT_ASSERT(s);
	va_start(args, format);

	sz = strNS->strlen(format);

	do {
		CWT_ASSERT(sz < CWT_SIZE_T_MAX/2 );

		sz *= 2;

		__clear(s);
		__resize(s, sz+1);
		n = stdioNS->vsnprintf(s->m_buffer, sz, format, args);
	} while( n < 0 || n == sz );

	va_end(args);

	__resize(s, (size_t)n);

	return __size(s);
}
