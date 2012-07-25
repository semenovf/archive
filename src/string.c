/*
 * strbuf.c
 *
 *  Created on: 13.01.2012
 *      Author: wladt
 */


#include <stdlib.h>
#include <cwt/string.h>
#include <cwt/str.h>

static CwtStringNS __CwtStringNS;

static append_string_f __append(CwtString *s, const CWT_CHAR *str);
static const CWT_CHAR* __cstr(CwtString *s);

/*static append_string_f __append_ptr = __append;*/

CWT_BEGIN_DEF_VECTOR_NS(CwtStringNS, CwtString, CWT_CHAR)
	, __appendElem
	, __appendElems
	, __prependElem
	, __insertElem
	, __removeElem
    , __append
	, __cstr
CWT_END_DEF_VECTOR_NS(CwtStringNS)


DLL_API_EXPORT CwtStringNS* cwtStringNS(void)
{
	return &__CwtStringNS;
}

CWT_VECTOR_METHODS(CwtString,CWT_CHAR)


static append_string_f __append(CwtString *s, const CWT_CHAR *str)
{
	__appendElems(s, str, cwtStrNS()->strlen(str));
	return &__append;
}


static const CWT_CHAR* __cstr(CwtString *sb)
{
	__appendElem(sb, _T('\x0'));
	sb->m_count--;
	return sb->m_buffer;
}


#ifdef __COMMENT__
static const size_t __default_initial_size = 64;
static const size_t __default_max_size = CWT_SIZE_T_MAX;

static CwtStringBufferPtr __createSized(size_t initial_size, size_t max_size);
static CwtStringBufferPtr __create(void);
static void               __init(CwtStringBufferPtr sb);
static void               __initSized(CwtStringBufferPtr sb, size_t initial_size, size_t max_size);
static void               __destroy(CwtStringBufferPtr sb);
static void               __free(CwtStringBufferPtr sb);
static CwtStringBufferPtr __clone(CwtStringBufferPtr sb);
static BOOL               __reserve(CwtStringBufferPtr sb, size_t n);
static BOOL               __lreserve(CwtStringBufferPtr sb, size_t n);
static BOOL               __mreserve(CwtStringBufferPtr sb, size_t n, size_t pos);
static size_t             __capacity(CwtStringBufferPtr sb);
static BOOL               __isEmpty(CwtStringBufferPtr sb);
static void               __clear(CwtStringBufferPtr sb);
static size_t             __size(CwtStringBufferPtr sb);
static void               __appendChar(CwtStringBufferPtr sb, CWT_CHAR ch);
static void               __appendChars(CwtStringBufferPtr sb, const CWT_CHAR *s, size_t n);
static void               __append(CwtStringBufferPtr sb, const CWT_CHAR *s);
static void               __prependChar(CwtStringBufferPtr sb, CWT_CHAR ch);
static void               __insertChar(CwtStringBufferPtr sb, CWT_CHAR ch, size_t pos);
static void               __insert(CwtStringBufferPtr sb, const CWT_CHAR *chars, size_t nchars, size_t pos);
static void               __removeChar(CwtStringBufferPtr sb, size_t pos);
static const CWT_CHAR*    __cstr(CwtStringBufferPtr sb);
static CWT_CHAR*          __substr(CwtStringBufferPtr sb, size_t start, size_t nchars);
static BOOL               __find(CwtStringBufferPtr sb, CWT_CHAR ch, size_t *offset);

static CwtStringNS  __cwtStringBufferNS = {
	  0
	, __create
	, __createSized
	, __init
	, __destroy
	, __free
	, __clone
	, __reserve
	, __lreserve
	, __mreserve
	, __capacity
	, __isEmpty
	, __clear
	, __size
	, __size
	, __appendChar
	, __appendChars
	, __append
	, __prependChar
	, __insertChar
	, __insert
	, __removeChar
	, __cstr
	, __substr
	, __find
};


static CwtStringBufferPtr __create(void)
{
	return __createSized((size_t)0, (size_t)0);
}

static CwtStringBufferPtr __createSized(size_t initial_size, size_t max_size)
{
	CwtStringBufferPtr sb;

	sb = CWT_MALLOC(CwtStringBuffer);
	CWT_ASSERT(sb);
	__initSized(sb, initial_size, max_size);
	__cwtStringBufferNS.refs++;
	return sb;
}

static void __initSized(CwtStringBufferPtr sb, size_t initial_size, size_t max_size)
{
	if( !initial_size ) {
		initial_size = __default_initial_size;
	}

	if( !max_size ) {
		max_size = __default_max_size;
	}

	if( initial_size > max_size ) {
		initial_size = max_size;
	}

	sb->m_buffer = CWT_MALLOCA(CWT_CHAR, initial_size);
	CWT_ASSERT(sb->m_buffer);

	sb->m_capacity = initial_size;
	sb->m_count = 0;
	sb->m_max_capacity = max_size;
}

static void __init(CwtStringBufferPtr sb)
{
	__initSized(sb, 0, 0);
}


static void __destroy(CwtStringBufferPtr sb)
{
	if( sb && sb->m_buffer ) {
		CWT_FREE(sb->m_buffer);
		sb->m_buffer = NULL;
	}
}

static void __free(CwtStringBufferPtr sb)
{
	__destroy(sb);
	CWT_FREE(sb);
	__cwtStringBufferNS.refs--;
}


static CwtStringBufferPtr __clone(CwtStringBufferPtr sb)
{
	if( sb ) {
		CwtStringBufferPtr clone = __createSized(sb->m_count, __default_max_size);
		if( sb->m_count > 0 ) {
			CwtStrNS *stringNS = cwtStrNS();
			cwtStrNS()->memcpy(clone->m_buffer, sb->m_buffer, sb->m_count * sizeof(CWT_CHAR));
			clone->m_count = sb->m_count;
		}
		return clone;
	}
	return NULL;
}


static BOOL __reserve(CwtStringBufferPtr sb, size_t n)
{
	size_t available;
	size_t inc;

	CWT_ASSERT(sb);

	available = sb->m_capacity - sb->m_count;
	inc = n - available;

	if( available < n ) {
		CWT_CHAR *buffer;
		size_t capacity;

		capacity = sb->m_capacity + inc;

		if( capacity > sb->m_max_capacity )
			return FALSE;

		buffer = CWT_MALLOCA(CWT_CHAR, capacity);

		cwtStrNS()->memcpy(buffer, sb->m_buffer, sb->m_count * sizeof(CWT_CHAR));

		CWT_FREE(sb->m_buffer);
		sb->m_buffer = buffer;
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
static BOOL __lreserve(CwtStringBufferPtr sb, size_t n)
{
	size_t inc;
	CWT_CHAR *buffer;
	size_t capacity;

	CWT_ASSERT(sb);

	inc = n;

	capacity = sb->m_capacity + inc;

	if( capacity > sb->m_max_capacity )
		return FALSE;

	buffer = CWT_MALLOCA(CWT_CHAR, capacity);

	cwtStrNS()->memcpy(buffer + n, sb->m_buffer, sb->m_count * sizeof(CWT_CHAR));

	CWT_FREE(sb->m_buffer);
	sb->m_buffer = buffer;
	sb->m_capacity = capacity;

	return TRUE;
}


/**
 * Reserve specified space in the middle of string buffer
 *
 * @param sb
 * @param n
 * @return
 */
static BOOL __mreserve(CwtStringBufferPtr sb, size_t n, size_t pos)
{
	size_t inc;
	CWT_CHAR *buffer;
	size_t capacity;
	CwtStrNS *stringNS = cwtStrNS();

	CWT_ASSERT(sb);

	if( pos >= sb->m_count )
		return __reserve(sb, n);

	if( pos == 0 )
		return __lreserve(sb, n);

	inc = n;

	capacity = sb->m_capacity + inc;

	if( capacity > sb->m_max_capacity )
		return FALSE;

	buffer = CWT_MALLOCA(CWT_CHAR, capacity);

	stringNS->memcpy(buffer, sb->m_buffer, pos * sizeof(CWT_CHAR));
	stringNS->memcpy(buffer+pos+n, sb->m_buffer + pos, (sb->m_count - pos) * sizeof(CWT_CHAR));

	CWT_FREE(sb->m_buffer);
	sb->m_buffer = buffer;
	sb->m_capacity = capacity;

	return TRUE;
}

static size_t __capacity(CwtStringBufferPtr sb)
{
	CWT_ASSERT(sb);
	return sb->m_capacity;
}


static BOOL __isEmpty(CwtStringBufferPtr sb)
{
	CWT_ASSERT(sb);
	return (sb->m_count == 0 ? TRUE : FALSE);
}


static void __clear(CwtStringBufferPtr sb)
{
	CWT_ASSERT(sb);
	sb->m_count = 0;
}

static size_t __size(CwtStringBufferPtr sb)
{
	CWT_ASSERT(sb);
	return sb->m_count;
}


static void __appendChar(CwtStringBufferPtr sb, CWT_CHAR ch)
{
	CWT_ASSERT(sb);
	CWT_ASSERT(__reserve(sb, 1));

	sb->m_buffer[sb->m_count] = ch;
	sb->m_count++;
}


static void __appendChars(CwtStringBufferPtr sb, const CWT_CHAR *s, size_t n)
{
	CWT_ASSERT(__reserve(sb, n));

	cwtStrNS()->memcpy(sb->m_buffer + sb->m_count, s, n * sizeof(CWT_CHAR));
	sb->m_count += n;
}

static void __append(CwtStringBufferPtr sb, const CWT_CHAR *s)
{
	__appendChars(sb, s, cwtStrLen(s));
}

static void __prependChar(CwtStringBufferPtr sb, CWT_CHAR ch)
{
	CWT_ASSERT(sb);

	CWT_ASSERT(__lreserve(sb, 1));
	sb->m_buffer[0] = ch;
	sb->m_count++;
}


static void __insertChar(CwtStringBufferPtr sb, CWT_CHAR ch, size_t pos)
{
	CWT_ASSERT(sb);

	CWT_ASSERT(__mreserve(sb, 1, pos));
	sb->m_buffer[pos] = ch;
	sb->m_count++;
}

static void __insert(CwtStringBufferPtr sb, const CWT_CHAR *chars, size_t nchars, size_t pos)
{
	size_t i;
	CWT_ASSERT(sb);

	CWT_ASSERT(__mreserve(sb, nchars, pos));

	/* TODO replace this code by memcpy function call */
	for( i = 0; i < nchars; i++ )
		sb->m_buffer[pos + i] = chars[i];
	sb->m_count += nchars;
}

static void __removeChar(CwtStringBufferPtr sb, size_t pos)
{
	CWT_ASSERT(sb);

	if( !sb->m_count ) /* empty buffer */
		return;

	if( pos < sb->m_count ) {
		sb->m_count--;
		cwtStrNS()->memmove(&sb->m_buffer[pos], &sb->m_buffer[pos + 1], sb->m_count * sizeof(CWT_CHAR) );
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




static CWT_CHAR* __substr(CwtStringBufferPtr sb, size_t start, size_t nchars)
{
	CWT_CHAR *chars = NULL;

	if( start < sb->m_count ) {
		nchars = CWT_MIN(nchars, sb->m_count - start);
		chars = CWT_MALLOCA(CWT_CHAR, nchars+1);
		cwtStrNS()->strncpy(chars, sb->m_buffer + start, nchars);
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
static BOOL __find(CwtStringBufferPtr sb, CWT_CHAR ch, size_t *offset)
{
	CWT_CHAR *ptr;
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
#endif
