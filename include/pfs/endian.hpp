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

class DLL_API endian
{
public:
	enum type_enum { LittleEndian, BigEndian, NetworkEndian = BigEndian };

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

	static char           swap (char i) { return i; }
	static unsigned char  swap (unsigned char i) { return i; }
	static short          swap (short);
	static unsigned short swap (unsigned short i) { return (unsigned short)swap((short)i); }
	static int            swap (int);
	static unsigned int   swap (unsigned int i) { return (unsigned int)swap((int)i); }
	static long           swap (long);
	static unsigned long  swap (unsigned long i) { return (unsigned long)swap((long)i); }
#ifdef HAVE_LONGLONG
	static long long          swap (long long);
	static unsigned long long swap (unsigned long long i) { return (unsigned long long)swap((long long)i); }
#endif
	static float  swap (float);
	static double swap (double);
};

} // namespace jq

#endif /* __PFS_ENDIAN_HPP__ */
