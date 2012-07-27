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


static CwtRingBuf*   __create     (void);
static CwtRingBuf*   __createSized(size_t initial_size, size_t max_size);
static void          __free       (CwtRingBuf*);
static CwtRingBuf*   __clone      (CwtRingBuf*);
static BOOL          __reserve    (CwtRingBuf*, size_t n);
static size_t        __capacity   (CwtRingBuf*);
static BOOL          __isEmpty    (CwtRingBuf*);
static void          __clear      (CwtRingBuf*);
static size_t        __size       (CwtRingBuf*);
static BYTE          __at         (CwtRingBuf*, size_t index);
static BYTE          __atFront    (CwtRingBuf*);
/*static BYTE          __first      (CwtRingBuf*);*/
static BYTE          __atBack     (CwtRingBuf*);
/*static BYTE          __last       (CwtRingBuf*);*/
static ssize_t       __read       (CwtRingBuf*, BYTE* bytes, size_t n);
static ssize_t       __write      (CwtRingBuf*, const BYTE* chars, size_t n);
static BYTE          __get        (CwtRingBuf*);
static void          __popFront   (CwtRingBuf*, size_t n);
static void          __popBack    (CwtRingBuf*, size_t n);
static BOOL          __put        (CwtRingBuf*, BYTE b);
static BOOL          __pushBack   (CwtRingBuf*, const BYTE* bytes, size_t n);
static BOOL          __find       (CwtRingBuf *rb, const BYTE* bytes, size_t n, size_t from, size_t* index);
static BOOL          __findAny    (CwtRingBuf*, const BYTE* bytes, size_t n, size_t from, size_t* index);

static CwtRingBufNS __cwtRingBufNS  = {
	  __create
	, __createSized
	, __free
	, __clone
	, __reserve
	, __capacity
	, __isEmpty
	, __clear
	, __size
	, __at
	, __atFront
	, __atFront
	, __atBack
	, __atBack
	, __read
	, __write
	, __get
	, __popFront
	, __popBack
	, __put
	, __pushBack
	, __find
	, __findAny
};


DLL_API_EXPORT CwtRingBufNS* cwtRingBufNS(void)
{
	return &__cwtRingBufNS;
}


static size_t __get_bare_index(CwtRingBuf* rb, size_t index)
{
	CWT_ASSERT(index < rb->m_count);

	/* avoid overflow (because rb->m_head + index can cause an overflow) */
	if( rb->m_capacity - rb->m_head > index )
		return rb->m_head + index;

	return index - (rb->m_capacity - rb->m_head);
}


static CwtRingBuf* __create(void)
{
	return __createSized((size_t)0, (size_t)0);
}


static CwtRingBuf* __createSized(size_t initial_size, size_t max_size)
{
	CwtRingBuf* rb;

	rb = CWT_MALLOC(CwtRingBuf);
	CWT_ASSERT(rb);

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

	return rb;
}


static void __free(CwtRingBuf* rb)
{
	if( rb ) {
		if( rb->m_buffer )
			CWT_FREE(rb->m_buffer);
		CWT_FREE(rb);
	}
}


static CwtRingBuf* __clone(CwtRingBuf *rb)
{
	if( rb ) {
		CwtRingBuf *clone = __createSized(rb->m_count, default_max_size);
		if( rb->m_count > 0 ) {
			/* '墮��' ��室���� �ࠢ�� '������' */
			if( rb->m_capacity - rb->m_head >= rb->m_count ) {
				memcpy(clone->m_buffer, rb->m_buffer + rb->m_head, rb->m_count);
			} else { /* '墮��' ��室���� ����� '������' */
				memcpy(clone->m_buffer, rb->m_buffer + rb->m_head, rb->m_capacity - rb->m_head);
				memcpy(clone->m_buffer + rb->m_capacity - rb->m_head, rb->m_buffer, rb->m_count - (rb->m_capacity - rb->m_head));
			}
			clone->m_count = rb->m_count;
		}
		return clone;
	}
	return NULL;
}



static BOOL __reserve(CwtRingBuf *rb, size_t n )
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

		/* '墮��' ��室���� �ࠢ�� '������' */
		if( rb->m_capacity - rb->m_head >= rb->m_count ) {
			memcpy(buffer, rb->m_buffer + rb->m_head, rb->m_count);
		} else { /* '墮��' ��室���� ����� '������' */
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


static size_t __capacity(CwtRingBuf *rb)
{
	CWT_ASSERT(rb);
	return rb->m_capacity;
}


static BOOL __isEmpty(CwtRingBuf *rb)
{
	CWT_ASSERT(rb);
	return (rb->m_count == 0 ? TRUE : FALSE);
}


static void __clear(CwtRingBuf *rb)
{
	CWT_ASSERT(rb);
	rb->m_head = 0;
	rb->m_count = 0;
}


static size_t __size(CwtRingBuf *rb)
{
	CWT_ASSERT(rb);
	return rb->m_count;
}


static BYTE __at(CwtRingBuf *rb, size_t index )
{
	CWT_ASSERT(rb);
	return rb->m_buffer[__get_bare_index(rb, index)];
}

static BYTE __atFront(CwtRingBuf *rb)
{
	CWT_ASSERT(rb);
	return rb->m_buffer[__get_bare_index(rb, 0)];
}

static BYTE __atBack(CwtRingBuf *rb)
{
	CWT_ASSERT(rb);
	CWT_ASSERT(rb->m_count > 0);
	return rb->m_buffer[__get_bare_index(rb, rb->m_count-1)];
}


/**
 * Read bytes from ring buffer w/o removing header position.
 * Use @c popFront to move header position.
 *
 * @param rb
 * @param bytes
 * @param n
 * @return
 *
 * @see pop
 * @see popFront
 */
static ssize_t __read(CwtRingBuf *rb, BYTE* bytes, size_t n)
{

	CWT_ASSERT(rb);

	if( !n )
		return (ssize_t)0;

	n = CWT_MIN(n, rb->m_count);

	if( rb->m_capacity - rb->m_head >= n ) {
		memcpy(bytes, rb->m_buffer + rb->m_head, n);
	} else {
		size_t c0;
		size_t c1;
		c0 = rb->m_capacity - rb->m_head;
		c1 = (size_t)n - c0;
		memcpy(bytes, rb->m_buffer + rb->m_head, c0);
		if( c1 ) {
			memcpy(bytes + c0, rb->m_buffer, c1);
		}
	}

	return (ssize_t)n;
}


static ssize_t __write(CwtRingBuf *rb, const BYTE* bytes, size_t n)
{
	CWT_ASSERT(rb);

	if( !n )
		return (ssize_t)0;

	if( !__pushBack(rb, bytes, (size_t)n) ) {
		return (ssize_t)-1;
	}

	return (ssize_t)n;
}


static BYTE __get(CwtRingBuf *rb)
{
	BYTE ch;
	CWT_ASSERT(rb);

	ch = __atFront(rb);
	__popFront(rb, 1);
	return ch;
}


/* TODO need to optimize */
static void __popFront(CwtRingBuf* rb, size_t n)
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


static void __popBack(CwtRingBuf* rb, size_t n)
{
	CWT_ASSERT(rb);

	if( !n )
		return;

	if( rb->m_count > 0 ) {
		n = CWT_MIN(n, rb->m_count);
		rb->m_count -= n;
	}
}


static BOOL __put(CwtRingBuf* rb, BYTE b)
{
	CWT_ASSERT(rb);

	if( !__reserve(rb, 1) )
		return FALSE;

	rb->m_count++;
	rb->m_buffer[__get_bare_index(rb, rb->m_count-1)] = b;
	return TRUE;
}


static BOOL __pushBack(CwtRingBuf* rb, const BYTE* bytes, size_t n)
{
	size_t tail;

	CWT_ASSERT(rb);

	if( !__reserve(rb, n) )
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


static BOOL __find(CwtRingBuf *rb, const BYTE* bytes, size_t n, size_t from, size_t* index)
{
	size_t i = 0;
	size_t j = 0;

	CWT_ASSERT(rb);

	for( i = from; i < rb->m_count; i++ ) {
		if( __at(rb, i) == bytes[0] ) {

			if( i + n > rb->m_count ) {
				return FALSE;
			}

			for( j = 1; j < n; j++ ) {
				if( __at(rb, i+j) != bytes[j] )
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
static BOOL __findAny(CwtRingBuf *rb, const BYTE* bytes, size_t n, size_t from, size_t* index)
{
	size_t i = 0, j = 0;

	CWT_ASSERT(rb);

	for( i = from; i < rb->m_count; i++ ) {
		for( j = 0; j < n; j++ ) {
			if( __at(rb, i) == bytes[j] ) {
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
ssize_t rb_write_from_file(CwtRingBuf* rb, int fd, size_t n)
{
	BYTE ibuf[256];
	size_t total_br = 0;
	CwtUnistdNS *ns = cwtUnistdNS();

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
