/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "pfs/ratio.hpp"

#if __cplusplus < 201103L

#include "pfs/triple.hpp"

namespace pfs {

// Some double-precision utilities, where numbers are represented as
// Hi * 2 ^ (8 * sizeof(uintmax_t)) + Lo.

namespace details {

typedef std::pair<intmax_t, intmax_t> ratio_pair_type;
typedef std::pair<uintmax_t, uintmax_t> big_pair_type;
typedef pfs::triple<uintmax_t, uintmax_t, uintmax_t> big_triple_type;

inline ratio_pair_type ratio_pair (intmax_t num, intmax_t denom)
{
    return ratio_pair_type(ratio_num(num, denom), ratio_den(num, denom));
}

inline bool big_less (uintmax_t hi1, uintmax_t lo1, uintmax_t hi2, uintmax_t lo2)
{
    return (hi1 < hi2 || (hi1 == hi2 && lo1 < lo2));
}

inline void big_add (uintmax_t hi1, uintmax_t lo1, uintmax_t hi2, uintmax_t lo2, uintmax_t & hi, uintmax_t & lo)
{
    hi = (hi1 + hi2 + (lo1 + lo2 < lo1));
    lo = lo1 + lo2;
}

inline big_pair_type big_add (uintmax_t hi1, uintmax_t lo1, uintmax_t hi2, uintmax_t lo2)
{
    big_pair_type r;
    big_add(hi1, lo1, hi2, lo2, r.first, r.second);
    return r;
}

// Subtract a number from a bigger one.
inline void big_sub (uintmax_t hi1, uintmax_t lo1
        , uintmax_t hi2, uintmax_t lo2
        , uintmax_t & hi, uintmax_t & lo)
{
    PFS_ASSERT_X(! big_less(hi1, lo1, hi2, lo2), "Internal library error");
    lo = lo1 - lo2;
    hi = hi1 - hi2 - (lo1 < lo2);
};

inline big_pair_type big_sub (uintmax_t hi1, uintmax_t lo1
        , uintmax_t hi2, uintmax_t lo2)
{
    big_pair_type r;
    big_sub(hi1, lo1, hi2, lo2, r.first, r.second);
    return r;
}

void big_mul (uintmax_t x, uintmax_t y, uintmax_t & hi, uintmax_t & lo)
{
    uintmax_t c = uintmax_t(1) << (sizeof(intmax_t) * 4);
    uintmax_t x0 = x % c;
    uintmax_t x1 = x / c;
    uintmax_t y0 = y % c;
    uintmax_t y1 = y / c;
    uintmax_t x0y0 = x0 * y0;
    uintmax_t x0y1 = x0 * y1;
    uintmax_t x1y0 = x1 * y0;
    uintmax_t x1y1 = x1 * y1;
    uintmax_t mix = x0y1 + x1y0;
    uintmax_t mix_lo = mix * c;
    uintmax_t mix_hi  = mix / c + ((mix < x0y1) ? c : 0);
    
    big_add(mix_hi, mix_lo, x1y1, x0y0, hi, lo);
};

inline big_pair_type big_mul (uintmax_t x, uintmax_t y)
{
    big_pair_type r;
    big_mul(x, y, r.first, r.second);
    return r;
}

// This version assumes that the high bit of __d is 1.
//
void big_div_impl (uintmax_t n1, uintmax_t n0, uintmax_t d, uintmax_t & quot, uintmax_t & rem)
{
    PFS_ASSERT_X(d >= (uintmax_t(1) << (sizeof(intmax_t) * 8 - 1)), "Internal library error");
    PFS_ASSERT_X(n1 < d, "Internal library error");
    
    uintmax_t c = uintmax_t(1) << (sizeof(intmax_t) * 4);
    uintmax_t d1 = d / c;
    uintmax_t d0 = d % c;

    uintmax_t q1x = n1 / d1;
    uintmax_t r1x = n1 % d1;
    uintmax_t m = q1x * d0;
    uintmax_t r1y = r1x * c + n0 / c;
    uintmax_t r1z = r1y + d;
    
    uintmax_t r1 = ((r1y < m) 
            ? ((r1z >= d) && (r1z < m))
                ? (r1z + d) 
                : r1z 
            : r1y) - m;
    
    uintmax_t q1 = q1x - ((r1y < m)
            ? ((r1z >= d) && (r1z < m)) 
                ? 2 
                : 1
            : 0);
    
    uintmax_t q0x = r1 / d1;
    uintmax_t r0x = r1 % d1;
    uintmax_t n   = q0x * d0;
    uintmax_t r0y = r0x * c + n0 % c;
    uintmax_t r0z = r0y + d;
    
    uintmax_t r0  = ((r0y < n) 
            ? ((r0z >= d) && (r0z < n))
                ? (r0z + d)
                : r0z 
            : r0y) - n;
    
    uintmax_t q0 = q0x - ((r0y < n) 
            ? ((r0z >= d) && (r0z < n)) 
                ? 2 
                : 1
            : 0);

    quot = q1 * c + q0;
    rem  = r0;

    big_pair_type prod = big_mul(quot, d);
    big_pair_type sum  = big_add(prod.first, prod.second, 0, rem);

    PFS_ASSERT_X(sum.first == n1 && sum.second == n0, "Internal library error");
};

inline big_pair_type big_div_impl (uintmax_t n1, uintmax_t n0, uintmax_t d)
{
    big_pair_type r;
    big_div_impl(n1, n0, d, r.first, r.second);
    return r;
}

int clzll (unsigned long long x)
{
#ifdef PFS_CC_GNUC
    return __builtin_clzll(x);
#else
#   error "No implementation clzl() call for this platform"
#endif
}
            
int clzl (unsigned long x)
{
#ifdef PFS_CC_GNUC
    return __builtin_clzl(x);
#else
#   error "No implementation clzl() call for this platform"
#endif
}

void big_div (uintmax_t n1, uintmax_t n0, uintmax_t d, uintmax_t & quot_hi, uintmax_t & quot_lo, uintmax_t & rem)
{
    PFS_ASSERT_X(d != 0, "Internal library error");
    PFS_ASSERT_X((sizeof(uintmax_t) == sizeof(unsigned long long))
            || (sizeof(uintmax_t) == sizeof(unsigned long)),
        "This library calls __builtin_clzl[l] on uintmax_t, which "
        "is unsafe on your platform.");
    
    int shift = (sizeof(uintmax_t) == sizeof(unsigned long long)) 
            ? clzll(d)
            : clzl(d);

    int coshift_ = sizeof(uintmax_t) * 8 - shift;
    int coshift = (shift != 0) ? coshift_ : 0;
    uintmax_t c1 = uintmax_t(1) << shift;
    uintmax_t c2 = uintmax_t(1) << coshift;
    uintmax_t new_d = d * c1;
    uintmax_t new_n0 = n0 * c1;
    uintmax_t n1_shifted = (n1 % d) * c1;
    uintmax_t n0_top = (shift != 0) ? (n0 / c2) : 0;
    uintmax_t new_n1 = n1_shifted + n0_top;
    
    big_pair_type Res = big_div_impl(new_n1, new_n0, new_d);

    quot_hi = n1 / d;
    quot_lo = Res.first; //__quot
    rem = Res.second / c1;

    big_pair_type p0 = big_mul(quot_lo, d);
    big_pair_type p1 = big_mul(quot_hi, d);
    big_pair_type sum = big_add(p0.first, p0.second, p1.second, rem);
    
    // No overflow.
    PFS_ASSERT_X(p1.first == 0, "Internal library error");
    PFS_ASSERT_X(sum.first >= p0.first, "Internal library error");
    
    // Matches the input data.
    PFS_ASSERT_X(sum.first == n1 && sum.second == n0,"Internal library error");
    PFS_ASSERT_X(rem < d, "Internal library error");
};

big_triple_type big_div (uintmax_t n1, uintmax_t n0, uintmax_t d)
{
    big_triple_type r;
    big_div(n1, n0, d, r.first, r.second, r.third);
    return r;
}

bool ratio_less (intmax_t num1, intmax_t den1, intmax_t num2, intmax_t den2)
{
    int s1 = num1 < 0 ? -1 : 1;
    int s2 = num2 < 0 ? -1 : 1;
    bool b1 = num1 == 0 || num2 == 0 || (s1 != s2);
    bool b2 = (s1 == -1 && s2 == -1);
    
    if (b1 == true && b2 == false) {
        return  num1 < num2;
    }

    uintmax_t hi1, lo1, hi2, lo2;
    
    // Both are negative
    if (b1 == false && b2 == true) {
        big_mul(-num2, den1, hi1, lo1);
        big_mul(-num1, den2, hi2, lo2);
    } else {
        big_mul(num1, den2, hi1, lo1);
        big_mul(num2, den1, hi2, lo2);
    }

    return big_less(hi1, lo1, hi2, lo2);
}

inline bool ratio_less (ratio_pair_type const rp1, ratio_pair_type const rp2)
{
    return ratio_less(rp1.first, rp1.second, rp2.first, rp2.second);
}

void ratio_add (intmax_t num1, intmax_t den1, intmax_t num2, intmax_t den2, intmax_t & num, intmax_t & den)
{
    // First addendum value is positive ?
    //
    bool b1 = num1 >= 0; 
    
    // Second addendum value is positive ?
    //
    bool b2 = num2 >= 0; 
    
    // Absolute value of first addendum is less than absolute value of second addendum ?
    //
    ratio_pair_type rp1 = ratio_pair(math::details::integral_abs(num1), den1);
    ratio_pair_type rp2 = ratio_pair(math::details::integral_abs(num2), den2);
    bool b3 = ratio_less(rp1, rp2);
    
    // First addendum is negative
    // Second addendum is positive
    // Absolute value of first addendum is less than absolute value of second addendum
    //
    if (b1 == false &&  b2 == true && b3 == true) {
        ratio_add(num2, den2, num1, den1, num, den);
    } else if (b1 == true && b2 == false && b3 == false) {
    // First addendum is positive
    // Second addendum is negative
    // Absolute value of first addendum is grater or equal to absolute value of second addendum
    //
        uintmax_t g = math::details::integral_gcd(den1, den2);
        uintmax_t d2 = den2 / g;
        big_pair_type d = big_mul(den1, d2);
        big_pair_type x = big_mul(num1, den2 / g);
        big_pair_type y = big_mul(- num2, den1 / g);
        big_pair_type n = big_sub(x.first, x.second, y.first, y.second);
        big_triple_type ng = big_div(n.first, n.second, g);
        uintmax_t g2 = math::details::integral_gcd(ng.third, g);
        big_triple_type n_final = big_div(n.first, n.second, g2);
        
        PFS_ASSERT_X(n_final.third == 0, "Internal library error");
        PFS_ASSERT_X(n_final.first == 0 && n_final.second <= pfs::max_value<intmax_t>(), "Overflow in addition");
        
        big_pair_type d_final = big_mul(den1 / g2, d2);
      
        PFS_ASSERT_X(d_final.first == 0 && d_final.second <= pfs::max_value<intmax_t>(), "Overflow in addition");
        
        num = ratio_num(n_final.second, d_final.second);
        den = ratio_den(n_final.second, d_final.second);
    } else if (b1 == true && b2 == true) {
        // True addition of nonnegative numbers.
        //
        uintmax_t g = math::details::integral_gcd(den1, den2);
        uintmax_t d2 = den2 / g;
        big_pair_type d = big_mul(den1, d2);
        big_pair_type x = big_mul(num1, den2 / g);
        big_pair_type y = big_mul(num2, den1 / g);
        big_pair_type n = big_add(x.first, x.second, y.first, y.second);
        
        PFS_ASSERT_X(n.first >= x.first, "Internal library error");
        
        big_triple_type ng = big_div(n.first, n.second, g);
        uintmax_t g2 = math::details::integral_gcd(ng.third, g);
        big_triple_type n_final = big_div(n.first, n.second, g2);
        
        PFS_ASSERT_X(n_final.third == 0, "Internal library error");
        PFS_ASSERT_X(n_final.first == 0 && n_final.second <= pfs::max_value<intmax_t>(), "Overflow in addition");
        
        big_pair_type d_final = big_mul(den1 / g2, d2);
        
        PFS_ASSERT_X(d_final.first == 0 && d_final.second <= pfs::max_value<intmax_t>(), "Overflow in addition");
        
        num = ratio_num(n_final.second, d_final.second);
        den = ratio_den(n_final.second, d_final.second);
    } else {
        intmax_t n, d;
        ratio_add(-num1, den1, -num2, den2, n, d);
        
        num = ratio_num(-n, d);
        den = ratio_den(-n, d);
    }
}

} // details
    
} // pfs

#endif
        