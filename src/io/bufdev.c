/*
 * bufdev.c
 *
 *  Created on: 01.03.2012
 *      Author: wladt
 */

#include <cwt/ringbuf.h>
#include <cwt/logger.h>
#include <cwt/io/bufdev.h>

static void    __dev_close          (CwtIODevice*);
static size_t  __dev_bytesAvailable (CwtIODevice *pdev);
static ssize_t __dev_read           (CwtIODevice*, BYTE*, size_t);
static ssize_t __dev_write          (CwtIODevice*, const BYTE*, size_t);
/*static CwtIODevice* __dev_accept    (CwtIODevice *);*/

typedef struct CwtBufferDevice
{
	CwtIODevice __base;

	BOOL master;
	CwtRingBuf *in;
	CwtRingBuf *out;
} CwtBufferDevice;


CwtIODevice* cwtBufferDeviceOpen()
{
	CwtBufferDevice *bufd;

	bufd = CWT_MALLOC(struct CwtBufferDevice);

	bufd->master = TRUE;
	bufd->in = cwtRingBufNS()->create();
	bufd->out = cwtRingBufNS()->create();

	bufd->__base.close = __dev_close;
	bufd->__base.bytesAvailable = __dev_bytesAvailable;
	bufd->__base.read  = __dev_read;
	bufd->__base.write = __dev_write;
	/*bufd->__base.accept = __dev_accept;*/

	/*CWT_SHARED_INC(bufd);*/
	return (CwtIODevice*)bufd;
}


CwtIODevice* cwtBufferDeviceOpenPeer(CwtIODevice *bufd)
{
	CwtBufferDevice *peer_bufd;

	peer_bufd = CWT_MALLOC(CwtBufferDevice);

	peer_bufd->master = FALSE;
	peer_bufd->in = ((CwtBufferDevice*)bufd)->out;
	peer_bufd->out =((CwtBufferDevice*)bufd)->in;

	peer_bufd->__base.close = bufd->close;
	peer_bufd->__base.bytesAvailable = bufd->bytesAvailable;
	peer_bufd->__base.read  = bufd->read;
	peer_bufd->__base.write = bufd->write;

	return (CwtIODevice*)peer_bufd;
}


void __dev_close(CwtIODevice *dev)
{
	CwtBufferDevice *bufd;

	CWT_ASSERT(dev);

	bufd = (CwtBufferDevice*)dev;

	if( bufd->master ) {
		cwtRingBufNS()->free(bufd->in);
		cwtRingBufNS()->free(bufd->out);
	}
}


size_t __dev_bytesAvailable(CwtIODevice *dev)
{
	CwtBufferDevice *bufd;
	CWT_ASSERT(dev);
	bufd = (CwtBufferDevice*)dev;
	return cwtRingBufNS()->size(bufd->in);
}

ssize_t __dev_read(CwtIODevice *dev, BYTE* buf, size_t sz)
{
	CWT_ASSERT(dev);
	return cwtRingBufNS()->read(((CwtBufferDevice*)dev)->in, buf, sz);
}

ssize_t __dev_write(CwtIODevice *dev, const BYTE* buf, size_t sz)
{
	CwtBufferDevice *bufd;

	CWT_ASSERT(dev);

	bufd = (CwtBufferDevice*)dev;
	return cwtRingBufNS()->write(bufd->out, buf, sz);
}

/*
static CwtIODevice* __dev_accept (CwtIODevice *dev)
{
	CWT_ASSERT(dev);
	if (__dev_bytesAvailable(dev) > 0) {
		CWT_SHARED_INC(dev);
		return dev;
	}
	return NULL;
}
*/
