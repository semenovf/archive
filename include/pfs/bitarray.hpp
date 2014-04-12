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

#ifdef PFS_CC_MSVC
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif

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

	pimpl _d;

public:
	bitarray ();
	bitarray (size_t size, bool value = false);

	bool	at        (size_t i) const { return testBit(i); }
	void	clear     ()               { _d.detach(); _d.cast<impl>()->nbits = 0; }
	void	clearBit  (size_t i);
	void	setBit    (size_t i);
	void	setBit    (size_t i, bool value) { if (value) setBit(i); else clearBit(i); }
	bool	testBit   (size_t i) const;
	bool	toggleBit (size_t i);

	size_t  count () const { return _d.cast<impl>()->nbits; }
	size_t  size () const  { return _d.cast<impl>()->nbits; }
	size_t  count (bool on) const;

	void	fill (bool value) { fill(value, size()); }
	void	fill (bool value, size_t size) { _d.detach(); *this = bitarray(size, value); }
	void	fill (bool value, size_t begin, size_t end);
	bool	isEmpty () const { return _d.cast<impl>()->nbits == 0; }
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
	_d.detach();
	PFS_ASSERT(i < size());
	*(_d.cast<impl>()->a.data() + (i >> 5)) &= ~ uint32_t(1 << (i & 31));
}

inline void bitarray::setBit(size_t i)
{
	_d.detach();
	PFS_ASSERT(i < size());
	*(_d.cast<impl>()->a.data() + (i >> 5)) |= uint32_t(1 << (i & 31));
}


inline bool bitarray::testBit(size_t i) const
{
	PFS_ASSERT(i < size());
	return  (*(_d.cast<impl>()->a.constData() + (i >> 5)) & (1 << (i & 31))) != 0;
}

inline bool bitarray::toggleBit(size_t i)
{
	_d.detach();
	PFS_ASSERT(i < size());
	uint32_t b = uint32_t(1 << (i & 31));
	uint32_t *p = _d.cast<impl>()->a.data() + (i >> 5);
	uint32_t c = uint32_t(*p & b);
	*p ^= b;
	return c != 0;
}

} // pfs

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

#endif /* __PFS_BITARRAY_HPP__ */
