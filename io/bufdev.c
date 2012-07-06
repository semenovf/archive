/*
 * bufdev.c
 *
 *  Created on: 01.03.2012
 *      Author: wladt
 */

#include <cwt/ringbuf.h>
#include <cwt/logger.h>
#include <cwt/io/bufdev.h>

static void    __cwtBufferDeviceClose(CwtIODevicePtr);
static size_t  __cwtBufferDeviceBytesAvailable(CwtIODevicePtr dev);
static ssize_t __cwtBufferDeviceRead(CwtIODevicePtr, BYTE*, size_t);
static ssize_t __cwtBufferDeviceWrite(CwtIODevicePtr, const BYTE*, size_t);

static void    __cwtBufferDeviceReadBegin(CwtIODevicePtr);
static void    __cwtBufferDeviceReadCommit(CwtIODevicePtr);
static void    __cwtBufferDeviceReadRollback(CwtIODevicePtr);

struct CwtBufferDevice
{
	CwtIODevice base;

	BOOL master;
	CwtRingBufferPtr in;
	CwtRingBufferPtr out;
	size_t   head_saved;
	size_t   count_saved;

};

typedef struct CwtBufferDevice  CwtBufferDevice;
typedef struct CwtBufferDevice* CwtBufferDevicePtr;


CwtIODevicePtr cwtBufferDeviceOpen()
{
	CwtBufferDevicePtr bufd;

	bufd = CWT_MALLOC(struct CwtBufferDevice);

	bufd->master = TRUE;
	bufd->in = rb_new_defaults();
	bufd->out = rb_new_defaults();
	bufd->head_saved  = 0;
	bufd->count_saved = 0;

	bufd->base.close = __cwtBufferDeviceClose;
	bufd->base.bytesAvailable = __cwtBufferDeviceBytesAvailable;
	bufd->base.read  = __cwtBufferDeviceRead;
	bufd->base.write = __cwtBufferDeviceWrite;

	bufd->base.readBegin      = __cwtBufferDeviceReadBegin;
	bufd->base.readCommit     = __cwtBufferDeviceReadCommit;
	bufd->base.readRollback   = __cwtBufferDeviceReadRollback;

	return (CwtIODevicePtr)bufd;
}


CwtIODevicePtr cwtBufferDeviceOpenPeer(CwtIODevicePtr bufd)
{
	CwtBufferDevicePtr peer_bufd;

	peer_bufd = CWT_MALLOC(CwtBufferDevice);

	peer_bufd->master = FALSE;
	peer_bufd->in = ((CwtBufferDevicePtr)bufd)->out;
	peer_bufd->out =((CwtBufferDevicePtr)bufd)->in;
	peer_bufd->head_saved  = 0;
	peer_bufd->count_saved = 0;


	peer_bufd->base.close = bufd->close;
	peer_bufd->base.bytesAvailable = bufd->bytesAvailable;
	peer_bufd->base.read  = bufd->read;
	peer_bufd->base.write = bufd->write;

	peer_bufd->base.readBegin      = bufd->readBegin;
	peer_bufd->base.readCommit     = bufd->readCommit;
	peer_bufd->base.readRollback   = bufd->readRollback;

	return (CwtIODevicePtr)peer_bufd;
}


void __cwtBufferDeviceClose(CwtIODevicePtr dev)
{
	CwtBufferDevicePtr bufd;

	CWT_ASSERT(dev);

	bufd = (CwtBufferDevicePtr)dev;

	if( bufd->master ) {
		rb_delete(bufd->in);
		rb_delete(bufd->out);
	}
}


size_t __cwtBufferDeviceBytesAvailable(CwtIODevicePtr dev)
{
	CwtBufferDevicePtr bufd;
	CWT_ASSERT(dev);
	bufd = (CwtBufferDevicePtr)dev;
	return rb_size(bufd->in);
}

ssize_t __cwtBufferDeviceRead(CwtIODevicePtr dev, BYTE* buf, size_t sz)
{
	CwtBufferDevicePtr bufd;
	ssize_t br;

	CWT_ASSERT(dev);

	bufd = (CwtBufferDevicePtr)dev;
	br = rb_read(bufd->in, buf, sz);

	if( br > 0 ) {
		rb_pop_front(bufd->in, (size_t)br);
	}
	return br;
}

ssize_t __cwtBufferDeviceWrite(CwtIODevicePtr dev, const BYTE* buf, size_t sz)
{
	CwtBufferDevicePtr bufd;

	CWT_ASSERT(dev);

	bufd = (CwtBufferDevicePtr)dev;
	return rb_write(bufd->out, buf, sz);
}


static void __cwtBufferDeviceReadBegin(CwtIODevicePtr dev)
{
	CwtBufferDevicePtr bufd;
	CWT_ASSERT(dev);

	bufd = (CwtBufferDevicePtr)dev;
	bufd->head_saved = bufd->in->m_head;
	bufd->count_saved = bufd->in->m_count;
}

static void __cwtBufferDeviceReadCommit(CwtIODevicePtr dev)
{
	CWT_UNUSED(dev);
}

static void __cwtBufferDeviceReadRollback(CwtIODevicePtr dev)
{
	CwtBufferDevicePtr bufd;
	CWT_ASSERT(dev);

	bufd = (CwtBufferDevicePtr)dev;

	bufd->in->m_head  = bufd->head_saved;
	bufd->in->m_count = bufd->count_saved;
}

/*
void file_read_begin(CwtIODevicePtr dev)
{
	FILE_DEVICE_PTR fd;
	CWT_ASSERT(dev);

	fd = (FILE_DEVICE_PTR)dev;
	fd->read_pos_saved = fd->read_pos;
}

void file_read_commit(CwtIODevicePtr dev)
{
	FILE_DEVICE_PTR fd;
	CWT_ASSERT(dev);

	fd = (FILE_DEVICE_PTR)dev;
	fd->read_pos_saved = fd->read_pos;
}

void file_read_rollback(CwtIODevicePtr dev)
{
	FILE_DEVICE_PTR fd;
	CWT_ASSERT(dev);

	fd = (FILE_DEVICE_PTR)dev;
	fd->read_pos = fd->read_pos_saved;
}*/

