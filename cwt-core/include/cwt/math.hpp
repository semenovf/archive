/*
 * math.hpp
 *
 *  Created on: Sep 13, 2013
 *      Author: wladt
 */

#ifndef __CWT_MATH_HPP__
#define __CWT_MATH_HPP__

#include <cwt/cwt.hpp>

CWT_NS_BEGIN

class DLL_API Math
{
public:
	// Integer Division (Floor function)
	// http://mathworld.wolfram.com/IntegerDivision.html
	// Floor function symbols: |_a/b_| or a\b
	static long_t floorDiv(long_t a, long_t b);
	static long_t floorDiv(long_t a, int b);
	static int    floorDiv(int a, int b);
};

inline long_t Math::floorDiv(long_t a, long_t b)
{
    return (a - (a < 0 ? b-1 : 0)) / b;
}

inline long_t Math::floorDiv(long_t a, int b)
{
    return (a - (a < 0 ? b-1 : 0)) / b;
}

inline int Math::floorDiv(int a, int b)
{
    return (a - (a < 0 ? b-1 : 0)) / b;
}


CWT_NS_END

#endif /* __CWT_MATH_HPP__ */
