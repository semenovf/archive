#ifndef __CWT_IO_CHANNEL_H__
#define __CWT_IO_CHANNEL_H__

#include <cwt/types.h>
#include <cwt/global.h>

typedef struct CwtIODevice
{
	void    (*close)(struct CwtIODevice*);
	size_t  (*bytesAvailable)(struct CwtIODevice*);
	ssize_t (*read)(struct CwtIODevice*, BYTE*, size_t);
	ssize_t (*write)(struct CwtIODevice*, const BYTE*, size_t);

	void    (*readBegin)(struct CwtIODevice*);    /* may be NULL, so CHANNEL implements this feature */
	void    (*readCommit)(struct CwtIODevice*);   /* may be NULL, so CHANNEL implements this feature */
	void    (*readRollback)(struct CwtIODevice*); /* may be NULL, so CHANNEL implements this feature */
} CwtIODevice, *CwtIODevicePtr;

typedef struct CwtChannel
{
	CwtIODevicePtr dev;
    unsigned long total_br; /* total bytes read */
    unsigned long total_bw; /* total bytes written */
} CwtChannel, *CwtChannelPtr;


typedef struct CwtChannelNS {
	CwtChannel*  (*create)         (CwtIODevice *pdev);
	void         (*init)           (CwtChannel*, CwtIODevice *pdev);
	void         (*destroy)        (CwtChannel*);
	void         (*free)           (CwtChannel*);
	BOOL         (*canRead)        (CwtChannel*);
	BOOL         (*canWrite)       (CwtChannel*);
	CwtIODevice* (*device)         (CwtChannel*);

	ssize_t      (*read)           (CwtChannel*, BYTE *buf, size_t sz);
	ssize_t      (*write)          (CwtChannel*, const BYTE *buf, size_t sz);
	ssize_t      (*writeByte)      (CwtChannel*, BYTE ch);
	size_t       (*bytesAvailable) (CwtChannel*);

	BOOL         (*canReadTransaq) (CwtChannel*);
	void         (*readBegin)      (CwtChannel*);
	void         (*readCommit)     (CwtChannel*);
	void         (*readRollback)   (CwtChannel*);
} CwtChannelNS;

EXTERN_C_BEGIN
DLL_API_EXPORT CwtChannelNS* cwtChannelNS(void);
EXTERN_C_END

#endif /* __CWT_IO_CHANNEL_H__ */
