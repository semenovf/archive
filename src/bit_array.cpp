/**
 * @file   bit_array.cpp
 * @author wladt
 * @date   Jan 24, 2013 3:00:52 PM
 *
 * @brief
 */

#include "../include/pfs/bit_array.hpp"
#include <pfs/array.hpp>

namespace pfs {

struct bit_array_pdata
{
	array<uint32_t> _a;
	size_t          _nbits;
};

bit_array::bit_array (size_t size, bool value)
	: base_class(new bit_array_pdata)
{
	bit_array_pdata * pd = base_class::cast();

    if (!size) {
    	pd->_nbits = 0;
    	pd->_a.realloc(0);
        return;
    }

    pd->_a.alloc(size/32 + ((size % 32) ? 1 : 0));
    pd->_a.set(value ? 0xffffffff : 0);

    pd->_nbits = size;
}

/**
 * @brief Returns the number of bits stored in the bit array.
 *
 * @return Number of bits stored in the bit array.
 */
bit_array::size_type bit_array::size () const
{
	const bit_array_pdata * pd = base_class::cast();
	return base_class::isNull()
		? 0
		: pd->_nbits;
}

/**
 * @brief Returns the number of 1-bits/0-bits stored in the bit array.
 *
 * @param on Specifies what result to return.
 * @return If @c on is @c true, this function returns the number of 1-bits stored in the bit array; otherwise the number of 0-bits is returned.
 */
bit_array::size_type bit_array::count (bool on) const
{
	const bit_array_pdata * pd = base_class::cast();
    size_type r = 0;
    size_type len = size();

    // See http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
    const uint32_t * bits = pd->_a.data();

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
 * @brief Sets the bit at index position i to 0.
 *
 * @param i Index position, must be a valid index position in the bit array (i.e., 0 <= i < size()).
 */
void bit_array::clearBit (size_type i)
{
	PFS_ASSERT(i < size());
	base_class::detach();
	bit_array_pdata * pd = base_class::cast();
	*(pd->_a.data() + (i >> 5)) &= ~ uint32_t(1 << (i & 31));
}

/**
 * @brief Sets the bit at index position @c i to 1.
 *
 * @param i Index position of the bit.
 */
void bit_array::setBit (size_type i)
{
	PFS_ASSERT(i < size());
	base_class::detach();
	bit_array_pdata * pd = base_class::cast();
	*(pd->_a.data() + (i >> 5)) |= uint32_t(1 << (i & 31));
}

/**
 * @brief Sets the bit at index position @c i to value.
 *
 * @param i Index position of the bit.
 * @param value Value to set the bit.
 */
void bit_array::setBit (size_type i, bool value)
{
	PFS_ASSERT(i < size());
	//base_class::detach(); // will detached in bellow methods
	if (value) setBit(i);
	else clearBit(i);
}

/**
 * @brief Tests the value of the bit in the specified pisition.
 *
 * @param i Index position of the bit, must be a valid index position in the bit array (i.e., 0 <= i < size()).
 * @return @c true if the bit at index position @c i is 1; otherwise returns @c false.
 */
bool bit_array::testBit (size_type i) const
{
	PFS_ASSERT(i < size());
	const bit_array_pdata * pd = base_class::cast();
	return  (*(pd->_a.constData() + (i >> 5)) & (1 << (i & 31))) != 0;
}

/**
 * @brief Inverts the value of the bit at index position i.
 *
 * @param i Index position of the bit.
 * @return Previous value of that bit as either @c true (if it was set) or @c false (if it was unset).
 */
bool bit_array::toggleBit (size_type i)
{
	PFS_ASSERT(i < size());
	base_class::detach();
	bit_array_pdata * pd = base_class::cast();

	uint32_t b = uint32_t(1 << (i & 31));
	uint32_t *p = pd->_a.data() + (i >> 5);
	uint32_t c = uint32_t(*p & b);
	*p ^= b;
	return c != 0;
}

/**
 * @brief Sets every bit in the bit array to value.
 *
 * @details  If size is different from original size of the bit array,
 *           the bit array is resized to size beforehand.
 *
 * @param value Value to set every bit.
 * @param size Result size of the bit array after filling.
 * @return @c true if successful; otherwise returns @c false.
 */
void bit_array::fill (bool value, size_type n)
{
	base_class::detach();
	if (n == size()) {
		fill(value);
	} else {
		this->swap(bit_array(n, value));
	}
}

/**
 * @brief Sets bits at index positions @c begin up to and excluding @c end to @c value.
 *
 * @param value
 * @param begin Begin position, must be a valid index position in the bit array (i.e., 0 <= begin <= size()).
 * @param end End position, must be a valid index position in the bit array (i.e., 0 <= end <= size()).
 */
void bit_array::fill (bool value, size_t begin, size_t end)
{
	PFS_ASSERT(end >= begin);
	base_class::detach();

    while (begin < end && (begin & 31))
        setBit(begin++, value);

    size_t len = end - begin;

    if (!len)
    	return;

    bit_array_pdata * pd = base_class::cast();
    size_t sz = (len & ~31) >> 5;
    uint32_t * c = pd->_a.data();

    memset(c + (begin >> 5), value ? 0xff : 0, sizeof(uint32_t) * sz);
    begin += sz * 32;

    while (begin < end)
    	setBit(begin++, value);
}

/**
 * @brief Resizes the bit array to size bits.
 *
 * @details If size is greater than the current size, the bit array is extended
 * 			to make it size bits with the extra bits added to the end.
 * 			The new bits are initialized to false (0).
 * 			If size is less than the current size, bits are removed from the end.
 *
 * @param size New bit array size.
 */
void bit_array::resize (size_type new_size)
{
	base_class::detach();

	bit_array_pdata * pd = base_class::cast();

    if (!new_size) {
    	pd->_nbits = 0;
    } else {
    	if (new_size > size()) {
    		size_t old_nchunks = pd->_a.size();
    		size_t new_nchunks = new_size/32 + ((new_size % 32) ? 1 : 0);

    		pd->_a.realloc(new_nchunks);
    		if (new_nchunks > old_nchunks) {
    			pd->_a.set(0, old_nchunks + 1);
    		}

    		if (pd->_nbits % 32) {
    			pd->_a[old_nchunks] &= (1 << (pd->_nbits % 32)) - 1;
    		}
    	}
    	pd->_nbits = new_size;
    }
}

/**
 * @fn bool bit_array::at (size_t i) const
 * @brief Returns the value of the bit at index position @c i.
 *
 * @param i Index position, must be a valid index position in the bit array (i.e., 0 <= i < size()).
 * @return The value of the bit at index position @c i.
 */

/**
 * @fn void bit_array::clear()
 *
 * @brief Clears the contents of the bit array and makes it empty.
 */

/**
 * @fn size_type bit_array::count () const
 *
 * @brief same as @c bit_array::size()
 */

/**
 * @fn bool bit_array::fill (bool value)
 * @brief Sets every bit in the bit array to value.
 *
 * @param value Value to set every bit.
 * @return @c true if successful; otherwise returns @c false.
 */


/**
 * @fn bool bit_array::isEmpty () const
 *
 * @brief Checks if bit array is empty.
 *
 * @return @c true if this bit array has size 0; otherwise returns false.
 */

/**
 * @fn void bit_array::truncate (size_t pos)
 *
 * @brief Truncates the bit array at index position @c pos.
 *
 * @details If @c pos is beyond the end of the array, nothing happens.
 *
 * @param pos Position for truncate the bit array.
 */

/**
 * @brief Compares bit arrays for inequality.
 *
 * @param other bit array for comparison.
 * @return Returns @c true if other is not equal to this bit array; otherwise returns @c false.
 */
bool bit_array::operator != (const bit_array & other) const
{
	const bit_array_pdata * pd = base_class::cast();
	const bit_array_pdata * pdo = other.base_class::cast();
	return !(pd->_nbits == pdo->_nbits
			&& pd->_a.compare(pdo->_a, pd->_a.size()) == 0);
}

/**
 * @brief Compares bit arrays for equality.
 *
 * @param other bit array for comparison.
 * @return @c true if other is equal to this bit array; otherwise returns @c false.
 */
bool bit_array::operator == (const bit_array & other) const
{
	const bit_array_pdata * pd = base_class::cast();
	const bit_array_pdata * pdo = other.base_class::cast();

	return pd->_nbits == pdo->_nbits
			&& pd->_a.compare(pdo->_a, pd->_a.size()) == 0;
}

/**
 * @fn bool bit_array::operator [] (size_t i ) const
 *
 * @brief Returns the bit value at index position @c i.
 *
 * @param i Index position.
 * @return The bit value at index position @c i.
 */

/**
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
bit_array & bit_array::operator &= (const bit_array & other)
{
	base_class::detach();
	bit_array_pdata * pd = base_class::cast();
	const bit_array_pdata * pdo = other.base_class::cast();

    resize(pfs::max(size(), other.size()));

    uint32_t * a1 = pd->_a.data();
    const uint32_t * a2 = pdo->_a.constData();
    size_t n = pdo->_a.size();
    size_t p = pd->_a.size() - n;

    while (n-- > 0)
        *a1++ &= *a2++;
    while (p-- > 0)
        *a1++ = 0;
    return *this;
}

/**
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
bit_array& bit_array::operator ^= (const bit_array & other)
{
	base_class::detach();
	bit_array_pdata * pd = base_class::cast();
	const bit_array_pdata * pdo = other.base_class::cast();

    resize(pfs::max(size(), other.size()));

    uint32_t * a1 = pd->_a.data();
    const uint32_t * a2 = pdo->_a.constData();
    size_t n = pdo->_a.size();
    while (n-- > 0)
        *a1++ ^= *a2++;
    return *this;
}

/**
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
bit_array & bit_array::operator |= (const bit_array & other)
{
	base_class::detach();
	bit_array_pdata * pd = base_class::cast();
	const bit_array_pdata * pdo = other.base_class::cast();

    resize(pfs::max(size(), other.size()));

    uint32_t * a1 = pd->_a.data();
    const uint32_t * a2 = pdo->_a.constData();
    size_t n = pdo->_a.size();
    while (n-- > 0)
        *a1++ |= *a2++;
    return *this;
}

/**
 * @brief Returns a bit array that contains the inverted bits of this bit array.
 *
 * @return A bit array that contains the inverted bits of this bit array.
 */
bit_array bit_array::operator ~ () const
{
    size_t sz = size();
    bit_array a(sz);

    const bit_array_pdata * pd = base_class::cast();
	bit_array_pdata * pdo = a.base_class::cast();

    const uint32_t * a1 = pd->_a.constData();
    uint32_t * a2 = pdo->_a.data();

    size_t n = pd->_a.size();

    while (n-- > 0)
        *a2++ = ~*a1++;

    return a;
}

/**
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
DLL_API bit_array operator & (const bit_array & a1, const bit_array & a2)
{
	bit_array tmp(a1);
	tmp &= a2;
	return tmp;
}

/**
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
DLL_API bit_array operator ^ (const bit_array & a1, const bit_array & a2)
{
	bit_array tmp(a1);
	tmp ^= a2;
	return tmp;
}

/**
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
DLL_API bit_array operator | (const bit_array & a1, const bit_array & a2)
{
	bit_array tmp(a1);
	tmp |= a2;
	return tmp;
}

} // pfs
