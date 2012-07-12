/*
 * utils.c
 *
 *  Created on: 29.03.2012
 *      Author: wladt
 */

#include <cwt/utils.h>
#include <time.h>
#include <cwt/str.h>

extern INT64 __calculateCRC64(const void *pdata, size_t nbytes, INT64 crc64);
static void  __swapPtr       (void **p1, void **p2);
static void  __now           (CWT_TIME *dt);
static void  __int16ToBytes  (INT16 i, BYTE bytes[2]);
static INT16 __bytesToInt16  (BYTE bytes[2]);
static void  __int32ToBytes  (INT32 i, BYTE bytes[4]);
static INT32 __bytesToInt32  (BYTE bytes[4]);
static void  __int64ToBytes  (INT64 i, BYTE bytes[8]);
static INT64 __bytesToInt64  (BYTE bytes[8]);


static CwtUtilsNS __cwtUtilsNS = {
	  __calculateCRC64
	, __swapPtr
	, __now
	, __int16ToBytes
	, __bytesToInt16
	, __int32ToBytes
	, __bytesToInt32
	, __int64ToBytes
	, __bytesToInt64
};

DLL_API_EXPORT CwtUtilsNS* cwtUtilsNS(void)
{
	return &__cwtUtilsNS;
}

static void __swapPtr(void **p1, void **p2)
{
	void *p = *p1;
	*p1 = *p2;
	*p2 = p;
}

static void  __now(CWT_TIME *dt)
{
	struct tm tm_;

	CWT_ASSERT(dt);
	cwtStrNS()->memcpy(&tm_, localtime(NULL), sizeof(struct tm));
	dt->year     = tm_.tm_year + 1970;
	dt->mon      = tm_.tm_mon + 1;
	dt->day      = tm_.tm_mday;
	dt->hour     = tm_.tm_hour;
	dt->min      = tm_.tm_min;
	dt->sec      = tm_.tm_sec;
	dt->sec_part = 0L;
}

static void __int16ToBytes(INT16 i, BYTE bytes[2])
{
	bytes[0] = (BYTE)(((UINT16)i) >> 8);
	bytes[1] = (BYTE)((UINT16)i);
}

static INT16 __bytesToInt16(BYTE bytes[2])
{
	return ((bytes[0] & 0xFF) << 8)
		| (bytes[1] & 0xFF);
}


static void __int32ToBytes(INT32 i, BYTE bytes[4])
{
	bytes[0] = (BYTE)(((UINT32)i) >> 24);
	bytes[1] = (BYTE)(((UINT32)i) >> 16);
	bytes[2] = (BYTE)(((UINT32)i) >> 8);
	bytes[3] = (BYTE)((UINT32)i);
}


static INT32 __bytesToInt32(BYTE bytes[4])
{
	return (bytes[0] << 24)
		| ((bytes[1] & 0xFF) << 16)
		| ((bytes[2] & 0xFF) << 8)
		| (bytes[3] & 0xFF);
}

static void __int64ToBytes(INT64 i, BYTE bytes[8])
{
	bytes[0] = (BYTE)(((UINT64)i) >> 56);
	bytes[1] = (BYTE)(((UINT64)i) >> 48);
	bytes[2] = (BYTE)(((UINT64)i) >> 40);
	bytes[3] = (BYTE)(((UINT64)i) >> 32);
	bytes[4] = (BYTE)(((UINT64)i) >> 24);
	bytes[5] = (BYTE)(((UINT64)i) >> 16);
	bytes[6] = (BYTE)(((UINT64)i) >> 8);
	bytes[7] = (BYTE)((UINT64)i);
}


static INT64 __bytesToInt64(BYTE bytes[8])
{
	UINT64 i0, i1, i2, i3, i4, i5, i6, i7, i;
	i0 = bytes[0] & 0xFF;
	i0 = i0 << 56;

	i1 = bytes[1] & 0xFF;
	i1 = i1 << 48;

	i2 = bytes[2] & 0xFF;
	i2 = i2 << 40;

	i3 = bytes[3] & 0xFF;
	i3 = i3 << 32;

	i4 = bytes[4] & 0xFF;
	i4 = i4 << 24;

	i5 = bytes[5] & 0xFF;
	i5 = i5 << 16;

	i6 = bytes[6] & 0xFF;
	i6 = i6 << 8;

	i7 = bytes[7] & 0xFF;

	i = i0 | i1 | i2 | i3 | i4 | i5 | i6 | i7;

	return (INT64)i;
}
