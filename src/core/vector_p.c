/**
 * @file   vector_p.c
 * @author wladt
 * @date   Dec 24, 2012 1:40:27 PM
 *
 * @brief
 */


#include "vector_p.h"


void v_deref_shared (DataShared *d)
{
	d->refs--;

	CWT_ASSERT(d->refs >=0);

	if (!d->refs) {
		CWT_FREE(d->buffer);
		d->buffer = NULL;
		CWT_FREE(d);
	}
}

void v_init_shared (DataShared *d, int sizeof_item, size_t initial_size, size_t max_size)
{
	d->buffer       = CWT_MALLOCA(char, initial_size * sizeof_item);
	d->sizeof_item  = sizeof_item;
	d->refs         = 1;
	d->capacity     = initial_size;
	d->max_capacity = max_size;
}

void v_copy_shared (DataShared *to, DataShared *from, size_t off, size_t count)
{
	v_init_shared (to, from->sizeof_item, from->capacity, from->max_capacity);

	CWT_ASSERT(to->buffer != from->buffer);
	memcpy(to->buffer
			, from->buffer + from->sizeof_item * off
			, from->sizeof_item * count);
}

DataShared* v_clone_shared (DataShared *d, size_t off, size_t count)
{
	DataShared *clone;

	clone = CWT_MALLOCT(DataShared);
	v_copy_shared(clone, d, off, count);
	return clone;
}

BOOL v_right_reserve_shared (CwtVectorImpl *v, size_t n)
{
	size_t available;
	size_t inc = 0;
	DataShared *d;

	CWT_ASSERT(v->d->refs > 0);
	available = v->d->capacity - v->count;

	if (available < n )
		inc = n - available;

	/* avoid an overflow */
	if ( inc > v->d->max_capacity
			|| v->d->max_capacity - inc < v->d->capacity)
		return FALSE;

	/* shift data to left */
	if (!inc && v->d->refs == 1) {
		if (v->head > 0) {
			memmove(v->d->buffer
					, v->d->buffer + v->d->sizeof_item * v->head
					, v->d->sizeof_item * v->count);
			v->head = 0;
		}
		return TRUE;
	}

	if (v->d->refs > 1) {
		d = CWT_MALLOCT(DataShared);
		v_init_shared(d, v->d->sizeof_item, v->d->capacity + inc, v->d->max_capacity);
		v_copy_shared(d, v->d, v->head, v->count);
		v_deref_shared(v->d);
		v->d = d;
		v->head = 0;
	} else {
		char *buffer = v->d->buffer;
		v_init_shared(d, v->d->sizeof_item, v->d->capacity + inc, v->d->max_capacity);
		memcpy(v->d->buffer
				, buffer + v->d->sizeof_item * v->head
				, v->d->sizeof_item * v->count);
	}

	return TRUE;
}
