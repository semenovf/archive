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
#include <cwt/bytearr.h>

typedef struct _CwtRingBuffer
{
	BYTE*    m_buffer;
    size_t   m_head;
    size_t   m_count;
    size_t   m_capacity;
    size_t   m_max_capacity;
} CwtRingBuffer;


typedef struct _CwtRingBufferNS {
	void           (*init)       (CwtRingBuffer *);
	void           (*initSized)  (CwtRingBuffer *, size_t initial_size, size_t max_size);
	void           (*destroy)    (CwtRingBuffer *);
	CwtRingBuffer* (*create)     (void);
	CwtRingBuffer* (*createSized)(size_t initial_size, size_t max_size);
	void           (*free)       (CwtRingBuffer *);
	CwtRingBuffer* (*clone)      (CwtRingBuffer *);
	BOOL           (*reserve)    (CwtRingBuffer *, size_t n);
	size_t         (*capacity)   (CwtRingBuffer *);
	BOOL           (*isEmpty)    (CwtRingBuffer *);
	void           (*clear)      (CwtRingBuffer *);
	size_t         (*size)       (CwtRingBuffer *);
	BYTE           (*at)         (CwtRingBuffer *, size_t index);
	BYTE           (*atFront)    (CwtRingBuffer *);
	BYTE           (*first)      (CwtRingBuffer *);
	BYTE           (*atBack)     (CwtRingBuffer *);
	BYTE           (*last)       (CwtRingBuffer *);
	ssize_t        (*read)       (CwtRingBuffer *, CwtByteArray *ba, size_t n);
	ssize_t        (*peek)       (CwtRingBuffer *, CwtByteArray *ba, size_t n);
	ssize_t        (*write)      (CwtRingBuffer *, const BYTE *chars, size_t n);
	BYTE           (*get)        (CwtRingBuffer *);
	void           (*popFront)   (CwtRingBuffer *, size_t n);
	void           (*popBack)    (CwtRingBuffer *, size_t n);
	BOOL           (*put)        (CwtRingBuffer *, BYTE b);
	BOOL           (*pushBack)   (CwtRingBuffer *, const BYTE *bytes, size_t n);
	BOOL           (*find)       (CwtRingBuffer *, const BYTE *bytes, size_t n, size_t from, size_t *index);
	BOOL           (*findAny)    (CwtRingBuffer *, const BYTE *bytes, size_t n, size_t from, size_t *index);
} CwtRingBufferNS;

EXTERN_C_BEGIN
DLL_API_EXPORT CwtRingBufferNS* cwt_ringbuffer_ns(void);
EXTERN_C_END;



#endif /* __CWT_RINGBUF_H__ */
