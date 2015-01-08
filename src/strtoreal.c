/*-----------------------------------------------------------------------------
// Copyright (c) 2014 - Fedor Semenov - All rights reserved
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//-----------------------------------------------------------------------------
// author	Fedor Semenov
// email	fedor.v.semenov (at) gmail.com
*/

#include <pfs.h>
#include <ctype.h>
#include <errno.h>

/* Grammar:
 *
 * double =   [sign] [integral-part] POINT fract-part [exp-part]
 * 			/ [sign] integral-part POINT [exp-part]
 *			/ [sign] integral-part exp-part
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

#ifdef PFS_HAVE_LONG_DOUBLE
static long double pfs_strtold (const char * arg_string, char decimalPoint, char ** arg_endptr);
#endif

real_t pfs_strtoreal (const char * arg_string, char decimalPoint, char ** arg_endptr)
{
	return pfs_strtold(arg_string, decimalPoint, arg_endptr);
}

/*-----------------------------------------------------------------------------
// Copyright (c) 2002 - Philip Howard - All rights reserved
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//-----------------------------------------------------------------------------
// author	Philip Howard
// email	phil@ipal.org
// homepage	http://phil.ipal.org/
//-----------------------------------------------------------------------------
// This file is best viewed using a fixed spaced font such as Courier
// and in a display at least 120 columns wide.
//-----------------------------------------------------------------------------
// program	test_strtold
// purpose	Test the strtold() function as well as pdh_strtold().
// compile	gcc -o test_strtold test_strtold.c
// execute	./test_strtold
//-----------------------------------------------------------------------------
*/

/* Original name: pdh_strtold()
 * Added new argument @c decimalPoint
 */

/* FIXME need recognition of overflow and underflow with errno ERANGE */
#ifdef PFS_HAVE_LONG_DOUBLE
long double pfs_strtold (const char * arg_string, char decimalPoint, char ** arg_endptr)
{
	static long double num_ld[16] = {
		0.0L, 1.0L, 2.0L, 3.0L, 4.0L, 5.0L, 6.0L, 7.0L,
		8.0L, 9.0L, 10.0L, 11.0L, 12.0L, 13.0L, 14.0L, 15.0L
    };

    static double num_d[10] = {
    	0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0
    };

    static double bits[4] = {
    	4194304.0, 8388608.0, 16777216.0, 33554432.0
    };

    const unsigned char * str_ptr;
    const unsigned char * end_ptr;
    long double		p2;
    long double		p5;
    long double		tmp;
    long double		num_hi;
    double			num_lo;
    int				accum;
    int				base;
    int				ch;
    int				digit;
    int				exp_ch;
    int				exponent;
    int				neg;
    int				neg_exp;
    int				point;
    int				scaling;


    /*----------------------------
    // Make sure we have a string.
    //----------------------------*/
    if (! arg_string) {
		errno = EINVAL;
		return 0.0L;
    }
    str_ptr = (const unsigned char *) arg_string;
    end_ptr = (const unsigned char *) arg_string;

    /*----------------------------------
    // Skip over any leading whitespace.
    //----------------------------------*/
    while (isspace(*str_ptr )) {
    	++ str_ptr;
    }

    /*-----------------------------------------------------------
    // Base value sign, if present, must be first non-whitespace.
    //-----------------------------------------------------------*/
    neg = 0;
    if ( * str_ptr == '-' ) {
    	neg = 1;
    	++ str_ptr;
    } else if ( * str_ptr == '+' ) {
    	++ str_ptr;
    }

    /*---------------------------------------
    // Skip over insignificant leading zeros.
    // They do not mean octal here.
    //---------------------------------------*/
    digit = 0;
    if (*str_ptr == '0') {
		digit = 1;
		while (*str_ptr == '0') {
			++ str_ptr;
		}
    }

/*---- CODE REPLACED
#ifndef DISALLOW_NAN
#ifdef NAN
    //-----------------
    // Check for "NaN".
    //-----------------
    if ( ( str_ptr[0] == 'N' || str_ptr[0] == 'n' ) &&
	 ( str_ptr[1] == 'a' || str_ptr[1] == 'A' ) &&
	 ( str_ptr[2] == 'N' || str_ptr[2] == 'n' ) ) {
	if ( arg_endptr ) {
	    * (const char * *) arg_endptr = str_ptr + 3;
	}
	return neg ? -((long double)(NAN)) : ((long double)(NAN));
    }
#endif
#endif
*/
	if (       (str_ptr[0] == 'I' || str_ptr[0] == 'i')
			&& (str_ptr[1] == 'N' || str_ptr[1] == 'n')
			&& (str_ptr[2] == 'F' || str_ptr[2] == 'f')) {

		if (   (str_ptr[3] == 'I' || str_ptr[3] == 'i')
			&& (str_ptr[4] == 'N' || str_ptr[4] == 'n')
			&& (str_ptr[5] == 'I' || str_ptr[5] == 'i')
			&& (str_ptr[6] == 'T' || str_ptr[6] == 't')
			&& (str_ptr[7] == 'Y' || str_ptr[7] == 'y')) {

			if ( arg_endptr ) {
			    * (const char **)arg_endptr = (char *)str_ptr + 8;
			}
		} else {
			if ( arg_endptr ) {
			    * (const char **)arg_endptr = (char *)str_ptr + 3;
			}
		}

		return neg ? -((long double)(PFS_INFINITY)) : ((long double)(PFS_INFINITY));
	} else if ((str_ptr[0] == 'N' || str_ptr[0] == 'n')
			&& (str_ptr[1] == 'A' || str_ptr[1] == 'a')
			&& (str_ptr[2] == 'N' || str_ptr[2] == 'n')) {

		if ( arg_endptr ) {
		    * (const char * *) arg_endptr = (char *)str_ptr + 3;
		}
		return neg ? -((long double)(PFS_NAN)) : ((long double)(PFS_NAN));
	}

    /*--------------------------------------
    // Validate first significant character.
    //--------------------------------------*/
    ch = * str_ptr;
    if ( '1' <= ch && ch <= '9' ) {
    	base = 10;
    } else if ( ch == decimalPoint ) {
    	base = 10;
    } else if ( ch == 'x' || ch == 'X' ) {
    	base = 16;
    	ch = * ++ str_ptr;
    } else {
		if ( arg_endptr ) {
			* (const char **) arg_endptr = (char *)end_ptr; /*str_ptr;*/
		}

		if ( ! digit ) {
			errno = EINVAL;
		}
		return 0.0L;
    }



    /*-------------------------------------------------------------------------
    // Now use different code for different bases.
    //-------------------------------------------------------------------------*/
    num_hi = 0.0L;
    num_lo = 0.0;
    scaling = 0;
    point = decimalPoint;
    accum = 1;

    /*-------------------------------------------------------------------------
    // Decimal.
    //-------------------------------------------------------------------------*/
    if ( base == 10 ) {
		for (;;) {
			if ( ch == point ) {
				point = -1;
				ch = * ++ str_ptr;
			} else if ( '0' <= ch && ch <= '9' ) {
				if ( accum ) {
					tmp = num_hi;
					num_hi *= 10.0L;

					if ( num_hi / 10.0L != tmp ) {
						num_hi = tmp;
						accum = 0;
					} else {
						num_lo *= 10.0;
					}
				}

				if ( accum ) {
					num_lo += num_d[ ch - '0' ];

					/*-- Move some bits over to num_hi before they get too high in num_lo.*/
					if ( num_lo >= bits[3] ) {
						num_lo -= bits[3];
						num_hi += bits[3];
					}

					if ( num_lo >= bits[2] ) {
						num_lo -= bits[2];
						num_hi += bits[2];
					}

					if ( num_lo >= bits[1] ) {
						num_lo -= bits[1];
						num_hi += bits[1];
					}

					if ( num_lo >= bits[0] ) {
						num_lo -= bits[0];
						num_hi += bits[0];
					}

					if ( point != decimalPoint )
						++ scaling;
				} else {
					if ( point == decimalPoint )
						-- scaling;
				}

				ch = * ++ str_ptr;
				end_ptr = str_ptr;

			} else {
				break;
			}

			/*ch = * ++ str_ptr;*/
		}

		/*-- Check for an exponent specification.*/
		exponent = 0;
		if ( ch == 'e' || ch == 'E' ) {
			neg_exp = 0;
			ch = * ++ str_ptr;

			/*-- Check for exponent sign.*/
			if ( * str_ptr == '-' ) {
				neg_exp = 1;
				ch = * ++ str_ptr;
			} else if ( * str_ptr == '+' ) {
				ch = * ++ str_ptr;
			}

			/*-- Scan exponent digits in decimal.*/
			for (;;) {
				if ( '0' <= ch && ch <= '9' ) {
					exponent *= 10;
					exponent += ch - '0';
				} else {
					break;
				}
				ch = * ++ str_ptr;
				end_ptr = str_ptr;
			}

			if ( neg_exp )
				exponent = - exponent;
		}

		/*-- Compensate exponent for point scaling.*/
		exponent -= scaling;

		/*-- Rescale converted base value for exponent.*/
		p2 = 1.0L;
		p5 = 1.0L;
		if ( exponent < 0 ) {
			while ( exponent <= -32 ) {
				p2 *= 4294967296.0L;
				p5 *= 23283064365386962890625.0L;
				exponent += 32;
			}

			if ( exponent <= -16 ) {
				p2 *= 65536.0L;
				p5 *= 152587890625.0L;
				exponent += 16;
			}

			if ( exponent <= -8 ) {
				p2 *= 256.0L;
				p5 *= 390625.0L;
				exponent += 8;
			}

			if ( exponent <= -4 ) {
				p2 *= 16.0L;
				p5 *= 625.0L;
				exponent += 4;
			}

			if ( exponent <= -2 ) {
				p2 *= 4.0L;
				p5 *= 25.0L;
				exponent += 2;
			}

			if ( exponent <= -1 ) {
				p2 *= 2.0L;
				p5 *= 5.0L;
				exponent += 1;
			}
			num_hi += num_lo;
			num_hi /= p5;
			num_hi /= p2;
		} else {
			while ( exponent >= 32 ) {
				p2 *= 4294967296.0L;
				p5 *= 23283064365386962890625.0L;
				exponent -= 32;
			}

			if ( exponent >= 16 ) {
				p2 *= 65536.0L;
				p5 *= 152587890625.0L;
				exponent -= 16;
			}

			if ( exponent >= 8 ) {
				p2 *= 256.0L;
				p5 *= 390625.0L;
				exponent -= 8;
			}

			if ( exponent >= 4 ) {
				p2 *= 16.0L;
				p5 *= 625.0L;
				exponent -= 4;
			}

			if ( exponent >= 2 ) {
				p2 *= 4.0L;
				p5 *= 25.0L;
				exponent -= 2;
			}

			if ( exponent >= 1 ) {
				p2 *= 2.0L;
				p5 *= 5.0L;
				exponent -= 1;
			}

			num_hi *= p5;
			num_lo *= p5;
			num_hi += num_lo;
			num_hi *= p2;
		}
    } /* if ( base == 10 ) */

    /*-------------------------------------------------------------------------
    // Cetal (hexadecimal).
    //-------------------------------------------------------------------------*/
    else if ( base == 16 ) {

		/*-- Outer loop scans base digits before the fraction point. */
		for (;;) {
			if (ch == decimalPoint) {
				/*-- Inner loop scans base digits after the fraction point.*/
				for (;;) {
					ch = * ++ str_ptr;

					if (( '0' <= ch
								&& ch <= '9'
								&& ((digit = ch - '0' ), 1 ))
						|| ('A' <= ch
								&& ch <= 'F'
								&& (( digit = 10 + ch - 'A' ), 1 ))
						|| ('a' <= ch
								&& ch <= 'f'
								&& ((digit = 10 + ch - 'a' ), 1 )))
					{
						num_hi *= 16.0L;
						num_hi += num_ld[ digit ];
						++ scaling;
					} else {
						break;
					}
				}
				break;
			}

			if ( ('0' <= ch
					&& ch <= '9'
					&& (( digit = ch - '0' ), 1 ))
				|| ('A' <= ch
						&& ch <= 'F'
						&& ((digit = 10 + ch - 'A' ), 1 ))
				|| ('a' <= ch
						&& ch <= 'f'
						&& ((digit = 10 + ch - 'a' ), 1))) {

				num_hi *= 16.0L;
				num_hi += num_ld[ digit ];
			} else {
				break;
			}
			ch = * ++str_ptr;
		}

		/*-- Check for an exponent specification.
		//-- 'p' or 'P' : radix 2 (C99)
		//-- 'q' or 'Q' : radix 16 (non-standard)*/
		exponent = 0;

		if ( ch == 'p' || ch == 'P' || ch == 'q' || ch == 'Q' ) {
			exp_ch = ch;
			neg_exp = 0;
			ch = * ++ str_ptr;

			/*-- Check for exponent sign.*/
			if ( * str_ptr == '-' ) {
				neg_exp = 1;
				ch = * ++ str_ptr;
			} else if ( * str_ptr == '+' ) {
				ch = * ++ str_ptr;
			}

			/*-- Scan exponent digits in decimal.*/
			for (;;) {
				if ( '0' <= ch && ch <= '9' ) {
					exponent *= 10;
					exponent += ch - '0';
				}
				else {
					break;
				}
				ch = * ++ str_ptr;
			}

			if (neg_exp)
				exponent = - exponent;

			/*-- If exponent radix is 'q' or 'Q' the
			//-- exponent is radix 16 instead of 2.*/
			if ( exp_ch == 'q' || exp_ch == 'Q' ) {
				exponent *= 4;
			}
		}

		/*-- Compensate exponent for radix 16 point scaling.*/
		exponent -= scaling * 4;

		/*-- Rescale converted base value for exponent.*/
		p2 = 1.0L;
		if ( exponent < 0 ) {
			while ( exponent <= -64 ) {
				p2 *= 18446744073709551616.0L;
				exponent += 64;
			}

			if ( exponent <= -32 ) {
				p2 *= 4294967296.0L;
				exponent += 32;
			}

			if ( exponent <= -16 ) {
				p2 *= 65536.0L;
				exponent += 16;
			}

			if ( exponent <= -8 ) {
				p2 *= 256.0L;
				exponent += 8;
			}

			if ( exponent <= -4 ) {
				p2 *= 16.0L;
				exponent += 4;
			}

			if ( exponent <= -2 ) {
				p2 *= 4.0L;
				exponent += 2;
			}

			if ( exponent <= -1 ) {
				p2 *= 2.0L;
				exponent += 1;
			}
			num_hi /= p2;
		} else {
			while ( exponent >= 64 ) {
				p2 *= 18446744073709551616.0L;
				exponent -= 64;
			}

			if ( exponent >= 32 ) {
				p2 *= 4294967296.0L;
				exponent -= 32;
			}

			if ( exponent >= 16 ) {
				p2 *= 65536.0L;
				exponent -= 16;
			}

			if ( exponent >= 8 ) {
				p2 *= 256.0L;
				exponent -= 8;
			}

			if ( exponent >= 4 ) {
				p2 *= 16.0L;
				exponent -= 4;
			}

			if ( exponent >= 2 ) {
				p2 *= 4.0L;
				exponent -= 2;
			}

			if ( exponent >= 1 ) {
				p2 *= 2.0L;
				exponent -= 1;
			}
			num_hi *= p2;
		}
    } else {
		/*-------------------------------------------------------------------------
		// The base cannot be unknown unless there is some logic error.
		// EDOM is not a perfect choice here.  Any better ideas?  abort()?
		//-------------------------------------------------------------------------*/
		errno = EDOM;
		return 0.0L;
    }

    //-----------------------------------------------------------
    // If the number is supposed to be negative, then make it so.
    //-----------------------------------------------------------
    if ( neg ) {
    	num_hi = - num_hi;
    }

    //---------------------------------------------------------
    // Store the ending pointer and return the converted value.
    //---------------------------------------------------------
    if ( arg_endptr ) {
    	*(const char **) arg_endptr = (char *)end_ptr; /*str_ptr;*/
    }
    return num_hi;
}
#endif /* PFS_HAVE_LONG_DOUBLE */

#ifdef __TRY_TO_IMPLEMENT__

static double_t __pfs_pow (double_t a, int b)
{
	double_t r = 1;
	int sign = b >= 0 ? 1 : -1;

	if (sign < 0)
		b *= -1;

	while (b--)
		r *= a;

	return sign < 0 ? 1/r : r;
}


#define _SKIP_WS        0
#define _SIGN           1
#define _CHECK_NAN_INF  2
#define _SKIP_LEAD_ZERO 3
#define _INTEGRAL_PART  4
#define _DECIMAL_POINT  5
#define _FRACT_PART     6
#define _EXP_CHAR       7
#define _EXP_SIGN       8
#define _EXP_PART       9
#define _END           -1

double_t pfs_strtoreal (const char * nptr, char decimalPoint, char ** endptr)
{
	const char * p    = nptr;
	const char * pend = nptr + strlen(nptr);
	const char * fract_part_begin = NULL;
	const char * fract_part_end   = NULL;
	const char * exp_part_begin   = NULL;
	const char * exp_part_end     = NULL;
	int inf = 0; /* is infinity */
	int nan = 0; /* is NaN */
	int sign = 1;
	int exp_sign = 1;
	double_t result = PFS_DOUBLE_LITERAL(0.0);
	double_t fract  = PFS_DOUBLE_LITERAL(0.0);
	double_t exp    = PFS_DOUBLE_LITERAL(0.0);
	int power = 0;

    if ( ! nptr ) {
    	errno = EINVAL;
    	return PFS_DOUBLE_LITERAL(0.0);
    }


	int state = _SKIP_WS;

	while (p < pend && state != _END) {
		switch (state) {
		case _SKIP_WS:
			if(isspace(*p))
				++p;
			else
				state = _SIGN;
			break;

		case _SIGN:
			if (*p == '-') {
				sign = -1;
				++p;
			} else if (*p == '+') {
				++p;
			}

			state = _CHECK_NAN_INF;
			break;

		case _CHECK_NAN_INF:
			/* An infinity is either "INF" or "INFINITY", disregarding case. */
			/* A NAN is "NAN" (disregarding case) optionally followed by '(', a sequence of characters, followed by ')'. */
			/* The character string specifies in an implementation-dependent way the type of NAN. */

			if(strlen(p) >= 8
					&& (p[0] == 'I' || p[0] == 'i')
					&& (p[1] == 'N' || p[1] == 'n')
					&& (p[2] == 'F' || p[2] == 'f')
					&& (p[3] == 'I' || p[3] == 'i')
					&& (p[4] == 'N' || p[4] == 'n')
					&& (p[5] == 'I' || p[5] == 'i')
					&& (p[6] == 'T' || p[6] == 't')
					&& (p[7] == 'Y' || p[7] == 'y')) {

				p += 8;
				inf = 1;
				state = _END;
			} else if (strlen(p) >= 3
					&& (p[0] == 'I' || p[0] == 'i')
					&& (p[1] == 'N' || p[1] == 'n')
					&& (p[2] == 'F' || p[2] == 'f')) {
				p += 3;
				inf = 1;
				state = _END;
			} else if (strlen(p) >= 3
					&& (p[0] == 'N' || p[0] == 'n')
					&& (p[1] == 'A' || p[1] == 'a')
					&& (p[2] == 'N' || p[2] == 'n')) {
				p += 3;
				nan = 1;
				state = _END;
			} else {
				state = _SKIP_LEAD_ZERO;
			}
			break;

		case _SKIP_LEAD_ZERO:
			if (*p == '0') {
				++p;
			} else {
				state = _INTEGRAL_PART;
			}
			break;

		case _INTEGRAL_PART:
			if (isdigit(*p)) {
				result = result * 10 + (*p - '0');
				++p;
			} else {
				state = _DECIMAL_POINT;
			}
			break;

		case _DECIMAL_POINT:
			if (*p == decimalPoint) {
				++p;
			}

			state = _FRACT_PART;
			fract_part_begin = p;
			fract_part_end = p;
			break;

		case _FRACT_PART:
			if (isdigit(*p)) {
				++fract_part_end;
				++p;
			} else {
				fract_part_end = p;
				state = _EXP_CHAR;
			}
			break;

		case _EXP_CHAR:
			if (*p == 'e' || *p == 'E') {
				++p;
				state = _EXP_SIGN;
			} else {
				state = _END;
			}
			break;

		case _EXP_SIGN:
			if (*p == '-') {
				exp_sign = -1;
				++p;
			} else if (*p == '+') {
				++p;
			}

			state = _EXP_PART;
			exp_part_begin = p;
			exp_part_end = p;

			break;

		case _EXP_PART:
			if (isdigit(*p)) {
				++exp_part_end;
				++p;
			} else {
				exp_part_end = p;
				state = _END;
			}
			break;
		}
	}

	if (endptr)
		*endptr = (char*)p;


	if (inf)
		return sign ? -((double_t)(PFS_INFINITY)) : ((double_t)(PFS_INFINITY));

	if (nan)
		return sign ? -((double_t)(PFS_NAN)) : ((double_t)(PFS_NAN));

	if (sign < 0)
		result = -result;

	p = fract_part_begin;
	fract = 0.0;
	power = -1;

	while (p < fract_part_end) {
		fract += __pfs_pow(10.0, power--) * (*p - '0');
		++p;
	}

	result += fract;

	(void)exp_part_begin;
	(void)exp_sign;
	(void)exp;

	return result;
}

#endif /* __TRY_TO_IMPLEMENT__ */