/*
 * locale.c
 *
 *  Created on: 10.08.2012
 *      Author: user
 */


#include <cwt/locale.h>
#include <locale.h>
/*
#ifdef CWT_OS_UNIX
#include <sys/localedef.h>
#endif
*/


static const CWT_CHAR* __loc_timeFormat(void);
static const CWT_CHAR* __loc_dateFormat(void);
static const CWT_CHAR* __loc_dateTimeFormat(void);
static const CWT_CHAR* __loc_amPmTimeFormat(void);
static const CWT_CHAR* const* __loc_weekDays(void);
static const CWT_CHAR* const* __loc_weekDaysAbr(void);
static const CWT_CHAR* const* __loc_months(void);
static const CWT_CHAR* const* __loc_monthsAbr(void);
static const CWT_CHAR* const* __loc_amPm(void);



static CwtLocaleNS __cwtLocaleNS = {
	  __loc_timeFormat
	, __loc_dateFormat
	, __loc_dateTimeFormat
	, __loc_amPmTimeFormat
	, __loc_weekDays
	, __loc_weekDaysAbr
	, __loc_months
	, __loc_monthsAbr
	, __loc_amPm
};


DLL_API_EXPORT CwtLocaleNS *cwtLocaleNS(void)
{
	return &__cwtLocaleNS;
}

#ifdef CWT_NO_LOCALE
#	include "locale_p/default.c"
#elif defined(CWT_OS_WIN)
#	include "locale_p/default.c"
/*#	include "locale_p/win.c" *//*TODO mus be implemented */
#elif defined(CWT_OS_UNIX)
#	include "locale_p/default.c"
/*#	include "locale_p/unix.c" *//*TODO mus be implemented */
#else
#	include "locale_p/default.c"
#endif

