/*
 * strptime.c
 *
 *  Created on: 10.08.2012
 *      Author: user
 */


/*	$NetBSD: strptime.c,v 1.36 2012/03/13 21:13:48 christos Exp $	*/

/*-
 * Copyright (c) 1997, 1998, 2005, 2008 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code was contributed to The NetBSD Foundation by Klaus Klein.
 * Heavily optimised by David Laight
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*#include <sys/cdefs.h>*/
/*

#if defined(LIBC_SCCS) && !defined(lint)
__RCSID("$NetBSD: strptime.c,v 1.36 2012/03/13 21:13:48 christos Exp $");
#endif
*/

/*#include <sys/localedef.h>*/
/*
#include <ctype.h>
#include <locale.h>
#include <string.h>
#include <tzfile.h>
#include "private.h"
*/
#include <cwt/locale.h>
#include <cwt/str.h>
#include <cwt/txtcodec.h>
#include <time.h>

#ifndef CWT_CC_MSC
extern char* tzname[2];
#endif

#define TM_YEAR_BASE	1900

static CwtStrNS    *__strNS = NULL;
static CwtLocaleNS *__locNS = NULL;

/*
#ifdef __weak_alias
__weak_alias(strptime,_strptime)
#endif
*/

/*#define	_ctloc(x)		(_CurrentTimeLocale->x)*/

/*
 * We do not implement alternate representations. However, we always
 * check whether a given modifier is allowed for a certain conversion.
 */
#define ALT_E			0x01
#define ALT_O			0x02
#define	LEGAL_ALT(x)	{ if (alt_format & ~(x)) return NULL; }

static CWT_CHAR __gmt[] = { _T("GMT") };
static CWT_CHAR __utc[] = { _T("UTC") };

/* RFC-822/RFC-2822 */
static const CWT_CHAR* const nast[5] = {
       _T("EST"),    _T("CST"),    _T("MST"),    _T("PST"),    _T("\0\0\0")
};
static const CWT_CHAR* const nadt[5] = {
       _T("EDT"),    _T("CDT"),    _T("MDT"),    _T("PDT"),    _T("\0\0\0")
};

static const CWT_UCHAR *__conv_num(const CWT_UCHAR*, int *, UINT, UINT);
static const CWT_UCHAR *__find_string(const CWT_UCHAR*, int *, const CWT_CHAR* const*, const CWT_CHAR* const*, int);
static const CWT_CHAR* const* __cwt_tzname(void);


CWT_CHAR* __cwt_strptime(const CWT_CHAR *buf, const CWT_CHAR *fmt, struct tm *tm)
{
	CWT_UCHAR c;
	const CWT_UCHAR *bp, *ep;
	int alt_format, i, split_year = 0, neg = 0, offs;
	const CWT_CHAR *new_fmt;

	if( !__strNS )
		__strNS = cwtStrNS();

	if( !__locNS )
		__locNS = cwtLocaleNS();

	bp = (const CWT_UCHAR *)buf;

	while (bp != NULL && (c = *fmt++) != _T('\0')) {
		/* Clear `alternate' modifier prior to new conversion. */
		alt_format = 0;
		i = 0;

		/* Eat up white-space. */
		if( __strNS->isspace(c) ) {
			while( __strNS->isspace(*bp) )
				bp++;
			continue;
		}

		if (c != _T('%'))
			goto literal;


again:		switch (c = *fmt++) {
		case _T('%'):	/* "%%" is converted to "%". */
literal:
			if (c != *bp++)
				return NULL;
			LEGAL_ALT(0);
			continue;

		/*
		 * "Alternative" modifiers. Just set the appropriate flag
		 * and start over again.
		 */
		case _T('E'):	/* "%E?" alternative conversion modifier. */
			LEGAL_ALT(0);
			alt_format |= ALT_E;
			goto again;

		case _T('O'):	/* "%O?" alternative conversion modifier. */
			LEGAL_ALT(0);
			alt_format |= ALT_O;
			goto again;

		/*
		 * "Complex" conversion rules, implemented through recursion.
		 */
		case _T('c'):	/* Date and time, using the locale's format. */
			new_fmt = __locNS->dateTimeFormat()/*_ctloc(d_t_fmt)*/;
			goto recurse;

		case _T('D'):	/* The date as "%m/%d/%y". */
			new_fmt = _T("%m/%d/%y");
			LEGAL_ALT(0);
			goto recurse;

		case _T('F'):	/* The date as "%Y-%m-%d". */
			new_fmt = _T("%Y-%m-%d");
			LEGAL_ALT(0);
			goto recurse;

		case _T('R'):	/* The time as "%H:%M". */
			new_fmt = _T("%H:%M");
			LEGAL_ALT(0);
			goto recurse;

		case _T('r'):	/* The time in 12-hour clock representation. */
			new_fmt = __locNS->amPmTimeFormat(); /*_ctloc(t_fmt_ampm);*/
			LEGAL_ALT(0);
			goto recurse;

		case _T('T'):	/* The time as "%H:%M:%S". */
			new_fmt = _T("%H:%M:%S");
			LEGAL_ALT(0);
			goto recurse;

		case _T('X'):	/* The time, using the locale's format. */
			new_fmt = __locNS->timeFormat();/* ctloc(t_fmt);*/
			goto recurse;

		case _T('x'):	/* The date, using the locale's format. */
			new_fmt = __locNS->dateFormat(); /*_ctloc(d_fmt);*/
		    recurse:
			bp = (const CWT_UCHAR*)__cwt_strptime((const CWT_CHAR*)bp, new_fmt, tm);
			LEGAL_ALT(ALT_E);
			continue;

		/*
		 * "Elementary" conversion rules.
		 */
		case _T('A'):	/* The day of week, using the locale's form. */
		case _T('a'):
			bp = __find_string(bp, &tm->tm_wday, __locNS->weekDays()/*_ctloc(day)*/, __locNS->weekDaysAbr()/*_ctloc(abday)*/, 7);
			LEGAL_ALT(0);
			continue;

		case _T('B'):	/* The month, using the locale's form. */
		case _T('b'):
		case _T('h'):
			bp = __find_string(bp, &tm->tm_mon, __locNS->months()/*_ctloc(mon)*/, __locNS->monthsAbr()/*_ctloc(abmon)*/, 12);
			LEGAL_ALT(0);
			continue;

		case _T('C'):	/* The century number. */
			i = 20;
			bp = __conv_num(bp, &i, 0, 99);

			i = i * 100 - TM_YEAR_BASE;
			if (split_year)
				i += tm->tm_year % 100;
			split_year = 1;
			tm->tm_year = i;
			LEGAL_ALT(ALT_E);
			continue;

		case _T('d'):	/* The day of month. */
		case _T('e'):
			bp = __conv_num(bp, &tm->tm_mday, 1, 31);
			LEGAL_ALT(ALT_O);
			continue;

		case _T('k'):	/* The hour (24-hour clock representation). */
			LEGAL_ALT(0);
			break;
			/* FALLTHROUGH */
		case _T('H'):
			bp = __conv_num(bp, &tm->tm_hour, 0, 23);
			LEGAL_ALT(ALT_O);
			continue;

		case _T('l'):	/* The hour (12-hour clock representation). */
			LEGAL_ALT(0);
			break;
			/* FALLTHROUGH */
		case _T('I'):
			bp = __conv_num(bp, &tm->tm_hour, 1, 12);
			if (tm->tm_hour == 12)
				tm->tm_hour = 0;
			LEGAL_ALT(ALT_O);
			continue;

		case _T('j'):	/* The day of year. */
			i = 1;
			bp = __conv_num(bp, &i, 1, 366);
			tm->tm_yday = i - 1;
			LEGAL_ALT(0);
			continue;

		case _T('M'):	/* The minute. */
			bp = __conv_num(bp, &tm->tm_min, 0, 59);
			LEGAL_ALT(ALT_O);
			continue;

		case _T('m'):	/* The month. */
			i = 1;
			bp = __conv_num(bp, &i, 1, 12);
			tm->tm_mon = i - 1;
			LEGAL_ALT(ALT_O);
			continue;

		case _T('p'):	/* The locale's equivalent of AM/PM. */
			bp = __find_string(bp, &i, __locNS->amPm() /*_ctloc(am_pm)*/, NULL, 2);
			if (tm->tm_hour > 11)
				return NULL;
			tm->tm_hour += i * 12;
			LEGAL_ALT(0);
			continue;

		case _T('S'):	/* The seconds. */
			bp = __conv_num(bp, &tm->tm_sec, 0, 61);
			LEGAL_ALT(ALT_O);
			continue;

#ifndef TIME_MAX
#define TIME_MAX	CWT_UINT64_MAX
#endif
		case _T('s'):	/* seconds since the epoch */
			{
				time_t sse = 0;
				UINT64 rulim = TIME_MAX;

				if (*bp < _T('0') || *bp > _T('9')) {
					bp = NULL;
					continue;
				}

				do {
					sse *= 10;
					sse += *bp++ - _T('0');
					rulim /= 10;
				} while ((sse * 10 <= TIME_MAX) &&
					 rulim && *bp >= _T('0') && *bp <= _T('9'));

				if (sse < 0 || (UINT64)sse > TIME_MAX) {
					bp = NULL;
					continue;
				}
#ifdef CWT_CC_MSC
				if( localtime_s(tm, &sse) != 0 )
#else
				if( localtime_r(&sse, tm) == NULL )
#endif
					bp = NULL;
			}
			continue;

		case _T('U'):	/* The week of year, beginning on sunday. */
		case _T('W'):	/* The week of year, beginning on monday. */
			/*
			 * XXX This is bogus, as we can not assume any valid
			 * information present in the tm structure at this
			 * point to calculate a real value, so just check the
			 * range for now.
			 */
			 bp = __conv_num(bp, &i, 0, 53);
			 LEGAL_ALT(ALT_O);
			 continue;

		case _T('w'):	/* The day of week, beginning on sunday. */
			bp = __conv_num(bp, &tm->tm_wday, 0, 6);
			LEGAL_ALT(ALT_O);
			continue;

		case _T('u'):	/* The day of week, monday = 1. */
			bp = __conv_num(bp, &i, 1, 7);
			tm->tm_wday = i % 7;
			LEGAL_ALT(ALT_O);
			continue;

		case _T('g'):	/* The year corresponding to the ISO week
				 * number but without the century.
				 */
			bp = __conv_num(bp, &i, 0, 99);
			continue;

		case _T('G'):	/* The year corresponding to the ISO week
				 * number with century.
				 */
			do
				bp++;
			while (__strNS->isdigit(*bp));
			continue;

		case _T('V'):	/* The ISO 8601:1988 week number as decimal */
			bp = __conv_num(bp, &i, 0, 53);
			continue;

		case _T('Y'):	/* The year. */
			i = TM_YEAR_BASE;	/* just for data sanity... */
			bp = __conv_num(bp, &i, 0, 9999);
			tm->tm_year = i - TM_YEAR_BASE;
			LEGAL_ALT(ALT_E);
			continue;

		case _T('y'):	/* The year within 100 years of the epoch. */
			/* LEGAL_ALT(ALT_E | ALT_O); */
			bp = __conv_num(bp, &i, 0, 99);

			if (split_year)
				/* preserve century */
				i += (tm->tm_year / 100) * 100;
			else {
				split_year = 1;
				if (i <= 68)
					i = i + 2000 - TM_YEAR_BASE;
				else
					i = i + 1900 - TM_YEAR_BASE;
			}
			tm->tm_year = i;
			continue;

		case _T('Z'):
			CWT_ISO_CPP_NAME(tzset)();
			if( __strNS->strncmp((const CWT_CHAR*)bp, __gmt, 3) == 0) {
				tm->tm_isdst = 0;
#ifdef TM_GMTOFF
				tm->TM_GMTOFF = 0;
#endif
#ifdef TM_ZONE
				tm->TM_ZONE = __gmt;
#endif
				bp += 3;
			} else {
				ep = __find_string(bp, &i, (const CWT_CHAR* const*)__cwt_tzname, NULL, 2);
				if (ep != NULL) {
					tm->tm_isdst = i;
#ifdef TM_GMTOFF
					tm->TM_GMTOFF = -(timezone);
#endif
#ifdef TM_ZONE
					tm->TM_ZONE = tzname[i];
#endif
				}
				bp = ep;
			}
			continue;

		case _T('z'):
			/*
			 * We recognize all ISO 8601 formats:
			 * Z	= Zulu time/UTC
			 * [+-]hhmm
			 * [+-]hh:mm
			 * [+-]hh
			 * We recognize all RFC-822/RFC-2822 formats:
			 * UT|GMT
			 *          North American : UTC offsets
			 * E[DS]T = Eastern : -4 | -5
			 * C[DS]T = Central : -5 | -6
			 * M[DS]T = Mountain: -6 | -7
			 * P[DS]T = Pacific : -7 | -8
			 *          Military
			 * [A-IL-M] = -1 ... -9 (J not used)
			 * [N-Y]  = +1 ... +12
			 */
			while( __strNS->isspace(*bp) )
				bp++;

			switch (*bp++) {
			case _T('G'):
				if (*bp++ != _T('M'))
					return NULL;
				/*FALLTHROUGH*/
			case _T('U'):
				if (*bp++ != _T('T'))
					return NULL;
				/*FALLTHROUGH*/
			case _T('Z'):
				tm->tm_isdst = 0;
#ifdef TM_GMTOFF
				tm->TM_GMTOFF = 0;
#endif
#ifdef TM_ZONE
				tm->TM_ZONE = __utc;
#endif
				continue;
			case _T('+'):
				neg = 0;
				break;
			case _T('-'):
				neg = 1;
				break;
			default:
				--bp;
				ep = __find_string(bp, &i, nast, NULL, 4);
				if (ep != NULL) {
#ifdef TM_GMTOFF
					tm->TM_GMTOFF = -5 - i;
#endif
#ifdef TM_ZONE
					tm->TM_ZONE = __UNCONST(nast[i]);
#endif
					bp = ep;
					continue;
				}
				ep = __find_string(bp, &i, nadt, NULL, 4);
				if (ep != NULL) {
					tm->tm_isdst = 1;
#ifdef TM_GMTOFF
					tm->TM_GMTOFF = -4 - i;
#endif
#ifdef TM_ZONE
					tm->TM_ZONE = __UNCONST(nadt[i]);
#endif
					bp = ep;
					continue;
				}

				if ((*bp >= _T('A') && *bp <= _T('I')) ||
				    (*bp >= _T('L') && *bp <= _T('Y'))) {
#ifdef TM_GMTOFF
					/* Argh! No 'J'! */
					if (*bp >= _T('A') && *bp <= _T('I'))
						tm->TM_GMTOFF =
						    (_T('A') - 1) - (int)*bp;
					else if (*bp >= _T('L') && *bp <= _T('M'))
						tm->TM_GMTOFF = _T('A') - (int)*bp;
					else if (*bp >= _T('N') && *bp <= _T('Y'))
						tm->TM_GMTOFF = (int)*bp - _T('M');
#endif
#ifdef TM_ZONE
					tm->TM_ZONE = NULL; /* XXX */
#endif
					bp++;
					continue;
				}
				return NULL;
			}
			offs = 0;
			for (i = 0; i < 4; ) {
				if( __strNS->isdigit(*bp) ) {
					offs = offs * 10 + (*bp++ - _T('0'));
					i++;
					continue;
				}
				if (i == 2 && *bp == _T(':')) {
					bp++;
					continue;
				}
				break;
			}
			switch (i) {
			case 2:
				offs *= 100;
				break;
			case 4:
				i = offs % 100;
				if (i >= 60)
					return NULL;
				/* Convert minutes into decimal */
				offs = (offs / 100) * 100 + (i * 50) / 30;
				break;
			default:
				return NULL;
			}
			if (neg)
				offs = -offs;
			tm->tm_isdst = 0;	/* XXX */
#ifdef TM_GMTOFF
			tm->TM_GMTOFF = offs;
#endif
#ifdef TM_ZONE
			tm->TM_ZONE = NULL;	/* XXX */
#endif
			continue;

		/*
		 * Miscellaneous conversions.
		 */
		case _T('n'):	/* Any kind of white-space. */
		case _T('t'):
			while (__strNS->isspace(*bp))
				bp++;
			LEGAL_ALT(0);
			continue;


		default:	/* Unknown/unsupported conversion. */
			return NULL;
		}
	}

	return (CWT_CHAR*)bp;
}


static const CWT_UCHAR* __conv_num(const CWT_CHAR *buf, int *dest, UINT llim, UINT ulim)
{
	UINT result = 0;
	CWT_UCHAR ch;

	/* The limit also determines the number of valid digits. */
	UINT rulim = ulim;

	ch = *buf;
	if (ch < _T('0') || ch > _T('9'))
		return NULL;

	do {
		result *= 10;
		result += ch - _T('0');
		rulim /= 10;
		ch = *++buf;
	} while ((result * 10 <= ulim) && rulim && ch >= _T('0') && ch <= _T('9'));

	if (result < llim || result > ulim)
		return NULL;

	*dest = result;
	return buf;
}

static const CWT_UCHAR* __find_string(const CWT_CHAR *bp, int *tgt, const CWT_CHAR* const *n1,
		const CWT_CHAR* const *n2, int c)
{
	int i;
	size_t len;

	/* check full name - then abbreviated ones */
	for (; n1 != NULL; n1 = n2, n2 = NULL) {
		for (i = 0; i < c; i++, n1++) {
			len = __strNS->strlen(*n1);
			if (__strNS->strnicmp(*n1, (const CWT_CHAR*)bp, len) == 0) {
				*tgt = i;
				return bp + len;
			}
		}
	}

	/* Nothing matched */
	return NULL;
}


static const CWT_CHAR* const* __cwt_tzname(void)
{
#ifdef CWT_UNICODE
	CwtTextCodecNS *tcNS = cwtTextCodecNS();
	static CWT_CHAR* __cwt_tznames[2] = { NULL, NULL };
	static BOOL __cwt_tznames_is_set = FALSE;

	if( !__cwt_tznames_is_set ) {
#	ifndef CWT_CC_MSC
		extern char* tzname[2];
#	endif
		if( tzname[0] ) {
			__cwt_tznames[0] = tcNS->fromLatin1(tzname[0]);
		}
		if( tzname[1] ) {
			__cwt_tznames[1] = tcNS->fromLatin1(tzname[1]);
		}
		__cwt_tznames_is_set = TRUE;
	}
	return (const CWT_CHAR* const*)__cwt_tznames;
#else
#	ifdef CWT_CC_MSC
	return _tzname;
#	else
	extern char* tzname[2];
	return (const CWT_CHAR* const*)tzname;
#	endif
#endif
}

