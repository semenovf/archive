/*
 * endian.cpp
 *
 *  Created on: Jan 27, 2009
 *  Modified on: Apr 10, 2014
 *      Author: wladt
 */

#include "../include/pfs/endian.hpp"

namespace pfs
{

endian::type_enum endian::type ()
{
#if 1 // determine endianess at compile time
	return
#	if PFS_BYTE_ORDER == PFS_LITTLE_ENDIAN
		LittleEndian;
#	else
		BigEndian;
#	endif
#else // determine endianess at run-time
	char test[2] = { 1, 0 };

	return *((short*)test) == 1 ? LittleEndian : BigEndian;
#endif
}


short endian::swap (short int i)
{
	PFS_ASSERT(sizeof(i) == 2);

	byte_t b1, b2;

	b1 = i & 255;
	b2 = (i >> 8) & 255;
	short r = (b1 << 8) | b2;
	return r;
}

int endian::swap (int i)
{
	if (sizeof(i) == 2)
		return int(swap(short(i)));

	byte_t b1, b2, b3, b4;

	b1 = i & 255;
	b2 = ( i >>  8 ) & 255;
	b3 = ( i >> 16 ) & 255;
	b4 = ( i >> 24 ) & 255;

	return ((int)b1 << 24) + ((int)b2 << 16) + ((int)b3 << 8) + b4;
}

long endian::swap (long i)
{
	if (sizeof(i) == 4)
		return long(swap(int(i)));

	PFS_ASSERT(sizeof(i) == 8);
	return long(swap((long long)i));
}

#ifdef HAVE_LONGLONG
long long endian::swap (long long i)
{
	byte_t b1, b2, b3, b4, b5, b6, b7, b8;

	b1 = i & 255;
	b2 = (i >>  8) & 255;
	b3 = (i >> 16) & 255;
	b4 = (i >> 24) & 255;
	b5 = (i >> 32) & 255;
	b6 = (i >> 40) & 255;
	b7 = (i >> 48) & 255;
	b8 = (i >> 56) & 255;

	return    (((long long)b1) << 56)
			+ (((long long)b2) << 48)
			+ (((long long)b3) << 40)
			+ (((long long)b4) << 32)
			+ (((long long)b5) << 24)
			+ (((long long)b6) << 16)
			+ (((long long)b7) <<  8)
			+ b8;
}
#endif

float endian::swap (float f)
{
	union {
		float f;
		char b[4];
	} d1, d2;

	d1.f = f;
	d2.b[0] = d1.b[3];
	d2.b[1] = d1.b[2];
	d2.b[2] = d1.b[1];
	d2.b[3] = d1.b[0];
	return d2.f;
}

double endian::swap (double f)
{
	union {
		double f;
		char b[8];
	} d1, d2;

	d1.f = f;
	d2.b[0] = d1.b[7];
	d2.b[1] = d1.b[6];
	d2.b[2] = d1.b[5];
	d2.b[3] = d1.b[4];
	d2.b[4] = d1.b[3];
	d2.b[5] = d1.b[2];
	d2.b[6] = d1.b[1];
	d2.b[7] = d1.b[0];
	return d2.f;
}


} // namespace cdtl
