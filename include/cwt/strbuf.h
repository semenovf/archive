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
	CHAR*    m_buffer;   /* Динамический массив элементов буфера */
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
	void               (*appendChar)  (CwtStringBufferPtr sb, CHAR ch);
	void               (*appendChars) (CwtStringBufferPtr sb, const CHAR *str, size_t n);
	void               (*append)      (CwtStringBufferPtr sb, const CHAR *str);
	void               (*prependChar) (CwtStringBufferPtr sb, CHAR ch);
	void               (*insertChar)  (CwtStringBufferPtr sb, CHAR ch, size_t pos);
	void               (*insert)      (CwtStringBufferPtr sb, const CHAR *chars, size_t nchars, size_t pos);
	void               (*removeChar)  (CwtStringBufferPtr sb, size_t pos);
	const CHAR*        (*cstr)        (CwtStringBufferPtr sb);
	CHAR*              (*substr)      (CwtStringBufferPtr sb, size_t start, size_t nchars);
	BOOL               (*find)        (CwtStringBufferPtr sb, CHAR ch, size_t *offset);
} CwtStringBufferNS;

EXTERN_C_BEGIN

DLL_API_EXPORT CwtStringBufferNS* cwtStringBufferNS(void);

#ifdef __COMMENT__
DLL_API_EXPORT CwtStringBufferPtr strbuf_new_defaults(void);
DLL_API_EXPORT CwtStringBufferPtr strbuf_new(size_t initial_size, size_t max_size);
DLL_API_EXPORT void            strbuf_delete(CwtStringBufferPtr sb);
DLL_API_EXPORT CwtStringBufferPtr strbuf_clone(CwtStringBufferPtr sb);
DLL_API_EXPORT BOOL            strbuf_reserve(CwtStringBufferPtr sb, size_t n);
DLL_API_EXPORT BOOL            strbuf_lreserve(CwtStringBufferPtr sb, size_t n);
DLL_API_EXPORT BOOL            strbuf_mreserve(CwtStringBufferPtr sb, size_t n, size_t pos);
DLL_API_EXPORT size_t          strbuf_capacity(CwtStringBufferPtr sb);
DLL_API_EXPORT BOOL            strbuf_is_empty(CwtStringBufferPtr sb);
DLL_API_EXPORT void            strbuf_clear(CwtStringBufferPtr sb);
DLL_API_EXPORT size_t          strbuf_size(CwtStringBufferPtr sb);
#define strbuf_length(sb)      strbuf_size(sb)
DLL_API_EXPORT void            strbuf_append_char(CwtStringBufferPtr sb, CHAR ch);
DLL_API_EXPORT void            strbuf_append_chars(CwtStringBufferPtr sb, const CHAR *str, size_t n);
DLL_API_EXPORT void            strbuf_append(CwtStringBufferPtr sb, const CHAR *str);
DLL_API_EXPORT void            strbuf_prepend_char(CwtStringBufferPtr sb, CHAR ch);
DLL_API_EXPORT void            strbuf_insert_char(CwtStringBufferPtr sb, CHAR ch, size_t pos);
DLL_API_EXPORT void            strbuf_insert(CwtStringBufferPtr sb, const CHAR *chars, size_t nchars, size_t pos);
DLL_API_EXPORT void            strbuf_remove_char(CwtStringBufferPtr sb, size_t pos);
/*DLL_API_EXPORT void            strbuf_remove(CwtStringBufferPtr sb, size_t nchars, size_t pos);*/
DLL_API_EXPORT const CHAR*     strbuf_cstr(CwtStringBufferPtr sb);
DLL_API_EXPORT CHAR*           strbuf_substr(CwtStringBufferPtr sb, size_t start, size_t nchars);
DLL_API_EXPORT BOOL 			strbuf_find(CwtStringBufferPtr sb, CHAR ch, size_t *offset);
#endif

#ifdef __COMMENT__
#define cwtNewStringBuffer         strbuf_new_defaults
#define cwtNewStringBufferSize     strbuf_new
#define cwtDeleteStringBuffer      strbuf_delete
#define cwtStringBufferClone       strbuf_clone
#define cwtStringBufferReserve     strbuf_reserve
#define cwtStringBufferLreserve    strbuf_lreserve
#define cwtStringBufferMreserve    strbuf_mreserve
#define cwtStringBufferCapacity    strbuf_capacity
#define cwtStringBufferIsEmpty     strbuf_is_empty
#define cwtStringBufferClear       strbuf_clear
#define cwtStringBufferSize        strbuf_size
#define cwtStringBufferLength      strbuf_length
#define cwtStringBufferAppendChar  strbuf_append_char
#define cwtStringBufferAppendChars strbuf_append_chars
#define cwtStringBufferAppend      strbuf_append
#define cwtStringBufferPrependChar strbuf_prepend_char
#define cwtStringBufferInsertChar  strbuf_insert_char
#define cwtStringBufferInsert      strbuf_insert
#define cwtStringBufferRemoveChar  strbuf_remove_char
/*#define cwtStringBuffer   strbuf_remove(CwtStringBufferPtr sb, size_t nchars, size_t pos);*/
#define cwtStringBufferCstr        strbuf_cstr
#define cwtStringBufferSubstr      strbuf_substr
#define cwtStringBufferFind        strbuf_find
#endif

EXTERN_C_END


#endif /* __CWT_STRBUF_H__ */

