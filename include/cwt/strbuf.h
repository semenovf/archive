/*
 * strbuf.h
 *
 *  Created on: 13.01.2012
 *      Author: wladt
 */

#ifndef __CWT_STRBUF_H__
#define __CWT_STRBUF_H__

#include <cwt/global.h>
#include <cwt/types.h>

typedef struct CwtStringBuffer
{
	CWT_CHAR*    m_buffer;   /* Динамический массив элементов буфера */
/*	size_t   m_head;*/
    size_t   m_count;    /* Количество элементов, доступных для чтения */
    size_t   m_capacity;
    size_t   m_max_capacity;
} CwtStringBuffer, *CwtStringBufferPtr;

typedef struct CwtStringBufferNS {
	size_t             refs;
	CwtStringBufferPtr (*create)      (void);
	CwtStringBufferPtr (*createSized) (size_t initial_size, size_t max_size);
	void               (*init)        (CwtStringBufferPtr sb);
	void               (*destroy)     (CwtStringBufferPtr sb);
	void               (*free)        (CwtStringBufferPtr sb);
	CwtStringBufferPtr (*clone)       (CwtStringBufferPtr sb);
	BOOL               (*reserve)     (CwtStringBufferPtr sb, size_t n);
	BOOL               (*lreserve)    (CwtStringBufferPtr sb, size_t n);
	BOOL               (*mreserve)    (CwtStringBufferPtr sb, size_t n, size_t pos);
	size_t             (*capacity)    (CwtStringBufferPtr sb);
	BOOL               (*isEmpty)     (CwtStringBufferPtr sb);
	void               (*clear)       (CwtStringBufferPtr sb);
	size_t             (*size)        (CwtStringBufferPtr sb);
	size_t             (*length)      (CwtStringBufferPtr sb);
	void               (*appendChar)  (CwtStringBufferPtr sb, CWT_CHAR ch);
	void               (*appendChars) (CwtStringBufferPtr sb, const CWT_CHAR *str, size_t n);
	void               (*append)      (CwtStringBufferPtr sb, const CWT_CHAR *str);
	void               (*prependChar) (CwtStringBufferPtr sb, CWT_CHAR ch);
	void               (*insertChar)  (CwtStringBufferPtr sb, CWT_CHAR ch, size_t pos);
	void               (*insert)      (CwtStringBufferPtr sb, const CWT_CHAR *chars, size_t nchars, size_t pos);
	void               (*removeChar)  (CwtStringBufferPtr sb, size_t pos);
	const CWT_CHAR*        (*cstr)        (CwtStringBufferPtr sb);
	CWT_CHAR*              (*substr)      (CwtStringBufferPtr sb, size_t start, size_t nchars);
	BOOL               (*find)        (CwtStringBufferPtr sb, CWT_CHAR ch, size_t *offset);
} CwtStringBufferNS;

EXTERN_C_BEGIN

DLL_API_EXPORT CwtStringBufferNS* cwtStringBufferNS(void);

EXTERN_C_END


#endif /* __CWT_STRBUF_H__ */

