/*
 * utils.c
 *
 *  Created on: 29.03.2012
 *      Author: wladt
 */

#include <cwt/utils.h>

DLL_API_EXPORT void cwtSwapPtr(void **p1, void **p2)
{
	void *p = *p1;
	*p1 = *p2;
	*p2 = p;
}

DLL_API_EXPORT void cwtInt16ToBytes(INT16 i, BYTE bytes[2])
{
	bytes[0] = (BYTE)(((UINT16)i) >> 8);
	bytes[1] = (BYTE)((UINT16)i);
}

DLL_API_EXPORT INT16 cwtBytesToInt16(BYTE bytes[2])
{
	return ((bytes[0] & 0xFF) << 8)
		| (bytes[1] & 0xFF);
}


DLL_API_EXPORT void cwtInt32ToBytes(INT32 i, BYTE bytes[4])
{
	bytes[0] = (BYTE)(((UINT32)i) >> 24);
	bytes[1] = (BYTE)(((UINT32)i) >> 16);
	bytes[2] = (BYTE)(((UINT32)i) >> 8);
	bytes[3] = (BYTE)((UINT32)i);
}


DLL_API_EXPORT INT32 cwtBytesToInt32(BYTE bytes[4])
{
	return (bytes[0] << 24)
		| ((bytes[1] & 0xFF) << 16)
		| ((bytes[2] & 0xFF) << 8)
		| (bytes[3] & 0xFF);
}

DLL_API_EXPORT void cwtInt64ToBytes(INT64 i, BYTE bytes[8])
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


DLL_API_EXPORT INT64 cwtBytesToInt64(BYTE bytes[8])
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
