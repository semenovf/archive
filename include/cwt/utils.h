/*
 * utils.h
 *
 *  Created on: 29.03.2012
 *      Author: user
 */

#ifndef __CWT_UTILS_H__
#define __CWT_UTILS_H__

#include <cwt/types.h>

typedef struct _CwtUtilsNS {
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
	void   (*doubleToBytes)  (double d, BYTE bytes[sizeof(double)]);
	double (*bytesToDouble)  (const BYTE bytes[sizeof(double)]);

	BOOL   (*pack)           (const CWT_CHAR *template_str
			, BYTE *buf, size_t buf_sz
			, void *data[], size_t data_count);
	BOOL   (*unpack)         (const CWT_CHAR *template_str
			, BYTE *buf, size_t buf_sz
			, void *data[], size_t data_count);
} CwtUtilsNS;


EXTERN_C_BEGIN
DLL_API_EXPORT CwtUtilsNS* cwtUtilsNS(void);
EXTERN_C_END

#endif /* __CWT_UTILS_H__ */
