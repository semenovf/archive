/*
 * string.c
 *
 *  Created on: 18.06.2012
 *      Author: wladt
 */


#include <cwt/stdio.h>

static int cwt_snprintf(CWT_CHAR *buffer, size_t count, const CWT_CHAR *format, ...);
extern int cwt_vsnprintf(CWT_CHAR *buffer, size_t count, const CWT_CHAR *format, va_list);

static CwtStdioNS __cwtStdioNS = {
#ifdef CWT_UNICODE
	  wprintf
	, fwprintf

     /* Note: warning C4996: '_vswprintf': swprintf has been changed to conform with the ISO C standard,
	  * adding an extra character count parameter. To use traditional Microsoft swprintf,
	  * set _CRT_NON_CONFORMING_SWPRINTFS (before including <stdio.h>).
	  * Need for compatibly with vsprintf for single-byte characters */
	/*, CWT_ISO_CPP_NAME(vswprintf) *//* vswprintf defers for C and C++ */
	#ifdef CWT_CC_MSC
	#	define _CRT_NON_CONFORMING_SWPRINTFS 1
	#endif
#else
	  printf
	, fprintf
	, vsprintf
#endif
	, cwt_snprintf
	, cwt_vsnprintf
};


DLL_API_EXPORT CwtStdioNS* cwt_stdio_ns(void)
{
	return &__cwtStdioNS;
}

/**
 * @brief Write formatted output using a pointer to a list of arguments.
 *
 * @param buffer Storage location for output.
 * @param count Maximum number of characters to store.
 * @param format Format specification.
 * @return The number of characters written,
 * 	not including the terminating null character,
 * 	or a negative value if an output error occurs.
 */
static int cwt_snprintf(CWT_CHAR *buffer, size_t count, const CWT_CHAR *format, ...)
{
	int rval;
	va_list args;

	va_start(args, format);
	rval = cwt_vsnprintf (buffer, count, format, args);
	va_end(args);

	return rval;
}
