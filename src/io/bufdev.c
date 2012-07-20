/*
 * bufdev.c
 *
 *  Created on: 01.03.2012
 *      Author: wladt
 */

#include <cwt/ringbuf.h>
#include <cwt/logger.h>
#include <cwt/io/bufdev.h>

static void    __сlose(CwtIODevice*);
static size_t  __bytesAvailable(CwtIODevice *pdev);
static ssize_t __read(CwtIODevice*, BYTE*, size_t);
static ssize_t __write(CwtIODevice*, const BYTE*, size_t);

static void    __readBegin(CwtIODevice*);
static void    __readCommit(CwtIODevice*);
static void    __readRollback(CwtIODevice*);

typedef struct CwtBufferDevice
{
	CwtIODevice base;

	BOOL master;
	CwtRingBuf *in;
	CwtRingBuf *out;
	size_t   head_saved;
	size_t   count_saved;
} CwtBufferDevice;


CwtIODevice* cwtBufferDeviceOpen()
{
	CwtBufferDevice *bufd;

	bufd = CWT_MALLOC(struct CwtBufferDevice);

	bufd->master = TRUE;
	bufd->in = cwtRingBufNS()->create();
	bufd->out = cwtRingBufNS()->create();
	bufd->head_saved  = 0;
	bufd->count_saved = 0;

	bufd->base.close = __сlose;
	bufd->base.bytesAvailable = __bytesAvailable;
	bufd->base.read  = __read;
	bufd->base.write = __write;

	bufd->base.readBegin      = __readBegin;
	bufd->base.readCommit     = __readCommit;
	bufd->base.readRollback   = __readRollback;

	return (CwtIODevice*)bufd;
}


CwtIODevice* cwtBufferDeviceOpenPeer(CwtIODevice *bufd)
{
	CwtBufferDevice *peer_bufd;

	peer_bufd = CWT_MALLOC(CwtBufferDevice);

	peer_bufd->master = FALSE;
	peer_bufd->in = ((CwtBufferDevice*)bufd)->out;
	peer_bufd->out =((CwtBufferDevice*)bufd)->in;
	peer_bufd->head_saved  = 0;
	peer_bufd->count_saved = 0;


	peer_bufd->base.close = bufd->close;
	peer_bufd->base.bytesAvailable = bufd->bytesAvailable;
	peer_bufd->base.read  = bufd->read;
	peer_bufd->base.write = bufd->write;

	peer_bufd->base.readBegin      = bufd->readBegin;
	peer_bufd->base.readCommit     = bufd->readCommit;
	peer_bufd->base.readRollback   = bufd->readRollback;

	return (CwtIODevice*)peer_bufd;
}


void __сlose(CwtIODevice *dev)
{
	CwtBufferDevice *bufd;

	CWT_ASSERT(dev);

	bufd = (CwtBufferDevice*)dev;

	if( bufd->master ) {
		cwtRingBufNS()->free(bufd->in);
		cwtRingBufNS()->free(bufd->out);
	}
}


size_t __bytesAvailable(CwtIODevice *dev)
{
	CwtBufferDevice *bufd;
	CWT_ASSERT(dev);
	bufd = (CwtBufferDevice*)dev;
	return cwtRingBufNS()->size(bufd->in);
}

ssize_t __read(CwtIODevice *dev, BYTE* buf, size_t sz)
{
	CwtBufferDevice *bufd;
	ssize_t br;

	CWT_ASSERT(dev);

	bufd = (CwtBufferDevice*)dev;
	br = cwtRingBufNS()->read(bufd->in, buf, sz);

	if( br > 0 ) {
		cwtRingBufNS()->popFront(bufd->in, (size_t)br);
	}
	return br;
}

ssize_t __write(CwtIODevice *dev, const BYTE* buf, size_t sz)
{
	CwtBufferDevice *bufd;

	CWT_ASSERT(dev);

	bufd = (CwtBufferDevice*)dev;
	return cwtRingBufNS()->write(bufd->out, buf, sz);
}


static void __readBegin(CwtIODevice *dev)
{
	CwtBufferDevice *bufd;
	CWT_ASSERT(dev);

	bufd = (CwtBufferDevice*)dev;
	bufd->head_saved = bufd->in->m_head;
	bufd->count_saved = bufd->in->m_count;
}

static void __readCommit(CwtIODevice *dev)
{
	CWT_UNUSED(dev);
}

static void __readRollback(CwtIODevice *dev)
{
	CwtBufferDevice *bufd;
	CWT_ASSERT(dev);

	bufd = (CwtBufferDevice*)dev;

	bufd->in->m_head  = bufd->head_saved;
	bufd->in->m_count = bufd->count_saved;
}
