/*
 * vector.hpp
 *
 *  Created on: Dec 3, 2015
 *      Author: wladt
 */

#ifndef __PFS_VECTOR_HPP__
#define __PFS_VECTOR_HPP__

#include <vector>

namespace pfs {

template <typename T, typename Alloc = std::allocator<T> >
struct vector
{
	typedef ::std::vector<T, Alloc> type;
};

} // pfs

#endif /* __PFS_VECTOR_HPP__ */
