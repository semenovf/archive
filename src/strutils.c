/*
 * strutils.c
 *
 *  Created on: 10.01.2012
 *      Author: wladt
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <cwt/string.h>
#include <cwt/strbuf.h>

#define _cwtStrtoul strtoul
#define _cwtStrtol  strtol

#define __CWT_STR_TO_LONGTYPE(_LongType,_strToLongFunc,_maxLong,_minLong) \
DLL_API_EXPORT _LongType __to##_LongType(const CWT_CHAR *str, int radix, BOOL *ok) { \
	_LongType val;                                                            \
	CWT_CHAR *endptr;                                                         \
	CwtStringNS* stringNS = cwtStringNS();                                    \
																		      \
	if( radix <= 0 ) {                                                        \
		if( stringNS->strncmp(_T("0x"), str, 2) == 0 ) {                      \
			radix = 16;                                                       \
			str += 2;                                                         \
		} else if( stringNS->strncmp(_T("0"), str, 1) == 0 ) {                \
			radix = 8;                                                        \
			str++;                                                            \
		}                                                                     \
	}                                                                         \
	                                                                          \
   /* Need to reset errno because it will not reset */                        \
   /* in the subsequent call if error will not occurred */                    \
	errno = 0;                                                                \
	val = _strToLongFunc(str, &endptr, radix);                                \
																		      \
	if((errno == ERANGE	&& (val == _maxLong || val == _minLong))		      \
            || (errno != 0 && val == (_LongType)0)						      \
            || endptr == str												  \
            || *endptr != (CWT_CHAR)0 ) {	     							  \
																              \
		if( ok ) {														      \
			*ok = FALSE;													  \
		}														              \
		val = (_LongType)0;											          \
     } else {														          \
 		if( ok ) {														      \
 			*ok = TRUE;														  \
 		}														              \
     }														                  \
																		      \
	return val;                                                               \
}


#define __CWT_STR_TO_INTTYPE(_IntType,_LongType,_maxInt,_minInt)              \
DLL_API_EXPORT _IntType __to##_IntType(const CWT_CHAR *str, int radix, BOOL *ok) {\
	BOOL okk = TRUE;                                                          \
	CwtStringNS* stringNS = cwtStringNS();                                    \
	_LongType val = stringNS->to##_LongType(str, radix, &okk);                \
                                                                              \
	if( !okk || val > _maxInt || val < _minInt ) {                            \
		okk = FALSE;                                                          \
	}                                                                         \
                                                                              \
	if( ok )                                                                  \
		*ok = okk;                                                            \
                                                                              \
	return (_IntType)val;                                                     \
}



/*TODO remove this block */
#ifdef __COMMENT__
DLL_API_EXPORT LONG cwtStrToLONG_DEBUG(const CWT_CHAR *str, int radix, BOOL *ok)
{
	LONG val;
	CWT_CHAR *endptr;

	if( radix <= 0 ) {
		if( cwtStrNcmp("0x", str, 2) == 0 ) {
			radix = 16;
			str += 2;
		} else if( cwtStrNcmp("0", str, 1) == 0 ) {
			radix = 8;
			str++;
		}
	}

	val = _cwtStrtol(str, &endptr, radix);

	if((errno == ERANGE
			&& (val == CWT_LONG_MAX || val == CWT_LONG_MIN))
            || (errno != 0 && val == 0)
            || endptr == str
            || *endptr != '\0' ) {
		if( ok ) {
			*ok = FALSE;
		}
		val = 0L;
     } else {
 		if( ok ) {
 			*ok = TRUE;
 		}
     }


	return val;
}

DLL_API_EXPORT LONGLONG cwtStrToLONGLONG_DEBUG(const CWT_CHAR *str, int radix, BOOL *ok)
{
	LONGLONG val;
	CWT_CHAR *endptr;
	int errn;

	if( radix <= 0 ) {
		if( cwtStrNcmp("0x", str, 2) == 0 ) {
			radix = 16;
			str += 2;
		} else if( cwtStrNcmp("0", str, 1) == 0 ) {
			radix = 8;
			str++;
		}
	}

	val = _cwtStrtoll(str, &endptr, radix);

	errn = errno;

	if((errno == ERANGE
			&& (val == CWT_LONGLONG_MAX || val == CWT_LONGLONG_MIN))
            || (errno != 0 && val == (LONGLONG)0)
            || endptr == str
            || *endptr != '\0' ) {
		if( ok ) {
			*ok = FALSE;
		}
		val = (LONGLONG)0;
     } else {
 		if( ok ) {
 			*ok = TRUE;
 		}
     }


	return val;
}
#endif

__CWT_STR_TO_LONGTYPE(ULONGLONG, cwtStringNS()->strtoull, CWT_ULONGLONG_MAX, 0LL)
__CWT_STR_TO_LONGTYPE(LONGLONG, cwtStringNS()->strtoll, CWT_LONGLONG_MAX, CWT_LONGLONG_MIN)
__CWT_STR_TO_LONGTYPE(ULONG, cwtStringNS()->strtoul, CWT_ULONG_MAX, 0L)
__CWT_STR_TO_LONGTYPE(LONG, cwtStringNS()->strtol, CWT_LONG_MAX, CWT_LONG_MIN)
__CWT_STR_TO_INTTYPE(INT, LONG, CWT_INT_MAX, CWT_INT_MIN)
__CWT_STR_TO_INTTYPE(UINT, ULONG, CWT_UINT_MAX, 0)
__CWT_STR_TO_INTTYPE(SHORT, LONG, CWT_SHORT_MAX, CWT_SHORT_MIN)
__CWT_STR_TO_INTTYPE(USHORT, ULONG, CWT_USHORT_MAX, 0);
__CWT_STR_TO_INTTYPE(SBYTE, LONG, CWT_SBYTE_MAX, CWT_SBYTE_MIN)
__CWT_STR_TO_INTTYPE(BYTE, ULONG, CWT_BYTE_MAX, 0);


#if 0
#ifndef __DJGPP__
/**
 *
 * @param s
 * @param max
 * @param format
 * @param tm
 * @return
 *
 * Supported conversion specifications:
 * %C The century number (year/100) as a 2-digit integer. (SU)
 * %d The day of the month as a decimal number (range 01 to 31).
 * %m The month as a decimal number (range 01 to 12).
 * %Y The year as a decimal number including the century.
 * %y The year as a decimal number without a century (range 00 to 99).
 * %H The hour as a decimal number using a 24-hour clock (range 00 to 23).
 * %M The minute as a decimal number (range 00 to 59).
 * %S The second as a decimal number (range 00 to 59).
 * */
#define _ST_CHAR       0
#define _ST_SPEC_CHAR  1
size_t strftime(CWT_CHAR *s, size_t max, const CWT_CHAR *format, const struct tm *tm)
{
	const CWT_CHAR *ptrf = format;
	size_t sz = 0;
	int state  = _ST_CHAR;
	CWT_CHAR chars[16];
	StringBufferPtr sb;

	sb = strbuf_new_defaults();

	while(ptrf != '\x0') {
		if( sz >= max-1 ) {
			sz = (size_t)0;
			break;
		}

		switch( state ) {
			case _ST_CHAR:
				if( *ptrf == '%' ) {
					state = _ST_SPEC_CHAR;
				} else {
					strbuf_append_char(sb, *ptrf);
					sz++;
				}
				ptrf++;
				break;

			case _ST_SPEC_CHAR:
				switch( *ptrf ) {
					case '%':
						strbuf_append_char(sb, '%');
						sz++;
						break;
					case 'C':
						sprintf(chars, "%02d", (int)(tm->tm_year+1900)/100);
						strbuf_append(sb, chars);
						sz+=2;
						break;
					case 'd':
						sprintf(chars, "%02d", tm->tm_mday);
						strbuf_append(sb, chars);
						sz+=2;
						break;
					case 'm':
						sprintf(chars, "%02d", tm->tm_mon);
						strbuf_append(sb, chars);
						sz+=2;
						break;
					case 'Y':
						sprintf(chars, "%04d", tm->tm_year+1900);
						strbuf_append(sb, chars);
						sz+=4;
						break;
					case 'y':
						sprintf(chars, "%02d", (tm->tm_year+1900)%1000);
						strbuf_append(sb, chars);
						sz+=2;
						break;
					case 'H':
						sprintf(chars, "%02d", tm->tm_hour);
						strbuf_append(sb, chars);
						sz++;
						break;
					case 'M':
						sprintf(chars, "%02d", tm->tm_min);
						strbuf_append(sb, chars);
						sz++;
						break;
					case 'S':
						sprintf(chars, "%02d", tm->tm_sec);
						strbuf_append(sb, chars);
						sz++;
						break;
				}
				ptrf++;
				state = _ST_CHAR;
				break;

			default:
				JQ_ASSERT(*ptrf != *ptrf);
				break;
		}
	}

	if( sz != (size_t)0 ) {
		strcpy(s, strbuf_cstr(sb));
	}

	strbuf_delete(sb);

	return sz;
}
#endif
#endif
