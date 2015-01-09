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
template <typename IntT1, typename IntT2>
inline IntT1 floorDiv(IntT1 a, IntT2 b)
{
    return (a - (a < 0 ? b - IntT2(1) : 0)) / b;
}

}} // pfs::math

#endif /* __CWT_MATH_HPP__ */
