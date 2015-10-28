/*
 * advance.hpp
 *
 *  Created on: Oct 25, 2015
 *      Author: wladt
 */

#ifndef __PFS_BITS_ITERATOR_ADVANCE_HPP__
#define __PFS_BITS_ITERATOR_ADVANCE_HPP__

#include <pfs.hpp>

#ifdef PFS_CC_GCC
#include <iterator>

namespace pfs {
/**
 *  @brief A generalization of pointer arithmetic.
 *  @param  __i  An input iterator.
 *  @param  __n  The @a delta by which to change @p __i.
 *  @return  Nothing.
 *
 *  This increments @p i by @p n.  For bidirectional and random access
 *  iterators, @p __n may be negative, in which case @p __i is decremented.
 *
 *  For random access iterators, this uses their @c + and @c - operations
 *  and are constant time.  For other %iterator classes they are linear time.
*/
template<typename InputIterator, typename Distance>
inline void advance (InputIterator & i, Distance n)
{
    std::advance(i, n);
}
} // pfs

#else
#	error "Need to implement advance() iterator function"
#endif


#endif /* __PFS_BITS_ITERATOR_ADVANCE_HPP__ */
