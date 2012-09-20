#ifndef __CWT_IO_CHANNEL_H__
#define __CWT_IO_CHANNEL_H__

#include <cwt/types.h>
#include <cwt/global.h>
#include <cwt/ringbuf.h>
#include <cwt/bytearr.h>

/*
#define CWT_SHARED_DATA struct { size_t nrefs; } d
#define CWT_SHARED_INC(p) ((CwtIODevice*)p)->d.nrefs++
#define CWT_SHARED_DEC(p) ((CwtIODevice*)p)->d.nrefs--
#define CWT_SHARED_NUL(p) (((CwtIODevice*)p)->d.nrefs==0)
*/

typedef struct _CwtIODevice
{
/*	CWT_SHARED_DATA;*/
	void    (*close)(struct _CwtIODevice*);
	size_t  (*bytesAvailable)(struct _CwtIODevice*);
	ssize_t (*read)(struct _CwtIODevice*, BYTE*, size_t);
	ssize_t (*write)(struct _CwtIODevice*, const BYTE*, size_t);
	/*struct _CwtIODevice* (*accept) (struct _CwtIODevice*);*/
} CwtIODevice;

typedef struct _CwtChannel
{
	CwtIODevice  *dev;
    unsigned long total_br; /* total bytes read */
    unsigned long total_bw; /* total bytes written */
    CwtRingBuf    *rb;
} CwtChannel;


typedef struct _CwtChannelNS {
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
	/*CwtIODevice* (*accept)         (CwtChannel*);*/
} CwtChannelNS;

EXTERN_C_BEGIN
DLL_API_EXPORT CwtChannelNS* cwtChannelNS(void);
EXTERN_C_END

#endif /* __CWT_IO_CHANNEL_H__ */
