/*
 * bytearr.c
 *
 *  Created on: 07.07.2012
 *      Author: wladt
 */


#include <cwt/bytearr.h>
#include <cwt/str.h>
#include <string.h>

static const char* __cstr(CwtByteArray *ba);
static void        __ltrim(CwtByteArray *ba);
static void        __rtrim(CwtByteArray *ba);
static void        __trim(CwtByteArray *ba);

CWT_BEGIN_DEF_VECTOR_NS(CwtByteArrayNS, CwtByteArray, BYTE)
	, __cstr
	, __ltrim
	, __rtrim
	, __trim
CWT_END_DEF_VECTOR_NS(CwtByteArrayNS)


DLL_API_EXPORT CwtByteArrayNS* cwtByteArrayNS(void)
{
	return &__CwtByteArrayNS;
}

CWT_VECTOR_METHODS(CwtByteArray,BYTE)


static const char* __cstr(CwtByteArray *ba)
{
	CWT_ASSERT(ba);

	__appendElem(ba, '\x0');
	ba->m_count--;
	return (const char*)ba->m_buffer;
}

static void __ltrim(CwtByteArray *ba)
{
	size_t i;

	CWT_ASSERT(ba);

	i = 0;

	while( i < ba->m_count
			&& ( ba->m_buffer[i] == ' '
				|| ba->m_buffer[i] == '\t'
				|| ba->m_buffer[i] == '\n'
				|| ba->m_buffer[i] == '\v'
				|| ba->m_buffer[i] == '\f'
				|| ba->m_buffer[i] == '\r' ) ) {
		i++;
	}

	if( i == ba->m_count) {
		__resize(ba, 0);
	} else {
		memmove( ba->m_buffer, ba->m_buffer + i, ba->m_count - i);
		__resize(ba, ba->m_count - i);
	}
}

static void __rtrim(CwtByteArray *ba)
{
	ssize_t i;

	CWT_ASSERT(ba);

	i = (ssize_t)ba->m_count;

	while( --i >= 0
			&& ( ba->m_buffer[i] == ' '
				|| ba->m_buffer[i] == '\t'
				|| ba->m_buffer[i] == '\n'
				|| ba->m_buffer[i] == '\v'
				|| ba->m_buffer[i] == '\f'
				|| ba->m_buffer[i] == '\r' )) {
		;
	}

	__resize(ba, i + 1);
}

static void __trim(CwtByteArray *ba)
{
	CWT_ASSERT(ba);
	__ltrim(ba);
	__rtrim(ba);
}
