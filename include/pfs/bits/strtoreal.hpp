/*
 * strtoreal.hpp
 *
 *  Created on: Jan 19, 2015
 *      Author: wladt
 */

#ifndef __PFS_BITS_STRTOREAL_HPP_
#define __PFS_BITS_STRTOREAL_HPP_

#include <pfs.hpp>
#include <pfs/ctype.hpp>
#include <cerrno>

/* Grammar:
 *
 * double =   [sign] [integral-part] POINT fract-part [exp-part]
 * 			/ [sign] integral-part POINT [exp-part]
 *			/ [sign] integral-part exp-part
 *			/ [sign] (N | n ) (A | a ) (N | n )
 *			/ [sign] (I | i ) (N | n ) (F | f ) [(I | i ) (N | n ) (I | i ) (T | t ) (Y | y )]
 *
 * integral-part = digit-seq
 *
 * fract-part    = digit-seq
 *
 * exp-part = exp-char [exp-sign] digit-seq
 *
 * exp-char = 'e' / 'E'
 *
 * sign = '+' / '-'
 *
 * exp-sign = '+' / '-'
 *
 * digit-seq = digit
 * 			/ digit-seq digit
 *
 */

namespace pfs {

enum state_enum {
	  parse_sign_state
	, skip_zeros_state
	, parse_nan_state
	, parse_inf_state
	, parse_mantissa_state
	, parse_exp_sign_state
	, parse_exp_state
	, finish_state
	, nan_state
	, infinity_state
};

template <typename Iter>
Iter check_str_nan (Iter begin, Iter end)
{
	Iter pos(begin);

	if (pos < end && (eq_latin1(*pos, 'N') || eq_latin1(*pos, 'n'))) {
		if (++pos < end && (eq_latin1(*pos, 'A') || eq_latin1(*pos, 'a'))) {
			if (++pos < end && (eq_latin1(*pos, 'N') || eq_latin1(*pos, 'n'))) {
				++pos;
				return pos;
			}
		}
	}

	return begin;
}

template <typename Iter>
Iter check_str_inf (Iter begin, Iter end)
{
	Iter pos(begin);

	if (pos < end && (eq_latin1(*pos, 'I') || eq_latin1(*pos, 'i'))) {
		if (++pos < end && (eq_latin1(*pos, 'N') || eq_latin1(*pos, 'n'))) {
			if (++pos < end && (eq_latin1(*pos, 'F') || eq_latin1(*pos, 'f'))) {
				if (++pos < end && (eq_latin1(*pos, 'I') || eq_latin1(*pos, 'i'))) {
					if (++pos < end && (eq_latin1(*pos, 'N') || eq_latin1(*pos, 'n'))) {
						if (++pos < end && (eq_latin1(*pos, 'I') || eq_latin1(*pos, 'i'))) {
							if (++pos < end && (eq_latin1(*pos, 'T') || eq_latin1(*pos, 't'))) {
								if (++pos < end && (eq_latin1(*pos, 'Y') || eq_latin1(*pos, 'y'))) {
									++pos;
									return pos;
								}
							}
						}
					}
				} else { // 'INF' string
					return pos;
				}
			}
		}
	}

	return begin;
}


/**
 *
 * @param begin
 * @param end
 * @param endref
 * @return
 *          If no conversion is performed, zero is returned and the value of @c begin
 *          is stored in the location referenced by @c endref.
 */
template <typename CharT, typename Iter>
real_t strtoreal (Iter begin, Iter end, CharT decimalPoint, Iter * endref = nullptr)
{
	Iter pos(begin);
	Iter pos_saved(begin);
	real_t r = real_t(0.0f);
	int sign = 1;
	int expSign = 1;
	int fractExp = 0;
	int exp = 0;
	int expInc = 1;

	uint32_t mant1 = 0
		, mant2 = 0
		, mant3 = 0
		, mant4 = 0;
	size_t mant_size = 0;
	bool hasDecPoint = false;

	/* Table giving binary powers of 10.  Entry
	   is 10^2^i.  Used to convert decimal
	   exponents into floating-point numbers. */
	static real_t powersOf10[] = {
	      10.
	    , 100.
		, 1.0e4
		, 1.0e8
		, 1.0e16
		, 1.0e32
		, 1.0e64
		, 1.0e128
		, 1.0e256
#ifdef PFS_HAVE_LONG_DOUBLE
		, 1.0e512L
		, 1.0e1024L
		, 1.0e2048L
		, 1.0e4096L
#endif
	};

	errno = 0;

    if (begin == end) {
    	if (endref)
    		*endref = begin;
		return real_t(0.0f);
    }

    //
    // Skip over any leading whitespace.
    //
    while (pos < end && is_space(*pos )) {
    	++pos;
    }

    state_enum state = parse_sign_state;

    while (state < finish_state && pos < end) {
    	switch (state) {
    	case parse_sign_state:
    		if (eq_latin1(*pos, '-')) {
    			sign = -1;
    			++pos;
    		} else if (eq_latin1(*pos, '+')) {
    			++pos;
    		}

    		state = skip_zeros_state;
    		break;

    	case skip_zeros_state:
    		if (eq_latin1(*pos, '0')) {
    			while (pos < end && eq_latin1(*pos, '0'))
    				++pos;
    		} else {
    			state = parse_nan_state;
    		}
    		break;

    	case parse_nan_state: {
				Iter pos1 = check_str_nan(pos, end);
				if (pos1 != pos) {
					r = sign < 0 ? - real_t(PFS_NAN) : real_t(PFS_NAN);
					pos = pos1;
					state = nan_state;
				} else {
					state = parse_inf_state;
				}
    		}
    		break;

    	case parse_inf_state: {
				Iter pos1 = check_str_inf(pos, end);
				if (pos1 != pos) {
					r = sign < 0 ? - real_t(PFS_INFINITY) : real_t(PFS_INFINITY);
					pos = pos1;
					state = infinity_state;
				} else {
					state = parse_mantissa_state;
				}
    		}
    		break;

    	case parse_mantissa_state:
    		while (pos < end) {
				if (is_digit(*pos)) {
					int digit = to_digit(*pos);

					if (mant_size < 9) {
						mant1 = mant1 * 10 + digit;
					} else if (mant_size < 18) {
						mant2 = mant2 * 10 + digit;
					} else if (mant_size < 27) {
						mant3 = mant3 * 10 + digit;
					} else if (mant_size < 36) {
						mant4 = mant4 * 10 + digit;
					} // else insignificant digits
					fractExp += expInc;
					++mant_size;
				} else if (eq_latin1(*pos, decimalPoint)) {
					if (!hasDecPoint) {
						hasDecPoint = true;
						expInc = -1;
						if (mant_size > 0)
							--fractExp;
					} else {
						state = finish_state;
						break;
					}
				} else if (eq_latin1(*pos, 'e') || eq_latin1(*pos, 'E')) {
					pos_saved = pos; // save position for state of incomplete of exponent part
					state = parse_exp_sign_state;
					++pos;
					break;
				} else {
					state = finish_state;
					break;
				}
				++pos;
    		}
    		break;

    	case parse_exp_sign_state:
    		if (eq_latin1(*pos, '-')) {
    			expSign = -1;
    			++pos;
    		} else if (eq_latin1(*pos, '+')) {
    			++pos;
    		}

    		state = parse_exp_state;
    		break;

    	case parse_exp_state:
//    		if (!is_digit(*pos)) {
//    			pos = pos_saved; // exponent part is incomplete
//    			state = finish_state;
//    		} else {
				while (pos < end) {
					if (is_digit(*pos)) {
						int digit = to_digit(*pos);
						exp = exp * 10 + digit;
						++pos;

						if (exp > PFS_REAL_MAX_10_EXP) { // overflow
							errno = ERANGE;
							break;
						}
					} else {
						state = finish_state;
						break;
					}
				}
//    		}
    		break;

    	case finish_state:
    	default:
    		break;
    	}
    }

    if (state > finish_state) {
    	;
    } else if (mant_size == 0) {
    	pos = begin;
    } else if (state <= finish_state) {

//    	if (state != finish_state)
//    		pos = pos_saved;
    	(void)pos_saved;

		if (expSign < 0)
			exp = - exp;

		exp += fractExp;

		if (exp < PFS_REAL_MIN_10_EXP) { // underflow
			errno = ERANGE;
			r = 0.0f;
		} else if (exp > PFS_REAL_MAX_10_EXP) { // overflow
			errno = ERANGE;
			if (sign < 0) {
				r = - PFS_HUGE_VAL;
			} else {
				r = PFS_HUGE_VAL;
			}
		} else {
			real_t dblExp, *d;

			dblExp = 1.0f;

			if (exp < 0) {
				exp = - exp;
				expSign = -1;
			} else {
				expSign = 1;
			}

			for (d = powersOf10; exp != 0; exp >>= 1, d += 1) {
				if (exp & 01) {
					dblExp *= *d;
				}
			}

			r = 1.e27 * mant4 + 1.e18 * mant3 + 1.e9 * mant2 + mant1;

			if (expSign < 0) {
				r /= dblExp;
			} else {
				r *= dblExp;
			}

			if (sign < 0)
				r = - r;
		}
    }

    if (endref) {
    	*endref = pos;
    }

    return r;
}


} // pfs



#endif /* __PFS_BITS_STRTOREAL_HPP_ */
