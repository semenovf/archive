/*
 * endian.hpp
 *
 *  Created on: Jan 27, 2009
 *  Modified on: Apr 10, 2014
 *  Modified on: May 08, 2015
 *
 *  Author: wladt
 */

#ifndef __PFS_ENDIAN_HPP__
#define __PFS_ENDIAN_HPP__

#include <pfs.hpp>
#include <pfs/sys/endian.h>

namespace pfs {

class DLL_API endian
{
public:
	enum type_enum { LittleEndian, BigEndian, NetworkEndian = BigEndian };

private:
	endian ();

public:
	static type_enum type ();
	static type_enum nativeOrder () { return type(); } // XXX DEPRECATED, use native_order() unstead.
	static type_enum native_order () { return type(); }
	static type_enum network_order () { return NetworkEndian; }

	static bool isBigEndianSystem ();
	static bool isLittleEndianSystem ();

	template<typename T>
	static T toBigEndian (T i)
	{
		return type() == LittleEndian ? bswap(i) : i;
	}

	template<typename T>
	static T toLittleEndian (T i)
	{
		return type() == LittleEndian ? i : bswap(i);
	}

	template<typename T>
	static T toNetworkOrder (T i) // to big-endian
	{
		return toBigEndian(i);
	}

	template<typename T>
	static T toHostOrder (T i)
	{
		return type() == LittleEndian ? swap(i) : i;
	}

	static char           bswap (char i);
	static signed char    bswap (signed char i);
	static unsigned char  bswap (unsigned char i);
    static short          bswap (short i);
    static unsigned short bswap (unsigned short i);
    static int            bswap (int i);
    static unsigned int   bswap (unsigned int i);
    static long           bswap (long i);
    static unsigned long  bswap (unsigned long i);

#ifdef PFS_HAVE_LONGLONG
    static long long  bswap (long long i);
    static unsigned long long bswap (unsigned long long i);
#endif

	static int16_t    bswap_16 (int16_t i);
	static int32_t    bswap_32 (int32_t i);

#ifdef PFS_HAVE_INT64
	static int64_t    bswap_64 (int64_t i);
#endif
};

} // namespace pfs

#endif /* __PFS_ENDIAN_HPP__ */
