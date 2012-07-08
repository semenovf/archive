/*
 * bytearr.c
 *
 *  Created on: 07.07.2012
 *      Author: wladt
 */


#include <cwt/bytearr.h>
#include <cwt/str.h>

static CwtByteArrayNS __cwtByetArrayNS;

static const char*    __cstr(CwtByteArray *ba);

CWT_BEGIN_DEF_VECTOR_NS(CwtByteArrayNS, CwtByteArray, BYTE)
CWT_END_DEF_VECTOR_NS(CwtByteArrayNS)


DLL_API_EXPORT CwtByteArrayNS* cwtByteArrayNS(void)
{
	return &__cwtByetArrayNS;
}

CWT_VECTOR_METHODS(CwtByteArray,BYTE)


static const char* __cstr(CwtByteArray *ba)
{
	__appendElem(ba, '\x0');
	ba->m_count--;
	return ba->m_buffer;
}
