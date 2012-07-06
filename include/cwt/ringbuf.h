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

struct RingBuffer
{
	BYTE*    m_buffer;   /* Динамический массив элементов буфера */
    size_t   m_head;     /* Индекс первого элемента */
    size_t   m_count;    /* Количество элементов, доступных для чтения */
    size_t   m_capacity;
    size_t   m_max_capacity;
};

typedef struct RingBuffer  RingBuffer;
typedef struct RingBuffer* RingBufferPtr;
typedef struct RingBuffer  CwtRingBuffer;
typedef struct RingBuffer* CwtRingBufferPtr;

EXTERN_C_BEGIN

DLL_API_EXPORT RingBufferPtr rb_new_defaults(void);
DLL_API_EXPORT RingBufferPtr rb_new(size_t initial_size, size_t max_size);
DLL_API_EXPORT void          rb_delete(RingBufferPtr rb);
DLL_API_EXPORT RingBufferPtr rb_clone(RingBufferPtr rb);
DLL_API_EXPORT BOOL          rb_reserve(RingBufferPtr rb, size_t n);
DLL_API_EXPORT size_t        rb_capacity(RingBufferPtr rb);
DLL_API_EXPORT BOOL          rb_is_empty(RingBufferPtr rb);
DLL_API_EXPORT void          rb_clear(RingBufferPtr rb);
DLL_API_EXPORT size_t        rb_size(RingBufferPtr rb);
DLL_API_EXPORT BYTE          rb_at(RingBufferPtr rb, size_t index);
DLL_API_EXPORT BYTE          rb_at_front(RingBufferPtr rb);
DLL_API_EXPORT BYTE          rb_first(RingBufferPtr rb);
DLL_API_EXPORT BYTE          rb_at_back(RingBufferPtr rb);
DLL_API_EXPORT BYTE          rb_last(RingBufferPtr rb);
DLL_API_EXPORT ssize_t       rb_read(RingBufferPtr rb, BYTE* bytes, size_t n);
DLL_API_EXPORT ssize_t       rb_write(RingBufferPtr rb, const BYTE* chars, size_t n);
DLL_API_EXPORT ssize_t       rb_write_from_file(RingBufferPtr rb, int fd, size_t n);
DLL_API_EXPORT BYTE          rb_get(RingBufferPtr rb);
DLL_API_EXPORT void          rb_pop_front(RingBufferPtr rb, size_t n);
DLL_API_EXPORT void          rb_pop_back(RingBufferPtr rb, size_t n);
DLL_API_EXPORT BOOL          rb_put(RingBufferPtr rb, BYTE b);
DLL_API_EXPORT BOOL          rb_push_back(RingBufferPtr rb, const BYTE* bytes, size_t n);
DLL_API_EXPORT BOOL          rb_find_byte(RingBufferPtr rb, BYTE ch, size_t from, size_t* index);
DLL_API_EXPORT BOOL          rb_find(RingBufferPtr rb, const BYTE* bytes, size_t n, size_t from, size_t* index);
DLL_API_EXPORT int           rb_split(BYTE delim, RingBufferPtr rb, size_t len, int maxcount, void (*on_token)(RingBufferPtr, void*), void* extra);

#define cwtNewRingBuffer       rb_new_defaults
#define cwtNewRingBufferSize   rb_new
#define cwtDeleteRingBuffer    rb_delete
#define cwtRingBufferClone     rb_clone
#define cwtRingBufferReserve   rb_reserve
#define cwtRingBufferCapacity  rb_capacity
#define cwtRingBufferIsEmpty   rb_is_empty
#define cwtRingBufferCler      rb_clear
#define cwtRingBufferSize      rb_size
#define cwtRingBufferAt        rb_at
#define cwtRingBufferAtFront   rb_at_front
#define cwtRingBufferFirst     rb_first
#define cwtRingBufferAtBack    rb_at_back
#define cwtRingBufferLast      rb_last
#define cwtRingBufferRead      rb_read
#define cwtRingBufferWrite     rb_write
#define cwtRingBufferWriteFromFile rb_write_from_file
#define cwtRingBufferGet       rb_get
#define cwtRingBufferPopFront  rb_pop_front
#define cwtRingBufferPopBack   rb_pop_back
#define cwtRingBufferPut       rb_put
#define cwtRingBufferPushBack  rb_push_back
#define cwtRingBufferFindByte  rb_find_byte
#define cwtRingBufferFind      rb_find
#define cwtRingBufferSplit     rb_split

EXTERN_C_END


#endif /* __CWT_RINGBUF_H__ */
