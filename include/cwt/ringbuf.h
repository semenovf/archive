/*
 * ringbuf.h
 *
 *  Created on: 27.11.2011
 *      Author: wladt
 */

#ifndef __CWT_RINGBUF_H__
#define __CWT_RINGBUF_H__

#include <cwt/global.h>
#include <cwt/types.h>

#define RBE_QUOTE_CHAR_UNBALANCED -2

typedef struct CwtRingBuf
{
	BYTE*    m_buffer;
    size_t   m_head;
    size_t   m_count;
    size_t   m_capacity;
    size_t   m_max_capacity;
} CwtRingBuf;


typedef struct _CwtRingBufNS {
	CwtRingBuf*   (*create)     (void);
	CwtRingBuf*   (*createSized)(size_t initial_size, size_t max_size);
	void          (*free)       (CwtRingBuf*);
	CwtRingBuf*   (*clone)      (CwtRingBuf*);
	BOOL          (*reserve)    (CwtRingBuf*, size_t n);
	size_t        (*capacity)   (CwtRingBuf*);
	BOOL          (*isEmpty)    (CwtRingBuf*);
	void          (*clear)      (CwtRingBuf*);
	size_t        (*size)       (CwtRingBuf*);
	BYTE          (*at)         (CwtRingBuf*, size_t index);
	BYTE          (*atFront)    (CwtRingBuf*);
	BYTE          (*first)      (CwtRingBuf*);
	BYTE          (*atBack)     (CwtRingBuf*);
	BYTE          (*last)       (CwtRingBuf*);
	ssize_t       (*read)       (CwtRingBuf*, BYTE* bytes, size_t n);
	ssize_t       (*peek)       (CwtRingBuf*, BYTE* bytes, size_t n);
	ssize_t       (*write)      (CwtRingBuf*, const BYTE* chars, size_t n);
	BYTE          (*get)        (CwtRingBuf*);
	void          (*popFront)   (CwtRingBuf*, size_t n);
	void          (*popBack)    (CwtRingBuf*, size_t n);
	BOOL          (*put)        (CwtRingBuf*, BYTE b);
	BOOL          (*pushBack)   (CwtRingBuf*, const BYTE* bytes, size_t n);
	BOOL          (*find)       (CwtRingBuf*, const BYTE* bytes, size_t n, size_t from, size_t* index);
	BOOL          (*findAny)    (CwtRingBuf*, const BYTE* bytes, size_t n, size_t from, size_t* index);
	/*ssize_t       rb_write_from_file(CwtRingBuf*, int fd, size_t n);*/
} CwtRingBufNS;

EXTERN_C_BEGIN
DLL_API_EXPORT CwtRingBufNS* cwtRingBufNS(void);
EXTERN_C_END;



#endif /* __CWT_RINGBUF_H__ */
