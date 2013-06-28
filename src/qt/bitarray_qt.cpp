/**
 * @file   bitarray.cpp
 * @author wladt
 * @date   Jan 24, 2013 3:00:52 PM
 *
 * @brief
 */

#include "../../include/cwt/bitarray.hpp"
#include <QBitArray>

CWT_NS_BEGIN

class BitArray::Impl : public QBitArray
{
public:
	Impl () : QBitArray () {}
	Impl (size_t size, bool value = false) : QBitArray (int(size), value) { CWT_ASSERT(size <= CWT_INT_MAX); }
	Impl (const Impl& impl) : QBitArray(impl) {}
	Impl (const QBitArray& ba) : QBitArray(ba) {}
};

CWT_PIMPL_IMPL_COPYABLE(BitArray);

BitArray::BitArray()                        : pimpl(new BitArray::Impl()) {}
BitArray::BitArray(size_t size, bool value) : pimpl(new BitArray::Impl(size, value)) { }


/**
 * @fn bool BitArray::at (size_t i) const
 * @brief Returns the value of the bit at index position @c i.
 *
 * @param i Index position, must be a valid index position in the bit array (i.e., 0 <= i < size()).
 * @return The value of the bit at index position @c i.
 */
bool BitArray::at (size_t i) const
{
	CWT_ASSERT(i <= CWT_INT_MAX);
	return pimpl->at(int(i));
}

/**
 * @fn void BitArray::clear()
 *
 * @brief Clears the contents of the bit array and makes it empty.
 */
void BitArray::clear ()
{
	detach();
	pimpl->clear();
}

/**
 * @fn void BitArray::clearBit (size_t i)
 *
 * @brief Sets the bit at index position i to 0.
 *
 * @param i Index position, must be a valid index position in the bit array (i.e., 0 <= i < size()).
 */
void BitArray::clearBit (size_t i)
{
	CWT_ASSERT(i <= CWT_INT_MAX);
	detach();
	pimpl->clearBit(int(i));
}

/**
 * @fn size_t BitArray::count () const
 *
 * @brief same as @c BitArray::size()
 */
size_t BitArray::count () const
{
	return size();
}

/**
 * @fn size_t  BitArray::count (bool on) const
 *
 * @brief Returns the number of 1-bits/0-bits stored in the bit array.
 *
 * @param on Specifies what result to return.
 * @return If @c on is @c true, this function returns the number of 1-bits stored in the bit array; otherwise the number of 0-bits is returned.
 */
size_t BitArray::count (bool on) const
{
	int r = pimpl->count(on);
	CWT_ASSERT(r >= 0);
	return size_t(r);
}

/**
 * @fn bool BitArray::fill (bool value)
 * @brief Sets every bit in the bit array to value.
 *
 * @param value Value to set every bit.
 * @return @c true if successful; otherwise returns @c false.
 */
bool BitArray::fill (bool value)
{
	detach();
	return pimpl->fill(value);
}

/**
 * @fn bool BitArray::fill (bool value, size_t size)
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
bool BitArray::fill (bool value, size_t size)
{
	CWT_ASSERT(size <= CWT_INT_MAX);
	detach();
	return pimpl->fill(value, int(size));
}

/**
 * @fn void BitArray::fill (bool value, size_t begin, size_t end)
 *
 * @brief Sets bits at index positions @c begin up to and excluding @c end to @c value.
 *
 * @param value
 * @param begin Begin position, must be a valid index position in the bit array (i.e., 0 <= begin <= size()).
 * @param end End position, must be a valid index position in the bit array (i.e., 0 <= end <= size()).
 */
void BitArray::fill (bool value, size_t begin, size_t end)
{
	CWT_ASSERT(begin <= CWT_INT_MAX);
	CWT_ASSERT(end <= CWT_INT_MAX);
	detach();
	return pimpl->fill(value, int(begin), int(end));
}

/**
 * @fn bool BitArray::isEmpty () const
 *
 * @brief Checks if bit array is empty.
 *
 * @return @c true if this bit array has size 0; otherwise returns false.
 */
bool BitArray::isEmpty () const
{
	return pimpl->isEmpty();
}

/**
 * @fn void BitArray::resize (size_t size)
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
void BitArray::resize (size_t size)
{
	CWT_ASSERT(size <= CWT_INT_MAX);
	detach();
	pimpl->resize(int(size));
}

/**
 * @fn void BitArray::setBit (size_t i)
 *
 * @brief Sets the bit at index position @c i to 1.
 *
 * @param i Index position of the bit.
 */
void BitArray::setBit (size_t i)
{
	CWT_ASSERT(i <= CWT_INT_MAX);
	detach();
	pimpl->setBit(int(i));
}

/**
 * @fn void BitArray::setBit (size_t i, bool value)
 *
 * @brief Sets the bit at index position @c i to value.
 *
 * @param i Index position of the bit.
 * @param value Value to set the bit.
 */
void BitArray::setBit (size_t i, bool value)
{
	CWT_ASSERT(i <= CWT_INT_MAX);
	detach();
	pimpl->setBit(i, value);
}


/**
 * @fn size_t BitArray::size () const
 *
 * @brief Returns the number of bits stored in the bit array.
 *
 * @return Number of bits stored in the bit array.
 */
size_t BitArray::size () const
{
	int r = pimpl->size();
	CWT_ASSERT(r >= 0);
	return size_t(r);
}

/**
 * @fn bool BitArray::testBit (size_t i) const
 *
 * @brief Tests the value of the bit in the specified pisition.
 *
 * @param i Index position of the bit, must be a valid index position in the bit array (i.e., 0 <= i < size()).
 * @return @c true if the bit at index position @c i is 1; otherwise returns @c false.
 */
bool BitArray::testBit (size_t i) const
{
	CWT_ASSERT(i <= CWT_INT_MAX);
	return pimpl->testBit(int(i));
}

/**
 * @fn bool BitArray::toggleBit (size_t i)
 *
 * @brief Inverts the value of the bit at index position i.
 *
 * @param i Index positionj of the bit.
 * @return previous value of that bit as either @c true (if it was set) or @c false (if it was unset).
 */
bool BitArray::toggleBit (size_t i)
{
	CWT_ASSERT(i <= CWT_INT_MAX);
	detach();
	return pimpl->toggleBit(int(i));
}

/**
 * @fn void BitArray::truncate (size_t pos)
 *
 * @brief Truncates the bit array at index position @c pos.
 *
 * @details If @c pos is beyond the end of the array, nothing happens.
 *
 * @param pos Position for truncate the bit array.
 */
void BitArray::truncate (size_t pos)
{
	CWT_ASSERT(pos <= CWT_INT_MAX);
	detach();
	pimpl->truncate(int(pos));
}


/**
 * @fn bool BitArray::operator != (const BitArray& other) const
 *
 * @brief Compares bit arrays for inequality.
 *
 * @param other bit array for comparison.
 * @return Returns @c true if other is not equal to this bit array; otherwise returns @c false.
 */
bool BitArray::operator != (const BitArray& other) const
{
	return pimpl->operator != (*other.pimpl);
}

/**
 * @fn bool BitArray::operator != (const BitArray& other) const
 *
 * @brief Compares bit arrays for equality.
 *
 * @param other bit array for comparison.
 * @return @c true if other is equal to this bit array; otherwise returns @c false.
 */
bool BitArray::operator == (const BitArray& other) const
{
	return pimpl->operator == (*other.pimpl);
}


//QBitRef	operator[] ( uint i )

/**
 * @fn bool BitArray::operator [] (size_t i ) const
 *
 * @brief Returns the bit value at index position @c i.
 *
 * @param i Index position.
 * @return The bit value at index position @c i.
 */
bool BitArray::operator [] (size_t i ) const
{
	CWT_ASSERT(i <= CWT_INT_MAX);
	return pimpl->operator [] (int(i));
}

/**
 * @fn BitArray& BitArray::operator &= (const BitArray& other).
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

BitArray& BitArray::operator &= (const BitArray& other)
{
	detach();
	pimpl->operator &= (*other.pimpl);
	return *this;
}
/**
 * @fn BitArray& BitArray::operator ^= (const BitArray& other)
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
BitArray& BitArray::operator ^= (const BitArray& other)
{
	detach();
	pimpl->operator ^= (*other.pimpl);
	return *this;
}

/**
 * @fn BitArray& BitArray::operator |= (const BitArray& other )
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
BitArray& BitArray::operator |= (const BitArray& other )
{
	detach();
	pimpl->operator |= (*other.pimpl);
	return *this;
}

/**
 * @fn BitArray BitArray::operator ~ () const
 *
 * @brief Returns a bit array that contains the inverted bits of this bit array.
 *
 * @return A bit array that contains the inverted bits of this bit array.
 */
BitArray BitArray::operator ~ () const
{
	BitArray ba;
	*ba.pimpl = BitArray::Impl(pimpl->operator ~ ());
	return ba;
}

/**
 * @fn BitArray operator & (const BitArray& a1, const BitArray& a2)
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
BitArray operator & (const BitArray& a1, const BitArray& a2)
{
	BitArray ba;
	*ba.pimpl = BitArray::Impl(operator & (*a1.pimpl, *a2.pimpl));
	return ba;
}

/**
 * @fn BitArray operator ^ (const BitArray& a1, const BitArray& a2)
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
BitArray operator ^ (const BitArray& a1, const BitArray& a2)
{
	BitArray ba;
	*ba.pimpl = BitArray::Impl(operator ^ (*a1.pimpl, *a2.pimpl));
	return ba;
}

/**
 * @fn BitArray operator | (const BitArray& a1, const BitArray& a2)
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
BitArray operator | (const BitArray& a1, const BitArray& a2)
{
	BitArray ba;
	*ba.pimpl = BitArray::Impl(operator | (*a1.pimpl, *a2.pimpl));
	return ba;
}


CWT_NS_END
