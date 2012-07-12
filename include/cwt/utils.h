/*
 * utils.h
 *
 *  Created on: 29.03.2012
 *      Author: user
 */

#ifndef __CWT_UTILS_H__
#define __CWT_UTILS_H__

#include <cwt/types.h>

typedef struct CwtUtilsNS {
	INT64  (*calculateCRC64) (const void *pdata, size_t nbytes, INT64 crc64);
	void   (*swapPtr)        (void **p1, void **p2);
	void   (*now)            (CWT_TIME *dt);

	void   (*int16ToBytes)   (INT16 i, BYTE bytes[2]);
	INT16  (*bytesToInt16)   (BYTE bytes[2]);
	void   (*int32ToBytes)   (INT32 i, BYTE bytes[4]);
	INT32  (*bytesToInt32)   (BYTE bytes[4]);
	void   (*int64ToBytes)   (INT64 i, BYTE bytes[8]);
	INT64  (*bytesToInt64)   (BYTE bytes[8]);
} CwtUtilsNS;


EXTERN_C_BEGIN
DLL_API_EXPORT CwtUtilsNS* cwtUtilsNS(void);
EXTERN_C_END

#endif /* __CWT_UTILS_H__ */
