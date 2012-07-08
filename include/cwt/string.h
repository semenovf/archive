/*
 * string.h
 *
 *  Created on: 13.01.2012
 *      Author: wladt
 */

#ifndef __CWT_STRING_H__
#define __CWT_STRING_H__

#include <cwt/global.h>
#include <cwt/types.h>
#include <cwt/vector.h>

CWT_BEGIN_DECL_VECTOR_NS(CwtStringNS, CwtString, CWT_CHAR)
void               (*appendChar)  (CwtString* p, CWT_CHAR ch);
void               (*appendChars) (CwtString* p, const CWT_CHAR *elems, size_t n);
void               (*prependChar) (CwtString* p, CWT_CHAR ch);
void               (*insertChar)  (CwtString* p, CWT_CHAR ch, size_t pos);
void               (*removeChar)  (CwtString* p, size_t pos);
void               (*append)      (CwtString *s, const CWT_CHAR *str);
const CWT_CHAR*    (*cstr)        (CwtString *s);
CWT_END_DECL_VECTOR_NS(CwtStringNS)

EXTERN_C_BEGIN
DLL_API_EXPORT CwtStringNS* cwtStringNS(void);
EXTERN_C_END


#ifdef __COMMENT__
typedef struct CwtString
{
	CWT_CHAR* m_buffer;
    size_t    m_count;
    size_t    m_capacity;
    size_t    m_max_capacity;
} CwtString, *CwtStringBufferPtr;

typedef struct CwtStringNS {
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
	const CWT_CHAR*    (*cstr)        (CwtStringBufferPtr sb);
	CWT_CHAR*          (*substr)      (CwtStringBufferPtr sb, size_t start, size_t nchars);
	BOOL               (*find)        (CwtStringBufferPtr sb, CWT_CHAR ch, size_t *offset);
} CwtStringNS;
#endif

#endif /* __CWT_STRING_H__ */

