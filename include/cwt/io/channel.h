#ifndef __CWT_IO_CHANNEL_H__
#define __CWT_IO_CHANNEL_H__

#include <cwt/types.h>
#include <cwt/global.h>
#include <cwt/ringbuf.h>
#include <cwt/bytearr.h>

typedef struct CwtIODevice
{
	void    (*close)(struct CwtIODevice*);
	size_t  (*bytesAvailable)(struct CwtIODevice*);
	ssize_t (*read)(struct CwtIODevice*, BYTE*, size_t);
	ssize_t (*write)(struct CwtIODevice*, const BYTE*, size_t);

	void    (*readBegin)(struct CwtIODevice*);    /* may be NULL, so CHANNEL implements this feature */
	void    (*readCommit)(struct CwtIODevice*);   /* may be NULL, so CHANNEL implements this feature */
	void    (*readRollback)(struct CwtIODevice*); /* may be NULL, so CHANNEL implements this feature */
} CwtIODevice/*, *CwtIODevicePtr*/;

typedef struct CwtChannel
{
	CwtIODevice  *dev;
    unsigned long total_br; /* total bytes read */
    unsigned long total_bw; /* total bytes written */
    CwtRingBuf    *rb;
} CwtChannel;


typedef struct CwtChannelNS {
	CwtChannel*  (*create)         (CwtIODevice *pdev);
	void         (*free)           (CwtChannel*);
	BOOL         (*canRead)        (CwtChannel*);
	BOOL         (*canWrite)       (CwtChannel*);
	CwtIODevice* (*device)         (CwtChannel*);

	BOOL         (*atEnd)          (CwtChannel*);
	BOOL         (*canReadLine)    (CwtChannel*);
	BOOL         (*readLine)       (CwtChannel*, CwtByteArray *ba);
	ssize_t      (*poll)           (CwtChannel*);
	ssize_t      (*read)           (CwtChannel*, BYTE *buf, size_t sz);
	ssize_t      (*write)          (CwtChannel*, const BYTE *buf, size_t sz);
	ssize_t      (*writeByte)      (CwtChannel*, BYTE ch);
	size_t       (*bytesAvailable) (CwtChannel*);

	BOOL         (*canReadTransaq) (CwtChannel*); /* TODO obsolete */
	void         (*readBegin)      (CwtChannel*); /* TODO obsolete */
	void         (*readCommit)     (CwtChannel*); /* TODO obsolete */
	void         (*readRollback)   (CwtChannel*); /* TODO obsolete */
} CwtChannelNS;

EXTERN_C_BEGIN
DLL_API_EXPORT CwtChannelNS* cwtChannelNS(void);
EXTERN_C_END

#endif /* __CWT_IO_CHANNEL_H__ */
