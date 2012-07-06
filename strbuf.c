/*
 * strbuf.c
 *
 *  Created on: 13.01.2012
 *      Author: wladt
 */


#include <stdlib.h>
#include <cwt/string.h>
#include <cwt/strbuf.h>

static const size_t __default_initial_size = 64;
static const size_t __default_max_size = CWT_SIZE_T_MAX;


StringBufferPtr strbuf_new_defaults(void)
{
	return strbuf_new((size_t)0, (size_t)0);
}

StringBufferPtr strbuf_new(size_t initial_size, size_t max_size)
{
	StringBufferPtr sb;

	sb = CWT_MALLOC(StringBuffer);
	CWT_ASSERT(sb);

	if( !initial_size ) {
		initial_size = __default_initial_size;
	}

	if( !max_size ) {
		max_size = __default_max_size;
	}

	if( initial_size > max_size ) {
		initial_size = max_size;
	}

	sb->m_buffer = CWT_MALLOCA(CHAR, initial_size);
	CWT_ASSERT(sb->m_buffer);

	sb->m_capacity = initial_size;
	sb->m_count = 0;
	sb->m_max_capacity = max_size;

	return sb;
}

void strbuf_delete(StringBufferPtr sb)
{
	if( sb ) {
		if( sb->m_buffer )
			CWT_FREE(sb->m_buffer);
		CWT_FREE(sb);
	}
}


StringBufferPtr strbuf_clone(StringBufferPtr sb)
{
	if( sb ) {
		StringBufferPtr clone = strbuf_new(sb->m_count, __default_max_size);
		if( sb->m_count > 0 ) {
			memcpy(clone->m_buffer, sb->m_buffer/* + sb->m_head*/, sb->m_count);
			clone->m_count = sb->m_count;
		}
		return clone;
	}
	return NULL;
}


BOOL strbuf_reserve(StringBufferPtr sb, size_t n)
{
	size_t available;
	size_t inc;

	CWT_ASSERT(sb);

	available = sb->m_capacity - sb->m_count;
	inc = n - available;

	if( available < n ) {
		CHAR *buffer;
		size_t capacity;

		capacity = sb->m_capacity + inc;

		if( capacity > sb->m_max_capacity )
			return FALSE;

		buffer = CWT_MALLOCA(CHAR, capacity);

		memcpy(buffer, sb->m_buffer/* + sb->m_head*/, sb->m_count);

		CWT_FREE(sb->m_buffer);
		sb->m_buffer = buffer;
		/*sb->m_head = 0;*/
		sb->m_capacity = capacity;
	}

	return TRUE;
}


/**
 * Reserve space from leftmost side of string buffer's head
 *
 * @param sb
 * @param n
 * @return
 */
BOOL strbuf_lreserve(StringBufferPtr sb, size_t n)
{
	size_t inc;
	CHAR *buffer;
	size_t capacity;

	CWT_ASSERT(sb);

/*	if( sb->m_head < n ) {*/

		inc = n/* - sb->m_head*/;

		capacity = sb->m_capacity + inc;

		if( capacity > sb->m_max_capacity )
			return FALSE;

		buffer = CWT_MALLOCA(CHAR, capacity);

		memcpy(buffer + n, sb->m_buffer/* + sb->m_head*/, sb->m_count);

		CWT_FREE(sb->m_buffer);
		sb->m_buffer = buffer;
		sb->m_capacity = capacity;
/*	}*/

	return TRUE;
}


/**
 * Reserve specified space in the middle of string buffer
 *
 * @param sb
 * @param n
 * @return
 */
BOOL strbuf_mreserve(StringBufferPtr sb, size_t n, size_t pos)
{
	size_t inc;
	CHAR *buffer;
	size_t capacity;

	CWT_ASSERT(sb);

	if( pos >= sb->m_count )
		return strbuf_reserve(sb, n);

	if( pos == 0 )
		return strbuf_lreserve(sb, n);

	inc = n;

	capacity = sb->m_capacity + inc;

	if( capacity > sb->m_max_capacity )
		return FALSE;

	buffer = CWT_MALLOCA(CHAR, capacity);

	memcpy(buffer, sb->m_buffer/* + sb->m_head*/, pos);
	memcpy(buffer+pos+n, sb->m_buffer /*+ sb->m_head*/ + pos, sb->m_count - pos);

	CWT_FREE(sb->m_buffer);
	sb->m_buffer = buffer;
	sb->m_capacity = capacity;

	return TRUE;
}

size_t strbuf_capacity(StringBufferPtr sb)
{
	CWT_ASSERT(sb);
	return sb->m_capacity;
}


BOOL strbuf_is_empty(StringBufferPtr sb)
{
	CWT_ASSERT(sb);
	return (sb->m_count == 0 ? TRUE : FALSE);
}


void strbuf_clear(StringBufferPtr sb)
{
	CWT_ASSERT(sb);
/*	sb->m_head = 0;*/
	sb->m_count = 0;
}

size_t strbuf_size(StringBufferPtr sb)
{
	CWT_ASSERT(sb);
	return sb->m_count;
}


void strbuf_append_char(StringBufferPtr sb, CHAR ch)
{
	CWT_ASSERT(sb);
	CWT_ASSERT(strbuf_reserve(sb, 1));

	sb->m_buffer[/*sb->m_head + */sb->m_count] = ch;
	sb->m_count++;
}


void strbuf_append_chars(StringBufferPtr sb, const CHAR *s, size_t n)
{
	CWT_ASSERT(strbuf_reserve(sb, n));

	memcpy(sb->m_buffer/* + sb->m_head*/ + sb->m_count, s, n);
	sb->m_count += n;
}

void strbuf_append(StringBufferPtr sb, const CHAR *s)
{
	strbuf_append_chars(sb, s, strlen(s));
}

void strbuf_prepend_char(StringBufferPtr sb, CHAR ch)
{
	CWT_ASSERT(sb);

	CWT_ASSERT(strbuf_lreserve(sb, 1));
/*	sb->m_head--;*/
	sb->m_buffer[0] = ch;
	sb->m_count++;
}


void strbuf_insert_char(StringBufferPtr sb, CHAR ch, size_t pos)
{
	CWT_ASSERT(sb);

	CWT_ASSERT(strbuf_mreserve(sb, 1, pos));
	sb->m_buffer[/*sb->m_head + */pos] = ch;
	sb->m_count++;
}

void strbuf_insert(StringBufferPtr sb, const CHAR *chars, size_t nchars, size_t pos)
{
	size_t i;
	CWT_ASSERT(sb);

	CWT_ASSERT(strbuf_mreserve(sb, nchars, pos));

	/* TODO replace this code by memcpy function call */
	for( i = 0; i < nchars; i++ )
		sb->m_buffer[/*sb->m_head + */pos + i] = chars[i];
	sb->m_count += nchars;
}

void strbuf_remove_char(StringBufferPtr sb, size_t pos)
{
	CWT_ASSERT(sb);

	if( !sb->m_count ) /* empty buffer */
		return;

	if( pos < sb->m_count ) {
		sb->m_count--;
		memmove(&sb->m_buffer[/*sb->m_head + */pos], &sb->m_buffer[/*sb->m_head + */pos + 1], sb->m_count * sizeof(CHAR) );
	}
}



/* TODO
void strbuf_remove(StringBufferPtr sb, size_t nchars, size_t pos)
{
	CWT_ASSERT(sb);

	if( !sb->m_count )  empty buffer
		return;

	nchars = CWT_MIN(nchars, sb->m_count - pos);

	if( pos >= sb->m_count - 1 ) {  remove last char
		sb->m_count--;
	} else {
		memmove(&sb->m_buffer[sb->m_head + pos], &sb->m_buffer[sb->m_head + pos + 1], sizeof(CHAR) );
	}
}
*/



const CHAR* strbuf_cstr(StringBufferPtr sb)
{
	strbuf_append_char(sb, '\x0');
	sb->m_count--;
	return sb->m_buffer;
}


CHAR* strbuf_substr(StringBufferPtr sb, size_t start, size_t nchars)
{
	CHAR *chars = NULL;

	if( start < sb->m_count ) {
		nchars = CWT_MIN(nchars, sb->m_count - start);
		chars = CWT_MALLOCA(CHAR, nchars+1);
		strncpy(chars, sb->m_buffer/* + sb->m_head*/ + start, nchars);
		chars[nchars] = '\x0';
	}

	return chars;
}


/**
 * @brief Searches first occurrence of character @c ch in string buffer
 * starting from the position @c offset
 *
 * @param sb string buffer
 * @param ch character to search
 * @param offset starting position
 * @return @c true if found and *offset is set to a valid position, or @c false
 */
BOOL strbuf_find(StringBufferPtr sb, CHAR ch, size_t *offset)
{
	CHAR *ptr;
	size_t off;

	CWT_ASSERT(sb);
	CWT_ASSERT(offset);

	if( *offset >= sb->m_count )
		return FALSE;

	off = *offset;
	ptr = sb->m_buffer + off;

	while( off < sb->m_count ) {
		if( *ptr == ch ) {
			*offset = off;
			return TRUE;
		}
		ptr++;
		off++;
	}

	return FALSE;
}
