/**
 * @file   bitarray.hpp
 * @author wladt
 * @date   Jun 27, 2013
 *
 * @brief
 */

#ifndef __CWT_BITARRAY_HPP__
#define __CWT_BITARRAY_HPP__

#include <cwt/cwt.hpp>
#include <cwt/array.hpp>
#include <cwt/pimpl.hpp>

CWT_NS_BEGIN

// STL  std::bitset
// Qt   BitArray
// C#   BitArray
// Java BitSet


class DLL_API BitArray
{
	struct Impl {
		Array<uint32_t> a;
		size_t          nbits;
	};

	CWT_PIMPL_COPYABLE(BitArray);

public:
	BitArray ();
	BitArray (size_t size, bool value = false);

	bool	at        (size_t i) const { return testBit(i); }
	void	clear     ()               { detach(); pimpl->nbits = 0; }
	void	clearBit  (size_t i);
	void	setBit    (size_t i);
	void	setBit    (size_t i, bool value) { if (value) setBit(i); else clearBit(i); }
	bool	testBit   (size_t i) const;
	bool	toggleBit (size_t i);

	size_t  count () const { return pimpl->nbits; }
	size_t  size () const  { return pimpl->nbits; }
	size_t  count (bool on) const;

	void	fill (bool value) { fill(value, size()); }
	void	fill (bool value, size_t size) { detach(); *this = BitArray(size, value); }
	void	fill (bool value, size_t begin, size_t end);
	bool	isEmpty () const { return pimpl->nbits == 0; }
	void	resize (size_t size);
	void	truncate (size_t pos) { if (pos < size()) resize(pos); }

	bool	  operator [] (size_t i ) const { return testBit(i); }
	bool	  operator != (const BitArray & other) const;
	bool	  operator == (const BitArray & other) const;
	BitArray& operator &= (const BitArray & other);
	BitArray& operator ^= (const BitArray & other);
	BitArray& operator |= (const BitArray & other);
	BitArray  operator ~  () const;

	friend DLL_API BitArray	operator & (const BitArray & a1, const BitArray & a2);
	friend DLL_API BitArray	operator ^ (const BitArray & a1, const BitArray & a2);
	friend DLL_API BitArray	operator | (const BitArray & a1, const BitArray & a2);
};

inline void BitArray::clearBit(size_t i)
{
	detach();
	CWT_ASSERT(i < size());
	*(pimpl->a.data() + (i >> 5)) &= ~ uint32_t(1 << (i & 31));
}

inline void BitArray::setBit(size_t i)
{
	detach();
	CWT_ASSERT(i < size());
	*(pimpl->a.data() + (i >> 5)) |= uint32_t(1 << (i & 31));
}


inline bool BitArray::testBit(size_t i) const
{
	CWT_ASSERT(i < size());
	return  (*(pimpl->a.constData() + (i >> 5)) & (1 << (i & 31))) != 0;
}

inline bool BitArray::toggleBit(size_t i)
{
	detach();
	CWT_ASSERT(i < size());
	uint32_t b = uint32_t(1 << (i & 31));
	uint32_t *p = pimpl->a.data() + (i >> 5);
	uint32_t c = uint32_t(*p & b);
	*p ^= b;
	return c != 0;
}

CWT_NS_END

#endif /* __CWT_BITARRAY_HPP__ */
