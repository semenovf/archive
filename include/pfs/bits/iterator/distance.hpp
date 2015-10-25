/*
 * distance.hpp
 *
 *  Created on: Oct 25, 2015
 *      Author: wladt
 */

#ifndef __PFS_BITS_ITERATOR_DISTANCE_HPP__
#define __PFS_BITS_ITERATOR_DISTANCE_HPP__

#include <pfs.hpp>

#ifdef PFS_CC_GCC

#include <iterator>

namespace pfs {

/**
 *  @brief A generalization of pointer arithmetic.
 *  @param  first  An input iterator.
 *  @param  last  An input iterator.
 *  @return  The distance between them.
 *
 *  Returns @c n such that first + n == last.  This requires
 *  that @p __last must be reachable from @p first.  Note that @c
 *  n may be negative.
 *
 *  For random access iterators, this uses their @c + and @c - operations
 *  and are constant time.  For other %iterator classes they are linear time.
*/
template <typename InputIterator>
inline typename InputIterator::difference_type
distance (InputIterator first, InputIterator last)
{
    return std::distance(first, last);
}

} // pfs

#else
#	error "Need to implement distance() iterator function"
#endif

#endif /* __PFS_BITS_ITERATOR_DISTANCE_HPP__ */
