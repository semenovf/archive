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

static void        v_init_peer    (CwtVector *v, CwtVector *peer);
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
static void        v_pop_back     (CwtVector *v, size_t n);
static void        v_pop_front    (CwtVector *v, size_t n);
static void        v_remove_n     (CwtVector *v, size_t pos, size_t n);
static void        v_remove       (CwtVector *v, size_t pos) { v_remove_n (v, pos, 1); }
static BOOL        v_sub          (CwtVector *to, CwtVector *from, size_t off_from, size_t n);

static void        v_init         (CwtVector *p, int sizeof_item);
static void        v_init_sized   (CwtVector *v, int sizeof_item, size_t initial_size, size_t max_capacity);
static CwtVector*  v_create       (int sizeof_item);
static CwtVector*  v_create_sized (int sizeof_item, size_t initial_size, size_t max_capacity);
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
static BOOL        v_index_of     (CwtVector *v, void *item, size_t off_from, size_t *index);
static BOOL        v_replace      (CwtVector *v, const void *item, size_t pos);

static CwtArrayNS *__array_ns = NULL;

static CwtVectorNS __vector_ns = {
	  v_init_peer
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
	, v_pop_back
	, v_pop_front
	, v_remove
	, v_remove_n
	, v_sub

	, v_init
	, v_init_sized
	, v_create
	, v_create_sized
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
	, v_index_of
	, v_replace
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
		if (!__v_reserve(v, n, _WHENCE_BACK, 0))
			return FALSE;
		v->count = n;
	} else {
		if (v->d->refs > 1) {
			CwtSharedArray *d;
			d = __alloc_shared (v->d->buffer->sizeof_item, n);
			__array_ns->copy(d->buffer, v->d->buffer, 0, v->head, CWT_MIN(n, v->count));
			__deref_shared(v->d);
			v->d = d;
		} else {
			if (v->head > 0)
				__array_ns->move(v->d->buffer, 0, v->head, v->count);
		}
		v->head = 0;
		v->count = n;
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

static inline void v_pop_back (CwtVector *v, size_t n)
{
	CWT_ASSERT(v);
	v->count -= CWT_MIN(n, v->count);
}

static inline void v_pop_front (CwtVector *v, size_t n)
{
	CWT_ASSERT(v);
	n = CWT_MIN(n, v->count);
	v->head += n;
	v->count -= n;
}


static void v_remove_n (CwtVector *v, size_t pos, size_t n)
{
	CWT_ASSERT(v);
	CWT_ASSERT(v->d);

	if (pos == 0) {
		v_pop_front(v, n);
	} else if (pos >= v->count) {
		v_pop_back(v, n);
	} else {
		n = CWT_MIN(n, v->count - pos);

		if (v->d->refs > 1) {
			CwtSharedArray *d;
			d = __alloc_shared (v->d->buffer->sizeof_item, v->count - n);
			__array_ns->copy(d->buffer, v->d->buffer, 0, v->head, pos);
			__array_ns->copy(d->buffer, v->d->buffer
					, pos
					, v->head + pos + n
					, v->count - pos - n);
			__deref_shared(v->d);
			v->d = d;
			v->head = 0;
		} else {
			__array_ns->move(v->d->buffer
					, v->head + pos
					, v->head + pos + n
					, v->count - pos - n);
		}
	}
}


static BOOL v_index_of (CwtVector *v, void *item, size_t off_from, size_t *pindex)
{
	size_t i;

	CWT_ASSERT(v);
	CWT_ASSERT(v->d);

	if (off_from >= v->count)
		return FALSE;


	i = off_from;

	switch (v->d->buffer->sizeof_item) {
	case 1: {
			const char *ptr = (const char*)v_at(v, off_from);
			while (i < v->count && *ptr != *((const char*)item))
				ptr++;
		}
		break;
	case 2: {
			const INT16 *ptr = (const INT16*)v_at(v, off_from);
			while (i < v->count && *ptr != *((const INT16*)item))
				ptr++;
		}
		break;
	case 4: {
			const INT32 *ptr = (const INT32*)v_at(v, off_from);
			while (i < v->count && *ptr != *((const INT32*)item))
				ptr++;
		}
		break;
	case 8: {
			const INT64 *ptr = (const INT64*)v_at(v, off_from);
			while (i < v->count && *ptr != *((const INT64*)item))
				ptr++;
		}
		break;
	default: {
			int sizeof_item = v->d->buffer->sizeof_item;
			const char *ptr = (const char*)v_at(v, off_from);
			while (i < v->count && memcmp(ptr, item, sizeof_item) == 0)
				ptr += sizeof_item;
		}
		break;
	}

	if (i < v->count && pindex)
		*pindex = i;

	return i > v->count ? FALSE : TRUE;
}


static BOOL v_sub (CwtVector *to, CwtVector *from, size_t off_from, size_t n)
{
	CWT_ASSERT(to);
	CWT_ASSERT(to->d);
	CWT_ASSERT(from);
	CWT_ASSERT(from->d);

	if (off_from >= from->count)
		return FALSE;

	n = CWT_MIN(from->count - off_from, n);

	if (!__v_reserve (to, n, _WHENCE_BACK, 0))
		return FALSE;


	__array_ns->copy(to->d->buffer
			, from->d->buffer
			, 0
			, from->head + off_from
			, n);
	to->head = 0;
	to->count = n;
	return TRUE;
}


static BOOL v_replace (CwtVector *v, const void *item, size_t pos)
{
	CWT_ASSERT(v);
	CWT_ASSERT(v->d);

	if (pos >= v->count)
		return FALSE;

	if (v->count == 0)
		return FALSE;

	__array_ns->put(v->d->buffer, pos, item);
	return TRUE;
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
