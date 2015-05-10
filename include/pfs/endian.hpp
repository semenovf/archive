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

namespace pfs {

class DLL_API endian
{
public:
	enum type_enum { LittleEndian, BigEndian, NetworkEndian = BigEndian };

private:
	endian ();

public:
	static type_enum type ();
	static type_enum nativeOrder () { return type(); }

	static bool isBigEndianSystem ();
	static bool isLittleEndianSystem ();

	template<typename T>
	static T toBigEndian (T i)
	{
		return type() == LittleEndian ? swap(i) :  i;
	}

	template<typename T>
	static T toLittleEndian (T i)
	{
		return type() == LittleEndian ? i : swap(i);
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

	static char       swap (char i);
	static int8_t     swap (int8_t i);
	static uint8_t    swap (uint8_t i);
	static int16_t    swap (int16_t i);
	static uint16_t   swap (uint16_t i);
	static int32_t    swap (int32_t i);
	static uint32_t   swap (uint32_t i);

#ifdef PFS_HAVE_INT64
	static int64_t    swap (int64_t i);
	static uint64_t   swap (uint64_t i);
#endif
};

} // namespace pfs

#endif /* __PFS_ENDIAN_HPP__ */
