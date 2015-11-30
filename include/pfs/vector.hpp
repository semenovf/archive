/*
 * vector.hpp
 *
 *  Created on: Nov 21, 2013
 *      Author: wladt
 */

#ifndef __PFS_VECTOR_HPP__
#define __PFS_VECTOR_HPP__

#if __cplusplus >= 201103L

#include <vector>

namespace pfs {

template <typename T, typename Allocator = std::allocator<T> >
using vector = std::vector<T, Allocator>;

} // pfs

#else // !C++11

namespace pfs {

#include <vector>

using std;

} // pfs

#endif /* __PFS_VECTOR_HPP__ */
