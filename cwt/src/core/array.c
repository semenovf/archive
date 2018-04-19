/**
 * @file   array.c
 * @author wladt
 * @date   Dec 25, 2012 8:16:47 AM
 *
 * @brief
 */


#include "cwt/array.h"
#include <string.h> /* for memcpy */

static CwtArray* a_alloc       (int sizeof_item, size_t capacity);
static CwtArray* a_allocz      (int sizeof_item, size_t capacity);
static CwtArray* a_realloc     (CwtArray *, size_t new_capacity);
static void      a_free        (CwtArray *);
static CwtArray* a_clone       (CwtArray *);
static size_t    a_copy        (CwtArray *, CwtArray *, size_t off_to, size_t off_from, size_t n);
static size_t    a_move        (CwtArray *, size_t off_to, size_t off_from, size_t n);
static void      a_bzero       (CwtArray *);
static void*     a_data        (CwtArray *);
static const void* a_at        (CwtArray *, size_t index);
static void*     a_put         (CwtArray *, size_t index, const void *item);
static size_t    a_size        (CwtArray *a)     { CWT_ASSERT(a); return a->capacity; }
static BOOL      a_eq          (CwtArray *, CwtArray *);
static CwtArray* a_alloc_char  (size_t capacity) { return a_allocz(sizeof(char), capacity); }
static CwtArray* a_alloc_short (size_t capacity) { return a_allocz(sizeof(short), capacity); }
static CwtArray* a_alloc_int   (size_t capacity) { return a_allocz(sizeof(int), capacity); }
static CwtArray* a_alloc_long  (size_t capacity) { return a_allocz(sizeof(long), capacity); }
static CwtArray* a_alloc_llong (size_t capacity) { return a_allocz(sizeof(long long), capacity); }

static int __a_head_size = sizeof(int) + sizeof(size_t);

static CwtArrayNS __array_ns = {
	  a_alloc
	, a_allocz
	, a_realloc
	, a_free
	, a_clone
	, a_copy
	, a_move
	, a_bzero
	, a_data
	, a_at
	, a_put
	, a_size
	, a_eq

	, a_alloc_char
	, a_alloc_short
	, a_alloc_int
	, a_alloc_long
	, a_alloc_llong
};

DLL_API_EXPORT CwtArrayNS* cwt_array_ns(void)
{
	return &__array_ns;
}


static inline CwtArray* a_alloc (int sizeof_item, size_t capacity)
{
	CwtArray *a;

	if (!sizeof_item)
		return NULL;

	if (!capacity)
		return NULL;

	a = CWT_MALLOC(__a_head_size + capacity * sizeof_item);
	a->sizeof_item = sizeof_item;
	a->capacity = capacity;
	a->alloc = 1;
	return a;
}

CwtArray* a_allocz (int sizeof_item, size_t capacity)
{
	CwtArray *a;
	a = a_alloc(sizeof_item, capacity);
	a_bzero(a);
	return a;
}

/**
 * @brief Reallocate array with preserving the data.
 *
 * @param a Source array.
 * @param new_capacity New capacity.
 * @return Reallocated array.
 */
static CwtArray* a_realloc (CwtArray *a, size_t new_capacity)
{
	CwtArray *aa;

	if (!a)
		return NULL;

	if (!a->alloc)
		return a;

	if (new_capacity == a->capacity) {
		aa = a;
	} else if (new_capacity < a->capacity) {
		aa = realloc(a, __a_head_size + new_capacity * a->sizeof_item);
		a->capacity = new_capacity;
	} else {
		aa = CWT_MALLOC(__a_head_size + new_capacity * a->sizeof_item);
		memcpy(aa, a, __a_head_size + a->capacity * a->sizeof_item);
		aa->capacity = new_capacity;
		aa->sizeof_item = a->sizeof_item;
		CWT_FREE(a);
	}

	return aa;
}

static void a_free (CwtArray *a)
{
	if (a && a->alloc)
		CWT_FREE(a);
}

static CwtArray* a_clone (CwtArray *a)
{
	if (a) {
		CwtArray *clone;
		clone = a_alloc(a->sizeof_item, a->capacity);
		a_copy(clone, a, 0, 0, a->capacity);
		return clone;
	}

	return NULL;
}

/**
 * @fn size_t CwtArrayNS::copy(CwtArray *to, CwtArray *from, size_t n)
 *
 * @brief Copy data from array @c from to array @c to.
 *
 * @details Actually copied minimum number of items
 *          calculated as MIN(from->size, to->size, n).
 *          Items' sizes of arrays must be equal.
 *
 * @param to       Destination array.
 * @param from     Source array.
 * @param off_to   Offset in destination array.
 * @param off_from Offset in source array.
 * @param n        Number of items to copy.
 * @return         Actually items copied.
 */
static size_t a_copy (CwtArray *to, CwtArray *from, size_t off_to, size_t off_from, size_t n)
{
	if (!to || !from)
		return 0;

	if (to->sizeof_item != from->sizeof_item)
		return 0;

	n = CWT_MIN(from->capacity - off_from, n);
	n = CWT_MIN(to->capacity - off_to, n);
	memcpy(to->buffer + off_to * to->sizeof_item
		, from->buffer + off_from * from->sizeof_item
		, n * to->sizeof_item);
	return n;
}

/**
 * @fn size_t CwtArrayNS::move(CwtArray *array, size_t off_to, size_t off_from, size_t n)
 *
 * @brief Move data inside an array.
 *
 * @param a Array.
 * @param off_to Offset to move data.
 * @param off_from Offset from move data.
 * @param n Number of items to move.
 * @return Number of actually moved items.
 */
static size_t a_move (CwtArray *a, size_t off_to, size_t off_from, size_t n)
{
	CWT_ASSERT(a);

	if (off_to >= a->capacity
			|| off_from >= a->capacity)
		return 0;

	n = CWT_MIN(n, a->capacity - off_from);
	n = CWT_MIN(n, a->capacity - off_to);

	memmove(a->buffer + off_to * a->sizeof_item
			, a->buffer + off_from * a->sizeof_item
			, n * a->sizeof_item);

	return n;
}

static inline void a_bzero (CwtArray *a)
{
	if (a)
		memset(&a->buffer[0], 0, a->capacity * a->sizeof_item);
}

static void* a_data (CwtArray *a)
{
	if (!a)
		return NULL;
	return &a->buffer[0];
}

static const void* a_at (CwtArray *a, size_t index)
{
	CWT_ASSERT(a);
	if (index >= a->capacity)
		return NULL;

	return &a->buffer[index * a->sizeof_item];
}

static void* a_put (CwtArray *a, size_t index, const void *item)
{
	char *ptr;

	CWT_ASSERT(a);
	if (index >= a->capacity)
		return NULL;

	ptr = &a->buffer[index * a->sizeof_item];

	switch(a->sizeof_item) {
	case 1: {
			INT8 *d = (INT8 *)ptr;
			*d = *((INT8*)item);
		}
		break;
	case 2: {
			INT16 *d = (INT16 *)ptr;
			*d = *((INT16*)item);
		}
		break;
	case 4: {
			INT32 *d = (INT32 *)ptr;
			*d = *((INT32*)item);
		}
		break;
	case 8: {
			INT64 *d = (INT64 *)ptr;
			*d = *((INT64*)item);
		}
		break;
	default:
		memcpy(ptr, item, a->sizeof_item);
		break;
	}

	return ptr;
}

static BOOL a_eq (CwtArray *a1, CwtArray *a2)
{
	if ( a1 && a2
			&& a1->sizeof_item == a2->sizeof_item
			&& a1->capacity == a2->capacity
			&& memcmp(a1->buffer, a2->buffer, a1->capacity * a1->sizeof_item) == 0 )
		return TRUE;

	return FALSE;
}
