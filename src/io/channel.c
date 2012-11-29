#include <stdlib.h>
#include <cwt/global.h>
#include <cwt/logger.h>
#include <cwt/io/channel.h>


static CwtChannel*  __open    (CwtIODevice *pdev);
static void         __close   (CwtChannel *);
static BOOL         __canRead (CwtChannel *);
static BOOL         __canWrite(CwtChannel *);
static CwtIODevice* __device  (CwtChannel *);

static BOOL         __atEnd     (CwtChannel *);
static BOOL         __canReadLine (CwtChannel *);
static BOOL         __readLine  (CwtChannel *, CwtByteArray *ba);
static ssize_t      __poll      (CwtChannel *);
static ssize_t      __read      (CwtChannel *, BYTE *buf, size_t sz);
static ssize_t      __write     (CwtChannel *, const BYTE *buf, size_t sz);
static ssize_t      __writeByte (CwtChannel *, BYTE ch);
static size_t       __bytesAvailable (CwtChannel *);
/*static CwtIODevice* __accept    (CwtChannel *);*/


/* helper functions */
static void         __init    (CwtChannel*, CwtIODevice *pdev);
static void         __destroy (CwtChannel*);

static CwtChannelNS __cwtChannelNS = {
	  __open
/*
	, __init
	, __destroy
*/
	, __close
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
	/*, __accept*/
};


static CwtRingBufferNS *__rbNS = NULL;
static CwtByteArrayNS *__baNS = NULL;

DLL_API_EXPORT CwtChannelNS* cwt_channel_ns(void)
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

        cwt_ringbuffer_ns()->free(pchan->rb);
        pchan->rb = NULL;
	}
}

static CwtChannel* __open(CwtIODevice *pdev)
{
	CwtChannel* pchan;

	if( !pdev ) {
		cwt_logger_ns()->error(_Tr("device is null"));
		return (CwtChannel*)NULL;
	}

	if( !__rbNS )
		__rbNS = cwt_ringbuffer_ns();

	if( !__baNS )
		__baNS = cwt_bytearray_ns();

	pchan = CWT_MALLOC(CwtChannel);
	__init((CwtChannel*)pchan, pdev);

	return pchan;
}

static void __close(CwtChannel *pchan)
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


/*
static CwtIODevice* __accept (CwtChannel *pchan)
{
	CWT_ASSERT(pchan && pchan->dev);
	return pchan->dev->accept(pchan->dev);
}
*/

/**
 * @brief Checks ending of reading.
 *
 * @param pchan
 * @return
 */
static BOOL __atEnd( CwtChannel *pchan )
{
	return ( __poll(pchan) == 0 && __rbNS->size(pchan->rb) == 0 )
			? TRUE : FALSE;
}



static BOOL __canReadLine(CwtChannel *pchan)
{
	BYTE eolChars[] = "\n\r";
	CWT_ASSERT(pchan && pchan->dev);

	__poll(pchan);
	return __rbNS->findAny(pchan->rb, eolChars, 2, 0, NULL)
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
	BYTE eolChars[] = "\r\n";
	size_t index;
	size_t ba_off;
	ssize_t br;

	CWT_ASSERT(pchan);
	CWT_ASSERT(ba);

	ba_off = __baNS->size(ba);

	br = __poll(pchan);

	if( __rbNS->findAny(pchan->rb, eolChars, 2, 0, &index) ) {
		BYTE nl;
		__baNS->resize(ba, ba_off + index);
		__rbNS->peek(pchan->rb, ba->m_buffer + ba_off, index);

		__rbNS->popFront(pchan->rb, index);
		nl = __rbNS->first(pchan->rb);
		__rbNS->popFront(pchan->rb, 1);

		/* may be \r\n delimiter */
		if( __rbNS->size(pchan->rb) > 0 ) {
			if( nl == '\r' && __rbNS->first(pchan->rb) == '\n' /*|| __rbNS->first(pchan->rb) == '\r'*/ ) {
				__rbNS->popFront(pchan->rb, 1);
			}
		}

		return TRUE;
	} else if( br == 0 && __rbNS->size(pchan->rb) > 0 ) {
		__baNS->resize(ba, ba_off + __rbNS->size(pchan->rb));
		__rbNS->peek(pchan->rb, ba->m_buffer + ba_off, __rbNS->size(pchan->rb));
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

	if (__rbNS->size(pchan->rb) > 0) {
		br = __rbNS->read(pchan->rb, buf, sz);
	} else {
		__poll(pchan);
		if( __rbNS->size(pchan->rb) > 0 )
			br = __rbNS->read(pchan->rb, buf, sz);
	}

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
