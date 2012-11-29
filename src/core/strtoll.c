/*-
 * Copyright (c) 1992, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <cwt/str.h>
#include <string.h>
#include <errno.h>

extern BOOL str_isspace   (CWT_CHAR ch);

static ULONGLONG  __strtoull_helper (const CWT_CHAR *nptr, CWT_CHAR **endptr, int radix, BOOL is_unsigned)
{
	const CWT_CHAR *s;
	CWT_CHAR c;
	ULONGLONG acc;
	ULONGLONG cutoff;
	int neg, any, cutlim;

	/*
	 * See strtoll for comments as to the logic used.
	 */
	s = nptr;
	do {
		c = *s++;
	} while (str_isspace(c));

	if (c == _T('-')) {
		neg = 1;
		c = *s++;
	} else {
		neg = 0;
		if (c == _T('+'))
			c = *s++;
	}

	if ((radix == 0 || radix == 16) &&
	    c == _T('0') && (*s == _T('x') || *s == _T('X'))) {
		c = s[1];
		s += 2;
		radix = 16;
	}

	if (radix == 0)
		radix = c == _T('0') ? 8 : 10;

	acc = any = 0;

	if (radix < 2 || radix > 36) {
		errno = EINVAL;
		if (endptr != NULL)
			*endptr = (CWT_CHAR *)(any ? s - 1 : nptr);
		return acc;
	}

	if( is_unsigned ) {
		cutoff = CWT_ULONGLONG_MAX / radix;
		cutlim = CWT_ULONGLONG_MAX % radix;
	} else {
		cutoff = neg
			? (ULONGLONG)-(CWT_LONGLONG_MIN + CWT_LONGLONG_MAX) + CWT_LONGLONG_MAX
			: CWT_LONGLONG_MAX;
		cutlim = cutoff % radix;
		cutoff /= radix;
	}

	for ( ; ; c = *s++) {
		if (c >= _T('0') && c <= _T('9'))
			c -= _T('0');
		else if (c >= _T('A') && c <= _T('Z'))
			c -= _T('A') - 10;
		else if (c >= _T('a') && c <= _T('z'))
			c -= _T('a') - 10;
		else
			break;

		if (c >= radix)
			break;
		if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
			any = -1;
		else {
			any = 1;
			acc *= radix;
			acc += c;
		}
	}

	if (any < 0) {
		if( is_unsigned ) {
			acc = CWT_ULONGLONG_MAX;
		} else {
			acc = neg ? CWT_LONGLONG_MIN : CWT_LONGLONG_MAX;
		}
		errno = ERANGE;
	} else if (!any) {
		errno = EINVAL;
	} else if (neg) {
		acc = -acc;
	}

	if (endptr != NULL)
		*endptr = (CWT_CHAR *)(any ? s - 1 : nptr);

	return acc;
}

LONGLONG  str_strtoll (const CWT_CHAR *nptr, CWT_CHAR **endptr, int radix)
{
	return (LONGLONG)__strtoull_helper (nptr, endptr, radix, FALSE);
}

ULONGLONG str_strtoull(const CWT_CHAR *nptr, CWT_CHAR **endptr, int radix)
{
	return __strtoull_helper (nptr, endptr, radix, TRUE);
}
