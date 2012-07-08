/*
 * vector.h
 *
 *  Created on: 07.07.2012
 *      Author: wladt
 */

#ifndef __CWT_VECTOR_H__
#define __CWT_VECTOR_H__

#include <cwt/types.h>

#define CWT_BEGIN_DECL_VECTOR_NS(_NS,_CollectionT,_ElemT)                     \
typedef struct _CollectionT                                                   \
{                                                                             \
	_ElemT*   m_buffer;                                                       \
    size_t    m_count;                                                        \
    size_t    m_capacity;                                                     \
    size_t    m_max_capacity;                                                 \
} _CollectionT, *_CollectionT##Ptr;                                           \
                                                                              \
typedef struct _NS {                                                          \
	_CollectionT* (*create)      (void);                                      \
	_CollectionT* (*createSized) (size_t initial_size, size_t max_size);      \
	void          (*init)        (_CollectionT* p);                           \
	void          (*destroy)     (_CollectionT* p);                           \
	void          (*free)        (_CollectionT* p);                           \
	_CollectionT* (*clone)       (_CollectionT* p);                           \
	BOOL          (*reserve)     (_CollectionT* p, size_t n);                 \
	BOOL          (*lreserve)    (_CollectionT* p, size_t n);                 \
	BOOL          (*mreserve)    (_CollectionT* p, size_t n, size_t pos);     \
	BOOL          (*resize)      (_CollectionT* p, size_t n);                 \
	size_t        (*capacity)    (_CollectionT* p);                           \
	BOOL          (*isEmpty)     (_CollectionT* p);                           \
	void          (*clear)       (_CollectionT* p);                           \
	size_t        (*size)        (_CollectionT* p);                           \
	size_t        (*length)      (_CollectionT* p);                           \
	void          (*appendElem)  (_CollectionT* p, _ElemT ch);                \
	void          (*appendElems) (_CollectionT* p, const _ElemT *elems, size_t n); \
	void          (*prependElem) (_CollectionT* p, _ElemT ch);                \
	void          (*insertElem)  (_CollectionT* p, _ElemT ch, size_t pos);    \
	void          (*insert)      (_CollectionT* p, const _ElemT *elems, size_t nelems, size_t pos); \
	void          (*removeElem)  (_CollectionT* p, size_t pos);               \
	_ElemT*       (*substr)      (_CollectionT* p, size_t start, size_t nelems); \
	BOOL          (*find)        (_CollectionT* p, _ElemT ch, size_t *offset);

#define CWT_END_DECL_VECTOR_NS(_NS) } _NS;

#define CWT_BEGIN_DEF_VECTOR_NS(_NS,_CollectionT,_ElemT)                      \
	static const size_t __default_initial_size = 64;                          \
	static const size_t __default_max_size = CWT_SIZE_T_MAX;                  \
                                                                              \
	static _CollectionT* __create      (void);                                \
	static _CollectionT* __createSized (size_t initial_size, size_t max_size);\
	static void          __init        (_CollectionT* p);                     \
	static void          __initSized   (_CollectionT* p, size_t initial_size, size_t max_size); \
	static void          __destroy     (_CollectionT* p);                     \
	static void          __free        (_CollectionT* p);                     \
	static _CollectionT* __clone       (_CollectionT* p);                     \
	static BOOL          __reserve     (_CollectionT* p, size_t n);           \
	static BOOL          __lreserve    (_CollectionT* p, size_t n);           \
	static BOOL          __mreserve    (_CollectionT* p, size_t n, size_t pos);\
	static BOOL          __resize      (_CollectionT* p, size_t n);           \
	static size_t        __capacity    (_CollectionT* p);                     \
	static BOOL          __isEmpty     (_CollectionT* p);                     \
	static void          __clear       (_CollectionT* p);                     \
	static size_t        __size        (_CollectionT* p);                     \
	static size_t        __length      (_CollectionT* p);                     \
	static void          __appendElem  (_CollectionT* p, _ElemT ch);          \
	static void          __appendElems (_CollectionT* p, const _ElemT *elems, size_t n); \
	static void          __prependElem (_CollectionT* p, _ElemT ch);          \
	static void          __insertElem  (_CollectionT* p, _ElemT ch, size_t pos);\
	static void          __insert      (_CollectionT* p, const _ElemT *elems, size_t nelems, size_t pos); \
	static void          __removeElem  (_CollectionT* p, size_t pos);          \
	static _ElemT*       __substr      (_CollectionT* p, size_t start, size_t nelems); \
	static BOOL          __find        (_CollectionT* p, _ElemT ch, size_t *offset); \
                                                                              \
static _NS  __##_NS = {                                                       \
	  __create                                                                \
	, __createSized                                                           \
	, __init                                                                  \
	, __destroy                                                               \
	, __free                                                                  \
	, __clone                                                                 \
	, __reserve                                                               \
	, __lreserve                                                              \
	, __mreserve                                                              \
	, __resize                                                                \
	, __capacity                                                              \
	, __isEmpty                                                               \
	, __clear                                                                 \
	, __size                                                                  \
	, __size                                                                  \
	, __appendElem                                                            \
	, __appendElems                                                           \
	, __prependElem                                                           \
	, __insertElem                                                            \
	, __insert                                                                \
	, __removeElem                                                            \
	, __substr                                                                \
	, __find

#define CWT_END_DEF_VECTOR_NS(_NS) };


#define CWT_VECTOR_METHODS(_CollectionT,_ElemT)                               \
_CollectionT*  __create(void)                                                 \
{                                                                             \
	return __createSized((size_t)0, (size_t)0);                               \
}                                                                             \
                                                                              \
static _CollectionT* __createSized(size_t initial_size, size_t max_size)      \
{                                                                             \
	_CollectionT* sb;                                                         \
                                                                              \
	sb = CWT_MALLOC(_CollectionT);                                            \
	CWT_ASSERT(sb);                                                           \
	__initSized(sb, initial_size, max_size);                                  \
	return sb;                                                                \
}                                                                             \
                                                                              \
static void __initSized(_CollectionT* sb, size_t initial_size, size_t max_size)\
{                                                                             \
	if( !initial_size ) {                                                     \
		initial_size = __default_initial_size;                                \
	}                                                                         \
                                                                              \
	if( !max_size ) {                                                         \
		max_size = __default_max_size;                                        \
	}                                                                         \
                                                                              \
	if( initial_size > max_size ) {                                           \
		initial_size = max_size;                                              \
	}                                                                         \
                                                                              \
	sb->m_buffer = CWT_MALLOCA(_ElemT, initial_size);                         \
	CWT_ASSERT(sb->m_buffer);                                                 \
                                                                              \
	sb->m_capacity = initial_size;                                            \
	sb->m_count = 0;                                                          \
	sb->m_max_capacity = max_size;                                            \
}                                                                             \
                                                                              \
static void __init(_CollectionT* sb)                                          \
{                                                                             \
	__initSized(sb, 0, 0);                                                    \
}                                                                             \
                                                                              \
static void __destroy(_CollectionT* sb)                                       \
{                                                                             \
	if( sb && sb->m_buffer ) {                                                \
		CWT_FREE(sb->m_buffer);                                               \
		sb->m_buffer = NULL;                                                  \
	}                                                                         \
}                                                                             \
                                                                              \
static void __free(_CollectionT* sb)                                          \
{                                                                             \
	__destroy(sb);                                                            \
	CWT_FREE(sb);                                                             \
}                                                                             \
                                                                              \
static _CollectionT* __clone(_CollectionT* sb)                                \
{                                                                             \
	if( sb ) {                                                                \
		_CollectionT* clone = __createSized(sb->m_count, __default_max_size); \
		if( sb->m_count > 0 ) {                                               \
			cwtStrNS()->memcpy(clone->m_buffer                                \
				, sb->m_buffer, sb->m_count * sizeof(_ElemT));                \
			clone->m_count = sb->m_count;                                     \
		}                                                                     \
		return clone;                                                         \
	}                                                                         \
	return NULL;                                                              \
}                                                                             \
                                                                              \
static BOOL __reserve(_CollectionT* sb, size_t n)                             \
{                                                                             \
	size_t available;                                                         \
	size_t inc;                                                               \
                                                                              \
	CWT_ASSERT(sb);                                                           \
                                                                              \
	available = sb->m_capacity - sb->m_count;                                 \
	inc = n - available;                                                      \
                                                                              \
	if( available < n ) {                                                     \
		_ElemT *buffer;                                                       \
		size_t capacity;                                                      \
                                                                              \
		capacity = sb->m_capacity + inc;                                      \
                                                                              \
		if( capacity > sb->m_max_capacity )                                   \
			return FALSE;                                                     \
                                                                              \
		buffer = CWT_MALLOCA(_ElemT, capacity);                               \
                                                                              \
		cwtStrNS()->memcpy(buffer, sb->m_buffer, sb->m_count * sizeof(_ElemT)); \
                                                                              \
		CWT_FREE(sb->m_buffer);                                               \
		sb->m_buffer = buffer;                                                \
		sb->m_capacity = capacity;                                            \
	}                                                                         \
                                                                              \
	return TRUE;                                                              \
}                                                                             \
                                                                              \
                                                                              \
static BOOL __lreserve(_CollectionT* sb, size_t n)                            \
{                                                                             \
	size_t inc;                                                               \
	_ElemT *buffer;                                                           \
	size_t capacity;                                                          \
                                                                              \
	CWT_ASSERT(sb);                                                           \
                                                                              \
	inc = n;                                                                  \
                                                                              \
	capacity = sb->m_capacity + inc;                                          \
                                                                              \
	if( capacity > sb->m_max_capacity )                                       \
		return FALSE;                                                         \
                                                                              \
	buffer = CWT_MALLOCA(_ElemT, capacity);                                   \
                                                                              \
	cwtStrNS()->memcpy(buffer + n, sb->m_buffer                               \
		, sb->m_count * sizeof(_ElemT));                                      \
                                                                              \
	CWT_FREE(sb->m_buffer);                                                   \
	sb->m_buffer = buffer;                                                    \
	sb->m_capacity = capacity;                                                \
                                                                              \
	return TRUE;                                                              \
}                                                                             \
                                                                              \
static BOOL __mreserve(_CollectionT* sb, size_t n, size_t pos)                \
{                                                                             \
	size_t inc;                                                               \
	_ElemT *buffer;                                                           \
	size_t capacity;                                                          \
	CwtStrNS *strNS = cwtStrNS();                                             \
                                                                              \
	CWT_ASSERT(sb);                                                           \
                                                                              \
	if( pos >= sb->m_count )                                                  \
		return __reserve(sb, n);                                              \
                                                                              \
	if( pos == 0 )                                                            \
		return __lreserve(sb, n);                                             \
                                                                              \
	inc = n;                                                                  \
                                                                              \
	capacity = sb->m_capacity + inc;                                          \
                                                                              \
	if( capacity > sb->m_max_capacity )                                       \
		return FALSE;                                                         \
                                                                              \
	buffer = CWT_MALLOCA(_ElemT, capacity);                                   \
                                                                              \
	strNS->memcpy(buffer, sb->m_buffer, pos * sizeof(_ElemT));                \
	strNS->memcpy(buffer+pos+n, sb->m_buffer + pos                            \
		, (sb->m_count - pos) * sizeof(_ElemT));                              \
                                                                              \
	CWT_FREE(sb->m_buffer);                                                   \
	sb->m_buffer = buffer;                                                    \
	sb->m_capacity = capacity;                                                \
                                                                              \
	return TRUE;                                                              \
}                                                                             \
                                                                              \
static BOOL __resize(_CollectionT* sb, size_t sz)                             \
{                                                                             \
	CWT_ASSERT(sb);                                                           \
	if( __reserve(sb, sz) )                                                   \
		return FALSE;                                                         \
	sb->m_count = sz;                                                         \
	return TRUE;                                                              \
}                                                                             \
                                                                              \
static size_t __capacity(_CollectionT* sb)                                    \
{                                                                             \
	CWT_ASSERT(sb);                                                           \
	return sb->m_capacity;                                                    \
}                                                                             \
                                                                              \
static BOOL __isEmpty(_CollectionT* sb)                                       \
{                                                                             \
	CWT_ASSERT(sb);                                                           \
	return (sb->m_count == 0 ? TRUE : FALSE);                                 \
}                                                                             \
                                                                              \
static void __clear(_CollectionT* sb)                                         \
{                                                                             \
	CWT_ASSERT(sb);                                                           \
	sb->m_count = 0;                                                          \
}                                                                             \
                                                                              \
static size_t __size(_CollectionT* sb)                                        \
{                                                                             \
	CWT_ASSERT(sb);                                                           \
	return sb->m_count;                                                       \
}                                                                             \
                                                                              \
static void __appendElem(_CollectionT* sb, _ElemT ch)                         \
{                                                                             \
	CWT_ASSERT(sb);                                                           \
	CWT_ASSERT(__reserve(sb, 1));                                             \
                                                                              \
	sb->m_buffer[sb->m_count] = ch;                                           \
	sb->m_count++;                                                            \
}                                                                             \
                                                                              \
static void __appendElems(_CollectionT* sb, const _ElemT *s, size_t n)        \
{                                                                             \
	CWT_ASSERT(__reserve(sb, n));                                             \
                                                                              \
	cwtStrNS()->memcpy(sb->m_buffer + sb->m_count, s, n * sizeof(_ElemT)); \
	sb->m_count += n;                                                         \
}                                                                             \
                                                                              \
                                                                              \
static void __prependElem(_CollectionT* sb, _ElemT ch)                        \
{                                                                             \
	CWT_ASSERT(sb);                                                           \
                                                                              \
	CWT_ASSERT(__lreserve(sb, 1));                                            \
	sb->m_buffer[0] = ch;                                                     \
	sb->m_count++;                                                            \
}                                                                             \
                                                                              \
static void __insertElem(_CollectionT* sb, _ElemT ch, size_t pos)             \
{                                                                             \
	CWT_ASSERT(sb);                                                           \
                                                                              \
	CWT_ASSERT(__mreserve(sb, 1, pos));                                       \
	sb->m_buffer[pos] = ch;                                                   \
	sb->m_count++;                                                            \
}                                                                             \
                                                                              \
static void __insert(_CollectionT* sb, const _ElemT *chars, size_t nelems, size_t pos)\
{                                                                             \
	size_t i;                                                                 \
	CWT_ASSERT(sb);                                                           \
                                                                              \
	CWT_ASSERT(__mreserve(sb, nelems, pos));                                  \
                                                                              \
	/* TODO replace this code by memcpy function call */                      \
	for( i = 0; i < nelems; i++ )                                             \
		sb->m_buffer[pos + i] = chars[i];                                     \
	sb->m_count += nelems;                                                    \
}                                                                             \
                                                                              \
static void __removeElem(_CollectionT* sb, size_t pos)                        \
{                                                                             \
	CWT_ASSERT(sb);                                                           \
                                                                              \
	if( !sb->m_count ) /* empty buffer */                                     \
		return;                                                               \
                                                                              \
	if( pos < sb->m_count ) {                                                 \
		sb->m_count--;                                                        \
		cwtStrNS()->memmove(&sb->m_buffer[pos]                                \
		    , &sb->m_buffer[pos + 1], sb->m_count * sizeof(_ElemT) );         \
	}                                                                         \
}                                                                             \
                                                                              \
static _ElemT* __substr(_CollectionT* sb, size_t start, size_t nelems)        \
{                                                                             \
	_ElemT *elems = NULL;                                                     \
                                                                              \
	if( start < sb->m_count ) {                                               \
		nelems = CWT_MIN(nelems, sb->m_count - start);                        \
		elems = CWT_MALLOCA(_ElemT, nelems+1);                                \
		cwtStrNS()->memcpy(elems, sb->m_buffer + start                        \
			, nelems * sizeof(_ElemT));                                       \
	    elems[nelems] = '\x0';                                                \
	}                                                                         \
                                                                              \
	return elems;                                                             \
}                                                                             \
                                                                              \
static BOOL __find(_CollectionT* sb, _ElemT ch, size_t *offset)               \
{                                                                             \
	_ElemT *ptr;                                                              \
	size_t off;                                                               \
                                                                              \
	CWT_ASSERT(sb);                                                           \
	CWT_ASSERT(offset);                                                       \
                                                                              \
	if( *offset >= sb->m_count )                                              \
		return FALSE;                                                         \
                                                                              \
	off = *offset;                                                            \
	ptr = sb->m_buffer + off;                                                 \
                                                                              \
	while( off < sb->m_count ) {                                              \
		if( *ptr == ch ) {                                                    \
			*offset = off;                                                    \
			return TRUE;                                                      \
		}                                                                     \
		ptr++;                                                                \
		off++;                                                                \
	}                                                                         \
                                                                              \
	return FALSE;                                                             \
}

#endif /* __CWT_VECTOR_H__ */
