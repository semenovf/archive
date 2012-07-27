/*
 * strbuf.c
 *
 *  Created on: 13.01.2012
 *      Author: wladt
 */


#include <stdlib.h>
#include <cwt/string.h>
#include <cwt/str.h>

static CwtStringNS __CwtStringNS;

static void __append(CwtString *s, const CWT_CHAR *str);
static const CWT_CHAR* __cstr(CwtString *s);

/*static append_string_f __append_ptr = __append;*/

CWT_BEGIN_DEF_VECTOR_NS(CwtStringNS, CwtString, CWT_CHAR)
	, __appendElem
	, __appendElems
	, __prependElem
	, __insertElem
	, __removeElem
    , __append
	, __cstr
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

