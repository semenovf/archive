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


static size_t _get_bare_index(RingBufferPtr rb, size_t index)
{
	CWT_ASSERT(index < rb->m_count);

	/* avoid overflow (because rb->m_head + index can cause an overflow) */
	if( rb->m_capacity - rb->m_head > index )
		return rb->m_head + index;

	return index - (rb->m_capacity - rb->m_head);
}


RingBufferPtr rb_new_defaults(void)
{
	return rb_new((size_t)0, (size_t)0);
}

RingBufferPtr rb_new(size_t initial_size, size_t max_size)
{
	RingBufferPtr rb;

	rb = CWT_MALLOC(RingBuffer);
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

void rb_delete(RingBufferPtr rb)
{
	if( rb ) {
		if( rb->m_buffer )
			CWT_FREE(rb->m_buffer);
		CWT_FREE(rb);
	}
}


RingBufferPtr rb_clone(RingBufferPtr rb)
{
	if( rb ) {
		RingBufferPtr clone = rb_new(rb->m_count, default_max_size);
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

size_t rb_capacity(RingBufferPtr rb)
{
	CWT_ASSERT(rb);
	return rb->m_capacity;
}


BOOL rb_is_empty(RingBufferPtr rb)
{
	CWT_ASSERT(rb);
	return (rb->m_count == 0 ? TRUE : FALSE);
}

void rb_clear(RingBufferPtr rb)
{
	CWT_ASSERT(rb);
	rb->m_head = 0;
	rb->m_count = 0;
}

size_t rb_size(RingBufferPtr rb)
{
	CWT_ASSERT(rb);
	return rb->m_count;
}



BOOL rb_reserve(RingBufferPtr rb, size_t n )
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


BYTE rb_at(RingBufferPtr rb, size_t index )
{
	CWT_ASSERT(rb);
	return rb->m_buffer[_get_bare_index(rb, index)];
}

BYTE rb_at_front(RingBufferPtr rb)
{
	CWT_ASSERT(rb);
	return rb->m_buffer[_get_bare_index(rb, 0)];
}

BYTE rb_first(RingBufferPtr rb)
{
	CWT_ASSERT(rb);
	return rb->m_buffer[_get_bare_index(rb, 0)];
}

BYTE rb_at_back(RingBufferPtr rb)
{
	CWT_ASSERT(rb);
	CWT_ASSERT(rb->m_count > 0);
	return rb->m_buffer[_get_bare_index(rb, rb->m_count-1)];
}

BYTE rb_last(RingBufferPtr rb)
{
	CWT_ASSERT(rb);
	CWT_ASSERT(rb->m_count > 0);
	return rb->m_buffer[_get_bare_index(rb, rb->m_count-1)];
}


BOOL rb_push_back(RingBufferPtr rb, const BYTE* bytes, size_t n)
{
	size_t tail;

	CWT_ASSERT(rb);

	if( !rb_reserve(rb, n) )
		return FALSE;

	tail = (rb->m_head + rb->m_count) % rb->m_capacity;

	if( n > 0 ) {
/*
		if( tail + n <= rb->m_capacity ) {
			memcpy(rb->m_buffer + tail, bytes, n);
		} else {
*/
			size_t nn = CWT_MIN(n, rb->m_capacity - tail);
			memcpy(rb->m_buffer + tail, bytes, nn);
			if( nn < n )
				memcpy(rb->m_buffer, bytes + nn, n - nn);
/*		}*/
		rb->m_count += n;
	}

	return TRUE;
}


/* TODO need to optimize */
void rb_pop_front(RingBufferPtr rb, size_t n)
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

void rb_pop_back(RingBufferPtr rb, size_t n)
{
	CWT_ASSERT(rb);

	if( !n )
		return;

	if( rb->m_count > 0 ) {
		n = CWT_MIN(n, rb->m_count);
		rb->m_count -= n;
	}
}


/**
 * Read bytes from ring buffer w/o removing header position.
 * Use @c rb_pop_front to move header position.
 *
 * @param rb
 * @param bytes
 * @param n
 * @return
 *
 * @see rb_pop
 * @see rb_pop_front
 */
ssize_t rb_read(RingBufferPtr rb, BYTE* bytes, size_t n)
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

ssize_t rb_write(RingBufferPtr rb, const BYTE* bytes, size_t n)
{
	CWT_ASSERT(rb);

	if( !n )
		return (ssize_t)0;

	if( !rb_push_back(rb, bytes, (size_t)n) ) {
		return (ssize_t)-1;
	}

	return (ssize_t)n;
}


/**
 * Write bytes from file into ring buffer
 *
 * @param rb the ring buffer
 * @param fd file descriptor
 * @param n max bytes to write
 * @return bytes written into the ring buffer, or -1 if error
 */
ssize_t rb_write_from_file(RingBufferPtr rb, int fd, size_t n)
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

BYTE rb_get(RingBufferPtr rb)
{
	BYTE ch;
	CWT_ASSERT(rb);

	ch = rb_first(rb);
	rb_pop_front(rb, 1);
	return ch;
}

BOOL rb_put(RingBufferPtr rb, BYTE b)
{
	CWT_ASSERT(rb);

	if( !rb_reserve(rb, 1) )
		return FALSE;

	rb->m_count++;
	rb->m_buffer[_get_bare_index(rb, rb->m_count-1)] = b;
	return TRUE;
}


/* TODO can be optimized */
/**
 * Searches @c ch in the ring buffer started from @c *index.
 * Returns the position of the char @c ch found in @c *index.
 *
 * @param rb
 * @param ch
 * @param index
 * @return TRUE if @c ch found in the ring buffer or FALSE if @c ch occurance not found.
 * 	If search is successful position of the character stores in @c *index.
 */
BOOL rb_find_byte(RingBufferPtr rb, BYTE ch, size_t from, size_t* index)
{
	size_t i = 0;

	CWT_ASSERT(rb);

	for( i = from; i < rb->m_count; i++ ) {
		if( rb_at(rb, i) == ch ) {
			if( index )
				*index = i;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL rb_find(RingBufferPtr rb, const BYTE* bytes, size_t n, size_t from, size_t* index)
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

/* obsolete */
int rb_split(
      BYTE delim
	, RingBufferPtr rb
	, size_t len
	, int maxcount
	, void (*on_token)(RingBufferPtr, void*)
	, void* extra)
{
	size_t i;
	BOOL esc = FALSE;       /* escape char */
	BOOL qq_opened = FALSE; /* TRUE if double quote opened */
	BOOL q_opened = FALSE;  /* TRUE if single quote opened */
	RingBufferPtr token;
	int n = 0;

	if( maxcount <= 0 )
		maxcount = CWT_INT_MAX;

	token = rb_new_defaults();

	for( i = 0; i < len; i++ ) {
		BYTE ch = rb_at(rb, i);

		if( esc && delim != '\\' ) {
			rb_put(token, ch);
			esc = FALSE;
			continue;
		}

		/* 'ch' is a delimiter and it is not inside quotes */
		if( ch == delim && !(q_opened || qq_opened) ) {

			if( !rb_size(token) )
				continue;

			/* risk of buffer overflow, break the 'for' loop */
			if( n >= maxcount ) {
				i = len;
				rb_clear(token);
				continue;
			}

			if( on_token ) {
				on_token(token, extra);
			}
			rb_clear(token);
			n++;
			continue;
		}

		switch( ch ) {
		case '\\':
			esc = TRUE;
			continue;

		case '"':
			if( !q_opened ) {
				qq_opened = qq_opened ? FALSE : TRUE;
				continue;
			}
			break;

		case '\'':
			if( !qq_opened ) {
				q_opened = q_opened ? FALSE : TRUE;
				continue;
			}
			break;

		default:
			break;
		}
		rb_put(token, ch);
	}

	if( q_opened || qq_opened ) {
		n = RBE_QUOTE_CHAR_UNBALANCED;
	} else {
		if( rb_size(token) > 0 && n < maxcount ) {
			if( on_token )
				on_token(token, extra);
			n++;
		}
	}

	rb_delete(token);
	return n;
}
