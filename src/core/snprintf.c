
/*
   Unix snprintf implementation.
   Version 1.1
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
   
   Revision History:

   1.1:
      *  added changes from Miles Bader
      *  corrected a bug with %f
      *  added support for %#g
      *  added more comments :-)
   1.0:
      *  supporting must ANSI syntaxic_sugars
   0.0:
      *  suppot %s %c %d

 THANKS(for the patches and ideas):
     Miles Bader
     Cyrille Rustom
     Jacek Slabocewiz
     Mike Parker(mouse)

*/

#include "snprintf.h"
#include <cwt/str.h>
#include <ctype.h>

/*
 * Find the nth power of 10
 */
static double pow_10(int n)
{ 
	int i;
	double P;

	if (n < 0)
		for (i = 1, P = 1., n = -n ; i <= n ; i++) {P *= .1;}
	else
		for (i = 1, P = 1. ; i <= n ; i++) {P *= 10.0;}
	return P;
}

/*
 * Find the integral part of the log in base 10 
 * Note: this not a real log10()
         I just need and approximation(integerpart) of x in:
          10^x ~= r
 * log_10(200) = 2;
 * log_10(250) = 2;
 */
static int log_10(double r)
{ 
	int i = 0;
	double result = 1.;

	if (r < 0.)
		r = -r;

	if (r < 1.) {
		while (result >= r) {
			result *= .1;
			i++;
		}
		return (-i);
	} else {
		while (result <= r) {
			result *= 10.;
			i++;
		}
		return (i - 1);
	}
}

/*
 * This function return the fraction part of a double
 * and set in ip the integral part.
 * In many ways it resemble the modf() found on most Un*x
 */
static double integral(double real, double *ip)
{ 
	int j;
	double i, s, p;
	double real_integral = 0.;

	/* take care of the obvious */
	/* equal to zero ? */
	if (real == 0.) {
		*ip = 0.;
		return (0.);
	}

	/* negative number ? */
	if (real < 0.)
		real = -real;

	/* a fraction ? */
	if ( real < 1.) {
		*ip = 0.;
		return real;
	}

	/* the real work :-) */
	for (j = log_10(real); j >= 0; j--) {
		p = pow_10(j);
		s = (real - real_integral)/p;
		i = 0.;

		while (i + 1. <= s)
			i++;
		real_integral += i*p;
	}

	*ip = real_integral;
	return (real - real_integral);
}

#define PRECISION 1.e-6
/* 
 * return an ascii representation of the integral part of the number
 * and set fract to be an ascii representation of the fraction part
 * the container for the fraction and the integral part or staticly
 * declare with fix size 
 */
static CWT_CHAR* numtoa(double number, int base, int precision, CWT_CHAR ** fract)
{
	register int i, j;
	double ip, fp; /* integer and fraction part */
	double fraction;
	int digits = MAX_INT - 1;
	static CWT_CHAR integral_part[MAX_INT];
	static CWT_CHAR fraction_part[MAX_FRACT];
	double sign;
	int ch;

	/* taking care of the obvious case: 0.0 */
	if (number == 0.) {
		integral_part[0] = _T('0');
		integral_part[1] = _T('\0');
		fraction_part[0] = _T('0');
		fraction_part[1] = _T('\0');
		return integral_part;
	}

	/* for negative numbers */
	if ((sign = number) < 0.) {
		number = -number;
		digits--; /* sign consume one digit */
	}

	fraction = integral(number, &ip);
	number = ip;

  /* do the integral part */
	if ( ip == 0.) {
		integral_part[0] = _T('0');
		i = 1;
	} else {
		for ( i = 0; i < digits && number != 0.; ++i) {
			number /= base;
			fp = integral(number, &ip);
			ch = (int)((fp + PRECISION)*base); /* force to round */
			integral_part[i] = (ch <= 9) ? ch + _T('0') : ch + _T('a') - 10;

			if (! isxdigit(integral_part[i])) /* bail out overflow !! */
				break;
			number = ip;
		}
	}
     
	/* Oh No !! out of bound, ho well fill it up ! */
	if (number != 0.)
		for (i = 0; i < digits; ++i)
			integral_part[i] = _T('9');

	/* put the sign ? */
	if (sign < 0.)
		integral_part[i++] = _T('-');

	integral_part[i] = _T('\0');

	/* reverse every thing */
	for ( i--, j = 0; j < i; j++, i--)
		SWAP_INT(integral_part[i], integral_part[j]);

	/* the fractionnal part */
	for (i=0, fp=fraction; precision > 0 && i < MAX_FRACT ; i++, precision--	) {
		fraction_part[i] = (int)((fp + PRECISION)*10. + _T('0'));
		if (! isdigit(fraction_part[i])) /* underflow ? */
			break;
		fp = (fp*10.0) - (double)(long)((fp + PRECISION)*10.);
	}

	fraction_part[i] = _T('\0');

	if (fract != (CWT_CHAR **)0)
		*fract = fraction_part;

	return integral_part;
}

/* for %d and friends, it puts in holder
 * the representation with the right padding
 */
static void decimal(struct DATA *p, double d)
{
	CWT_CHAR *tmp;

	tmp = itoa(d);
	p->width -= (int)cwt_str_ns()->strLen(tmp);
	PAD_RIGHT(p);
	PUT_PLUS(d, p);
	PUT_SPACE(d, p);

	while (*tmp) { /* the integral */
		PUT_CHAR(*tmp, p);
		tmp++;
	}
	PAD_LEFT(p);
}

/* for %o octal representation */
static void octal(struct DATA *p, double d)
{
	CWT_CHAR *tmp;

	tmp = otoa(d);
	p->width -= (int)cwt_str_ns()->strLen(tmp);

	PAD_RIGHT(p);

	if (p->square == FOUND) /* had prefix '0' for octal */
		PUT_CHAR(_T('0'), p);

	while (*tmp) { /* octal */
		PUT_CHAR(*tmp, p);
		tmp++;
	}
	PAD_LEFT(p);
}


	/* for %x %X hexadecimal representation */
static void hexa(struct DATA *p, double d)
{
	CWT_CHAR *tmp;

	tmp = htoa(d);
	p->width -= (int)cwt_str_ns()->strLen(tmp);

	PAD_RIGHT(p);

	if (p->square == FOUND) { /* prefix '0x' for hexa */
		PUT_CHAR(_T('0'), p); PUT_CHAR(*p->pf, p);
	}

	while (*tmp) { /* hexa */
		PUT_CHAR((*p->pf == _T('X') ? cwt_str_ns()->toUpper(*tmp) : *tmp), p);
		tmp++;
	}

	PAD_LEFT(p);
}

/* %s strings */
static void strings(struct DATA *p, CWT_CHAR *tmp)
{
	int i;

	i = (int)cwt_str_ns()->strLen(tmp);

	if (p->precision != NOT_FOUND) /* the smallest number */
		i = (i < p->precision ? i : p->precision);

	p->width -= i;
	PAD_RIGHT(p);

	while (i-- > 0) { /* put the sting */
		PUT_CHAR(*tmp, p);
		tmp++;
	}

	PAD_LEFT(p);
}

/* %f or %g  floating point representation */
static void floating(struct DATA *p, double d)
{
	CWT_CHAR *tmp, *tmp2;
	int i;

	DEF_PREC(p);
	d = ROUND(d, p);
	tmp = dtoa(d, p->precision, &tmp2);

	/* calculate the padding. 1 for the dot */
	p->width = p->width -
			((d > 0. && p->justify == RIGHT) ? 1:0) -
            ((p->space == FOUND) ? 1:0) -
            ((int)cwt_str_ns()->strLen(tmp)) - p->precision - 1;

	PAD_RIGHT(p);
	PUT_PLUS(d, p);
	PUT_SPACE(d, p);

	while (*tmp) { /* the integral */
		PUT_CHAR(*tmp, p);
		tmp++;
	}

	if (p->precision != 0 || p->square == FOUND)
		PUT_CHAR(_T('.'), p);  /* put the '.' */

	if (*p->pf == _T('g') || *p->pf == _T('G')) /* smash the trailing zeros */
		for (i = ((int)cwt_str_ns()->strLen(tmp2)) - 1; i >= 0 && tmp2[i] == _T('0'); i--)
			tmp2[i] = _T('\0');

	for (; *tmp2; tmp2++)
		PUT_CHAR(*tmp2, p); /* the fraction */
  
	PAD_LEFT(p);
} 

/* %e %E %g exponent representation */
static void exponent(struct DATA *p, double d)
{
	CWT_CHAR *tmp, *tmp2;
	int j, i;

	DEF_PREC(p);
	j = log_10(d);
	d = d / pow_10(j);  /* get the Mantissa */
	d = ROUND(d, p);
	tmp = dtoa(d, p->precision, &tmp2);

	/* 1 for unit, 1 for the '.', 1 for 'e|E',
	 * 1 for '+|-', 3 for 'exp' */
	/* calculate how much padding need */
	p->width = p->width -
			((d > 0. && p->justify == RIGHT) ? 1:0) -
			((p->space == FOUND) ? 1:0) - p->precision - 7;

	PAD_RIGHT(p);
	PUT_PLUS(d, p);
	PUT_SPACE(d, p);

	while (*tmp) {/* the integral */
		PUT_CHAR(*tmp, p);
		tmp++;
	}

	if (p->precision != 0 || p->square == FOUND)
		PUT_CHAR(_T('.'), p);  /* the '.' */

	if (*p->pf == _T('g') || *p->pf == _T('G')) /* smash the trailing zeros */
		for (i = ((int)cwt_str_ns()->strLen(tmp2)) - 1; i >= 0 && tmp2[i] == _T('0'); i--)
			tmp2[i] = _T('\0');

	for (; *tmp2; tmp2++)
		PUT_CHAR(*tmp2, p); /* the fraction */

	if (*p->pf == _T('g') || *p->pf == _T('e')) { /* the exponent put the 'e|E' */
		PUT_CHAR(_T('e'), p);
	} else {
		PUT_CHAR(_T('E'), p);
	}

	if (j > 0) {  /* the sign of the exp */
		PUT_CHAR(_T('+'), p);
	} else {
		PUT_CHAR(_T('-'), p);
		j = -j;
	}

	tmp = itoa((double)j);

	if (j < 9) {  /* need to pad the exponent with 0 '000' */
		PUT_CHAR(_T('0'), p);
		PUT_CHAR(_T('0'), p);
	} else if (j < 99) {
		PUT_CHAR(_T('0'), p);
	}

	while (*tmp) { /* the exponent */
		PUT_CHAR(*tmp, p);
		tmp++;
	}
	PAD_LEFT(p);
}

/* initialize the conversion specifiers */
static void conv_flag(CWT_CHAR *s, struct DATA * p)
{
	CWT_CHAR number[MAX_FIELD/2];
	int i;

	p->precision = p->width = NOT_FOUND;
	p->star_w = p->star_p = NOT_FOUND;
	p->square = p->space = NOT_FOUND;
	p->a_long = p->justify = NOT_FOUND;
	p->pad = _T(' ');

	for(;s && *s ;s++) {
		switch(*s) {
		case _T(' '):
			p->space = FOUND;
			break;
		case _T('#'):
			p->square = FOUND;
			break;
		case _T('*'):
			if (p->width == NOT_FOUND)
				p->width = p->star_w = FOUND;
			else
				p->precision = p->star_p = FOUND;
			break;
		case _T('+'):
			p->justify = RIGHT;
			break;

		case _T('-'):
			p->justify = LEFT;
			break;

		case _T('.'):
			if (p->width == NOT_FOUND)
				p->width = 0;
			break;
      case _T('0'):
    	  p->pad = _T('0');
    	  break;
      case _T('1'): case _T('2'): case _T('3'):
      case _T('4'): case _T('5'): case _T('6'):
      case _T('7'): case _T('8'): case _T('9'):     /* gob all the digits */
    	  for (i = 0; isdigit(*s); i++, s++)
    		  if (i < MAX_FIELD/2 - 1)
    			  number[i] = *s;
    	  number[i] = _T('\0');

    	  if (p->width == NOT_FOUND)
    		  p->width = cwt_str_ns()->toINT(number, 10, NULL);
    	  else
    		  p->precision = cwt_str_ns()->toINT(number, 10, NULL);
    	  s--;   /* went to far go back */
    	  break;
		}
	}
}

int cwt_vsnprintf(CWT_CHAR *string, size_t length, const CWT_CHAR *format, va_list args)
{
	struct DATA data;
	CWT_CHAR conv_field[MAX_FIELD];
	double d; /* temporary holder */
	int state;
	int i;

	data.length = length - 1; /* leave room for '\0' */
	data.holder = string;
	data.pf = format;
	data.counter = 0;

	/* sanity check, the string must be > 1 */
	if (length < 1)
		return -1;


	for (; *data.pf && (data.counter < data.length); data.pf++) {
		if ( *data.pf == _T('%') ) { /* we got a magic % cookie */
			conv_flag((CWT_CHAR *)0, &data); /* initialise format flags */

			for (state = 1; *data.pf && state;) {
				switch (*(++data.pf)) {
				case _T('\0'): /* a NULL here ? ? bail out */
            		*data.holder = _T('\0');
					return data.counter;
					break;

				case _T('f'):  /* float, double */
            		STAR_ARGS(&data);
					d = va_arg(args, double);
					floating(&data, d);
					state = 0;
					break;

				case _T('g'):
				case _T('G'):
					STAR_ARGS(&data);
					DEF_PREC(&data);
					d = va_arg(args, double);
					i = log_10(d);

					/* for '%g|%G' ANSI: use f if exponent
					 * is in the range or [-4,p] exclusively
					 * else use %e|%E
					 */
					if (-4 < i && i < data.precision)
						floating(&data, d);
					else
						exponent(&data, d);
					state = 0;
					break;

				case _T('e'):
				case _T('E'):  /* Exponent double */
					STAR_ARGS(&data);
					d = va_arg(args, double);
					exponent(&data, d);
					state = 0;
					break;

				case _T('u'):
				case _T('d'):  /* decimal */
					STAR_ARGS(&data);
					if (data.a_long == FOUND)
						d = va_arg(args, long);
					else
						d = va_arg(args, int);
					decimal(&data, d);
					state = 0;
					break;

				case _T('o'):  /* octal */
					STAR_ARGS(&data);
					if (data.a_long == FOUND)
						d = va_arg(args, long);
					else
						d = va_arg(args, int);
					octal(&data, d);
					state = 0;
					break;

				case _T('x'):
				case _T('X'):  /* hexadecimal */
					STAR_ARGS(&data);
				  	if (data.a_long == FOUND)
				  		d = va_arg(args, long);
				  	else
				  		d = va_arg(args, int);
				  	hexa(&data, d);
				  	state = 0;
				  	break;

				case _T('c'): /* character */
					d = va_arg(args, int);
					PUT_CHAR((CWT_CHAR)d, &data);
					state = 0;
					break;

				case _T('s'):  /* string */
					STAR_ARGS(&data);
					strings(&data, va_arg(args, CWT_CHAR *));
					state = 0;
					break;

				case _T('n'):
					*(va_arg(args, int *)) = data.counter; /* what's the count ? */
				  	state = 0;
				  	break;

				case _T('l'):
					data.a_long = FOUND;
					break;

				case _T('h'):
					break;

				case _T('%'):  /* nothing just % */
					PUT_CHAR(_T('%'), &data);
					state = 0;
					break;

				case _T('#'): case _T(' '): case _T('+'): case _T('*'):
				case _T('-'): case _T('.'): case _T('0'): case _T('1'):
				case _T('2'): case _T('3'): case _T('4'): case _T('5'):
				case _T('6'): case _T('7'): case _T('8'): case _T('9'):
					/* initialize width and precision */
					for (i = 0; isflag(*data.pf); i++, data.pf++)
						if (i < MAX_FIELD - 1)
							conv_field[i] = *data.pf;

					conv_field[i] = _T('\0');
					conv_flag(conv_field, &data);
					data.pf--;   /* went to far go back */
					break;

				default:
					/* is this an error ? maybe bail out */
					state = 0;
					break;
				} /* end switch */
			} /* end of for state */
		} else { /* not % */
			PUT_CHAR(*data.pf, &data);  /* add the char the string */
		}
	}

	*data.holder = _T('\0'); /* the end ye ! */
	return data.counter;
}
