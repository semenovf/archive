/*
* strtod implementation.
* author: Yasuhiro Matsumoto
* license: public domain
*
* @see https://gist.github.com/mattn/1890186
*
* Changes by semenovf
*       - #include <ctype.h> (for isspace, isdigit)
*       - char * skipwhite (const char * q) => const char * skipwhite (const char * q)
*       	to avoid gcc warning: passing argument 1 of ‘skipwhite’ discards ‘const’ qualifier from pointer target type
* 		- added third argument decimalPoint:char
* 		- '.' replaced by decimalPoint
* 		- double replaced by double_t
* 		- replaced 'pow' function from math.h by 'pow_by_integer'
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <ctype.h>
#include "pfs.h"

static const char * skipwhite (const char * q)
{
	const char * p = q;
	while (isspace(*p))
		++p;
	return p;
}

static double_t pow_by_integer (double_t a, int b)
{
	double_t r = 1;
	int sign = b >= 0 ? 1 : -1;

	if (sign < 0)
		b *= -1;

	while (b--)
		r *= a;

	return sign < 0 ? 1/r : r;
}

#define vim_isdigit(x) isdigit(x)

// Returns zero on error
//
double vim_strtod (const char * str, char ** end, char decimalPoint)
{
	//double d = 0.0;
	double_t d = 0.0;
	int sign;
	int n = 0;
	const char *p, *a;

	a = p = str;
	p = skipwhite(p);

	/* decimal part */
	sign = 1;

	if (*p == '-') {
		sign = -1;
		++p;
	} else if (*p == '+')
		++p;

	if (vim_isdigit(*p)) {
		//d = (double)(*p++ - '0');
		d = (double_t)(*p++ - '0');

		while (*p && vim_isdigit(*p)) {
			//d = d * 10.0 + (double)(*p - '0');
			d = d * 10.0 + (double_t)(*p - '0');
			++p;
			++n;
		}
		a = p;
	} else if (*p != decimalPoint)
		goto done;

	d *= sign;

	/* fraction part */
	if (*p == decimalPoint) {
		//double f = 0.0;
		double_t f = 0.0;
		//double base = 0.1;
		double_t base = 0.1;
		++p;

		if (vim_isdigit(*p)) {
			while (*p && vim_isdigit(*p)) {
				f += base * (*p - '0') ;
				base /= 10.0;
				++p;
				++n;
			}
		}
		d += f * sign;
		a = p;
	}

	/* exponential part */
	if ((*p == 'E') || (*p == 'e')) {
		int e = 0;
		++p;

		sign = 1;
		if (*p == '-') {
			sign = -1;
			++p;
		} else if (*p == '+')
			++p;

		if (vim_isdigit(*p)) {
			while (*p == '0') // skip leading zeros
				++p;

			e = (int)(*p++ - '0');

			while (*p && vim_isdigit(*p)) {
				e = e * 10 + (int)(*p - '0');
				++p;
			}
			e *= sign;
		} else if (!vim_isdigit(*(a-1))) {
			a = str;
			goto done;
		} else if (*p == 0)
			goto done;

		//if (d == 2.2250738585072011 && e == -308) {
		if (d == 2.2250738585072011 && e == PFS_DOUBLE_MIN_10_EXP - 1) { // TODO FIXME
			d = 0.0;
			a = p;
			errno = ERANGE;
			goto done;
		}

		//if (d == 2.2250738585072012 && e <= -308) {
		if (d == 2.2250738585072012 && e <= PFS_DOUBLE_MIN_10_EXP - 1) { // TODO FIXME
			//d *= 1.0e-308;
			d = 0.0;
			a = p;
			goto done;
		}

		//d *= pow(10.0, (double) e);
		d *= pow_by_integer(10.0, e);
		a = p;
	} else if (p > str && !vim_isdigit(*(p-1))) {
		a = str;
		goto done;
	}

done:
	if (end)
		*end = (char*)a;
	return d;
}
