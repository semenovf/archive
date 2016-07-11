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

inline int __integral_sign (intmax_t Pn)
{
    return (Pn < 0) ? -1 : 1;
}

template <intmax_t IntegralConstant>
struct integral_sign
{ 
    static int const value;
};

template <intmax_t IntegralConstant>
int const integral_sign<IntegralConstant>::value = __integral_sign(IntegralConstant);

inline intmax_t __integral_abs (intmax_t Pn)
{ 
    return Pn * __integral_sign(Pn);
};

template <intmax_t IntegralConstant>
struct integral_abs
{ 
    static intmax_t const value;
};

template <intmax_t IntegralConstant>
intmax_t const integral_abs<IntegralConstant>::value =  __integral_abs(IntegralConstant);

inline intmax_t __integral_gcd (intmax_t IntegralConstant1, intmax_t IntegralConstant2)
{ 
    if (IntegralConstant2 == 0)
        return __integral_abs(IntegralConstant1);
    else if (IntegralConstant1 == 0)
        return __integral_abs(IntegralConstant2);
    
    return __integral_gcd(IntegralConstant2, IntegralConstant1 % IntegralConstant2);
};

template <intmax_t IntegralConstant1, intmax_t IntegralConstant2>
struct integral_gcd
{ 
    static intmax_t const value;
};

template <intmax_t IntegralConstant1, intmax_t IntegralConstant2>
intmax_t const integral_gcd<IntegralConstant1, IntegralConstant2>::value 
        = __integral_gcd(IntegralConstant1, IntegralConstant2);


// Some double-precision utilities, where numbers are represented as
// Hi * 2 ^ (8 * sizeof(uintmax_t)) + Lo.

namespace details {

inline bool big_less (uintmax_t hi1, uintmax_t lo1, uintmax_t hi2, uintmax_t lo2)
{
    return (hi1 < hi2 || (hi1 == hi2 && lo1 < lo2));
}

}

template <uintmax_t Hi1, uintmax_t Lo1, uintmax_t Hi2, uintmax_t Lo2>
struct big_less
{ 
    static bool const value;
};

template <uintmax_t Hi1, uintmax_t Lo1, uintmax_t Hi2, uintmax_t Lo2>
bool const big_less<Hi1, Lo1, Hi2, Lo2>::value = details::big_less(Hi1, Lo1, Hi2, Lo2);

template<uintmax_t Hi1, uintmax_t Lo1, uintmax_t Hi2, uintmax_t Lo2>
struct big_add
{
  static uintmax_t lo;
  static uintmax_t hi;
};

template <uintmax_t Hi1, uintmax_t Lo1, uintmax_t Hi2, uintmax_t Lo2>
uintmax_t big_add<Hi1, Lo1, Hi2, Lo2>::lo = Lo1 + Lo2;

template <uintmax_t Hi1, uintmax_t Lo1, uintmax_t Hi2, uintmax_t Lo2>
uintmax_t big_add<Hi1, Lo1, Hi2, Lo2>::hi = (Hi1 + Hi2 + (Lo1 + Lo2 < Lo1));

inline uintmax_t __assert_big_sub_parameters (uintmax_t hi1
        , uintmax_t lo1
        , uintmax_t hi2
        , uintmax_t lo2)
{
    PFS_ASSERT(! details::big_less(hi1, lo1, hi2, lo2));
    return 0;
}

// Subtract a number from a bigger one.
template <uintmax_t Hi1, uintmax_t Lo1, uintmax_t Hi2, uintmax_t Lo2>
struct big_sub
{
  static uintmax_t lo;
  static uintmax_t hi;
};

template <uintmax_t Hi1, uintmax_t Lo1, uintmax_t Hi2, uintmax_t Lo2>
uintmax_t big_sub<Hi1, Lo1, Hi2, Lo2>::lo = Lo1 - Lo2;

template <uintmax_t Hi1, uintmax_t Lo1, uintmax_t Hi2, uintmax_t Lo2>
uintmax_t big_sub<Hi1, Lo1, Hi2, Lo2>::hi = (Hi1 - Hi2 - (Lo1 < Lo2));

// Let c = 2 ^ (half # of bits in an intmax_t)
// then we find a1, a0, b1, b0 s.t. N = a1*c + a0, M = b1*c + b0
// The multiplication of N and M becomes,
// N * M = (a1 * b1)c^2 + (a0 * b1 + b0 * a1)c + a0 * b0
// Multiplication is safe if each term and the sum of the terms
// is representable by intmax_t.
template <intmax_t Pn, intmax_t Qn>
struct safe_multiply
{
    static intmax_t const value;
};

inline intmax_t __assert_safe_multiply_parameters (intmax_t Pn, intmax_t Qn)
{
    uintmax_t c = uintmax_t(1) << (sizeof(intmax_t) * 4);

    uintmax_t a0 = __integral_abs(Pn) % c;
    uintmax_t a1 = __integral_abs(Pn) / c;
    uintmax_t b0 = __integral_abs(Qn) % c;
    uintmax_t b1 = __integral_abs(Qn) / c;

    PFS_ASSERT_X(a1 == 0 || b1 == 0, "Overflow in multiplication");
    PFS_ASSERT_X(a0 * b1 + b0 * a1 < (c >> 1), "Overflow in multiplication");
    PFS_ASSERT_X(b0 * a0 <= max_value<intmax_t>(), "Overflow in multiplication");
    PFS_ASSERT_X((a0 * b1 + b0 * a1) * c<= max_value<intmax_t>() - b0 * a0, "Overflow in multiplication");
    
    return 0;
}

template <intmax_t Pn, intmax_t Qn>
intmax_t const safe_multiply<Pn, Qn>::value 
        = __assert_safe_multiply_parameters(Pn, Qn) + Pn * Qn;

// Same principle as __safe_multiply.
template <uintmax_t __x, uintmax_t __y>
struct big_mul
{
private:
  static uintmax_t __c = uintmax_t(1) << (sizeof(intmax_t) * 4);
  static uintmax_t __x0 = __x % __c;
  static uintmax_t __x1 = __x / __c;
  static uintmax_t __y0 = __y % __c;
  static uintmax_t __y1 = __y / __c;
  static uintmax_t __x0y0 = __x0 * __y0;
  static uintmax_t __x0y1 = __x0 * __y1;
  static uintmax_t __x1y0 = __x1 * __y0;
  static uintmax_t __x1y1 = __x1 * __y1;
  static uintmax_t __mix = __x0y1 + __x1y0;
  static uintmax_t __mix_lo = __mix * __c;
  static uintmax_t __mix_hi  = __mix / __c + ((__mix < __x0y1) ? __c : 0);
  typedef big_add<__mix_hi, __mix_lo, __x1y1, __x0y0> _Res;
  
public:
  static uintmax_t hi = _Res::hi;
  static uintmax_t lo = _Res::lo;
};

#endif

}} // pfs::math

#endif /* __PFS_MATH_HPP__ */
