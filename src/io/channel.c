#include <stdlib.h>
#include <cwt/global.h>
#include <cwt/logger.h>
#include <cwt/io/channel.h>


CwtChannelPtr cwtNewChannel(CwtIODevicePtr dev)
{
	CwtChannelPtr chan;

	if( !dev )
		return (CwtChannelPtr)NULL;

	chan = CWT_MALLOC(CwtChannel);
	chan->dev = dev;
	chan->total_br = 0L;
	chan->total_bw = 0L;
	return chan;
}

void cwtDeleteChannel(CwtChannelPtr chan)
{
	if( chan ) {

/*		printf_debug("total bytes read: %lu", chan->total_br);
		printf_debug("total bytes written: %lu", chan->total_bw); */

        if( chan->dev ) {
			chan->dev->close(chan->dev);
        }
        CWT_FREE(chan);
	}
}


BOOL cwtChannelCanRead(CwtChannelPtr chan)
{
	CWT_ASSERT(chan);
	if( chan->dev && chan->dev->read )
		return TRUE;
	return FALSE;
}

BOOL cwtChannelCanWrite(CwtChannelPtr chan)
{
	CWT_ASSERT(chan);
	if( chan->dev && chan->dev->write )
		return TRUE;
	return FALSE;
}

CwtIODevicePtr cwtChannelDevice(CwtChannelPtr chan)
{
	CWT_ASSERT(chan);
	return chan->dev;
}

size_t cwtChannelBytesAvailable(CwtChannelPtr chan)
{
	CWT_ASSERT(chan && chan->dev);
	return chan->dev->bytesAvailable(chan->dev);
}

ssize_t cwtChannelRead(CwtChannelPtr chan, BYTE *buf, size_t sz)
{
	ssize_t br;
	CWT_ASSERT(chan && chan->dev);
	br = chan->dev->read(chan->dev, buf, sz);
	if( br > 0 )
		chan->total_br += br;
	return br;
}

ssize_t cwtChannelWrite(CwtChannelPtr chan, const BYTE *buf, size_t sz)
{
	ssize_t bw;
	CWT_ASSERT(chan && chan->dev);
	bw = chan->dev->write(chan->dev, buf, sz);
	if( bw > 0 )
		chan->total_bw += bw;
	return bw;
}

ssize_t cwtChannelWriteByte(CwtChannelPtr chan, BYTE ch)
{
	BYTE buf[1];
	buf[0] = ch;
	return cwtChannelWrite(chan, buf, 1);
}


BOOL cwtChannelCanReadTransaq(CwtChannelPtr chan)
{
	CWT_ASSERT(chan && chan->dev);

	if( chan->dev->readBegin
			&& chan->dev->readCommit
			&& chan->dev->readRollback )
		return TRUE;

	return FALSE;
}

void cwtChannelReadBegin(CwtChannelPtr chan)
{
	CWT_ASSERT(chan && chan->dev);

	if( chan->dev->readBegin ) {
		chan->dev->readBegin(chan->dev);
	} else {
		/* TODO must be implemented */
	}
}

void cwtChannelReadCommit(CwtChannelPtr chan)
{
	CWT_ASSERT(chan && chan->dev);

	if( chan->dev->readCommit ) {
			chan->dev->readCommit(chan->dev);
	} else {
			/* TODO must be implemented */
	}
}

void cwtChannelReadRollback(CwtChannelPtr chan)
{
	CWT_ASSERT(chan && chan->dev);

	if( chan->dev->readRollback ) {
			chan->dev->readRollback(chan->dev);
	} else {
			/* TODO must be implemented */
	}
}


