/*
 * ringbuf.c
 *
 *  Created on: 27.11.2011
 *      Author: wladt
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <cwt/unistd.h>
#include <cwt/logger.h>
#include <cwt/ringbuf.h>

static const size_t default_initial_size = 64;
static const size_t default_max_size = CWT_SIZE_T_MAX;

static void           rb_init         (CwtRingBuffer *rb);
static void           rb_init_sized   (CwtRingBuffer *rb, size_t initial_size, size_t max_size);
static void           rb_destroy      (CwtRingBuffer *);
static CwtRingBuffer* rb_create       (void);
static CwtRingBuffer* rb_create_sized (size_t initial_size, size_t max_size);
static void           rb_free         (CwtRingBuffer *rb);
static CwtRingBuffer* rb_clone        (CwtRingBuffer *rb);
static BOOL           rb_reserve      (CwtRingBuffer *rb, size_t n);
static size_t         rb_capacity     (CwtRingBuffer *rb);
static BOOL           rb_is_empty     (CwtRingBuffer *rb);
static void           rb_clear        (CwtRingBuffer *rb);
static size_t         rb_size         (CwtRingBuffer *rb);
static BYTE           rb_at           (CwtRingBuffer *rb, size_t index);
static BYTE           rb_at_front     (CwtRingBuffer *rb);
static BYTE           rb_at_back      (CwtRingBuffer *rb);
static ssize_t        rb_read         (CwtRingBuffer *rb, CwtByteArray *ba, size_t n);
static ssize_t        rb_peek         (CwtRingBuffer *rb, CwtByteArray *ba, size_t n);
static ssize_t        rb_write        (CwtRingBuffer *rb, const BYTE *chars, size_t n);
static BYTE           rb_get          (CwtRingBuffer *rb);
static void           rb_pop_front    (CwtRingBuffer *rb, size_t n);
static void           rb_pop_back     (CwtRingBuffer *rb, size_t n);
static BOOL           rb_put          (CwtRingBuffer *rb, BYTE b);
static BOOL           rb_push_back    (CwtRingBuffer *rb, const BYTE *bytes, size_t n);
static BOOL           rb_find         (CwtRingBuffer *rb, const BYTE *bytes, size_t n, size_t from, size_t *index);
static BOOL           rb_find_any     (CwtRingBuffer *rb, const BYTE *bytes, size_t n, size_t from, size_t *index);

static CwtRingBufferNS __cwtRingBufNS  = {
	  rb_init
	, rb_init_sized
	, rb_destroy
	, rb_create
	, rb_create_sized
	, rb_free
	, rb_clone
	, rb_reserve
	, rb_capacity
	, rb_is_empty
	, rb_clear
	, rb_size
	, rb_at
	, rb_at_front
	, rb_at_front
	, rb_at_back
	, rb_at_back
	, rb_read
	, rb_peek
	, rb_write
	, rb_get
	, rb_pop_front
	, rb_pop_back
	, rb_put
	, rb_push_back
	, rb_find
	, rb_find_any
};

static CwtByteArrayNS *__ba_ns = NULL;

DLL_API_EXPORT CwtRingBufferNS* cwt_ringbuffer_ns(void)
{
	if (!__ba_ns)
		__ba_ns = cwt_bytearray_ns();
	return &__cwtRingBufNS;
}


static inline size_t __get_bare_index(CwtRingBuffer* rb, size_t index)
{
	CWT_ASSERT(index < rb->m_count);

	/* avoid overflow (because rb->m_head + index can cause an overflow) */
	if( rb->m_capacity - rb->m_head > index )
		return rb->m_head + index;

	return index - (rb->m_capacity - rb->m_head);
}


static void rb_init (CwtRingBuffer *rb)
{
	rb_init_sized(rb, (size_t)0, (size_t)0);
}

static void rb_init_sized (CwtRingBuffer *rb, size_t initial_size, size_t max_size)
{
	if (rb) {
		if( !initial_size ) {
			initial_size = default_initial_size;
		}

		if( !max_size ) {
			max_size = default_max_size;
		}

		if( initial_size > max_size ) {
			initial_size = max_size;
		}

		rb->m_buffer = CWT_MALLOCA(BYTE, initial_size);
		CWT_ASSERT(rb->m_buffer);

		rb->m_capacity = initial_size;
		rb->m_count = 0;
		rb->m_head = 0;
		rb->m_max_capacity = max_size;
	}
}

static void rb_destroy (CwtRingBuffer *rb)
{
	if( rb ) {
		if( rb->m_buffer )
			CWT_FREE(rb->m_buffer);
	}
}

static CwtRingBuffer* rb_create(void)
{
	return rb_create_sized((size_t)0, (size_t)0);
}


static CwtRingBuffer* rb_create_sized(size_t initial_size, size_t max_size)
{
	CwtRingBuffer* rb;

	rb = CWT_MALLOCT(CwtRingBuffer);
	CWT_ASSERT(rb);
	rb_init_sized(rb, initial_size, max_size);
	return rb;
}


static void rb_free(CwtRingBuffer* rb)
{
	if( rb ) {
		rb_destroy(rb);
		CWT_FREE(rb);
	}
}


static CwtRingBuffer*   rb_clone(CwtRingBuffer *rb)
{
	if( rb ) {
		CwtRingBuffer *clone = rb_create_sized(rb->m_count, default_max_size);
		if( rb->m_count > 0 ) {
			if( rb->m_capacity - rb->m_head >= rb->m_count ) {
				memcpy(clone->m_buffer, rb->m_buffer + rb->m_head, rb->m_count);
			} else {
				memcpy(clone->m_buffer, rb->m_buffer + rb->m_head, rb->m_capacity - rb->m_head);
				memcpy(clone->m_buffer + rb->m_capacity - rb->m_head, rb->m_buffer, rb->m_count - (rb->m_capacity - rb->m_head));
			}
			clone->m_count = rb->m_count;
		}
		return clone;
	}
	return NULL;
}



static BOOL    rb_reserve(CwtRingBuffer *rb, size_t n )
{
	size_t available;
	size_t inc;

	CWT_ASSERT(rb);

	available = rb->m_capacity - rb->m_count;
	inc = n - available;

	if( available < n ) {
		BYTE *buffer;
		size_t capacity;

		capacity = rb->m_capacity + inc;

		if( capacity > rb->m_max_capacity )
			return FALSE;

		buffer = CWT_MALLOCA(BYTE, capacity);

		if( rb->m_capacity - rb->m_head >= rb->m_count ) {
			memcpy(buffer, rb->m_buffer + rb->m_head, rb->m_count);
		} else {
			memcpy(buffer, rb->m_buffer + rb->m_head, rb->m_capacity - rb->m_head);
			memcpy(buffer + rb->m_capacity - rb->m_head, rb->m_buffer, rb->m_count - (rb->m_capacity - rb->m_head));
		}

		CWT_FREE(rb->m_buffer);
		rb->m_buffer = buffer;
		rb->m_head = 0;
		rb->m_capacity = capacity;
	}

	return TRUE;
}


static size_t rb_capacity(CwtRingBuffer *rb)
{
	CWT_ASSERT(rb);
	return rb->m_capacity;
}


static BOOL rb_is_empty(CwtRingBuffer *rb)
{
	CWT_ASSERT(rb);
	return (rb->m_count == 0 ? TRUE : FALSE);
}


static void rb_clear(CwtRingBuffer *rb)
{
	CWT_ASSERT(rb);
	rb->m_head = 0;
	rb->m_count = 0;
}


static size_t rb_size(CwtRingBuffer *rb)
{
	CWT_ASSERT(rb);
	return rb->m_count;
}


static BYTE rb_at(CwtRingBuffer *rb, size_t index )
{
	CWT_ASSERT(rb);
	return rb->m_buffer[__get_bare_index(rb, index)];
}

static BYTE rb_at_front(CwtRingBuffer *rb)
{
	CWT_ASSERT(rb);
	return rb->m_buffer[__get_bare_index(rb, 0)];
}

static BYTE rb_at_back(CwtRingBuffer *rb)
{
	CWT_ASSERT(rb);
	CWT_ASSERT(rb->m_count > 0);
	return rb->m_buffer[__get_bare_index(rb, rb->m_count-1)];
}


static ssize_t rb_read(CwtRingBuffer *rb, CwtByteArray *ba, size_t n)
{
	ssize_t br = rb_peek(rb, ba, n);
	if( br > 0 )
		rb_pop_front(rb, br);
	return br;
}

/**
 * Read bytes from ring buffer w/o removing header position.
 * Use @c popFront to move header position.
 *
 * @param rb
 * @param bytes
 * @param n Number of bytes to read, must be less or equal CWT_SSIZE_T_MAX.
 *          If n > CWT_SSIZE_T_MAX.
 * @return No more than MIN(n,CWT_SSIZE_T_MAX,rb_size) bytes from buffer.
 *
 * @see pop
 * @see popFront
 */
static ssize_t rb_peek(CwtRingBuffer *rb, CwtByteArray *ba, size_t n)
{

	CWT_ASSERT(rb);

	if( !n )
		return (ssize_t)0;

	n = CWT_MIN(n, rb->m_count);
	n = CWT_MIN(n, CWT_SSIZE_T_MAX);

	if( rb->m_capacity - rb->m_head >= n ) {
		__ba_ns->appendBytes(ba, rb->m_buffer + rb->m_head, n);
	} else {
		size_t c0;
		size_t c1;
		c0 = rb->m_capacity - rb->m_head;
		c1 = (size_t)n - c0;

		__ba_ns->appendBytes(ba, rb->m_buffer + rb->m_head, c0);
		if( c1 )
			__ba_ns->appendBytes(ba, rb->m_buffer, c1);
	}

	return (ssize_t)n;
}



static ssize_t rb_write(CwtRingBuffer *rb, const BYTE* bytes, size_t n)
{
	CWT_ASSERT(rb);

	if( !n )
		return (ssize_t)0;

	if( !rb_push_back(rb, bytes, (size_t)n) ) {
		return (ssize_t)-1;
	}

	return (ssize_t)n;
}


static BYTE rb_get(CwtRingBuffer *rb)
{
	BYTE ch;
	CWT_ASSERT(rb);

	ch = rb_at_front(rb);
	rb_pop_front(rb, 1);
	return ch;
}


/* TODO need to optimize */
static void rb_pop_front(CwtRingBuffer* rb, size_t n)
{
	CWT_ASSERT(rb);

	if( !n )
		return;

	if( rb->m_count > 0 ) {
		n = CWT_MIN(n, rb->m_count);
		while( n-- ) {
			rb->m_count--;
			rb->m_head++;
			if( rb->m_head >= rb->m_capacity )
				rb->m_head = 0;
		}
	}
}


static void rb_pop_back(CwtRingBuffer* rb, size_t n)
{
	CWT_ASSERT(rb);

	if( !n )
		return;

	if( rb->m_count > 0 ) {
		n = CWT_MIN(n, rb->m_count);
		rb->m_count -= n;
	}
}


static BOOL rb_put(CwtRingBuffer* rb, BYTE b)
{
	CWT_ASSERT(rb);

	if( !   rb_reserve(rb, 1) )
		return FALSE;

	rb->m_count++;
	rb->m_buffer[__get_bare_index(rb, rb->m_count-1)] = b;
	return TRUE;
}


static BOOL rb_push_back(CwtRingBuffer* rb, const BYTE* bytes, size_t n)
{
	size_t tail;

	CWT_ASSERT(rb);

	if( !   rb_reserve(rb, n) )
		return FALSE;

	tail = (rb->m_head + rb->m_count) % rb->m_capacity;

	if( n > 0 ) {
		size_t nn = CWT_MIN(n, rb->m_capacity - tail);
		memcpy(rb->m_buffer + tail, bytes, nn);
		if( nn < n )
			memcpy(rb->m_buffer, bytes + nn, n - nn);
		rb->m_count += n;
	}

	return TRUE;
}


static BOOL rb_find(CwtRingBuffer *rb, const BYTE* bytes, size_t n, size_t from, size_t* index)
{
	size_t i = 0;
	size_t j = 0;

	CWT_ASSERT(rb);

	for( i = from; i < rb->m_count; i++ ) {
		if( rb_at(rb, i) == bytes[0] ) {

			if( i + n > rb->m_count ) {
				return FALSE;
			}

			for( j = 1; j < n; j++ ) {
				if( rb_at(rb, i+j) != bytes[j] )
					break;
			}

			if( j == n ) {
				if( index )
					*index = i;
				return TRUE;
			}
		}
	}
	return FALSE;
}

/* TODO can be optimized */
/**
 * @brief Finds first occurrence of any character from @c bytes
 *
 * @param rb
 * @param bytes
 * @param n
 * @param from
 * @param index
 * @return
 */
static BOOL rb_find_any(CwtRingBuffer *rb, const BYTE* bytes, size_t n, size_t from, size_t* index)
{
	size_t i = 0, j = 0;

	CWT_ASSERT(rb);

	for( i = from; i < rb->m_count; i++ ) {
		for( j = 0; j < n; j++ ) {
			if( rb_at(rb, i) == bytes[j] ) {
				if( index )
					*index = i;
				return TRUE;
			}
		}
	}
	return FALSE;
}

#ifdef __COMMENT__

/**
 * Write bytes from file into ring buffer
 *
 * @param rb the ring buffer
 * @param fd file descriptor
 * @param n max bytes to write
 * @return bytes written into the ring buffer, or -1 if error
 */
ssize_t rb_write_from_file(CwtRingBuffer* rb, int fd, size_t n)
{
	BYTE ibuf[256];
	size_t total_br = 0;
	CwtUnistdNS *ns = cwt_unistd_ns();

	if( fd < 0 ) {
		print_error(_Tr("invalid file descriptor"));
		return 0;
	}

	while( total_br < n ) {
		/* TODO fix unsigned int cast */
		ssize_t br = ns->read(fd, ibuf, (UINT)(n - total_br));

		if( br == 0 ) /* end of file */
			break;

		if( br < 0 ) {
			printf_error(_Tr("read file error: %s"), strerror(errno));
			return (ssize_t)-1;
		}

		total_br += br;

		if( !rb_push_back(rb, ibuf, br) ) {
			return (ssize_t)-1;
		}
	}

	return (ssize_t)total_br;
}



#endif
