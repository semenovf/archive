/**
 * @file   bit_array.hpp
 * @author wladt
 * @date   Jun 27, 2013
 *
 * @brief
 */

#ifndef __PFS_BITARRAY_HPP__
#define __PFS_BITARRAY_HPP__

#include <pfs.hpp>
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

struct bit_array_pdata;

class DLL_API bit_array : public nullable<bit_array_pdata>
{
protected:
	typedef nullable<bit_array_pdata> base_class;
	typedef bit_array self_class;
	typedef size_t    size_type;

public:
	bit_array () : base_class() {}
	bit_array (size_type size, bool value = false);

	bool isEmpty () const { return base_class::isNull() || size() == 0; }
	bool empty () const { return isEmpty(); }
	void clear ();

	size_type  count () const { return size(); }
	size_type  size () const;
	size_type  count (bool on) const;

	bool	at        (size_type i) const { return testBit(i); }
	void	clearBit  (size_type i);
	void	setBit    (size_type i);
	void	setBit    (size_type i, bool value);
	bool	testBit   (size_type i) const;
	bool	toggleBit (size_type i);

	void	fill (bool value) { fill(value, 0, size()); }
	void	fill (bool value, size_t size);
	void	fill (bool value, size_type begin, size_type end);
	void	resize (size_t size);
	void	truncate (size_t pos) { if (pos < size()) resize(pos); }

	bool	    operator [] (size_t i ) const { return testBit(i); }
	bool	    operator != (const bit_array & other) const;
	bool	    operator == (const bit_array & other) const;
	bit_array & operator &= (const bit_array & other);
	bit_array & operator ^= (const bit_array & other);
	bit_array & operator |= (const bit_array & other);
	bit_array   operator ~  () const;

	friend DLL_API bit_array operator & (const bit_array & a1, const bit_array & a2);
	friend DLL_API bit_array operator ^ (const bit_array & a1, const bit_array & a2);
	friend DLL_API bit_array operator | (const bit_array & a1, const bit_array & a2);
};

} // pfs

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

#endif /* __PFS_BITARRAY_HPP__ */