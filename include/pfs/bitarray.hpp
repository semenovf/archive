/**
 * @file   bitarray.hpp
 * @author wladt
 * @date   Jun 27, 2013
 *
 * @brief
 */

#ifndef __PFS_BITARRAY_HPP__
#define __PFS_BITARRAY_HPP__

#include <pfs.hpp>
#include <pfs/array.hpp>
#include <pfs/pimpl.hpp>

namespace pfs {

// STL  std::bitset
// Qt   bitarray
// C#   bitarray
// Java BitSet


class DLL_API bitarray
{
	struct impl {
		array<uint32_t> a;
		size_t          nbits;
	};

	PFS_PIMPL_INLINE(bitarray, impl);

public:
	bitarray ();
	bitarray (size_t size, bool value = false);

	bool	at        (size_t i) const { return testBit(i); }
	void	clear     ()               { detach(); _pimpl->nbits = 0; }
	void	clearBit  (size_t i);
	void	setBit    (size_t i);
	void	setBit    (size_t i, bool value) { if (value) setBit(i); else clearBit(i); }
	bool	testBit   (size_t i) const;
	bool	toggleBit (size_t i);

	size_t  count () const { return _pimpl->nbits; }
	size_t  size () const  { return _pimpl->nbits; }
	size_t  count (bool on) const;

	void	fill (bool value) { fill(value, size()); }
	void	fill (bool value, size_t size) { detach(); *this = bitarray(size, value); }
	void	fill (bool value, size_t begin, size_t end);
	bool	isEmpty () const { return _pimpl->nbits == 0; }
	void	resize (size_t size);
	void	truncate (size_t pos) { if (pos < size()) resize(pos); }

	bool	  operator [] (size_t i ) const { return testBit(i); }
	bool	  operator != (const bitarray & other) const;
	bool	  operator == (const bitarray & other) const;
	bitarray& operator &= (const bitarray & other);
	bitarray& operator ^= (const bitarray & other);
	bitarray& operator |= (const bitarray & other);
	bitarray  operator ~  () const;

	friend DLL_API bitarray	operator & (const bitarray & a1, const bitarray & a2);
	friend DLL_API bitarray	operator ^ (const bitarray & a1, const bitarray & a2);
	friend DLL_API bitarray	operator | (const bitarray & a1, const bitarray & a2);
};

inline void bitarray::clearBit(size_t i)
{
	detach();
	PFS_ASSERT(i < size());
	*(_pimpl->a.data() + (i >> 5)) &= ~ uint32_t(1 << (i & 31));
}

inline void bitarray::setBit(size_t i)
{
	detach();
	PFS_ASSERT(i < size());
	*(_pimpl->a.data() + (i >> 5)) |= uint32_t(1 << (i & 31));
}


inline bool bitarray::testBit(size_t i) const
{
	PFS_ASSERT(i < size());
	return  (*(_pimpl->a.constData() + (i >> 5)) & (1 << (i & 31))) != 0;
}

inline bool bitarray::toggleBit(size_t i)
{
	detach();
	PFS_ASSERT(i < size());
	uint32_t b = uint32_t(1 << (i & 31));
	uint32_t *p = _pimpl->a.data() + (i >> 5);
	uint32_t c = uint32_t(*p & b);
	*p ^= b;
	return c != 0;
}

} // pfs

#endif /* __PFS_BITARRAY_HPP__ */
