#include <stdlib.h>
#include <cwt/global.h>
#include <cwt/logger.h>
#include <cwt/io/channel.h>

static CwtChannel*  __create  (CwtIODevice *pdev);
static void         __init    (CwtChannel*, CwtIODevice *pdev);
static void         __destroy (CwtChannel*);
static void         __free    (CwtChannel*);
static BOOL         __canRead (CwtChannel*);
static BOOL         __canWrite(CwtChannel*);
static CwtIODevice* __device  (CwtChannel*);

static ssize_t      __read      (CwtChannel*, BYTE *buf, size_t sz);
static ssize_t      __write     (CwtChannel*, const BYTE *buf, size_t sz);
static ssize_t      __writeByte (CwtChannel*, BYTE ch);
static size_t       __bytesAvailable (CwtChannel*);

static BOOL         __canReadTransaq(CwtChannel*);
static void         __readBegin     (CwtChannel*);
static void         __readCommit    (CwtChannel*);
static void         __readRollback  (CwtChannel*);


static CwtChannelNS __cwtChannelNS = {
	  __create
	, __init
	, __destroy
	, __free
	, __canRead
	, __canWrite
	, __device

	, __read
	, __write
	, __writeByte
	, __bytesAvailable

	, __canReadTransaq
	, __readBegin
	, __readCommit
	, __readRollback
};


DLL_API_EXPORT CwtChannelNS* cwtChannelNS(void)
{
	return &__cwtChannelNS;
}


static CwtChannel* __create(CwtIODevice *pdev)
{
	CwtChannel* chan;

	if( !pdev ) {
		printf_error(_Tr("device is null"));
		return (CwtChannel*)NULL;
	}

	chan = CWT_MALLOC(CwtChannel);
	__init(chan, pdev);
	return chan;
}


static void __init(CwtChannel *pchan, CwtIODevice *pdev)
{
	CWT_ASSERT(pchan);

	pchan->dev = pdev;
	pchan->total_br = 0L;
	pchan->total_bw = 0L;
}

static void __destroy(CwtChannel *pchan)
{
	if( pchan ) {
        if( pchan->dev ) {
			pchan->dev->close(pchan->dev);
        }
	}
}

static void __free(CwtChannel *pchan)
{
	if( pchan ) {
		__destroy(pchan);
		CWT_FREE(pchan);
	}
}



static BOOL __canRead(CwtChannel *pchan)
{
	CWT_ASSERT(pchan);

	if( pchan->dev && pchan->dev->read )
		return TRUE;
	return FALSE;
}

static BOOL __canWrite(CwtChannel *pchan)
{
	CWT_ASSERT(pchan);
	if( pchan->dev && pchan->dev->write )
		return TRUE;
	return FALSE;
}

static CwtIODevice* __device(CwtChannel *pchan)
{
	CWT_ASSERT(pchan);
	return pchan->dev;
}

static size_t __bytesAvailable(CwtChannel *pchan)
{
	CWT_ASSERT(pchan && pchan->dev);
	return pchan->dev->bytesAvailable(pchan->dev);
}

static ssize_t __read(CwtChannel *pchan, BYTE *buf, size_t sz)
{
	ssize_t br;
	CWT_ASSERT(pchan && pchan->dev);
	br = pchan->dev->read(pchan->dev, buf, sz);
	if( br > 0 )
		pchan->total_br += br;
	return br;
}

static ssize_t __write(CwtChannel *pchan, const BYTE *buf, size_t sz)
{
	ssize_t bw;
	CWT_ASSERT(pchan && pchan->dev);
	bw = pchan->dev->write(pchan->dev, buf, sz);
	if( bw > 0 )
		pchan->total_bw += bw;
	return bw;
}

static ssize_t __writeByte(CwtChannel *pchan, BYTE ch)
{
	BYTE buf[1];
	buf[0] = ch;
	return __write(pchan, buf, 1);
}


static BOOL __canReadTransaq(CwtChannel *pchan)
{
	CWT_ASSERT(pchan && pchan->dev);

	if( pchan->dev->readBegin
			&& pchan->dev->readCommit
			&& pchan->dev->readRollback )
		return TRUE;

	return FALSE;
}

static void __readBegin(CwtChannel *pchan)
{
	CWT_ASSERT(pchan && pchan->dev);

	if( pchan->dev->readBegin ) {
		pchan->dev->readBegin(pchan->dev);
	} else {
		/* TODO must be implemented */
	}
}

static void __readCommit(CwtChannel *pchan)
{
	CWT_ASSERT(pchan && pchan->dev);

	if( pchan->dev->readCommit ) {
			pchan->dev->readCommit(pchan->dev);
	} else {
			/* TODO must be implemented */
	}
}

static void __readRollback(CwtChannel *pchan)
{
	CWT_ASSERT(pchan && pchan->dev);

	if( pchan->dev->readRollback ) {
			pchan->dev->readRollback(pchan->dev);
	} else {
			/* TODO must be implemented */
	}
}


