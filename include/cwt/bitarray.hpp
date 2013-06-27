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
#include <cwt/pimpl.hpp>

CWT_NS_BEGIN

// STL  std::bitset
// Qt   BitArray
// C#   BitArray
// Java BitSet

class DLL_API BitArray
{
	CWT_PIMPL_DECL_COPYABLE(BitArray);

public:
	BitArray ();
	BitArray (size_t size, bool value = false);

	bool	at (size_t i) const;
	void	clear ();
	void	clearBit (size_t i);
	size_t  count () const;
	size_t  count (bool on) const;
	bool	fill (bool value);
	bool	fill (bool value, size_t size);
	void	fill (bool value, size_t begin, size_t end);
	bool	isEmpty () const;
	void	resize (size_t size);
	void	setBit (size_t i);
	void	setBit (size_t i, bool value);
	size_t  size () const;
	bool	testBit (size_t i) const;
	bool	toggleBit (size_t i);
	void	truncate (size_t pos);

	bool	  operator != (const BitArray& other) const;
	BitArray& operator &= (const BitArray& other);
	bool	  operator == (const BitArray& other) const;
	//QBitRef	operator[] ( uint i )
	bool	  operator [] (size_t i ) const;
	BitArray& operator ^= (const BitArray& other );
	BitArray& operator |= (const BitArray& other );
	BitArray  operator ~  () const;

	friend BitArray	operator & (const BitArray& a1, const BitArray& a2);
	friend BitArray	operator ^ (const BitArray& a1, const BitArray& a2);
	friend BitArray	operator | (const BitArray& a1, const BitArray& a2);
};

CWT_NS_END

#endif /* __CWT_BITARRAY_HPP__ */
