/*
 * vector.h
 *
 *  Created on: 07.07.2012
 *      Author: wladt
 */

#ifndef __CWT_VECTOR_H__
#define __CWT_VECTOR_H__

#include <cwt/types.h>
#include <cwt/array.h>

#define CWT_VECTOR_MAX_SIZE CWT_SIZE_T_MAX

typedef struct _CwtSharedArray {
	int       refs;
	CwtArray *buffer;
} CwtSharedArray;

typedef struct _CwtVector
{
	size_t head;
    size_t count;
    size_t max_capacity;
    CwtSharedArray *d;
} CwtVector;

#define _CWT_VECTOR_COMMON_NS                                                                                      \
		void          (*initPeer)      (CwtVector *v, CwtVector *peer);                                            \
		CwtVector*    (*createPeer)    (CwtVector *peer);                                                          \
		void          (*destroy)       (CwtVector *v);                                                             \
		void          (*free)          (CwtVector *v);                                                             \
		void          (*assign)        (CwtVector *v, CwtVector *peer);                                            \
		BOOL          (*isEmpty)       (CwtVector *v);                                                             \
		BOOL          (*reserve)       (CwtVector *v, size_t n);                                                   \
		BOOL          (*reserveBack)   (CwtVector *v, size_t n);                                                   \
		BOOL          (*reserveFront)  (CwtVector *v, size_t n);                                                   \
		BOOL          (*resize)        (CwtVector *v, size_t n);                                                   \
		size_t        (*capacity)      (CwtVector *v);                                                             \
		void          (*clear)         (CwtVector *v);                                                             \
		size_t        (*size)          (CwtVector *v);                                                             \
		void          (*popBack)       (CwtVector *v, size_t n);                                                   \
		void          (*popFront)      (CwtVector *v, size_t n);                                                   \
		void          (*remove)        (CwtVector *v, size_t pos);                                                 \
		void          (*remove_n)      (CwtVector *v, size_t pos, size_t n);                                       \
		BOOL          (*sub)           (CwtVector *to, CwtVector *from, size_t off_from, size_t n);                \

#define _CWT_VECTOR_COMMON_INIT                                 \
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, \
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL

#define _CWT_VECTOR_COMMON_N 18

#define _CWT_VECTOR_TYPED_NS                                                                                       \
		void          (*init)          (CwtVector *v, int sizeof_item);                                            \
		void          (*initSized)     (CwtVector *v, int sizeof_item, size_t initial_size, size_t max_capacity);  \
		CwtVector*    (*create)        (int sizeof_item);                                                          \
		CwtVector*    (*createSized)   (int sizeof_item, size_t initial_size, size_t max_capacity);                \
		const void*   (*at)            (CwtVector *v, size_t index);                                               \
		void*         (*first)         (CwtVector *v);                                                             \
		const void*   (*constFirst)    (CwtVector *v);                                                             \
		void*         (*last)          (CwtVector *v);                                                             \
		const void*   (*constLast)     (CwtVector *v);                                                             \
		void*         (*data)          (CwtVector *v);                                                             \
		const void*   (*constData)     (CwtVector *v);                                                             \
		void          (*append)        (CwtVector *v, void *item);                                                 \
		void          (*pushBack)      (CwtVector *v, void *item);                                                 \
		void          (*pushFront)     (CwtVector *v, void *item);                                                 \
		void          (*insert)        (CwtVector *v, void *item, size_t pos);                                     \
		void          (*insertRep)     (CwtVector *v, void *item, size_t pos, size_t n);                           \
		void          (*fill)          (CwtVector *v, void *item, size_t n);                                       \
		BOOL          (*indexOf)       (CwtVector *v, void *item, size_t off_from, size_t *index);                 \
		BOOL          (*replace)       (CwtVector *v, const void *item, size_t pos);


typedef struct _CwtVectorNS {
	_CWT_VECTOR_COMMON_NS
	_CWT_VECTOR_TYPED_NS
} CwtVectorNS;

EXTERN_C_BEGIN
DLL_API_EXPORT CwtVectorNS* cwt_vector_ns(void);
EXTERN_C_END



#include <string.h>
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
	void          (*init)        (_CollectionT* p);                           \
	void          (*initSized)   (_CollectionT* p, size_t initial_size, size_t max_size); \
	void          (*initWithBuffer) (_CollectionT* p, _ElemT *buf, size_t n); \
	_CollectionT* (*create)      (void);                                      \
	_CollectionT* (*createSized) (size_t initial_size, size_t max_size);      \
	void          (*destroy)     (_CollectionT* p);                           \
	void          (*free)        (_CollectionT* p);                           \
	void          (*disrobe)     (_CollectionT* p);                           \
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
	void          (*replace)     (_CollectionT* p, const _ElemT *elems, size_t nelems, size_t pos); \
	void          (*removeElem)  (_CollectionT* p, size_t pos);               \
	void          (*removeLast)  (_CollectionT* p);                           \
	_ElemT*       (*substr)      (_CollectionT* p, size_t start, size_t nelems); \
	BOOL          (*find)        (_CollectionT* p, _ElemT ch, size_t *offset);\
	_ElemT        (*at)          (_CollectionT* p, size_t index);             \
    _ElemT        (*first)       (_CollectionT* p);                           \
    _ElemT        (*last)        (_CollectionT* p);                           \
    const _ElemT* (*data)        (_CollectionT* p);

#define CWT_END_DECL_VECTOR_NS(_NS) } _NS;

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
static void __initWithBuffer (_CollectionT* p, _ElemT *buf, size_t n)         \
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
	sb = CWT_MALLOCT(_CollectionT);                                           \
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
				memcpy(clone->m_buffer                                        \
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
		memcpy(buffer, sb->m_buffer, sb->m_count * sizeof(_ElemT));           \
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
	memcpy(buffer + n, sb->m_buffer                                           \
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
	memcpy(buffer, sb->m_buffer, pos * sizeof(_ElemT));                       \
	memcpy(buffer+pos+n, sb->m_buffer + pos                                   \
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
	memcpy(sb->m_buffer + sb->m_count, s, n * sizeof(_ElemT));                \
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
		memmove(&sb->m_buffer[pos]                                            \
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
		memcpy(elems, sb->m_buffer + start                                    \
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
