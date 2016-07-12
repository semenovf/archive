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
inline Integral1 floor_div (Integral1 a, Integral2 b)
{
    return (a - (a < 0 ? b - Integral2(1) : 0)) / b;
}

#if __cplusplus >= 201103

template <intmax_t Pn>
struct integral_sign
    : integral_constant<intmax_t, (Pn < 0) ? -1 : 1>
{ };

#else

namespace details {

inline int integral_sign (intmax_t Pn)
{
    return (Pn < 0) ? -1 : 1;
}

inline intmax_t integral_abs (intmax_t Pn)
{ 
    return Pn * integral_sign(Pn);
};

inline intmax_t integral_gcd (intmax_t IntegralConstant1, intmax_t IntegralConstant2)
{ 
    if (IntegralConstant2 == 0)
        return integral_abs(IntegralConstant1);
    else if (IntegralConstant1 == 0)
        return integral_abs(IntegralConstant2);
    
    return integral_gcd(IntegralConstant2, IntegralConstant1 % IntegralConstant2);
};

}

template <intmax_t IntegralConstant>
struct integral_sign
{ 
    static int const value;
};

template <intmax_t IntegralConstant>
int const integral_sign<IntegralConstant>::value = details::integral_sign(IntegralConstant);

template <intmax_t IntegralConstant>
struct integral_abs
{ 
    static intmax_t const value;
};

template <intmax_t IntegralConstant>
intmax_t const integral_abs<IntegralConstant>::value =  details::integral_abs(IntegralConstant);

template <intmax_t IntegralConstant1, intmax_t IntegralConstant2>
struct integral_gcd
{ 
    static intmax_t const value;
};

template <intmax_t IntegralConstant1, intmax_t IntegralConstant2>
intmax_t const integral_gcd<IntegralConstant1, IntegralConstant2>::value 
        = details::integral_gcd(IntegralConstant1, IntegralConstant2);

#endif

}} // pfs::math

#endif /* __PFS_MATH_HPP__ */
