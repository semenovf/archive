/*
 * endian.hpp
 *
 *  Created on: Jan 27, 2009
 *  Modified on: Apr 10, 2014
 *
 *  Author: wladt
 */

#ifndef __PFS_ENDIAN_HPP__
#define __PFS_ENDIAN_HPP__

#include "pfs.hpp"

namespace pfs {

class endian
{
public:
	enum type_enum { LittleEndian, BigEndian };

private:
	endian ();

public:
	static type_enum type ();
	static type_enum nativeOrder () { return type(); }

	static bool isBigEndianSystem ()
	{
		return type() == LittleEndian ? false : true;
	}

	static bool isLittleEndianSystem ()
	{
		return type() == LittleEndian ? true : false;
	}


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

	static short swap (char i) { return i; }
	static short swap (short);
	static int   swap (int);
	static long  swap (long);
#ifdef HAVE_LONGLONG
	static long long    swap (long long);
#endif
	static float  swap (float);
	static double swap (double);
};

} // namespace jq

#endif /* __PFS_ENDIAN_HPP__ */
