/*
 * vector.h
 *
 *  Created on: 07.07.2012
 *      Author: wladt
 */

#ifndef __CWT_VECTOR_H__
#define __CWT_VECTOR_H__

#include <cwt/types.h>
#include <cwt/str.h>

typedef struct _CwtVector {
#ifndef CWT_NDEBUG
	INT32   magic;
#endif
	BOOL allocated;
} CwtVector;

typedef struct _CwtVectorNS {
	void          (*init)          (CwtVector *v, int sizeof_item);
	void          (*initSized)     (CwtVector *v, int sizeof_item, size_t initial_size, size_t max_size);
	/*void          (*initRawBuffer) (CwtVector *v, int sizeof_item, const char *buf, size_t nitems);*/
	CwtVector*    (*create)        (int sizeof_item);
	CwtVector*    (*createSized)   (int sizeof_item, size_t initial_size, size_t max_size);
	void          (*destroy)       (CwtVector *v);
	void          (*free)          (CwtVector *v);
/*	void          (*disrobe)       (CwtVector *v);*/
	void          (*assign)        (CwtVector *target, CwtVector *orig);
	CwtVector*    (*clone)         (CwtVector *v);
	BOOL          (*reserve)       (CwtVector *v, size_t n);
	BOOL          (*resize)        (CwtVector *v, size_t n);
	size_t        (*capacity)      (CwtVector *v);
	BOOL          (*isEmpty)       (CwtVector *v);
	void          (*clear)         (CwtVector *v);
	size_t        (*size)          (CwtVector *v);
	size_t        (*length)        (CwtVector *v);
	void          (*appendItem)    (CwtVector* p, void *item);

#ifdef __COMMENT_

	BOOL          (*lreserve)    (CwtVector* p, size_t n);
	BOOL          (*mreserve)    (CwtVector* p, size_t n, size_t pos);

	void          (*appendElems) (CwtVector* p, const _ElemT *elems, size_t n);
	void          (*prependElem) (CwtVector* p, _ElemT ch);
	void          (*insertElem)  (CwtVector* p, _ElemT ch, size_t pos);
	void          (*insert)      (CwtVector* p, const _ElemT *elems, size_t nelems, size_t pos);
	void          (*replace)     (CwtVector* p, const _ElemT *elems, size_t nelems, size_t pos);
	void          (*removeElem)  (CwtVector* p, size_t pos);
	void          (*removeLast)  (CwtVector* p);
	_ElemT*       (*substr)      (CwtVector* p, size_t start, size_t nelems);
	BOOL          (*find)        (CwtVector* p, _ElemT ch, size_t *offset);
	_ElemT        (*at)          (CwtVector* p, size_t index);
    _ElemT        (*first)       (CwtVector* p);
    _ElemT        (*last)        (CwtVector* p);
    const _ElemT* (*data)        (CwtVector* p);
#endif
} CwtVectorNS;

EXTERN_C_BEGIN
DLL_API_EXPORT CwtVectorNS* cwt_vector_ns(void);
EXTERN_C_END


#define CWT_BEGIN_DEF_VECTOR_NS(_NS,_CollectionT,_ElemT)                      \
	static const size_t __default_initial_size = 64;                          \
	static const size_t __default_max_size = CWT_SIZE_T_MAX;                  \
                                                                              \
    static void          __init        (_CollectionT* p);                     \
    static void          __initSized   (_CollectionT* p, size_t initial_size, size_t max_size); \
    static void          __initWithBuffer (_CollectionT* p, _ElemT *buf, size_t n);  \
	static _CollectionT* __create      (void);                                \
	static _CollectionT* __createSized (size_t initial_size, size_t max_size);\
	static void          __destroy     (_CollectionT* p);                     \
	static void          __free        (_CollectionT* p);                     \
	static void          __disrobe     (_CollectionT* p);                     \
	static _CollectionT* __clone       (_CollectionT* p);                     \
	static BOOL          __reserve     (_CollectionT* p, size_t n);           \
	static BOOL          __lreserve    (_CollectionT* p, size_t n);           \
	static BOOL          __mreserve    (_CollectionT* p, size_t n, size_t pos);\
	static BOOL          __resize      (_CollectionT* p, size_t n);           \
	static size_t        __capacity    (_CollectionT* p);                     \
	static BOOL          __isEmpty     (_CollectionT* p);                     \
	static void          __clear       (_CollectionT* p);                     \
	static size_t        __size        (_CollectionT* p);                     \
	/*static size_t        __length      (_CollectionT* p);*/                 \
	static void          __appendElem  (_CollectionT* p, _ElemT ch);          \
	static void          __appendElems (_CollectionT* p, const _ElemT *elems, size_t n); \
	static void          __prependElem (_CollectionT* p, _ElemT ch);          \
	static void          __insertElem  (_CollectionT* p, _ElemT ch, size_t pos);\
	static void          __insert      (_CollectionT* p, const _ElemT *elems, size_t nelems, size_t pos); \
	static void          __replace     (_CollectionT* p, const _ElemT *elems, size_t nelems, size_t pos); \
	static void          __removeElem  (_CollectionT* p, size_t pos);         \
	static void          __removeLast  (_CollectionT* p);                     \
	static _ElemT*       __substr      (_CollectionT* p, size_t start, size_t nelems); \
	static BOOL          __find        (_CollectionT* p, _ElemT ch, size_t *offset); \
	static _ElemT        __at          (_CollectionT* p, size_t index);       \
    static _ElemT        __first       (_CollectionT* p);                     \
    static _ElemT        __last        (_CollectionT* p);                     \
    static const _ElemT* __data        (_CollectionT* p);                     \
                                                                              \
static _NS  __##_NS = {                                                       \
	  __init                                                                  \
    , __initSized                                                             \
    , __initWithBuffer                                                        \
	, __create                                                                \
	, __createSized                                                           \
	, __destroy                                                               \
	, __free                                                                  \
	, __disrobe                                                               \
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
	, __replace                                                               \
	, __removeElem                                                            \
	, __removeLast                                                            \
	, __substr                                                                \
	, __find                                                                  \
    , __at                                                                    \
    , __first                                                                 \
    , __last                                                                  \
    , __data

#define CWT_END_DEF_VECTOR_NS(_NS) };


#define CWT_VECTOR_METHODS(_CollectionT,_ElemT)                               \
static void __init(_CollectionT* sb)                                          \
{                                                                             \
	__initSized(sb, CWT_CAST_SIZE_T(0), CWT_CAST_SIZE_T(0));                  \
}                                                                             \
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
static void __initRawBuffer (_CollectionT* p, _ElemT *buf, size_t n)         \
{                                                                             \
	if (!buf || n == 0) {                                                     \
		__init(p);                                                            \
		return;                                                               \
	}                                                                         \
                                                                              \
	p->m_buffer = buf;                                                        \
	p->m_capacity = n;                                                        \
	p->m_count = n;                                                           \
	p->m_max_capacity = __default_max_size;                                   \
}                                                                             \
                                                                              \
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
static void __disrobe(_CollectionT* sb)                                       \
{                                                                             \
	CWT_FREE(sb);                                                             \
}                                                                             \
                                                                              \
static _CollectionT* __clone(_CollectionT* sb)                                \
{                                                                             \
	if( sb ) {                                                                \
		_CollectionT* clone = __createSized(sb->m_count, __default_max_size); \
		if( sb->m_count > 0 ) {                                               \
			cwt_str_ns()->memcpy(clone->m_buffer                              \
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
		cwt_str_ns()->memcpy(buffer, sb->m_buffer, sb->m_count * sizeof(_ElemT)); \
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
	cwt_str_ns()->memcpy(buffer + n, sb->m_buffer                             \
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
	CwtStrNS *strNS = cwt_str_ns();                                           \
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
	if( !__reserve(sb, sz) )                                                  \
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
	cwt_str_ns()->memcpy(sb->m_buffer + sb->m_count, s, n * sizeof(_ElemT)); \
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
static void __insert(_CollectionT* sb, const _ElemT *elems, size_t nelems, size_t pos)\
{                                                                             \
	size_t i;                                                                 \
	CWT_ASSERT(sb);                                                           \
	CWT_ASSERT(__mreserve(sb, nelems, pos));                                  \
                                                                              \
	/* TODO replace this code by memcpy function call */                      \
	for( i = 0; i < nelems; i++ )                                             \
		sb->m_buffer[pos + i] = elems[i];                                     \
	sb->m_count += nelems;                                                    \
}                                                                             \
                                                                              \
static void  __replace (_CollectionT* p                                       \
	, const _ElemT *elems                                                     \
	, size_t nelems                                                           \
	, size_t pos)                                                             \
{                                                                             \
    size_t i;                                                                 \
    CWT_ASSERT(p);                                                            \
	CWT_ASSERT(pos <= p->m_count);                                            \
    if (p->m_count - pos < nelems)                                            \
    	__resize(p, pos + nelems);                                            \
	/* TODO replace this code by memcpy function call */                      \
	for( i = 0; i < nelems; i++ )                                             \
		p->m_buffer[pos + i] = elems[i];                                      \
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
		cwt_str_ns()->memmove(&sb->m_buffer[pos]                              \
		    , &sb->m_buffer[pos + 1], sb->m_count * sizeof(_ElemT) );         \
	}                                                                         \
}                                                                             \
	                                                                          \
static void __removeLast (_CollectionT* p)                                    \
{                                                                             \
	CWT_ASSERT(p);                                                            \
	if( p->m_count > 0)                                                       \
		__removeElem(p, p->m_count - 1);                                      \
}                                                                             \
                                                                              \
static _ElemT* __substr(_CollectionT* sb, size_t start, size_t nelems)        \
{                                                                             \
	_ElemT *elems = NULL;                                                     \
                                                                              \
	if( start < sb->m_count ) {                                               \
		nelems = CWT_MIN(nelems, sb->m_count - start);                        \
		elems = CWT_MALLOCA(_ElemT, nelems+1);                                \
		cwt_str_ns()->memcpy(elems, sb->m_buffer + start                      \
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
}                                                                             \
                                                                              \
static _ElemT __at(_CollectionT* p, size_t index)                             \
{                                                                             \
	CWT_ASSERT(p);                                                            \
	CWT_ASSERT(index < p->m_count);                                           \
	return p->m_buffer[index];                                                \
}                                                                             \
                                                                              \
static _ElemT __first(_CollectionT* p)                                        \
{                                                                             \
	CWT_ASSERT(p);                                                            \
	CWT_ASSERT(p->m_count > 0);                                               \
	return p->m_buffer[0];                                                    \
}                                                                             \
                                                                              \
static _ElemT __last(_CollectionT* p)                                         \
{                                                                             \
	CWT_ASSERT(p);                                                            \
	CWT_ASSERT(p->m_count > 0);                                               \
	return p->m_buffer[p->m_count-1];                                         \
}                                                                             \
                                                                              \
static const _ElemT* __data (_CollectionT* p)                                 \
{                                                                             \
	CWT_ASSERT(p);                                                            \
	return p->m_count > 0 ? p->m_buffer : NULL;                               \
}

#endif /* __CWT_VECTOR_H__ */
