#ifndef __CWT_IO_CHANNEL_H__
#define __CWT_IO_CHANNEL_H__

#include <cwt/types.h>
#include <cwt/global.h>
#include <cwt/ringbuf.h>
#include <cwt/bytearr.h>

typedef struct _CwtIODevice
{
	void          (*close)         (struct _CwtIODevice*);
	size_t        (*bytesAvailable)(struct _CwtIODevice*);
	ssize_t       (*read)          (struct _CwtIODevice*, BYTE*, size_t);
	ssize_t       (*write)         (struct _CwtIODevice*, const BYTE*, size_t);
} CwtIODevice;

typedef struct _CwtChannel
{
	CwtIODevice   *dev;
    unsigned long  total_br; /* total bytes read */
    unsigned long  total_bw; /* total bytes written */
    CwtRingBuffer *rb;
} CwtChannel;


typedef struct _CwtChannelNS {
	CwtChannel*   (*open)          (CwtIODevice *pdev);
	void          (*close)         (CwtChannel*);
	BOOL          (*canRead)       (CwtChannel*);
	BOOL          (*canWrite)      (CwtChannel*);
	CwtIODevice*  (*device)        (CwtChannel*);

	BOOL          (*atEnd)         (CwtChannel*);
	BOOL          (*canReadLine)   (CwtChannel*);
	BOOL          (*readLine)      (CwtChannel*, CwtByteArray *ba);
	ssize_t       (*poll)          (CwtChannel*);
	ssize_t       (*read)          (CwtChannel*, BYTE *buf, size_t sz);
	ssize_t       (*write)         (CwtChannel*, const BYTE *buf, size_t sz);
	ssize_t       (*writeByte)     (CwtChannel*, BYTE ch);
	size_t        (*bytesAvailable)(CwtChannel*);
	BOOL          (*slurp)         (CwtChannel*, CwtByteArray *buffer);
} CwtChannelNS;

EXTERN_C_BEGIN
DLL_API_EXPORT CwtChannelNS* cwt_channel_ns(void);
EXTERN_C_END

#endif /* __CWT_IO_CHANNEL_H__ */
