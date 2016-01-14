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
	enum type_enum { little_endian, big_endian, network_endian = big_endian };

private:
	type_enum _value;

public:
	endian ()
		: _value(native_order())
	{}

	endian (type_enum type)
		: _value(type)
	{}

public:
	type_enum type () const
	{
		return _value;
	}

	static type_enum native_order ();

	static type_enum network_order ()
	{
		return network_endian;
	}

	static bool big_endian_system ()
	{
		return native_order() == little_endian ? false : true;
	}

	static bool little_endian_system ()
	{
		return native_order() == little_endian ? true : false;
	}

	template<typename T>
	static T to_big_endian (T i)
	{
		return native_order() == little_endian ? bswap(i) : i;
	}

	template<typename T>
	static T to_little_endian (T i)
	{
		return native_order() == little_endian ? i : bswap(i);
	}

	template<typename T>
	static T to_network_order (T i) // to big-endian
	{
		return to_big_endian(i);
	}

	template<typename T>
	static T to_host_order (T i)
	{
		return native_order() == little_endian ? swap(i) : i;
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
