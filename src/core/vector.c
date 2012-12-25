/*
 * vector.h
 *
 *  Created on: 07.07.2012
 *      Author: wladt
 */

#ifdef __COMMENT__

#include "vector_p.h"
#include <cwt/str.h>

#define _CAST_IMPL(v) ((CwtVectorImpl*)(v))

#ifndef CWT_NDEBUG
#	include <cwt/magic.h>
#	define _ASSERT_MAGIC(v) CWT_ASSERT(_CAST_IMPL(v)->__base.magic == CWT_MAGIC_VECTOR)
#	define _EQ_MAGIC(v)     (_CAST_IMPL(v)->__base.magic == CWT_MAGIC_VECTOR)
#	define _SET_MAGIC(v)    _CAST_IMPL(v)->__base.magic = CWT_MAGIC_VECTOR
#	define _RESET_MAGIC(v)  _CAST_IMPL(v)->__base.magic = 0
#else
#	define _ASSERT_MAGIC(v)
#	define _EQ_MAGIC(v)
#	define _SET_MAGIC(v)
#	define _RESET_MAGIC(v)
#endif

static const size_t __default_initial_size = 64;
static const size_t __default_max_size = CWT_SIZE_T_MAX;


static void        v_init         (CwtVector *p, int sizeof_item);
static void        v_init_sized   (CwtVector *v, int sizeof_item, size_t initial_size, size_t max_size);
static CwtVector*  v_create       (int sizeof_item);
static CwtVector*  v_create_sized (int sizeof_item, size_t initial_size, size_t max_size);
static void        v_destroy      (CwtVector *v);
static void        v_free         (CwtVector *v);
static void        v_assign       (CwtVector *target, CwtVector *orig);
static CwtVector*  v_clone        (CwtVector *v);
static BOOL        v_reserve      (CwtVector *v, size_t n);
static BOOL        v_resize       (CwtVector *v, size_t n);
static size_t      v_capacity     (CwtVector *v);
static BOOL        v_is_empty     (CwtVector *v);
static void        v_clear        (CwtVector *v);
static size_t      v_size         (CwtVector *v);
static void        v_append_item  (CwtVector *v, void *item);


static CwtVectorNS __vector_ns = {
	  v_init
	, v_init_sized
	, v_create
	, v_create_sized
	, v_destroy
	, v_free
	, v_assign
	, v_clone
	, v_reserve
	, v_resize
	, v_capacity
	, v_is_empty
	, v_clear
	, v_size
	, v_size /* length */
	, v_append_item
};

DLL_API_EXPORT CwtVectorNS* cwt_vector_ns(void)
{
	return &__vector_ns;
}

static void v_init (CwtVector *p, int sizeof_item)
{
	v_init_sized(p, sizeof_item, CWT_CAST_SIZE_T(0), CWT_CAST_SIZE_T(0));
}


static void v_init_sized (CwtVector *v, int sizeof_item, size_t initial_size, size_t max_size)
{
	DataShared *d;

	if (!v)
		return;

	if( !initial_size )
		initial_size = __default_initial_size;

	if( !max_size )
		max_size = __default_max_size;

	if( initial_size > max_size )
		initial_size = max_size;

	cwt_bzero(v, sizeof(CwtVectorImpl));
	_SET_MAGIC(v);
	v->allocated = FALSE;

	d = CWT_MALLOCT(DataShared);
	v_init_shared(d, sizeof_item, initial_size, max_size);
}

static CwtVector* v_create (int sizeof_item)
{
	return v_create_sized(sizeof_item, CWT_CAST_SIZE_T(0), CWT_CAST_SIZE_T(0));
}

static CwtVector* v_create_sized (int sizeof_item, size_t initial_size, size_t max_size)
{
	CwtVectorImpl *v;

	v = CWT_MALLOCT(CwtVectorImpl);
	CWT_ASSERT(v);
	v_init_sized(v, sizeof_item, initial_size, max_size);
	v->__base.allocated = TRUE;
	return v;
}



static void v_destroy (CwtVector *v)
{
	DataShared *d;

	_ASSERT_MAGIC(v);

	if (!v)
		return;

	if (!_CAST_IMPL(v)->d)
		return;

	v_deref_shared(_CAST_IMPL(v)->d);
	_CAST_IMPL(v)->d = NULL;
	_RESET_MAGIC(v);
}

static void v_free (CwtVector *v)
{
	v_destroy(v);
	if (v->allocated)
		CWT_FREE(v);
}


/**
 * @brief Assigns @c orig to @c target.
 *
 * @param target Left operand of assignment.
 * @param orig Right operand of assignment.
 *        Must be initialized or created before or may be NULL.
 *        In the latter case target will be freed.
 */
static void v_assign(CwtVector *target, CwtVector *orig)
{
	CwtVectorImpl *t = _CAST_IMPL(target);
	CwtVectorImpl *o = _CAST_IMPL(orig);

	CWT_ASSERT(target);

	if (!orig) {
		v_free(target);
		return;
	}

	_ASSERT_MAGIC(o);
	CWT_ASSERT(o->d);

	if (target == orig)
		return;

	/* target already initialized */
	if (_EQ_MAGIC(t)) {
		CWT_ASSERT(t->d);
		v_deref_shared(t->d);
		t->d = NULL;
	}

	t->head = o->head;
	t->count = o->count;
	t->d = o->d;
	o->d->refs++;
}


static CwtVector* v_clone (CwtVector *orig)
{
	CwtVectorImpl *v, o;

	if (!orig)
		return NULL;

	o = _CAST_IMPL(orig);

	_ASSERT_MAGIC(o);
	CWT_ASSERT(o->d);

	v = (CwtVectorImpl*)v_create_sized(o->d->sizeof_item, o->count, o->d->max_capacity);
	v->count = o->count;
	v_copy_shared(v->d, o->d, o->head, o->count);
	return (CwtVector*)v;
}


static BOOL v_reserve (CwtVector *vec, size_t n, int where, size_t pos)
{
	size_t available;
	size_t inc;
	DataShared    *from, *to;
	CwtVectorImpl *v;
	BOOL ok;

	v = _CAST_IMPL(vec);

	CWT_ASSERT(v);
	_ASSERT_MAGIC(v);
	CWT_ASSERT(v->d);

	available = v->d->capacity - v->count;

	if (where < 0) {       /* reserve space from left */
		size_t leftmost;
		leftmost = v->head;

	} else if (where > 0) { /* reserve space from right */
		size_t rightmost;
		rightmost = v->d->capacity - v->head - v->count;

		/* enough space from right */
		if (rightmost >= n) {
			v_shift_left_shared(v);
		} else {
		}

	} else {  /* reserve space in the middle of buffer */
		ok = v_mreserve_shared(v);
	}

	return FALSE;
}

static BOOL v_resize (CwtVector *v, size_t n)
{
	CWT_ASSERT(v);

	if( !v_reserve(v, n) )
		return FALSE;
	_CAST_IMPL(v)->count = n;
	return TRUE;
}

static size_t v_capacity (CwtVector *v)
{
	CWT_ASSERT(v);
	_ASSERT_MAGIC(v);
	return _CAST_IMPL(v)->d->capacity;
}

static BOOL v_is_empty (CwtVector *v)
{
	CWT_ASSERT(v);
	_ASSERT_MAGIC(v);
	return _CAST_IMPL(v)->count == 0 ? TRUE : FALSE;
}

static void v_clear (CwtVector *v)
{
	CWT_ASSERT(v);
	_ASSERT_MAGIC(v);
	_CAST_IMPL(v)->count = 0;
}

static size_t v_size (CwtVector *v)
{
	CWT_ASSERT(v);
	_ASSERT_MAGIC(v);
	return _CAST_IMPL(v)->count;
}

static void v_append_item (CwtVector *v, void *item)
{
	size_t i, off;
	DataShared *d;
	char *pitem = (char*)item;

	CWT_ASSERT(v);
	_ASSERT_MAGIC(v);
	CWT_ASSERT(v_reserve(v, 1));

	if (d->refs > 1) {

	}

	off = _CAST_IMPL(v)->head * d->sizeof_item;
	switch(d->sizeof_item) {
	case 1:
		d->buffer[off] = *pitem++;
		break;
	case 2:
		d->buffer[off++] = *pitem++;
		d->buffer[off]   = *pitem++;
		break;
	case 4:
		d->buffer[off++] = *pitem++;
		d->buffer[off++] = *pitem++;
		d->buffer[off++] = *pitem++;
		d->buffer[off]   = *pitem++;
		break;
	default:
		memcpy(&d->buffer[off], pitem, d->sizeof_item);
		break;
	}

	_CAST_IMPL(v)->count++;
}

#define CWT_VECTOR_METHODS(_CollectionT,_ElemT)                               \
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
static void __disrobe(_CollectionT* sb)                                       \
{                                                                             \
	CWT_FREE(sb);                                                             \
}                                                                             \
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
                                                                             \
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
	_ElemT *elems = NULL;                                             			;
        \
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
#endif
