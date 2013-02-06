/**
 * @file   bitarray.cpp
 * @author wladt
 * @date   Jan 24, 2013 3:00:52 PM
 *
 * @brief
 */


#include <cwt/bitarray.hpp>

CWT_NS_BEGIN

BitArray::BitArray() : __impl() {}
BitArray::BitArray(int size, bool value) : __impl(size, value) {}
BitArray::BitArray(const BitArray &other) : __impl(other.__impl) {}

CWT_NS_END
