/*
 * endian.cpp
 *
 *  Created on: Jan 27, 2009
 *  Modified on: Apr 10, 2014
 *      Author: wladt
 */

#include "pfs/endian.hpp"

namespace pfs {

endian::type_enum endian::s_native_order ()
{
#if 1 // determine endianess at compile time
	return
#	if PFS_BYTE_ORDER == PFS_LITTLE_ENDIAN
		little_endian;
#	else
		big_endian;
#	endif
#else // determine endianess at run-time
	char test[2] = { 1, 0 };

	return *((short*)test) == 1 ? little_endian : big_endian;
#endif
}

char endian::bswap (char i)
{
    return i;
}

signed char endian::bswap (signed char i)
{
    return i;
}

unsigned char endian::bswap (unsigned char i)
{
    return i;
}

short endian::bswap (short i)
{
    return static_cast<short>(bswap_16(static_cast<int16_t>(i)));
}

unsigned short endian::bswap (unsigned short i)
{
    return static_cast<unsigned short>(bswap_16(static_cast<int16_t>(i)));
}

int endian::bswap (int i)
{
    return sizeof(i) == 4
            ? static_cast<int>(bswap_32(static_cast<int32_t>(i)))
#ifdef PFS_HAVE_INT64 // for ILP64
            : static_cast<int>(bswap_64(static_cast<int64_t>(i)));
#else // for 16-bit OS (MS-DOS)
            : static_cast<int>(bswap_16(static_cast<int16_t>(i)));
#endif
}

unsigned int endian::bswap (unsigned int i)
{
    return static_cast<unsigned int>(bswap(static_cast<int>(i)));
}

long endian::bswap (long i)
{
    return sizeof(i) == 4
            ? static_cast<long>(bswap_32(static_cast<int32_t>(i)))
#ifdef PFS_HAVE_INT64 // for LP64 and ILP64
            : static_cast<long>(bswap_64(static_cast<int64_t>(i)));
#else // for 16-bit OS (MS-DOS)
            : static_cast<long>(bswap_32(static_cast<int32_t>(i)));
#endif
}

unsigned long endian::bswap (unsigned long i)
{
    return static_cast<unsigned long>(bswap(static_cast<long>(i)));
}

#ifdef PFS_HAVE_LONGLONG
long long endian::bswap (long long i)
{
    // TODO Check for Win32

    return sizeof(i) == 4
            ? static_cast<long long>(bswap_32(static_cast<int32_t>(i)))
#ifdef PFS_HAVE_INT64
            : static_cast<long long>(bswap_64(static_cast<int64_t>(i)));
#else // for 16-bit OS (MS-DOS)
            : static_cast<long long>(bswap_32(static_cast<int32_t>(i)));
#endif
}

unsigned long long endian::bswap (unsigned long long i)
{
    return static_cast<unsigned long long>(bswap(static_cast<long long>(i)));
}
#endif

int16_t endian::bswap_16 (int16_t i)
{
	byte_t b1, b2;

	b1 = i & 255;
	b2 = (i >> 8) & 255;
	short r = (b1 << 8) | b2;
	return r;
}

int32_t endian::bswap_32 (int32_t i)
{
	byte_t b1, b2, b3, b4;

	b1 = i & 255;
	b2 = ( i >>  8 ) & 255;
	b3 = ( i >> 16 ) & 255;
	b4 = ( i >> 24 ) & 255;

	return ((int)b1 << 24) + ((int)b2 << 16) + ((int)b3 << 8) + b4;
}

#ifdef PFS_HAVE_INT64
int64_t endian::bswap_64 (int64_t i)
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

	return    (int64_t(b1) << 56)
			+ (int64_t(b2) << 48)
			+ (int64_t(b3) << 40)
			+ (int64_t(b4) << 32)
			+ (int64_t(b5) << 24)
			+ (int64_t(b6) << 16)
			+ (int64_t(b7) <<  8)
			+ b8;
}
#endif

} // namespace pfs
