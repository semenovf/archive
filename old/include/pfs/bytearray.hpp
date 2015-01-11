/*
 * bytearray.hpp
 *
 *  Created on: Nov 21, 2013
 *      Author: wladt
 */

#ifndef __PFS_BYTEARRAY_HPP__
#define __PFS_BYTEARRAY_HPP__

#include "vector.hpp"
#include "bits/allocator.hpp"
#include "endian.hpp"
//#include "bits/cast_traits.hpp"
#include <string>
#include <cstring>
//#include <ostream>

namespace pfs {

class DLL_API bytearray : protected vector<char, bytearray_allocator<char> >
{
	friend struct bytearray_terminator;

protected:
	typedef vector<char, bytearray_allocator<char> > base_class;
	typedef bytearray self_class;

public: // typedefs
	typedef base_class::value_type             value_type;
	typedef base_class::pointer                pointer;
	typedef base_class::const_pointer          const_pointer;
	typedef base_class::size_type              size_type;
	typedef base_class::difference_type        difference_type;

	typedef base_class::reference              reference;
	typedef base_class::const_reference        const_reference;
	typedef base_class::iterator               iterator;
	typedef base_class::const_iterator         const_iterator;
	typedef base_class::reverse_iterator	   reverse_iterator;
	typedef base_class::const_reverse_iterator const_reverse_iterator;

	typedef value_type char_type;

public:
	reference front () const { return at(0); }
	reference first () const { return at(0); }
	reference back  () const { return at(size()-1); }
	reference last  () const { PFS_ASSERT(size() > 0); return at(size()-1); }

	bytearray substr (const_iterator from, const_iterator last) const
	{
		return bytearray(from, last);
	}

	bytearray substr (size_type pos) const
	{
		PFS_ASSERT(pos < size());
		return substr(cbegin() + pos, cbegin() + pos + size());
	}

	bytearray substr (size_type pos, size_type n) const
	{
		PFS_ASSERT(pos < size());
		if (pos + n > size())
			n = size() - pos;
		return substr(cbegin() + pos, cbegin() + pos + n);
	}

	bytearray mid (size_type pos, size_type n) const
	{
		return substr(pos, n);
	}

	bytearray left   (size_type n) const
	{
		return size() <= n
				? bytearray(*this)
				: bytearray(cbegin(), cbegin() + n);
	}
	bytearray right  (size_type n) const
	{
		return (size() <= n)
				? bytearray(*this)
				: bytearray(cbegin() + n, cend());
	}

	real_t   toReal   (bool * ok = 0, char decimalPoint = '.') const;
	long_t   toLong   (bool * ok = 0, int base = 10) const;
	ulong_t	 toULong  (bool * ok = 0, int base = 10) const;
	int_t	 toInt    (bool * ok = 0, int base = 10) const;
	uint_t	 toUInt   (bool * ok = 0, int base = 10) const;
	short_t	 toShort  (bool * ok = 0, int base = 10) const;
	ushort_t toUShort (bool * ok = 0, int base = 10) const;
	sbyte_t  toSByte  (bool * ok = 0, int base = 10) const;
	byte_t	 toByte   (bool * ok = 0, int base = 10) const;

};

} // pfs

#endif /* __PFS_BYTEARRAY_HPP__ */
