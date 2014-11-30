/*
 * math.hpp
 *
 *  Created on: Sep 13, 2013
 *      Author: wladt
 */

#ifndef __PFS_MATH_HPP__
#define __PFS_MATH_HPP__

namespace pfs {

class DLL_API math
{
public:
	// Integer Division (Floor function)
	// http://mathworld.wolfram.com/IntegerDivision.html
	// Floor function symbols: |_a/b_| or a\b
	static long_t floorDiv(long_t a, long_t b);
	static long_t floorDiv(long_t a, int b);
	static int    floorDiv(int a, int b);
};

inline long_t math::floorDiv(long_t a, long_t b)
{
    return (a - (a < 0 ? b-1 : 0)) / b;
}

inline long_t math::floorDiv(long_t a, int b)
{
    return (a - (a < 0 ? b-1 : 0)) / b;
}

inline int math::floorDiv(int a, int b)
{
    return (a - (a < 0 ? b-1 : 0)) / b;
}

} // pfs

#endif /* __CWT_MATH_HPP__ */
