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

private:
	static endian::type_enum s_native_order ();

public:
	endian ()
		: _value(s_native_order())
	{}

	endian (const endian & other)
		: _value(other._value)
	{}

	endian & operator = (const endian & other)
	{
		_value = other._value;
		return *this;
	}

	endian (type_enum type)
		: _value(type)
	{}

	type_enum type () const
	{
		return _value;
	}

	friend bool operator == (const endian & lhs, const endian & rhs)
	{
		return lhs._value == rhs._value;
	}

	friend bool operator != (const endian & lhs, const endian & rhs)
	{
		return ! operator == (lhs, rhs);
	}

	friend bool operator == (const endian & lhs, type_enum rhs)
	{
		return lhs._value == rhs;
	}

	friend bool operator != (const endian & lhs, type_enum rhs)
	{
		return ! operator == (lhs, rhs);
	}

	friend bool operator == (type_enum lhs, const endian & rhs)
	{
		return lhs == rhs._value;
	}

	friend bool operator != (type_enum lhs, const endian & rhs)
	{
		return ! operator == (lhs, rhs);
	}

public:
	static endian native_order ()
	{
		return s_native_order();
	}

	static endian network_order ()
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
	T to_big_endian (T i) const
	{
		return (_value == little_endian ? bswap(i) : i);
	}

	template<typename T>
	T to_little_endian (T i) const
	{
		return (_value == little_endian ? i : bswap(i));
	}

	template<typename T>
	T to_network_order (T i) const // to big-endian
	{
		return to_big_endian(i);
	}

	template<typename T>
	T to_host_order (T i) const
	{
		return _value != native_order().type() ? bswap(i) : i;
	}

   	template<typename T>
	T convert (T i) const
	{
		return _value != native_order().type() ? bswap(i) : i;
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

#undef bswap_16
#undef bswap_32
	static int16_t    bswap_16 (int16_t i);
	static int32_t    bswap_32 (int32_t i);

#ifdef PFS_HAVE_INT64
#undef bswap_64
	static int64_t    bswap_64 (int64_t i);
#endif
};

} // namespace pfs

#endif /* __PFS_ENDIAN_HPP__ */
