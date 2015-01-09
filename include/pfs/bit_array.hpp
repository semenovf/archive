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
#include <pfs/bits/bit_array_impl.hpp>

#ifdef PFS_CC_MSVC
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif

namespace pfs {

// STL  std::bitset
// Qt   bitarray
// C#   bitarray
// Java BitSet

class DLL_API bit_array : public nullable<bit_array_impl>
{
protected:
	typedef bit_array_impl       impl_class;
	typedef nullable<impl_class> base_class;
	typedef bit_array            self_class;

	typedef size_t               size_type;

public:
	bit_array ();
	bit_array (size_t size, bool value = false);

	bool isEmpty () const { return base_class::isNull() || size() == 0; }
	bool empty () const { return isEmpty(); }
	void clear () { base_class::detach(); swap(bit_array()); }

	bool	at        (size_type i) const { return testBit(i); }
	void	clearBit  (size_type i);
	void	setBit    (size_type i);
	void	setBit    (size_type i, bool value);
	bool	testBit   (size_type i) const;
	bool	toggleBit (size_type i);

	size_type  count () const { return size(); }
	size_type  size () const  { return base_class::isNull() ? 0 : base_class::cast()->size(); }
	size_type  count (bool on) const;

	void	fill (bool value) { fill(value, size()); }
//	void	fill (bool value, size_t size) { base_class::detach(); *this = bit_array(size, value); }
	void	fill (bool value, size_t begin, size_t end);
//	void	resize (size_t size);
	void	truncate (size_t pos) { if (pos < size()) resize(pos); }

	bool	    operator [] (size_t i ) const { return testBit(i); }
//	bool	    operator != (const bit_array & other) const;
//	bool	    operator == (const bit_array & other) const;
//	bit_array & operator &= (const bit_array & other);
//	bit_array & operator ^= (const bit_array & other);
//	bit_array & operator |= (const bit_array & other);
//	bit_array   operator ~  () const;
//
//	friend DLL_API bit_array operator & (const bit_array & a1, const bit_array & a2);
//	friend DLL_API bit_array operator ^ (const bit_array & a1, const bit_array & a2);
//	friend DLL_API bit_array operator | (const bit_array & a1, const bit_array & a2);
};

} // pfs

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

#endif /* __PFS_BITARRAY_HPP__ */
