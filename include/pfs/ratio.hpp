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

namespace pfs {

#if __FIXME__

// Adapted from __udiv_qrnnd_c in longlong.h
// This version assumes that the high bit of __d is 1.
template<uintmax_t __n1, uintmax_t __n0, uintmax_t __d>
struct __big_div_impl
{
private:
  static_assert(__d >= (uintmax_t(1) << (sizeof(intmax_t) * 8 - 1)),
        "Internal library error");
  static_assert(__n1 < __d, "Internal library error");
  static constexpr uintmax_t __c = uintmax_t(1) << (sizeof(intmax_t) * 4);
  static constexpr uintmax_t __d1 = __d / __c;
  static constexpr uintmax_t __d0 = __d % __c;

  static constexpr uintmax_t __q1x = __n1 / __d1;
  static constexpr uintmax_t __r1x = __n1 % __d1;
  static constexpr uintmax_t __m = __q1x * __d0;
  static constexpr uintmax_t __r1y = __r1x * __c + __n0 / __c;
  static constexpr uintmax_t __r1z = __r1y + __d;
  static constexpr uintmax_t __r1
  = ((__r1y < __m) ? ((__r1z >= __d) && (__r1z < __m))
 ? (__r1z + __d) : __r1z : __r1y) - __m;
  static constexpr uintmax_t __q1
  = __q1x - ((__r1y < __m)
     ? ((__r1z >= __d) && (__r1z < __m)) ? 2 : 1 : 0);
  static constexpr uintmax_t __q0x = __r1 / __d1;
  static constexpr uintmax_t __r0x = __r1 % __d1;
  static constexpr uintmax_t __n = __q0x * __d0;
  static constexpr uintmax_t __r0y = __r0x * __c + __n0 % __c;
  static constexpr uintmax_t __r0z = __r0y + __d;
  static constexpr uintmax_t __r0
  = ((__r0y < __n) ? ((__r0z >= __d) && (__r0z < __n))
 ? (__r0z + __d) : __r0z : __r0y) - __n;
  static constexpr uintmax_t __q0
  = __q0x - ((__r0y < __n) ? ((__r0z >= __d)
              && (__r0z < __n)) ? 2 : 1 : 0);

public:
  static constexpr uintmax_t __quot = __q1 * __c + __q0;
  static constexpr uintmax_t __rem = __r0;

private:
  typedef __big_mul<__quot, __d> _Prod;
  typedef __big_add<_Prod::__hi, _Prod::__lo, 0, __rem> _Sum;
  static_assert(_Sum::__hi == __n1 && _Sum::__lo == __n0,
        "Internal library error");
};

template<uintmax_t __n1, uintmax_t __n0, uintmax_t __d>
struct __big_div
{
private:
  static_assert(__d != 0, "Internal library error");
  static_assert(sizeof (uintmax_t) == sizeof (unsigned long long),
        "This library calls __builtin_clzll on uintmax_t, which "
        "is unsafe on your platform. Please complain to "
        "http://gcc.gnu.org/bugzilla/");
  static constexpr int __shift = __builtin_clzll(__d);
  static constexpr int __coshift_ = sizeof(uintmax_t) * 8 - __shift;
  static constexpr int __coshift = (__shift != 0) ? __coshift_ : 0;
  static constexpr uintmax_t __c1 = uintmax_t(1) << __shift;
  static constexpr uintmax_t __c2 = uintmax_t(1) << __coshift;
  static constexpr uintmax_t __new_d = __d * __c1;
  static constexpr uintmax_t __new_n0 = __n0 * __c1;
  static constexpr uintmax_t __n1_shifted = (__n1 % __d) * __c1;
  static constexpr uintmax_t __n0_top = (__shift != 0) ? (__n0 / __c2) : 0;
  static constexpr uintmax_t __new_n1 = __n1_shifted + __n0_top;
  typedef __big_div_impl<__new_n1, __new_n0, __new_d> _Res;

public:
  static constexpr uintmax_t __quot_hi = __n1 / __d;
  static constexpr uintmax_t __quot_lo = _Res::__quot;
  static constexpr uintmax_t __rem = _Res::__rem / __c1;

private:
  typedef __big_mul<__quot_lo, __d> _P0;
  typedef __big_mul<__quot_hi, __d> _P1;
  typedef __big_add<_P0::__hi, _P0::__lo, _P1::__lo, __rem> _Sum;
  // No overflow.
  static_assert(_P1::__hi == 0, "Internal library error");
  static_assert(_Sum::__hi >= _P0::__hi, "Internal library error");
  // Matches the input data.
  static_assert(_Sum::__hi == __n1 && _Sum::__lo == __n0,
        "Internal library error");
  static_assert(__rem < __d, "Internal library error");
};

#endif // __FIXME__

  /**
   *  @brief Provides compile-time rational arithmetic.
   *
   *  This class template represents any finite rational number with a
   *  numerator and denominator representable by compile-time constants of
   *  type intmax_t. The ratio is simplified when instantiated.
   *
   *  For example:
   *  @code
   *    std::ratio<7,-21>::num == -1;
   *    std::ratio<7,-21>::den == 3;
   *  @endcode
   *  
  */
template <intmax_t Num, intmax_t Denom = 1>
struct ratio
{
    static intmax_t num;
    static intmax_t den;
};

inline intmax_t __assert_ratio_parameters (intmax_t Num, intmax_t Denom)
{
    PFS_ASSERT_X(Denom != 0, "Denominator cannot be zero");
    //PFS_ASSERT_X(Num >= - max_value<intmax_t>() && Denom >= - max_value<intmax_t>(), "Out of range");
    return 0;
}

template <intmax_t Num, intmax_t Denom>
intmax_t pfs::ratio<Num, Denom>::num 
        = __assert_ratio_parameters(Num, Denom)
        + Num * math::integral_sign<Denom>::value / math::integral_gcd<Num, Denom>::value;

template <intmax_t Num, intmax_t Denom>
intmax_t pfs::ratio<Num, Denom>::den 
        = __assert_ratio_parameters(Num, Denom)
        + math::integral_abs<Denom>::value / math::integral_gcd<Num, Denom>::value;


// Comparison

template<typename R1, typename R2>
struct ratio_equal
{
    static bool const value;
};

template<typename R1, typename R2>
bool const ratio_equal<R1, R2>::value = (R1::num == R2::num && R1::den == R2::den);

template<typename R1, typename R2>
struct ratio_not_equal
{
    static bool const value;
};

template<typename R1, typename R2>
bool const ratio_not_equal<R1, R2>::value = ! ratio_equal<R1, R2>::value;


template<typename _R1, typename _R2,
    typename _Left = __big_mul<_R1::num,_R2::den>,
           typename _Right = __big_mul<_R2::num,_R1::den> >
    struct __ratio_less_impl_1
    : integral_constant<bool, __big_less<_Left::__hi, _Left::__lo,
           _Right::__hi, _Right::__lo>::value>
    { }; 

  template<typename _R1, typename _R2,
	   bool = (_R1::num == 0 || _R2::num == 0
		   || (__static_sign<_R1::num>::value
		       != __static_sign<_R2::num>::value)),
	   bool = (__static_sign<_R1::num>::value == -1
		   && __static_sign<_R2::num>::value == -1)>
    struct __ratio_less_impl
    : __ratio_less_impl_1<_R1, _R2>::type
    { };

  template<typename _R1, typename _R2>
    struct __ratio_less_impl<_R1, _R2, true, false>
    : integral_constant<bool, _R1::num < _R2::num>
    { };

  template<typename _R1, typename _R2>
    struct __ratio_less_impl<_R1, _R2, false, true>
    : __ratio_less_impl_1<ratio<-_R2::num, _R2::den>,
           ratio<-_R1::num, _R1::den> >::type
    { };


template<typename R1, typename R2>
struct ratio_less
    : __ratio_less_impl<_R1, _R2>::type
{ };


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

 

    
  /// ratio_less_equal
  template<typename _R1, typename _R2>
    struct ratio_less_equal
    : integral_constant<bool, !ratio_less<_R2, _R1>::value>
    { };
  
  /// ratio_greater
  template<typename _R1, typename _R2>
    struct ratio_greater
    : integral_constant<bool, ratio_less<_R2, _R1>::value>
    { };

  /// ratio_greater_equal
  template<typename _R1, typename _R2>
    struct ratio_greater_equal
    : integral_constant<bool, !ratio_less<_R1, _R2>::value>
    { };

  template<typename _R1, typename _R2,
      bool = (_R1::num >= 0),
      bool = (_R2::num >= 0),
      bool = ratio_less<ratio<__static_abs<_R1::num>::value, _R1::den>,
        ratio<__static_abs<_R2::num>::value, _R2::den> >::value>
    struct __ratio_add_impl
    {
    private:
      typedef typename __ratio_add_impl<
        ratio<-_R1::num, _R1::den>,
        ratio<-_R2::num, _R2::den> >::type __t;
    public:
      typedef ratio<-__t::num, __t::den> type;
    };

  // True addition of nonnegative numbers.
  template<typename _R1, typename _R2, bool __b>
    struct __ratio_add_impl<_R1, _R2, true, true, __b>
    {
    private:
      static constexpr uintmax_t __g = __static_gcd<_R1::den, _R2::den>::value;
      static constexpr uintmax_t __d2 = _R2::den / __g;
      typedef __big_mul<_R1::den, __d2> __d;
      typedef __big_mul<_R1::num, _R2::den / __g> __x;
      typedef __big_mul<_R2::num, _R1::den / __g> __y;
      typedef __big_add<__x::__hi, __x::__lo, __y::__hi, __y::__lo> __n;
      static_assert(__n::__hi >= __x::__hi, "Internal library error");
      typedef __big_div<__n::__hi, __n::__lo, __g> __ng;
      static constexpr uintmax_t __g2 = __static_gcd<__ng::__rem, __g>::value;
      typedef __big_div<__n::__hi, __n::__lo, __g2> __n_final;
      static_assert(__n_final::__rem == 0, "Internal library error");
      static_assert(__n_final::__quot_hi == 0 &&
        __n_final::__quot_lo <= __INTMAX_MAX__, "overflow in addition");
      typedef __big_mul<_R1::den / __g2, __d2> __d_final;
      static_assert(__d_final::__hi == 0 &&
        __d_final::__lo <= __INTMAX_MAX__, "overflow in addition");
    public:
      typedef ratio<__n_final::__quot_lo, __d_final::__lo> type;
    };

  template<typename _R1, typename _R2>
    struct __ratio_add_impl<_R1, _R2, false, true, true>
    : __ratio_add_impl<_R2, _R1>
    { };

  // True subtraction of nonnegative numbers yielding a nonnegative result.
  template<typename _R1, typename _R2>
    struct __ratio_add_impl<_R1, _R2, true, false, false>
    {
    private:
      static constexpr uintmax_t __g = __static_gcd<_R1::den, _R2::den>::value;
      static constexpr uintmax_t __d2 = _R2::den / __g;
      typedef __big_mul<_R1::den, __d2> __d;
      typedef __big_mul<_R1::num, _R2::den / __g> __x;
      typedef __big_mul<-_R2::num, _R1::den / __g> __y;
      typedef __big_sub<__x::__hi, __x::__lo, __y::__hi, __y::__lo> __n;
      typedef __big_div<__n::__hi, __n::__lo, __g> __ng;
      static constexpr uintmax_t __g2 = __static_gcd<__ng::__rem, __g>::value;
      typedef __big_div<__n::__hi, __n::__lo, __g2> __n_final;
      static_assert(__n_final::__rem == 0, "Internal library error");
      static_assert(__n_final::__quot_hi == 0 &&
        __n_final::__quot_lo <= __INTMAX_MAX__, "overflow in addition");
      typedef __big_mul<_R1::den / __g2, __d2> __d_final;
      static_assert(__d_final::__hi == 0 &&
        __d_final::__lo <= __INTMAX_MAX__, "overflow in addition");
    public:
      typedef ratio<__n_final::__quot_lo, __d_final::__lo> type;
    };

  template<typename _R1, typename _R2>
    struct __ratio_add
    {
      typedef typename __ratio_add_impl<_R1, _R2>::type type;
      static constexpr intmax_t num = type::num;
      static constexpr intmax_t den = type::den;
    };

  template<typename _R1, typename _R2>
    constexpr intmax_t __ratio_add<_R1, _R2>::num;

  template<typename _R1, typename _R2>
    constexpr intmax_t __ratio_add<_R1, _R2>::den;

  /// ratio_add
  template<typename _R1, typename _R2>
    using ratio_add = typename __ratio_add<_R1, _R2>::type;

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

