#include <stdlib.h>
#include <cwt/global.h>
#include <cwt/logger.h>
#include <cwt/io/channel.h>


static CwtChannel*   channel_open            (CwtIODevice *pdev);
static void          channel_close           (CwtChannel *);
static BOOL          channel_can_read        (CwtChannel *);
static BOOL          channel_can_write       (CwtChannel *);
static CwtIODevice*  channel_device          (CwtChannel *);

static inline BOOL   channel_at_end          (CwtChannel *);
static BOOL          channel_can_read_line   (CwtChannel *);
static BOOL          channel_read_line       (CwtChannel *, CwtByteArray *ba);
static ssize_t       channel_poll            (CwtChannel *);
static ssize_t       channel_read            (CwtChannel *, BYTE *buf, size_t sz);
static ssize_t       channel_write           (CwtChannel *, const BYTE *buf, size_t sz);
static ssize_t       channel_write_byte      (CwtChannel *, BYTE ch);
static size_t        channel_bytes_available (CwtChannel *);
static BOOL          channel_slurp           (CwtChannel *pchan, CwtByteArray *buffer);


/* helper functions */
static void         __init    (CwtChannel*, CwtIODevice *pdev);
static void         __destroy (CwtChannel*);

static CwtChannelNS __cwtChannelNS = {
	  channel_open
	, channel_close
	, channel_can_read
	, channel_can_write
	, channel_device

	, channel_at_end
	, channel_can_read_line
	, channel_read_line
	, channel_poll
	, channel_read
	, channel_write
	, channel_write_byte
	, channel_bytes_available
	, channel_slurp
};


static CwtRingBufferNS *__rb_ns = NULL;
static CwtByteArrayNS  *__ba_ns = NULL;

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
	pchan->rb = __rb_ns->create();
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

static CwtChannel* channel_open(CwtIODevice *pdev)
{
	CwtChannel* pchan;

	if( !pdev ) {
		cwt_logger_ns()->error(_Tr("device is null"));
		return (CwtChannel*)NULL;
	}

	if( !__rb_ns )
		__rb_ns = cwt_ringbuffer_ns();

	if( !__ba_ns )
		__ba_ns = cwt_bytearray_ns();

	pchan = CWT_MALLOCT(CwtChannel);
	__init((CwtChannel*)pchan, pdev);

	return pchan;
}

static void channel_close(CwtChannel *pchan)
{
	if( pchan ) {
		__destroy(pchan);
		CWT_FREE(pchan);
	}
}



static BOOL channel_can_read(CwtChannel *pchan)
{
	CWT_ASSERT(pchan);

	if( pchan->dev && pchan->dev->read )
		return TRUE;
	return FALSE;
}

static BOOL channel_can_write(CwtChannel *pchan)
{
	CWT_ASSERT(pchan);
	if( pchan->dev && pchan->dev->write )
		return TRUE;
	return FALSE;
}

static CwtIODevice* channel_device(CwtChannel *pchan)
{
	CWT_ASSERT(pchan);
	return pchan->dev;
}

static size_t channel_bytes_available(CwtChannel *pchan)
{
	CWT_ASSERT(pchan && pchan->dev);
	channel_poll(pchan);
	return __rb_ns->size(pchan->rb); /*pchan->dev->bytesAvailable(pchan->dev);*/
}


#define _BUFSZ 256
/**
 * @fn BOOL CwtChannelNS::slurp(CwtChannel *pchan, CwtByteArray *buffer)
 *
 * @brief Reads data from channel until zero bytes read or error occurred.
 *
 * @details Result stored in buffer.
 *
 * @param pchan Channel.
 * @param buffer Buffer to store result.
 * @return TRUE if all data read from channel or FALSE if error occurred
 *         while receiving data from channel.
 */
static BOOL channel_slurp (CwtChannel *pchan, CwtByteArray *buffer)
{
	size_t sz_saved;
	BYTE bytes[_BUFSZ];
	ssize_t br;

	CWT_ASSERT(buffer);
	CWT_ASSERT(pchan && pchan->dev);

	sz_saved = __ba_ns->size(buffer);

	while ( (br = channel_read(pchan, bytes, _BUFSZ)) > 0 ) {
		__ba_ns->appendElems(buffer, bytes, br);
	}

	if (br < 0) {
		if (sz_saved) {
			__ba_ns->resize(buffer, sz_saved);
		} else {
			__ba_ns->clear(buffer);
		}
		return FALSE;
	}

	return TRUE;
}

/**
 * @brief Checks ending of reading.
 *
 * @param pchan
 * @return
 */
static inline BOOL channel_at_end( CwtChannel *pchan )
{
	return ( channel_poll(pchan) == 0 && __rb_ns->size(pchan->rb) == 0 )
			? TRUE : FALSE;
}



static BOOL channel_can_read_line(CwtChannel *pchan)
{
	BYTE eolChars[] = "\n\r";
	CWT_ASSERT(pchan && pchan->dev);

	channel_poll(pchan);
	return __rb_ns->findAny(pchan->rb, eolChars, 2, 0, NULL)
		|| (channel_at_end(pchan) && __rb_ns->size(pchan->rb) > 0);
}


/**
 * @brief Reads line of bytes from channel and appends it into byte array
 *
 * @param pchan
 * @param ba
 * @return @c TRUE if line successfully read
 */
static BOOL channel_read_line(CwtChannel *pchan, CwtByteArray *ba)
{
	BYTE eolChars[] = "\r\n";
	size_t index;
	size_t ba_off;
	ssize_t br;

	CWT_ASSERT(pchan);
	CWT_ASSERT(ba);

	ba_off = __ba_ns->size(ba);

	br = channel_poll(pchan);

	if( __rb_ns->findAny(pchan->rb, eolChars, 2, 0, &index) ) {
		BYTE nl;
		__ba_ns->resize(ba, ba_off + index);
		__rb_ns->peek(pchan->rb, ba->m_buffer + ba_off, index);

		__rb_ns->popFront(pchan->rb, index);
		nl = __rb_ns->first(pchan->rb);
		__rb_ns->popFront(pchan->rb, 1);

		/* may be \r\n delimiter */
		if( __rb_ns->size(pchan->rb) > 0 ) {
			if( nl == '\r' && __rb_ns->first(pchan->rb) == '\n' /*|| __rbNS->first(pchan->rb) == '\r'*/ ) {
				__rb_ns->popFront(pchan->rb, 1);
			}
		}

		return TRUE;
	} else if( br == 0 && __rb_ns->size(pchan->rb) > 0 ) {
		__ba_ns->resize(ba, ba_off + __rb_ns->size(pchan->rb));
		__rb_ns->peek(pchan->rb, ba->m_buffer + ba_off, __rb_ns->size(pchan->rb));
		__rb_ns->clear(pchan->rb);

		return TRUE;
	}

	return FALSE;
}

#define __DATA_CHUNK_SZ 256
static ssize_t channel_poll(CwtChannel *pchan)
{
	BYTE buf[__DATA_CHUNK_SZ];
	int n = 5;
	ssize_t br, tbr = 0;
	CWT_ASSERT(pchan && pchan->dev);

	while( n-- ) {
		br = pchan->dev->read(pchan->dev, buf, __DATA_CHUNK_SZ);
		if( br > 0 ) {
			__rb_ns->pushBack(pchan->rb, buf, br);
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


static ssize_t channel_read(CwtChannel *pchan, BYTE *buf, size_t sz)
{
	ssize_t br = 0;
	CWT_ASSERT(pchan && pchan->dev);

	if (__rb_ns->size(pchan->rb) > 0) {
		br = __rb_ns->read(pchan->rb, buf, sz);
	} else {
		channel_poll(pchan);
		if( __rb_ns->size(pchan->rb) > 0 )
			br = __rb_ns->read(pchan->rb, buf, sz);
	}

	if( br > 0 )
		pchan->total_br += br;
	return br;
}


static ssize_t channel_write(CwtChannel *pchan, const BYTE *buf, size_t sz)
{
	ssize_t bw;
	CWT_ASSERT(pchan && pchan->dev);
	bw = pchan->dev->write(pchan->dev, buf, sz);
	if( bw > 0 )
		pchan->total_bw += bw;
	return bw;
}

static ssize_t channel_write_byte(CwtChannel *pchan, BYTE ch)
{
	BYTE buf[1];
	buf[0] = ch;
	return channel_write(pchan, buf, 1);
}
