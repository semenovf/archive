/*
 * cwt/event/clock.h
 *
 *  Created on: 24.06.2012
 *      Author: wladt
 */

#ifndef __CWT_EVENT_CLOCK_H__
#define __CWT_EVENT_CLOCK_H__

#include <cwt/event/types.h>

EXTERN_C_BEGIN

DLL_API_EXPORT CwtEventSourcePtr cwtEventClockSource(void);
DLL_API_EXPORT void cwtEventClockRefresh(void);
DLL_API_EXPORT CwtStack* cwtEventClockTimeHandler(void);
DLL_API_EXPORT CwtStack* cwtEventClockDateHandler(void);
DLL_API_EXPORT void cwtEventPeekClockTime(CwtEventPtr pevt, int *phour, int *pmin, int *psec);
DLL_API_EXPORT void cwtEventPeekClockDate(CwtEventPtr pevt, int *pday, int *pmon, int *pyear);
DLL_API_EXPORT void cwtEventPostClockTime(int hour, int min, int sec);
DLL_API_EXPORT void cwtEventPostClockDate(int day, int mon, int year);

EXTERN_C_END

#endif /* __CWT_EVENT_CLOCK_H__ */
