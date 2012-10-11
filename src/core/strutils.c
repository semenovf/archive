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
#include <math.h>
#include <cwt/str.h>
#include <cwt/string.h>

/*
#define _cwtStrtoul strtoul
#define _cwtStrtol  strtol
*/

#define __CWT_STR_TO_LONGTYPE(_LongType,_strToLongFunc,_maxLong,_minLong)     \
_LongType __cwt_to##_LongType(const CWT_CHAR *str, int radix, BOOL *ok) {     \
	_LongType val;                                                            \
	CWT_CHAR *endptr;                                                         \
	CwtStrNS* strNS = cwtStrNS();                                             \
																		      \
	if( radix <= 0 ) {                                                        \
		if( strNS->strNCmp(_T("0x"), str, 2) == 0 ) {                         \
			radix = 16;                                                       \
			str += 2;                                                         \
		} else if( strNS->strNCmp(_T("0"), str, 1) == 0 ) {                   \
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
_IntType __cwt_to##_IntType(const CWT_CHAR *str, int radix, BOOL *ok) {       \
	BOOL okk = TRUE;                                                          \
	CwtStrNS* strNS = cwtStrNS();                                             \
	_LongType val = strNS->to##_LongType(str, radix, &okk);                   \
	if( !okk || val > _maxInt || val < _minInt ) {                            \
		okk = FALSE;                                                          \
	}                                                                         \
                                                                              \
	if( ok )                                                                  \
		*ok = okk;                                                            \
                                                                              \
	return (_IntType)val;                                                     \
}

#define __CWT_STR_TO_UINTTYPE(_IntType,_LongType,_maxInt)                     \
_IntType __cwt_to##_IntType(const CWT_CHAR *str, int radix, BOOL *ok) {       \
	BOOL okk = TRUE;                                                          \
	CwtStrNS* strNS = cwtStrNS();                                             \
	_LongType val = strNS->to##_LongType(str, radix, &okk);                   \
	if( !okk || val > _maxInt ) {                                             \
		okk = FALSE;                                                          \
	}                                                                         \
                                                                              \
	if( ok )                                                                  \
		*ok = okk;                                                            \
                                                                              \
	return (_IntType)val;                                                     \
}


__CWT_STR_TO_LONGTYPE(ULONGLONG, cwtStrNS()->strToULLong, CWT_ULONGLONG_MAX, 0LL)
__CWT_STR_TO_LONGTYPE(LONGLONG, cwtStrNS()->strToLLONG, CWT_LONGLONG_MAX, CWT_LONGLONG_MIN)
__CWT_STR_TO_LONGTYPE(ULONG, cwtStrNS()->strToULong, CWT_ULONG_MAX, 0L)
__CWT_STR_TO_LONGTYPE(LONG, cwtStrNS()->strToLong, CWT_LONG_MAX, CWT_LONG_MIN)
__CWT_STR_TO_INTTYPE(INT, LONG, CWT_INT_MAX, CWT_INT_MIN)
__CWT_STR_TO_UINTTYPE(UINT, ULONG, CWT_UINT_MAX)
__CWT_STR_TO_INTTYPE(SHORT, LONG, CWT_SHORT_MAX, CWT_SHORT_MIN)
__CWT_STR_TO_UINTTYPE(USHORT, ULONG, CWT_USHORT_MAX);
__CWT_STR_TO_INTTYPE(SBYTE, LONG, CWT_SBYTE_MAX, CWT_SBYTE_MIN)
__CWT_STR_TO_UINTTYPE(BYTE, ULONG, CWT_BYTE_MAX);


double __cwt_toDouble(const CWT_CHAR *str, BOOL *ok)
{
	double val;
	CWT_CHAR *endptr;
	CwtStrNS* strNS = cwtStrNS();

   /* Need to reset errno because it will not reset */
   /* in the subsequent call if error will not occurred */
	errno = 0;
	val = strNS->strToDouble(str, &endptr);

	if((errno == ERANGE	&& (val == HUGE_VAL || val == -HUGE_VAL))
            || (errno != 0 && val == (double)0.0)
            || endptr == str
            || *endptr != (CWT_CHAR)0 ) {

		if( ok ) {
			*ok = FALSE;
		}
		val = (double)0.0;
     } else {
 		if( ok ) {
 			*ok = TRUE;
 		}
     }

	return val;
}

float __cwt_toFloat(const CWT_CHAR *str, BOOL *ok)
{
	BOOL okk = TRUE;
    double d = __cwt_toDouble(str, &okk);

    if(!okk || d > CWT_FLOAT_MAX || d < -CWT_FLOAT_MAX) {
        if( ok != 0 )
            *ok = FALSE;
        return 0.0;
    }
    if( ok != 0 )
        *ok = TRUE;

    return (float)d;

}
