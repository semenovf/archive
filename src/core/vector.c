/*
 * vector.h
 *
 *  Created on: 07.07.2012
 *      Author: wladt
 */

#include "cwt/vector.h"

static const size_t __default_initial_size = 64;

#define _WHENCE_FRONT  -1
#define _WHENCE_BACK    1
#define _WHENCE_MIDDLE  0

static inline CwtSharedArray* __alloc_shared (int sizeof_item, size_t capacity);
static void __deref_shared     (CwtSharedArray *d);
static BOOL __v_reserve        (CwtVector *v, size_t n, int whence, size_t pos);
static void __v_reserve_back   (CwtVector *v, size_t inc);
static void __v_reserve_front  (CwtVector *v, size_t inc);
static void __v_reserve_middle (CwtVector *v, size_t inc, size_t pos);

static void        v_init         (CwtVector *p, int sizeof_item);
static void        v_init_sized   (CwtVector *v, int sizeof_item, size_t initial_size, size_t max_capacity);
static void        v_init_peer    (CwtVector *v, CwtVector *peer);
static CwtVector*  v_create       (int sizeof_item);
static CwtVector*  v_create_sized (int sizeof_item, size_t initial_size, size_t max_capacity);
static CwtVector*  v_create_peer  (CwtVector *peer);
static void        v_destroy      (CwtVector *v);
static void        v_free         (CwtVector *v);
static void        v_assign       (CwtVector *v, CwtVector *peer);
static inline BOOL v_is_empty     (CwtVector *v);
static BOOL        v_reserve_back (CwtVector *v, size_t n) { return __v_reserve (v, n, _WHENCE_BACK, 0); }
static BOOL        v_reserve_front(CwtVector *v, size_t n) { return __v_reserve (v, n, _WHENCE_FRONT, 0); }
static BOOL        v_resize       (CwtVector *v, size_t n);
static inline size_t v_capacity   (CwtVector *v);
static void        v_clear        (CwtVector *v);
static size_t      v_size         (CwtVector *v);
static const void* v_at           (CwtVector *v, size_t index);
static void*       v_first        (CwtVector *v);
static const void* v_const_first  (CwtVector *v) { return v_first(v); }
static void*       v_last         (CwtVector *v);
static const void* v_const_last   (CwtVector *v) { return v_last(v); }
static void*       v_data         (CwtVector *v);
static const void* v_const_data   (CwtVector *v) { return v_data(v); }
static void        v_push_back    (CwtVector *v, void *item);
static void        v_push_front   (CwtVector *v, void *item);
static void        v_append       (CwtVector *v, void *item) { v_push_back(v, item); }
static void        v_insert       (CwtVector *v, void *item, size_t pos);
static void        v_insert_rep   (CwtVector *v, void *item, size_t pos, size_t n);
static void        v_fill         (CwtVector *v, void *item, size_t n);
static void        v_pop_back     (CwtVector *v);
static void        v_pop_front    (CwtVector *v);
static void        v_remove_n     (CwtVector *v, size_t pos, size_t n);
static void        v_remove       (CwtVector *v, size_t pos) { v_remove_n (v, pos, 1); }

static CwtArrayNS *__array_ns = NULL;

static CwtVectorNS __vector_ns = {
	  v_init
	, v_init_sized
	, v_init_peer
	, v_create
	, v_create_sized
	, v_create_peer
	, v_destroy
	, v_free
	, v_assign
	, v_is_empty
	, v_reserve_back
	, v_reserve_back
	, v_reserve_front
	, v_resize
	, v_capacity
	, v_clear
	, v_size
	, v_at
	, v_first
	, v_const_first
	, v_last
	, v_const_last
	, v_data
	, v_const_data
	, v_append
	, v_push_back
	, v_push_front
	, v_insert
	, v_insert_rep
	, v_fill
	, v_pop_back
	, v_pop_front
	, v_remove
	, v_remove_n
};

DLL_API_EXPORT CwtVectorNS* cwt_vector_ns(void)
{
	if (!__array_ns) {
		__array_ns = cwt_array_ns();
	}
	return &__vector_ns;
}


static void v_init (CwtVector *v, int sizeof_item)
{
	v_init_sized (v, sizeof_item, 0, 0);
}

static void v_init_sized (CwtVector *v, int sizeof_item, size_t initial_size, size_t max_capacity)
{
	if (!v)
		return;

	if( !initial_size )
		initial_size = __default_initial_size;

	if( !max_capacity )
		max_capacity = CWT_VECTOR_MAX_SIZE;

	if( initial_size > max_capacity )
		initial_size = max_capacity;

	v->head  = 0;
	v->count = 0;
    v->max_capacity = max_capacity;
    v->d = __alloc_shared (sizeof_item, initial_size);
}

static void v_init_peer (CwtVector *v, CwtVector *peer)
{
	CWT_ASSERT(v);
	CWT_ASSERT(peer);

	v->head         = peer->head;
	v->count        = peer->count;
	v->max_capacity = peer->max_capacity;
	v->d            = peer->d;
	v->d->refs++;
}

static CwtVector* v_create (int sizeof_item)
{
	CwtVector *v;

	v = CWT_MALLOCT(CwtVector);
	v_init(v, sizeof_item);
	return v;
}

static CwtVector* v_create_sized (int sizeof_item, size_t initial_size, size_t max_capacity)
{
	CwtVector *v;

	v = CWT_MALLOCT(CwtVector);
	v_init_sized(v, sizeof_item, initial_size, max_capacity);
	return v;
}

static CwtVector*  v_create_peer (CwtVector *peer)
{
	CwtVector *v;

	if (!peer)
		return NULL;

	v = CWT_MALLOCT(CwtVector);
	v_init_peer(v, peer);
	return v;
}

static void v_destroy (CwtVector *v)
{
	if (!v)
		return;

	__deref_shared(v->d);
	v->d = NULL;
}

static void v_free (CwtVector *v)
{
	if (v) {
		v_destroy(v);
		CWT_FREE(v);
	}
}

/**
 * @brief Assigns @c orig to @c target.
 *
 * @param v Left operand of assignment.
 * @param peer Right operand of assignment.
 */
static void v_assign(CwtVector *v, CwtVector *peer)
{
	CWT_ASSERT(v);

	if (!peer) {
		v_destroy(v);
		return;
	}

	__deref_shared(v->d);
	v->head         = peer->head;
	v->count        = peer->count;
	v->max_capacity = peer->max_capacity;
	v->d            = peer->d;
	peer->d->refs++;
}


static inline BOOL v_is_empty (CwtVector *v)
{
	CWT_ASSERT(v);
	return v->count == 0 ? TRUE : FALSE;
}


static BOOL v_resize (CwtVector *v, size_t n)
{
	CWT_ASSERT(v);
	CWT_ASSERT(v->d);

	if (!n) {
		v_destroy(v);
		return FALSE;
	}

	if (n > v->max_capacity)
		return FALSE;

	if (n > __array_ns->size(v->d->buffer)) {
		return __v_reserve(v, n, _WHENCE_BACK, 0);
	} else {
		CwtSharedArray *d;
		d = __alloc_shared (v->d->buffer->sizeof_item, n);
		__array_ns->copy(d->buffer, v->d->buffer, 0, v->head, CWT_MIN(n, v->count));
		__deref_shared(v->d);
		v->d = d;
		v->head = 0;
	}

	return TRUE;
}

/**
 * @fn size_t CwtVectorNS::capacity(CwtVector *v)
 *
 * @brief Returns the maximum number of items that can be
 *        stored in the vector without forcing a reallocation.
 *
 * @param v Vector instance.
 * @return maximum number of items that can be
 *        stored in the vector without forcing a reallocation.
 */
static inline size_t v_capacity (CwtVector *v)
{
	CWT_ASSERT(v);
	CWT_ASSERT(v->d);

	return __array_ns->size(v->d->buffer);
}


/**
 * @brief Clear vector: set count to zero or releases the memory used by the vector.
 *
 * @param v Vector instance.
 */
static void v_clear (CwtVector *v)
{
	int sizeof_item;
	CWT_ASSERT(v);
	CWT_ASSERT(v->d);

	sizeof_item = v->d->buffer->sizeof_item;
	v_destroy(v);
	v_init(v, sizeof_item);
}

static size_t v_size (CwtVector *v)
{
	CWT_ASSERT(v);
	return v->count;
}


static const void* v_at (CwtVector *v, size_t index)
{
	CWT_ASSERT(v);
	CWT_ASSERT(v->d);

	if (index >= v->count)
		return NULL;

	return __array_ns->at(v->d->buffer, v->head + index);
}

static void* v_first (CwtVector *v)
{
	CWT_ASSERT(v);
	CWT_ASSERT(v->d);

	if (!v->count)
		return NULL;

	return (void*)__array_ns->at(v->d->buffer, v->head);
}

static void* v_last (CwtVector *v)
{
	CWT_ASSERT(v);
	CWT_ASSERT(v->d);

	if (!v->count)
		return NULL;

	return (void*)__array_ns->at(v->d->buffer, v->head + v->count - 1);
}


static void* v_data (CwtVector *v)
{
	CWT_ASSERT(v);
	CWT_ASSERT(v->d);
	return (void*)__array_ns->at(v->d->buffer, v->head);
}

static void v_push_back (CwtVector *v, void *item)
{
	CWT_ASSERT(v);
	CWT_ASSERT(v->d);

	if (v_capacity(v) == v->count)
		__v_reserve(v, 32, _WHENCE_BACK, 0);

	if (!__v_reserve(v, 1, _WHENCE_BACK, 0))
		return;

	__array_ns->put(v->d->buffer, v->head + v->count, item);
	++v->count;
}

static void v_push_front (CwtVector *v, void *item)
{
	CWT_ASSERT(v);
	CWT_ASSERT(v->d);

	if (v_capacity(v) == v->count)
		__v_reserve(v, 32, _WHENCE_FRONT, 0);

	if (!__v_reserve(v, 1, _WHENCE_FRONT, 0))
		return;

	__array_ns->put(v->d->buffer, --v->head, item);
	++v->count;
}


static void v_insert (CwtVector *v, void *item, size_t pos)
{
	CWT_ASSERT(v);
	CWT_ASSERT(v->d);

	if (pos == 0) {
		v_push_front(v, item);
	} else if (pos >= v->count) {
		v_push_back(v, item);
	} else {
		if (!__v_reserve(v, 1, _WHENCE_MIDDLE, pos))
			return;

		__array_ns->put(v->d->buffer, v->head + pos, item);
		++v->count;
	}
}


static void v_insert_rep (CwtVector *v, void *item, size_t pos, size_t n)
{
	size_t i;

	CWT_ASSERT(v);
	CWT_ASSERT(v->d);

	if (!__v_reserve(v, n, _WHENCE_MIDDLE, pos))
		return;
	for (i = 0; i < n; i++)
		__array_ns->put(v->d->buffer, v->head + pos + i, item);
	v->count += n;
}


static void v_fill (CwtVector *v, void *item, size_t n)
{
	/* detach shared data */
	if (__v_reserve (v, CWT_MIN(n, v->count), _WHENCE_BACK, 0)) {
		size_t i;
		for (i = 0 ; i < v->count; ++i)
			__array_ns->put(v->d->buffer, v->head + i, item);
	}
}

static void v_pop_back (CwtVector *v)
{
	CWT_ASSERT(v);
	CWT_ASSERT(v->d);

	if (v->count > 0)
		--v->count;
}

static void v_pop_front (CwtVector *v)
{
	CWT_ASSERT(v);
	CWT_ASSERT(v->d);

	if (v->count > 0) {
		++v->head;
		--v->count;
	}
}

static void v_remove_n (CwtVector *v, size_t pos, size_t n)
{
	CWT_ASSERT(v);
	CWT_ASSERT(v->d);

	if (pos == 0) {
		v_pop_front(v);
	} else if (pos >= v->count) {
		v_pop_back(v);
	} else {
		size_t capacity = v->d->buffer->capacity;

		n = CWT_MIN(n, v->count - pos);

		if (n >= v->count) {
			v_clear(v);
			return;
		}


		if (v->d->refs > 1) {
			CwtSharedArray *d;
			d = __alloc_shared (v->d->buffer->sizeof_item, v->count - n);
			__array_ns->copy(d->buffer, v->d->buffer, 0, v->head, pos);
			__array_ns->copy(d->buffer, v->d->buffer, pos, v->head + pos + n, v->count);
			__deref_shared(v->d);
			v->d = d;
			v->head = 0;
		} else {
			v->d->buffer = __array_ns->realloc(v->d->buffer
					, capacity + inc);
			if (v->head > 0) {
				__array_ns->move(v->d->buffer, 0, v->head, v->count);
				v->head = 0;
			}
		}
	}
}


static inline CwtSharedArray* __alloc_shared (int sizeof_item, size_t capacity)
{
	CwtSharedArray * d;
	d = CWT_MALLOCT(CwtSharedArray);
	d->buffer = __array_ns->alloc(sizeof_item, capacity);
	d->refs = 1;
	return d;
}


static void __deref_shared (CwtSharedArray *d)
{
	d->refs--;

	CWT_ASSERT(d->refs >=0);

	if (!d->refs) {
		__array_ns->free(d->buffer);
		CWT_FREE(d);
	}
}


static BOOL __v_reserve (CwtVector *v, size_t n, int whence, size_t pos)
{
	size_t available;
	size_t inc = 0;

	CWT_ASSERT(v->d->refs > 0);

	available = v->d->buffer->capacity - v->count;
	if (available < n)
		inc = n - available;

	if (inc && v->max_capacity < inc)
		return FALSE;

	if (v->max_capacity - inc < v->d->buffer->capacity)
		return FALSE;

	if (whence == _WHENCE_BACK) { /* reserve from back */
		__v_reserve_back (v, inc);
	} else if (whence == _WHENCE_FRONT) { /* reserve from front */
		__v_reserve_front (v, inc);
	} else {
		__v_reserve_middle(v, inc, pos);
	}

	return TRUE;
}

static void __v_reserve_back (CwtVector *v, size_t inc)
{
	size_t capacity = v->d->buffer->capacity;

	if (v->d->refs > 1) {
		CwtSharedArray *d;
		d = __alloc_shared (v->d->buffer->sizeof_item
				, capacity + inc);
		__array_ns->copy(d->buffer, v->d->buffer, 0, v->head, v->count);
		__deref_shared(v->d);
		v->d = d;
		v->head = 0;
	} else {
		v->d->buffer = __array_ns->realloc(v->d->buffer
				, capacity + inc);
		if (v->head > 0) {
			__array_ns->move(v->d->buffer, 0, v->head, v->count);
			v->head = 0;
		}
	}
}

void __v_reserve_front (CwtVector *v, size_t inc)
{
	size_t new_capacity = v->d->buffer->capacity + inc;

	if (v->d->refs > 1) {
		CwtSharedArray *d;
		d = __alloc_shared (v->d->buffer->sizeof_item, new_capacity);
		__array_ns->copy(d->buffer
				, v->d->buffer
				, new_capacity - v->count
				, v->head, v->count);
		__deref_shared(v->d);
		v->d = d;
		v->head = 0;
	} else {
		v->d->buffer = __array_ns->realloc(v->d->buffer, new_capacity);
		__array_ns->move(v->d->buffer, new_capacity - v->count, v->head, v->count);
		v->head = new_capacity - v->count;
	}
}

/* Reserves space in the middle of buffer
 * */
static void __v_reserve_middle (CwtVector *v, size_t inc, size_t pos)
{
	size_t new_capacity = v->d->buffer->capacity + inc;
	size_t sz1, sz2;

	if (pos == 0) {
		__v_reserve_front(v, inc);
		return;
	}

	if (pos >= v->count) {
		__v_reserve_back(v, inc);
		return;
	}

	sz1 = pos;
	sz2 = v->count - sz1;

	if (v->d->refs > 1) {
		CwtSharedArray *d;

		d = __alloc_shared (v->d->buffer->sizeof_item, new_capacity);

		/* copy left part */
		__array_ns->copy(d->buffer, v->d->buffer, 0, v->head, sz1);

		/* copy right part */
		__array_ns->copy(d->buffer, v->d->buffer, sz1 + inc, v->head + sz1, sz2);

		__deref_shared(v->d);
		v->d = d;
		v->head = 0;
	} else {
		v->d->buffer = __array_ns->realloc(v->d->buffer, new_capacity);
		if (v->head > 0)
			__array_ns->move(v->d->buffer, 0, v->head, sz1);
		__array_ns->move(v->d->buffer, sz1 + inc, v->head + sz1, sz2);
		v->head = 0;
	}
}


#ifdef __COMMENT__
#include <cwt/str.h>

static BOOL        v_reserve      (CwtVector *v, size_t n);
static BOOL        v_resize       (CwtVector *v, size_t n);
static size_t      v_capacity     (CwtVector *v);
static BOOL        v_is_empty     (CwtVector *v);
static void        v_clear        (CwtVector *v);
static size_t      v_size         (CwtVector *v);
static void        v_append_item  (CwtVector *v, void *item);

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
