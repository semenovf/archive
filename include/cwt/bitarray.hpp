/**
 * @file   bitset.hpp
 * @author wladt
 * @date   Jan 24, 2013 2:37:45 PM
 *
 * @brief
 */

#ifndef __CWT_BITSET_HPP__
#define __CWT_BITSET_HPP__

#include <cwt/cwt.h>
#include <QBitArray>

CWT_NS_BEGIN

// STL  std::bitset
// Qt   BitArray
// C#   BitArray
// Java BitSet

class DLL_API BitArray {
public:
	BitArray();
	BitArray(int size, bool value = false);
	BitArray(const BitArray &other);

private:
	QBitArray __impl;
};

CWT_NS_END

#endif /* __CWT_BITSET_HPP__ */
