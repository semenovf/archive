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

struct StringBuffer
{
	CHAR*    m_buffer;   /* Динамический массив элементов буфера */
/*	size_t   m_head;*/
    size_t   m_count;    /* Количество элементов, доступных для чтения */
    size_t   m_capacity;
    size_t   m_max_capacity;
};

typedef struct StringBuffer  StringBuffer;
typedef struct StringBuffer  CwtStringBuffer;
typedef struct StringBuffer* StringBufferPtr;
typedef struct StringBuffer* CwtStringBufferPtr;

EXTERN_C_BEGIN

DLL_API_EXPORT StringBufferPtr strbuf_new_defaults(void);
DLL_API_EXPORT StringBufferPtr strbuf_new(size_t initial_size, size_t max_size);
DLL_API_EXPORT void            strbuf_delete(StringBufferPtr sb);
DLL_API_EXPORT StringBufferPtr strbuf_clone(StringBufferPtr sb);
DLL_API_EXPORT BOOL            strbuf_reserve(StringBufferPtr sb, size_t n);
DLL_API_EXPORT BOOL            strbuf_lreserve(StringBufferPtr sb, size_t n);
DLL_API_EXPORT BOOL            strbuf_mreserve(StringBufferPtr sb, size_t n, size_t pos);
DLL_API_EXPORT size_t          strbuf_capacity(StringBufferPtr sb);
DLL_API_EXPORT BOOL            strbuf_is_empty(StringBufferPtr sb);
DLL_API_EXPORT void            strbuf_clear(StringBufferPtr sb);
DLL_API_EXPORT size_t          strbuf_size(StringBufferPtr sb);
#define strbuf_length(sb)      strbuf_size(sb)
DLL_API_EXPORT void            strbuf_append_char(StringBufferPtr sb, CHAR ch);
DLL_API_EXPORT void            strbuf_append_chars(StringBufferPtr sb, const CHAR *str, size_t n);
DLL_API_EXPORT void            strbuf_append(StringBufferPtr sb, const CHAR *str);
DLL_API_EXPORT void            strbuf_prepend_char(StringBufferPtr sb, CHAR ch);
DLL_API_EXPORT void            strbuf_insert_char(StringBufferPtr sb, CHAR ch, size_t pos);
DLL_API_EXPORT void            strbuf_insert(StringBufferPtr sb, const CHAR *chars, size_t nchars, size_t pos);
DLL_API_EXPORT void            strbuf_remove_char(StringBufferPtr sb, size_t pos);
/*DLL_API_EXPORT void            strbuf_remove(StringBufferPtr sb, size_t nchars, size_t pos);*/
DLL_API_EXPORT const CHAR*     strbuf_cstr(StringBufferPtr sb);
DLL_API_EXPORT CHAR*           strbuf_substr(StringBufferPtr sb, size_t start, size_t nchars);
DLL_API_EXPORT BOOL 			strbuf_find(StringBufferPtr sb, CHAR ch, size_t *offset);

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
/*#define cwtStringBuffer   strbuf_remove(StringBufferPtr sb, size_t nchars, size_t pos);*/
#define cwtStringBufferCstr        strbuf_cstr
#define cwtStringBufferSubstr      strbuf_substr
#define cwtStringBufferFind        strbuf_find

EXTERN_C_END


#endif /* __CWT_STRBUF_H__ */

