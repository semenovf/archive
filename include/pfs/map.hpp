/*
 * map.hpp
 *
 *  Created on: Nov 21, 2013
 *      Author: wladt
 */

#ifndef __PFS_MAP_HPP__
#define __PFS_MAP_HPP__

#include <map>

namespace pfs {

template <typename Key
	, typename T
	, typename Compare = std::less<Key>
	, typename Alloc = std::allocator<std::pair<const Key, T> > >
struct map
{
	typedef ::std::map<Key, T, Compare, Alloc> type;
};

} // pfs

#endif /* __PFS_MAP_HPP__ */
