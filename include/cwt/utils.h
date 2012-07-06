/*
 * utils.h
 *
 *  Created on: 29.03.2012
 *      Author: user
 */

#ifndef __CWT_UTILS_H__
#define __CWT_UTILS_H__

#include <cwt/types.h>

EXTERN_C_BEGIN

DLL_API_EXPORT INT64 cwtCalculateCRC64(const void *pdata, size_t nbytes, INT64 crc64);
DLL_API_EXPORT void cwtSwapPtr(void **p1, void **p2);

DLL_API_EXPORT void cwtInt16ToBytes(INT16 i, BYTE bytes[2]);
DLL_API_EXPORT INT16 cwtBytesToInt16(BYTE bytes[2]);
DLL_API_EXPORT void cwtInt32ToBytes(INT32 i, BYTE bytes[4]);
DLL_API_EXPORT INT32 cwtBytesToInt32(BYTE bytes[4]);
DLL_API_EXPORT void  cwtInt64ToBytes(INT64 i, BYTE bytes[8]);
DLL_API_EXPORT INT64 cwtBytesToInt64(BYTE bytes[8]);

EXTERN_C_END

#endif /* __CWT_UTILS_H__ */
