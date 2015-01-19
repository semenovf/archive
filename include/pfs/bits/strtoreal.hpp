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
	, parse_exp_state
	, finish_state
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
	Iter pos = begin;
	real_t r = real_t(0.0f);
	real_t integral = real_t(0.0f);
	real_t fract = real_t(0.0f);
	int sign = 1;
	int exp = -1;
//	int integral_digits = 0;

	uint32_t mant1 = 0
		, mant2 = 0
		, mant3 = 0
		, mant4 = 0;

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

    while (state != finish_state && pos < end) {
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
					state = finish_state;
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
					state = finish_state;
				} else {
					state = parse_mantissa_state;
				}
    		}
    		break;

    	case parse_mantissa_state:
    		while (pos < end) {
				if (is_digit(*pos)) {
					int digit = to_digit(*pos);
					++ integral_digits;
					if (integral_digits < 18) {
						r = r * real_t(10.f) + real_t(digit);
					} else {

						++exp;
					}
					++pos;
				} else {
					break;
				}
    		}
    		state = parse_fractal_state;
    		break;

    	case parse_fractal_state:
    		if (eq_latin1(*pos, decimalPoint)) {
    			++pos;
        		while (pos < end) {
    				if (is_digit(*pos)) {
    					int digit = to_digit(*pos);
    					r = r * real_t(10.f) + real_t(digit);
    					--frac;
    					++pos;
    				}
        		}
    		} else {
    			state = parse_exp_state
    		}
    		break;

    	case parse_exp_state:
    		break;

    	case finish_state:
    	default:
    		break;
    	}

    }

    if (endref) {
    	*endref = pos;
    }

    return r;
}


} // pfs



#endif /* __PFS_BITS_STRTOREAL_HPP_ */
