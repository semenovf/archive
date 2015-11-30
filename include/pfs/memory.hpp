/*
 * memory.hpp
 *
 *  Created on: Nov 30, 2015
 *      Author: wladt
 */

#ifndef __PFS_MEMORY_HPP__
#define __PFS_MEMORY_HPP__

#if __cplusplus >= 201103L

#include <memory>

namespace pfs {

template <typename T>
using allocator = std::allocator<T>;

}

#else // !C++11

namespace pfs {

#include <memory>

using std;

} // pfs

#endif /* __PFS_MEMORY_HPP__ */
