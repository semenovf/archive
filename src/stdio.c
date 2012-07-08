/*
 * string.c
 *
 *  Created on: 18.06.2012
 *      Author: wladt
 */


#include <cwt/stdio.h>

static CwtStdioNS __cwtStdioNS = {
#ifdef CWT_UNICODE
	  wprintf
	, wsprintf
	, fwprintf

     /* Note: warning C4996: '_vswprintf': swprintf has been changed to conform with the ISO C standard,
	  * adding an extra character count parameter. To use traditional Microsoft swprintf,
	  * set _CRT_NON_CONFORMING_SWPRINTFS (before including <stdio.h>).
	  * Need for compatibly with vsprintf for single-byte characters */
	, _vswprintf /* vswprintf defers for C and C++ */
	#ifdef CWT_CC_MSC
	#	define _CRT_NON_CONFORMING_SWPRINTFS 1
	#endif

#else
	  printf
	, sprintf
	, fprintf
	, vsprintf
#endif
};


DLL_API_EXPORT CwtStdioNS* cwtStdioNS(void)
{
	return &__cwtStdioNS;
}


