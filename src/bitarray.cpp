/**
 * @file   bitarray.cpp
 * @author wladt
 * @date   Jan 24, 2013 3:00:52 PM
 *
 * @brief
 */

#include "../include/pfs/bitarray.hpp"

namespace pfs {

bitarray::bitarray() : _pimpl(new bitarray::impl)
{
	_pimpl->nbits = 0;
}

bitarray::bitarray(size_t size, bool value) : _pimpl(new bitarray::impl)
{
    if (!size) {
    	_pimpl->nbits = 0;
        _pimpl->a.realloc(0);
        return;
    }

    _pimpl->a.alloc(size/32 + ((size % 32) ? 1 : 0));
    _pimpl->a.set(value ? 0xffffffff : 0);

    _pimpl->nbits = size;
}

/**
 * @fn bool bitarray::at (size_t i) const
 * @brief Returns the value of the bit at index position @c i.
 *
 * @param i Index position, must be a valid index position in the bit array (i.e., 0 <= i < size()).
 * @return The value of the bit at index position @c i.
 */

/**
 * @fn void bitarray::clear()
 *
 * @brief Clears the contents of the bit array and makes it empty.
 */

/**
 * @fn void bitarray::clearBit (size_t i)
 *
 * @brief Sets the bit at index position i to 0.
 *
 * @param i Index position, must be a valid index position in the bit array (i.e., 0 <= i < size()).
 */

/**
 * @fn size_t bitarray::count () const
 *
 * @brief same as @c bitarray::size()
 */

/**
 * @fn size_t  bitarray::count (bool on) const
 *
 * @brief Returns the number of 1-bits/0-bits stored in the bit array.
 *
 * @param on Specifies what result to return.
 * @return If @c on is @c true, this function returns the number of 1-bits stored in the bit array; otherwise the number of 0-bits is returned.
 */
size_t bitarray::count (bool on) const
{
    size_t r = 0;
    size_t len = size();

    // See http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
    const uint32_t *bits = _pimpl->a.data();

    while (len >= 32) {
    	uint32_t v = *bits;
    	v = v - ((v >> 1) & 0x55555555);
    	v = (v & 0x33333333) + ((v >> 2) & 0x33333333);
    	uint32_t c = (((v + (v >> 4)) & 0xF0F0F0F) * 0x1010101) >> 24; // count

        len -= 32;
        ++bits;
        r += c;
    }

	while (len > 0) {
		if (*bits & (1 << ((len - 1) & 31)))
			++r;
		--len;
	}

    return on ? r : size() - r;
}

/**
 * @fn bool bitarray::fill (bool value)
 * @brief Sets every bit in the bit array to value.
 *
 * @param value Value to set every bit.
 * @return @c true if successful; otherwise returns @c false.
 */

/**
 * @fn bool bitarray::fill (bool value, size_t size)
 *
 * @brief Sets every bit in the bit array to value.
 *
 * @details  If size is different from original size of the bit array,
 *           the bit array is resized to size beforehand.
 *
 * @param value Value to set every bit.
 * @param size Result size of the bit array after filling.
 * @return @c true if successful; otherwise returns @c false.
 */

/**
 * @fn void bitarray::fill (bool value, size_t begin, size_t end)
 *
 * @brief Sets bits at index positions @c begin up to and excluding @c end to @c value.
 *
 * @param value
 * @param begin Begin position, must be a valid index position in the bit array (i.e., 0 <= begin <= size()).
 * @param end End position, must be a valid index position in the bit array (i.e., 0 <= end <= size()).
 */
void bitarray::fill (bool value, size_t begin, size_t end)
{
	PFS_ASSERT(end >= begin);

	detach();

    while (begin < end && (begin & 31))
        setBit(begin++, value);

    size_t len = end - begin;

    if (!len)
    	return;

    size_t sz = len & ~31;
    uint32_t *c = _pimpl->a.data();
    memset(c + (begin >> 5), value ? 0xff : 0, sizeof(uint32_t) * sz);
    begin += sz * 32;

    while (begin < end)
    	setBit(begin++, value);
}

/**
 * @fn bool bitarray::isEmpty () const
 *
 * @brief Checks if bit array is empty.
 *
 * @return @c true if this bit array has size 0; otherwise returns false.
 */

/**
 * @fn void bitarray::resize (size_t size)
 *
 * @brief Resizes the bit array to size bits.
 *
 * @details If size is greater than the current size, the bit array is extended
 * 			to make it size bits with the extra bits added to the end.
 * 			The new bits are initialized to false (0).
 * 			If size is less than the current size, bits are removed from the end.
 *
 * @param size New bit array size.
 */
void bitarray::resize (size_t new_size)
{
	detach();

    if (!new_size) {
    	_pimpl->nbits = 0;
    } else {
    	if (new_size > size()) {
    		array<uint32_t> *a = & _pimpl->a;
    		size_t old_nchunks = a->size();
    		size_t new_nchunks = new_size/32 + ((new_size % 32) ? 1 : 0);

    		a->realloc(new_nchunks);
    		if (new_nchunks > old_nchunks) {
    			a->set(0, old_nchunks + 1);
    		}

    		if (_pimpl->nbits % 32) {
    			(*a)[old_nchunks] &= (1 << (_pimpl->nbits % 32)) - 1;
    		}
    	}
    	_pimpl->nbits = new_size;
    }
}

/**
 * @fn void bitarray::setBit (size_t i)
 *
 * @brief Sets the bit at index position @c i to 1.
 *
 * @param i Index position of the bit.
 */

/**
 * @fn void bitarray::setBit (size_t i, bool value)
 *
 * @brief Sets the bit at index position @c i to value.
 *
 * @param i Index position of the bit.
 * @param value Value to set the bit.
 */

/**
 * @fn size_t bitarray::size () const
 *
 * @brief Returns the number of bits stored in the bit array.
 *
 * @return Number of bits stored in the bit array.
 */

/**
 * @fn bool bitarray::testBit (size_t i) const
 *
 * @brief Tests the value of the bit in the specified pisition.
 *
 * @param i Index position of the bit, must be a valid index position in the bit array (i.e., 0 <= i < size()).
 * @return @c true if the bit at index position @c i is 1; otherwise returns @c false.
 */

/**
 * @fn bool bitarray::toggleBit (size_t i)
 *
 * @brief Inverts the value of the bit at index position i.
 *
 * @param i Index positionj of the bit.
 * @return previous value of that bit as either @c true (if it was set) or @c false (if it was unset).
 */

/**
 * @fn void bitarray::truncate (size_t pos)
 *
 * @brief Truncates the bit array at index position @c pos.
 *
 * @details If @c pos is beyond the end of the array, nothing happens.
 *
 * @param pos Position for truncate the bit array.
 */

/**
 * @fn bool bitarray::operator != (const bitarray& other) const
 *
 * @brief Compares bit arrays for inequality.
 *
 * @param other bit array for comparison.
 * @return Returns @c true if other is not equal to this bit array; otherwise returns @c false.
 */
bool bitarray::operator != (const bitarray & other) const
{
	return !(_pimpl->nbits == other._pimpl->nbits
			&& _pimpl->a.compare(other._pimpl->a, _pimpl->a.size()) == 0);
}

/**
 * @fn bool bitarray::operator != (const bitarray& other) const
 *
 * @brief Compares bit arrays for equality.
 *
 * @param other bit array for comparison.
 * @return @c true if other is equal to this bit array; otherwise returns @c false.
 */
bool bitarray::operator == (const bitarray& other) const
{
	return _pimpl->nbits == other._pimpl->nbits
			&& _pimpl->a.compare(other._pimpl->a, _pimpl->a.size()) == 0;
}


//QBitRef	operator[] ( uint i )

/**
 * @fn bool bitarray::operator [] (size_t i ) const
 *
 * @brief Returns the bit value at index position @c i.
 *
 * @param i Index position.
 * @return The bit value at index position @c i.
 */

/**
 * @fn bitarray& bitarray::operator &= (const bitarray& other).
 *
 * @brief Performs the AND operation.
 *
 * @details Performs the AND operation between all bits in this bit array and other.
 *          Assigns the result to this bit array, and returns a reference to it.
 *          The result has the length of the longest of the two bit arrays,
 *          with any missing bits (if one array is shorter than the other) taken to be 0.
 *
 * @param other Second argument for operation.
 * @return Reference to modified bit array.
 */

bitarray& bitarray::operator &= (const bitarray & other)
{
	detach();

    resize(max(size(), other.size()));

    uint32_t * a1 = _pimpl->a.data();
    const uint32_t * a2 = other._pimpl->a.constData();
    size_t n = other._pimpl->a.size();
    size_t p = _pimpl->a.size() - n;

    while (n-- > 0)
        *a1++ &= *a2++;
    while (p-- > 0)
        *a1++ = 0;
    return *this;
}

/**
 * @fn bitarray& bitarray::operator ^= (const bitarray& other)
 *
 * @brief Performs the XOR operation.
 *
 * @details Performs the XOR operation between all bits in this bit array and other.
 *          Assigns the result to this bit array, and returns a reference to it.
 *          The result has the length of the longest of the two bit arrays,
 *          with any missing bits (if one array is shorter than the other) taken to be 0.
 *
 * @param other Second argument for operation.
 * @return Reference to modified bit array.
 */
bitarray& bitarray::operator ^= (const bitarray& other)
{
	detach();
    resize(max(size(), other.size()));

    uint32_t *a1 = _pimpl->a.data();
    const uint32_t *a2 = other._pimpl->a.constData();
    size_t n = other._pimpl->a.size();
    while (n-- > 0)
        *a1++ ^= *a2++;
    return *this;
}

/**
 * @fn bitarray& bitarray::operator |= (const bitarray& other )
 *
 * @brief Performs the OR operation.
 *
 * @details Performs the OR operation between all bits in this bit array and other.
 *          Assigns the result to this bit array, and returns a reference to it.
 *          The result has the length of the longest of the two bit arrays,
 *          with any missing bits (if one array is shorter than the other) taken to be 0.
 *
 * @param other Second argument for operation.
 * @return
 */
bitarray& bitarray::operator |= (const bitarray & other)
{
	detach();
    resize(max(size(), other.size()));

    uint32_t *a1 = _pimpl->a.data();
    const uint32_t *a2 = other._pimpl->a.constData();
    size_t n = other._pimpl->a.size();
    while (n-- > 0)
        *a1++ |= *a2++;
    return *this;
}

/**
 * @fn bitarray bitarray::operator ~ () const
 *
 * @brief Returns a bit array that contains the inverted bits of this bit array.
 *
 * @return A bit array that contains the inverted bits of this bit array.
 */
bitarray bitarray::operator ~ () const
{
    size_t sz = size();
    bitarray a(sz);

    const uint32_t *a1 = _pimpl->a.constData();
    uint32_t *a2 = a._pimpl->a.data();

    size_t n = _pimpl->a.size();

    while (n-- > 0)
        *a2++ = ~*a1++;

//	if (_pimpl->nbits && _pimpl->nbits % 32) {
//		*a2 &= (1 << (sz % 32)) - 1;
//	}

    return a;
}

/**
 * @fn bitarray operator & (const bitarray& a1, const bitarray& a2)
 *
 * @brief Performs the AND operation.
 *
 * @details Returns a bit array that is the AND of the bit arrays a1 and a2.
 *          The result has the length of the longest of the two bit arrays,
 *          with any missing bits (if one array is shorter than the other) taken to be 0.
 *
 * @param a1 First argument of operation.
 * @param a2 Second argument of operation.
 * @return Result of AND operation.
 */
DLL_API bitarray operator & (const bitarray & a1, const bitarray & a2)
{
	bitarray tmp(a1);
	tmp &= a2;
	return tmp;
}

/**
 * @fn bitarray operator ^ (const bitarray& a1, const bitarray& a2)
 *
 * @brief Performs the XOR operation.
 *
 * @details Returns a bit array that is the XOR of the bit arrays a1 and a2.
 *          The result has the length of the longest of the two bit arrays,
 *          with any missing bits (if one array is shorter than the other) taken to be 0.
 *
 * @param a1 First argument of operation.
 * @param a2 Second argument of operation.
 * @return Result of XOR operation.
 */
DLL_API bitarray operator ^ (const bitarray & a1, const bitarray & a2)
{
	bitarray tmp(a1);
	tmp ^= a2;
	return tmp;
}

/**
 * @fn bitarray operator | (const bitarray& a1, const bitarray& a2)
 *
 * @brief Performs the OR operation.
 *
 * @details Returns a bit array that is the OR of the bit arrays a1 and a2.
 *          The result has the length of the longest of the two bit arrays,
 *          with any missing bits (if one array is shorter than the other) taken to be 0.
 *
 * @param a1 First argument of operation.
 * @param a2 Second argument of operation.
 * @return Result of OR operation.
 */
DLL_API bitarray operator | (const bitarray & a1, const bitarray & a2)
{
	bitarray tmp(a1);
	tmp |= a2;
	return tmp;
}


} // pfs
