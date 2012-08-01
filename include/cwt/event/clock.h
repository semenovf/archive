/*
 * cwt/event/clock.h
 *
 *  Created on: 24.06.2012
 *      Author: wladt
 */

#ifndef __CWT_EVENT_CLOCK_H__
#define __CWT_EVENT_CLOCK_H__

#include <cwt/event/types.h>
#include <cwt/list.h>


typedef struct _CwtEventClockNS {
	CwtEventSource* (*source)        (void);

	void     (*refresh)(void);
	CwtList* (*timeHandler)(void);
	CwtList* (*dateHandler)(void);
	void     (*peekTime)(CwtEvent *pevt, int *phour, int *pmin, int *psec);
	void 	 (*peekDate)(CwtEvent *pevt, int *pday, int *pmon, int *pyear);
	void     (*postTime)(int hour, int min, int sec);
	void     (*postDate)(int day, int mon, int year);
} CwtEventClockNS;


EXTERN_C_BEGIN
DLL_API_EXPORT CwtEventClockNS* cwtEventClockNS(void);
EXTERN_C_END


#endif /* __CWT_EVENT_CLOCK_H__ */
