/*
 * math.hpp
 *
 *  Created on: Sep 13, 2013
 *      Author: wladt
 */

#ifndef __PFS_MATH_HPP__
#define __PFS_MATH_HPP__

#include <pfs.hpp>

namespace pfs { namespace math {

// Integer Division (Floor function)
// http://mathworld.wolfram.com/IntegerDivision.html
// Floor function symbols: |_a/b_| or a\b
//
template <typename Integral1, typename Integral2>
inline Integral1 floorDiv (Integral1 a, Integral2 b)
{
    return (a - (a < 0 ? b - Integral2(1) : 0)) / b;
}

}} // pfs::math

#endif /* __PFS_MATH_HPP__ */
