/**
 * @file   array_p.hpp
 * @author wladt
 * @date   Feb 18, 2013 4:25:32 PM
 *
 * @brief
 */

#ifndef __CWT_ARRAY_P_HPP__
#define __CWT_ARRAY_P_HPP__

#include <cwt/cwt.h>

CWT_NS_BEGIN

struct Array {
	size_t capacity;
	char buffer[1];

	static Array*   alloc   (size_t item_size, size_t capacity);
	static Array*   allocz  (size_t item_size, size_t capacity);

	static Array*   realloc (Array *a, size_t new_capacity);
	static void     free    (Array *a);
	static Array*   clone   (const Array &a);
	static size_t   copy    (Array *, Array *, size_t off_to, size_t off_from, size_t n);
	static size_t   move    (Array *a, size_t off_to, size_t off_from, size_t n);
	static void     bzero   (Array &a);
	static _T       data    (Array &a);
	static const _T data    (const Array &a);
	static size_t   size    (const Array &a);
	static bool     eq      (const Array &a1, const Array &a2);
};

//#include <string.h> /* for memcpy */

Array* Array::alloc (size_t item_size, size_t capacity)
{
	Array *a;

	if (!capacity)
		return NULL;

	a = (Array*)new char[sizeof(size_t) + capacity * item_size];
	a->capacity = capacity;
	return a;
}

Array* Array::allocz (size_t item_size, size_t capacity)
{
	Array *a = alloc(item_size, capacity);
	bzero(a);
	return a;
}


/**
* @brief Reallocate array with preserving the data.
*
* @param a Source array.
* @param new_capacity New capacity.
* @return Reallocated array.
*/
template <typename _T>
Array* Array::realloc (Array *a, size_t new_capacity)
{
	Array *aa;

	if (!a)
		return NULL;

	if (new_capacity == a->capacity) {
		aa = a;
	} else {
		aa = (Array*)new char[sizeof(size_t) + capacity * sizeof(_T)];
		memcpy(aa, a, sizeof(size_t) + a->capacity * sizeof(_T));
		aa->capacity = new_capacity;
		delete a;
	}

	return aa;
}

template <typename _T>
void Array::free (Array *a)
{
	if (a)
		delete a;
}

template <typename _T>
static CwtArray* Array::clone (const Array &a)
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
* calculated as MIN(from->size, to->size, n).
* Items' sizes of arrays must be equal.
*
* @param to Destination array.
* @param from Source array.
* @param off_to Offset in destination array.
* @param off_from Offset in source array.
* @param n Number of items to copy.
* @return Actually items copied.
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

CWT_NS_END

#endif /* __CWT_ARRAY_P_HPP__ */
