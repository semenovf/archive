/*
 * endian.cpp
 *
 *  Created on: Jan 27, 2009
 *  Modified on: Apr 10, 2014
 *      Author: wladt
 */

#include "pfs/endian.hpp"

namespace pfs {

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

bool endian::isBigEndianSystem ()
{
	return type() == LittleEndian ? false : true;
}

bool endian::isLittleEndianSystem ()
{
	return type() == LittleEndian ? true : false;
}

char endian::swap (char i)
{
	return i;
}

int8_t endian::swap (int8_t i)
{
	return i;
}

uint8_t endian::swap (uint8_t i)
{
	return i;
}

int16_t endian::swap (int16_t i)
{
	byte_t b1, b2;

	b1 = i & 255;
	b2 = (i >> 8) & 255;
	short r = (b1 << 8) | b2;
	return r;
}

uint16_t endian::swap (uint16_t i)
{
	return static_cast<uint16_t>(swap(static_cast<int16_t>(i)));
}

int32_t endian::swap (int32_t i)
{
	byte_t b1, b2, b3, b4;

	b1 = i & 255;
	b2 = ( i >>  8 ) & 255;
	b3 = ( i >> 16 ) & 255;
	b4 = ( i >> 24 ) & 255;

	return ((int)b1 << 24) + ((int)b2 << 16) + ((int)b3 << 8) + b4;
}

uint32_t endian::swap (uint32_t i)
{
	return static_cast<uint32_t>(swap(static_cast<int32_t>(i)));
}


#ifdef PFS_HAVE_INT64
int64_t endian::swap (int64_t i)
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

uint64_t endian::swap (uint64_t i)
{
	return static_cast<uint64_t>(swap(static_cast<int64_t>(i)));
}
#endif

} // namespace pfs
