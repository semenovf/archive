/*
 * locale.h
 *
 *  Created on: 10.08.2012
 *      Author: user
 */

#ifndef __CWT_LOCALE_H__
#define __CWT_LOCALE_H__

#include <cwt/types.h>

typedef struct _CwtLocaleNS {
	const CWT_CHAR* (*timeFormat)(void);
	const CWT_CHAR* (*dateFormat)(void);
	const CWT_CHAR* (*dateTimeFormat)(void);
	const CWT_CHAR* (*amPmTimeFormat)(void);
	const CWT_CHAR* const* (*weekDays)(void);
	const CWT_CHAR* const* (*weekDaysAbr)(void);
	const CWT_CHAR* const* (*months)(void);
	const CWT_CHAR* const* (*monthsAbr)(void);
	const CWT_CHAR* const* (*amPm)(void);
} CwtLocaleNS;

EXTERN_C_BEGIN
DLL_API_EXPORT CwtLocaleNS *cwtLocaleNS(void);
EXTERN_C_END


#endif /* __CWT_LOCALE_H__ */
