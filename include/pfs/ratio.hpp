/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ratio.hpp
 * Author: wladt
 *
 * Created on July 11, 2016, 6:55 PM
 */

#ifndef __PFS_RATIO_HPP__
#define __PFS_RATIO_HPP__


#if __cplusplus >= 201103L

#include <chrono>

namespace pfs {

using std::ratio;

}

#else // __cplusplus < 201103L

#include <utility> // for std::pair
#include <pfs.hpp>
#include <pfs/type_traits.hpp>
#include <pfs/limits.hpp>
#include <pfs/math.hpp>

namespace pfs {

namespace details {

inline intmax_t ratio_num (intmax_t num, intmax_t denom)
{
    PFS_ASSERT_X(denom != 0, "Denominator cannot be zero");
    PFS_ASSERT_X(num >= - max_value<intmax_t>() && denom >= - max_value<intmax_t>(), "Out of range");
    
    return num * math::details::integral_sign(denom) / math::details::integral_gcd(num, denom);
}

inline intmax_t ratio_den (intmax_t num, intmax_t denom)
{
    return math::details::integral_abs(denom) / math::details::integral_gcd(num, denom);
}

}

  /**
   *  @brief Provides rational arithmetic.
   *
   *  This class template represents any finite rational number with a
   *  numerator and denominator representable by compile-time constants of
   *  type intmax_t. The ratio is simplified when instantiated.
   *
   *  For example:
   *  @code
   *    pfs::ratio<7,-21>::num == -1;
   *    pfs::ratio<7,-21>::den == 3;
   *  @endcode
   *  
  */
template <intmax_t Num, intmax_t Denom = 1>
struct ratio
{
    static intmax_t num;
    static intmax_t den;
};

template <intmax_t Num, intmax_t Denom>
intmax_t pfs::ratio<Num, Denom>::num = details::ratio_num(Num, Denom);

template <intmax_t Num, intmax_t Denom>
intmax_t pfs::ratio<Num, Denom>::den = details::ratio_den(Num, Denom);

namespace details {

bool ratio_less (intmax_t num1, intmax_t den1, intmax_t num2, intmax_t den2);

void ratio_add (intmax_t num1, intmax_t den1, intmax_t num2, intmax_t den2, intmax_t & num, intmax_t & den);

void ratio_multiply (intmax_t num1, intmax_t den1, intmax_t num2, intmax_t den2, intmax_t & num, intmax_t & den);

inline void ratio_subtract (intmax_t num1
        , intmax_t den1
        , intmax_t num2
        , intmax_t den2
        , intmax_t & num
        , intmax_t & den)
{
    ratio_add(num1, den1, - num2, den2, num, den);
}

inline void ratio_divide (intmax_t num1
        , intmax_t den1
        , intmax_t num2
        , intmax_t den2
        , intmax_t & num
        , intmax_t & den)
{
    PFS_ASSERT_X(num2 != 0, "Division by 0");
    ratio_multiply(num1, den1, den2, num2, num, den);
}


inline intmax_t ratio_add_and_get_num(intmax_t num1, intmax_t den1, intmax_t num2, intmax_t den2)
{
    intmax_t num, den;
    ratio_add(num1, den1, num2, den2, num, den);
    return num;
}

inline intmax_t ratio_add_and_get_den(intmax_t num1, intmax_t den1, intmax_t num2, intmax_t den2)
{
    intmax_t num, den;
    ratio_add(num1, den1, num2, den2, num, den);
    return den;
}

inline intmax_t ratio_subtract_and_get_num(intmax_t num1, intmax_t den1, intmax_t num2, intmax_t den2)
{
    intmax_t num, den;
    ratio_subtract(num1, den1, num2, den2, num, den);
    return num;
}

inline intmax_t ratio_subtract_and_get_den(intmax_t num1, intmax_t den1, intmax_t num2, intmax_t den2)
{
    intmax_t num, den;
    ratio_subtract(num1, den1, num2, den2, num, den);
    return den;
}

inline intmax_t ratio_multiply_and_get_num (intmax_t num1, intmax_t den1, intmax_t num2, intmax_t den2)
{
    intmax_t num, den;
    ratio_multiply(num1, den1, num2, den2, num, den);
    return num;
}

inline intmax_t ratio_multiply_and_get_den (intmax_t num1, intmax_t den1, intmax_t num2, intmax_t den2)
{
    intmax_t num, den;
    ratio_multiply(num1, den1, num2, den2, num, den);
    return den;
}

inline intmax_t ratio_divide_and_get_num (intmax_t num1, intmax_t den1, intmax_t num2, intmax_t den2)
{
    intmax_t num, den;
    ratio_divide(num1, den1, num2, den2, num, den);
    return num;
}

inline intmax_t ratio_divide_and_get_den (intmax_t num1, intmax_t den1, intmax_t num2, intmax_t den2)
{
    intmax_t num, den;
    ratio_divide(num1, den1, num2, den2, num, den);
    return den;
}

} // details

// Comparison

template <typename R1, typename R2>
struct ratio_equal
{
    static bool const value;
};

template <typename R1, typename R2>
bool const ratio_equal<R1, R2>::value = (R1::num == R2::num && R1::den == R2::den);

template <typename R1, typename R2>
struct ratio_not_equal
{
    static bool const value;
};

template <typename R1, typename R2>
bool const ratio_not_equal<R1, R2>::value = ! ratio_equal<R1, R2>::value;

template <typename R1, typename R2>
struct ratio_less
{ 
    static bool const value;
};

template <typename R1, typename R2>
bool const ratio_less<R1, R2>::value = details::ratio_less(R1::num, R1::den, R2::num, R2::den);


template <typename R1, typename R2>
struct ratio_less_equal
{
    static bool const value;
};

template <typename R1, typename R2>
bool const ratio_less_equal<R1, R2>::value = ! details::ratio_less(R2::num, R2::den, R1::num, R1::den);

template <typename R1, typename R2>
struct ratio_greater
{
    static bool const value;
};

template <typename R1, typename R2>
bool const ratio_greater<R1, R2>::value = details::ratio_less(R2::num, R2::den, R1::num, R1::den);

template <typename R1, typename R2>
struct ratio_greater_equal
{
    static bool const value;
};

template <typename R1, typename R2>
bool const ratio_greater_equal<R1, R2>::value = ! details::ratio_less(R1::num, R1::den, R2::num, R2::den);

template <typename R1, typename R2>
struct ratio_add
{
    static intmax_t num;
    static intmax_t den;
};

template <typename R1, typename R2>
struct ratio_subtract
{
    static intmax_t num;
    static intmax_t den;
};

template<typename R1, typename R2>
struct ratio_multiply
{
    static intmax_t num;
    static intmax_t den;
};

template<typename R1, typename R2>
struct ratio_divide
{
    static intmax_t num;
    static intmax_t den;
};

// TODO May be there is a solution to avoid repeated calculation of ratio_add result.
// 
template <typename R1, typename R2>
intmax_t ratio_add<R1, R2>::num = details::ratio_add_and_get_num(R1::num, R1::den, R2::num, R2::den);

template<typename R1, typename R2>
intmax_t ratio_add<R1, R2>::den = details::ratio_add_and_get_den(R1::num, R1::den, R2::num, R2::den);

template <typename R1, typename R2>
intmax_t ratio_subtract<R1, R2>::num = details::ratio_subtract_and_get_num(R1::num, R1::den, R2::num, R2::den);

template<typename R1, typename R2>
intmax_t ratio_subtract<R1, R2>::den = details::ratio_subtract_and_get_den(R1::num, R1::den, R2::num, R2::den);

template <typename R1, typename R2>
intmax_t ratio_multiply<R1, R2>::num = details::ratio_multiply_and_get_num(R1::num, R1::den, R2::num, R2::den);

template<typename R1, typename R2>
intmax_t ratio_multiply<R1, R2>::den = details::ratio_multiply_and_get_den(R1::num, R1::den, R2::num, R2::den);

template <typename R1, typename R2>
intmax_t ratio_divide<R1, R2>::num = details::ratio_divide_and_get_num(R1::num, R1::den, R2::num, R2::den);

template<typename R1, typename R2>
intmax_t ratio_divide<R1, R2>::den = details::ratio_divide_and_get_den(R1::num, R1::den, R2::num, R2::den);

typedef ratio<1,       1000000000000000000> atto;
typedef ratio<1,          1000000000000000> femto;
typedef ratio<1,             1000000000000> pico;
typedef ratio<1,                1000000000> nano;
typedef ratio<1,                   1000000> micro;
typedef ratio<1,                      1000> milli;
typedef ratio<1,                       100> centi;
typedef ratio<1,                        10> deci;
typedef ratio<                       10, 1> deca;
typedef ratio<                      100, 1> hecto;
typedef ratio<                     1000, 1> kilo;
typedef ratio<                  1000000, 1> mega;
typedef ratio<               1000000000, 1> giga;
typedef ratio<            1000000000000, 1> tera;
typedef ratio<         1000000000000000, 1> peta;
typedef ratio<      1000000000000000000, 1> exa;

} // namespace pfs

#endif // __cplusplus < 201103L

#endif /* __PFS_RATIO_HPP__ */

