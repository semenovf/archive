/*
 * map.hpp
 *
 *  Created on: Nov 21, 2013
 *      Author: wladt
 */

#ifndef __PFS_MAP_HPP__
#define __PFS_MAP_HPP__

#if __cplusplus >= 201103L

#include <map>

namespace pfs {

template <typename Key, typename T, typename Compare = std::less<Key>, typename Alloc = std::allocator<std::pair<const Key, T> > >
using map  = std::map<Key, T, Compare, Alloc>;

} // pfs

#else // !C++11

namespace pfs {

#include <map>

using std;

} // pfs

#endif

#endif /* __PFS_MAP_HPP__ */
