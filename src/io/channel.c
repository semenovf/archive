#include <stdlib.h>
#include <cwt/global.h>
#include <cwt/logger.h>
#include <cwt/io/channel.h>


static CwtChannel*  __create  (CwtIODevice *pdev);
static void         __free    (CwtChannel*);
static BOOL         __canRead (CwtChannel*);
static BOOL         __canWrite(CwtChannel*);
static CwtIODevice* __device  (CwtChannel*);

static BOOL         __atEnd     (CwtChannel*);
static BOOL         __canReadLine (CwtChannel*);
static BOOL         __readLine  (CwtChannel*, CwtByteArray *ba);
static ssize_t      __poll      (CwtChannel*);
static ssize_t      __read      (CwtChannel*, BYTE *buf, size_t sz);
static ssize_t      __write     (CwtChannel*, const BYTE *buf, size_t sz);
static ssize_t      __writeByte (CwtChannel*, BYTE ch);
static size_t       __bytesAvailable (CwtChannel*);

static BOOL         __canReadTransaq(CwtChannel*);
static void         __readBegin     (CwtChannel*);
static void         __readCommit    (CwtChannel*);
static void         __readRollback  (CwtChannel*);

/* helper functions */
static void         __init    (CwtChannel*, CwtIODevice *pdev);
static void         __destroy (CwtChannel*);

static CwtChannelNS __cwtChannelNS = {
	  __create
/*
	, __init
	, __destroy
*/
	, __free
	, __canRead
	, __canWrite
	, __device

	, __atEnd
	, __canReadLine
	, __readLine
	, __poll
	, __read
	, __write
	, __writeByte
	, __bytesAvailable

	, __canReadTransaq
	, __readBegin
	, __readCommit
	, __readRollback
};


static CwtRingBufNS *__rbNS = NULL;
static CwtByteArrayNS *__baNS = NULL;

DLL_API_EXPORT CwtChannelNS* cwtChannelNS(void)
{
	return &__cwtChannelNS;
}


static void __init(CwtChannel *pchan, CwtIODevice *pdev)
{
	CWT_ASSERT(pchan);

	pchan->dev = pdev;
	pchan->total_br = 0L;
	pchan->total_bw = 0L;
	pchan->rb = __rbNS->create();
}

static void __destroy(CwtChannel *pchan)
{
	if( pchan ) {
        if( pchan->dev ) {
			pchan->dev->close(pchan->dev);
        }

        cwtRingBufNS()->free(pchan->rb);
        pchan->rb = NULL;
	}
}

static CwtChannel* __create(CwtIODevice *pdev)
{
	CwtChannel* pchan;

	if( !pdev ) {
		printf_error(_Tr("device is null"));
		return (CwtChannel*)NULL;
	}

	if( !__rbNS )
		__rbNS = cwtRingBufNS();

	if( !__baNS )
		__baNS = cwtByteArrayNS();

	pchan = CWT_MALLOC(CwtChannel);
	__init((CwtChannel*)pchan, pdev);

	return pchan;
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
	__poll(pchan);
	return __rbNS->size(pchan->rb); /*pchan->dev->bytesAvailable(pchan->dev);*/
}


/**
 * @brief Checks ending of reading.
 *
 * @param pchan
 * @return
 */
static BOOL __atEnd( CwtChannel *pchan )
{
	return (__rbNS->size(pchan->rb) == 0 && __poll(pchan) == 0)
			? TRUE : FALSE;
}



static BOOL __canReadLine(CwtChannel *pchan)
{
	CWT_ASSERT(pchan && pchan->dev);

	__poll(pchan);
	return __rbNS->findAny(pchan->rb, "\n\r", 2, 0, NULL)
		|| (__atEnd(pchan) && __rbNS->size(pchan->rb) > 0);
}


/**
 * @brief Reads line of bytes from channel and appends it into byte array
 *
 * @param pchan
 * @param ba
 * @return @c TRUE if line successfully read
 */
static BOOL __readLine(CwtChannel *pchan, CwtByteArray *ba)
{
	size_t index;
	size_t ba_off;

	CWT_ASSERT(pchan);
	CWT_ASSERT(ba);

	/*__baNS->clear(ba);*/
	ba_off = __baNS->size(ba);

	if( __rbNS->findAny(pchan->rb, "\r\n", 2, 0, &index) ) {
		__baNS->resize(ba, ba_off + index);
		__rbNS->read(pchan->rb, ba->m_buffer + ba_off, index);

		__rbNS->popFront(pchan->rb, index+1);

		/* may be \r\n or \n\r delimiter */
		if( __rbNS->size(pchan->rb) > 0 ) {
			if(__rbNS->first(pchan->rb) == '\n' || __rbNS->first(pchan->rb) == '\r' ) {
				__rbNS->popFront(pchan->rb, 1);
			}
		}

		return TRUE;
	} else if( __poll(pchan) == 0 && __rbNS->size(pchan->rb) > 0 ) {
		__baNS->resize(ba, ba_off + __rbNS->size(pchan->rb));
		__rbNS->read(pchan->rb, ba->m_buffer + ba_off, __rbNS->size(pchan->rb));
		__rbNS->clear(pchan->rb);

		return TRUE;
	}

	return FALSE;
}

#define __DATA_CHUNK_SZ 256
static ssize_t __poll(CwtChannel *pchan)
{
	BYTE buf[__DATA_CHUNK_SZ];
	int n = 5;
	ssize_t br, tbr = 0;
	CWT_ASSERT(pchan && pchan->dev);

	while( n-- ) {
		br = pchan->dev->read(pchan->dev, buf, __DATA_CHUNK_SZ);
		if( br > 0 ) {
			__rbNS->pushBack(pchan->rb, buf, br);
			tbr += br;

			if( br < __DATA_CHUNK_SZ )
				break;
		} else {
			if( br < 0 )
				tbr = br;
			break;
		}
	}
	return tbr;
}


static ssize_t __read(CwtChannel *pchan, BYTE *buf, size_t sz)
{
	ssize_t br = 0;
	CWT_ASSERT(pchan && pchan->dev);

	__poll(pchan);

	if( __rbNS->size(pchan->rb) > 0 )
		br = __rbNS->read(pchan->rb, buf, sz);

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
	}
}

static void __readCommit(CwtChannel *pchan)
{
	CWT_ASSERT(pchan && pchan->dev);

	if( pchan->dev->readCommit ) {
		pchan->dev->readCommit(pchan->dev);
	}
}

static void __readRollback(CwtChannel *pchan)
{
	CWT_ASSERT(pchan && pchan->dev);

	if( pchan->dev->readRollback ) {
		pchan->dev->readRollback(pchan->dev);
	}
}


