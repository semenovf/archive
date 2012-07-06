#ifndef __CWT_IO_CHANNEL_H__
#define __CWT_IO_CHANNEL_H__

#include <cwt/types.h>
#include <cwt/global.h>

struct CwtIODevice
{
	void    (*close)(struct CwtIODevice*);
	size_t  (*bytesAvailable)(struct CwtIODevice*);
	ssize_t (*read)(struct CwtIODevice*, BYTE*, size_t);
	ssize_t (*write)(struct CwtIODevice*, const BYTE*, size_t);

	void (*readBegin)(struct CwtIODevice*);    /* may be NULL, so CHANNEL implements this feature */
	void (*readCommit)(struct CwtIODevice*);   /* may be NULL, so CHANNEL implements this feature */
	void (*readRollback)(struct CwtIODevice*); /* may be NULL, so CHANNEL implements this feature */
};
typedef struct CwtIODevice  CwtIODevice;
typedef struct CwtIODevice* CwtIODevicePtr;

struct CwtChannel
{
	CwtIODevicePtr dev;
    unsigned long total_br; /* total bytes read */
    unsigned long total_bw; /* total bytes written */
};
typedef struct CwtChannel  CwtChannel;
typedef struct CwtChannel* CwtChannelPtr;

EXTERN_C_BEGIN

DLL_API_EXPORT CwtChannelPtr cwtNewChannel(CwtIODevicePtr dev);
DLL_API_EXPORT void cwtDeleteChannel(CwtChannelPtr chan);
DLL_API_EXPORT BOOL cwtChannelCanRead(CwtChannelPtr chan);
DLL_API_EXPORT BOOL cwtChannelCanWrite(CwtChannelPtr chan);
DLL_API_EXPORT CwtIODevicePtr cwtChannelDevice(CwtChannelPtr chan);

DLL_API_EXPORT ssize_t cwtChannelRead(CwtChannelPtr chan, BYTE *buf, size_t sz);
DLL_API_EXPORT ssize_t cwtChannelWrite(CwtChannelPtr chan, const BYTE *buf, size_t sz);
DLL_API_EXPORT ssize_t cwtChannelWriteByte(CwtChannelPtr chan, BYTE ch);
DLL_API_EXPORT size_t  cwtChannelBytesAvailable(CwtChannelPtr chan);

DLL_API_EXPORT BOOL cwtChannelCanReadTransaq(CwtChannelPtr chan);
DLL_API_EXPORT void cwtChannelReadBegin(CwtChannelPtr chan);
DLL_API_EXPORT void cwtChannelReadCommit(CwtChannelPtr chan);
DLL_API_EXPORT void cwtChannelReadRollback(CwtChannelPtr chan);

EXTERN_C_END

#endif /* __CWT_IO_CHANNEL_H__ */
