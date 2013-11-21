/*
 * map.hpp
 *
 *  Created on: Nov 21, 2013
 *      Author: wladt
 */

#ifndef __PFS_MAP_HPP__
#define __PFS_MAP_HPP__

#include <cwt/cwt.hpp>
#include <map>

namespace pfs {

template <typename Key, typename T>
class map : public std::map<Key, T> {};

} // pfs

#endif /* __PFS_MAP_HPP__ */
