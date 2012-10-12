/*
 * utils.h
 *
 *  Created on: 29.03.2012
 *      Author: user
 */

#ifndef __CWT_UTILS_H__
#define __CWT_UTILS_H__

#include <cwt/types.h>

typedef struct _CwtUri {
	CWT_CHAR *scheme;
	CWT_CHAR *host;
	UINT16    port;
	BOOL      host_is_ip;
	CWT_CHAR *path;
	CWT_CHAR *query;
} CwtUri;

typedef struct CwtUtilsNS {
	INT32  (*calculateCRC32) (const void *pdata, size_t nbytes, INT32 initial);
	INT64  (*calculateCRC64) (const void *pdata, size_t nbytes, INT64 initial);
	void   (*swapPtr)        (void **p1, void **p2);
	void   (*now)            (CWT_TIME *dt);

	void   (*int16ToBytes)   (INT16 i, BYTE bytes[2]);
	INT16  (*bytesToInt16)   (const BYTE bytes[2]);
	void   (*int32ToBytes)   (INT32 i, BYTE bytes[4]);
	INT32  (*bytesToInt32)   (const BYTE bytes[4]);
	void   (*int64ToBytes)   (INT64 i, BYTE bytes[8]);
	INT64  (*bytesToInt64)   (const BYTE bytes[8]);
	void   (*floatToBytes)   (float f, BYTE bytes[4]);
	float  (*bytesToFloat)   (const BYTE bytes[4]);

	BOOL   (*parseURI)       (const CWT_CHAR *uri_string, CwtUri *uri);
	void   (*initURI)        (CwtUri *uri);
	void   (*destroyURI)     (CwtUri *uri);
} CwtUtilsNS;


EXTERN_C_BEGIN
DLL_API_EXPORT CwtUtilsNS* cwtUtilsNS(void);
EXTERN_C_END

#endif /* __CWT_UTILS_H__ */
