/*
 * bitarray_impl.hpp
 *
 *  Created on: 09 Jan 2015
 *      Author: wladt
 */

#ifndef __PFS_BIT_ARRAY_IMPL_HPP__
#define __PFS_BIT_ARRAY_IMPL_HPP__

#include <pfs/array.hpp>

namespace pfs {

class bit_array_impl
{
public:
	typedef size_t size_type;

private:
	array<uint32_t> _a;
	size_type       _nbits;

public:
	bit_array_impl () : _nbits(0) {}
	virtual ~bit_array_impl () {}

	size_type size ()          const { return _nbits; }

	void clearBit (size_type i)
	{
		*(_a.data() + (i >> 5)) &= ~ uint32_t(1 << (i & 31));
	}

	void setBit (size_type i)
	{
		*(_a.data() + (i >> 5)) |= uint32_t(1 << (i & 31));
	}

	void setBit  (size_type i, bool value)
	{
		if (value) setBit(i);
		else clearBit(i);
	}

	bool testBit (size_type i) const
	{
		return  (*(_a.constData() + (i >> 5)) & (1 << (i & 31))) != 0;
	}

	bool toggleBit (size_type i)
	{
		uint32_t b = uint32_t(1 << (i & 31));
		uint32_t *p = _a.data() + (i >> 5);
		uint32_t c = uint32_t(*p & b);
		*p ^= b;
		return c != 0;
	}

	size_type count (bool on) const;
	void fill (bool value, size_type begin, size_type end);
	void resize (size_type size);
};

} // pfs

#endif // __PFS_BIT_ARRAY_IMPL_HPP__
