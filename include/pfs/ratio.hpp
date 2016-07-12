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

#else // __cplusplus >= 201103L

#include <pfs.hpp>
#include <pfs/type_traits.hpp>
#include <pfs/limits.hpp>
#include <pfs/math.hpp>
#include <utility> // for std::pair

namespace pfs {

#if __FIXME__

// Adapted from __udiv_qrnnd_c in longlong.h

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


#endif // __FIXME__

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

// TODO May be there is a solution to avoid repeated calculation of ratio_add result.
// 
template <typename R1, typename R2>
intmax_t ratio_add<R1, R2>::num = details::ratio_add_and_get_num(R1::num, R1::den, R2::num, R2::den);

template<typename R1, typename R2>
intmax_t ratio_add<R1, R2>::den = details::ratio_add_and_get_den(R1::num, R1::den, R2::num, R2::den);

#if __FIXME__
  template<typename _R1, typename _R2>
    struct __ratio_multiply
    {
    private:
      static const intmax_t __gcd1 =
        __static_gcd<_R1::num, _R2::den>::value;
      static const intmax_t __gcd2 =
        __static_gcd<_R2::num, _R1::den>::value;

    public:
      typedef ratio<
        __safe_multiply<(_R1::num / __gcd1),
                        (_R2::num / __gcd2)>::value,
        __safe_multiply<(_R1::den / __gcd2),
                        (_R2::den / __gcd1)>::value> type;

      static constexpr intmax_t num = type::num;
      static constexpr intmax_t den = type::den;
    };

  template<typename _R1, typename _R2>
    constexpr intmax_t __ratio_multiply<_R1, _R2>::num;

  template<typename _R1, typename _R2>
    constexpr intmax_t __ratio_multiply<_R1, _R2>::den;

  /// ratio_multiply
  template<typename _R1, typename _R2>
    using ratio_multiply = typename __ratio_multiply<_R1, _R2>::type;

  template<typename _R1, typename _R2>
    struct __ratio_divide
    {
      static_assert(_R2::num != 0, "division by 0");

      typedef typename __ratio_multiply<
        _R1,
        ratio<_R2::den, _R2::num>>::type type;

      static constexpr intmax_t num = type::num;
      static constexpr intmax_t den = type::den;
    };

  template<typename _R1, typename _R2>
    constexpr intmax_t __ratio_divide<_R1, _R2>::num;

  template<typename _R1, typename _R2>
    constexpr intmax_t __ratio_divide<_R1, _R2>::den;

  /// ratio_divide
  template<typename _R1, typename _R2>
    using ratio_divide = typename __ratio_divide<_R1, _R2>::type;

   template<typename _R1, typename _R2>
    struct __ratio_subtract
    {
      typedef typename __ratio_add<
        _R1,
        ratio<-_R2::num, _R2::den>>::type type;

      static constexpr intmax_t num = type::num;
      static constexpr intmax_t den = type::den;
    };

  template<typename _R1, typename _R2>
    constexpr intmax_t __ratio_subtract<_R1, _R2>::num;

  template<typename _R1, typename _R2>
    constexpr intmax_t __ratio_subtract<_R1, _R2>::den;

  /// ratio_subtract
  template<typename _R1, typename _R2>
    using ratio_subtract = typename __ratio_subtract<_R1, _R2>::type;

#endif
  
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

