/*
 * cwt/event/types.h
 *
 *  Created on: 24.06.2012
 *      Author: wladt
 */

#ifndef __CWT_EVENT_TYPES_H__
#define __CWT_EVENT_TYPES_H__

#include <cwt/global.h>
#include <cwt/types.h>
#include <cwt/stack.h>

#ifdef __COMMENT__
enum CwtEventType {
/* 0*/	  CwtEventType_NoEevnt
/* 1*/	, CwtEventType_Quit
/* 2*/ 	, CWT_EVT_TIMER
/* 3*/	, CwtEventType_ClockTime
/* 4*/	, CwtEventType_ClockDate
/* 5*/	, CWT_EVT_KEYBOARD
/* 6*/	, CWT_EVT_MOUSE_PRESS
/* 7*/	, CWT_EVT_MOUSE_RELEASE
/* 8*/ 	, CWT_EVT_MOUSE_MOVE
/* 9*/ 	, CWT_EVT_CHANNEL
/*10*/ 	, CWT_EVT_WIDGET_PAINT  /* ev_data is widget object */
/*11*/ 	, CWT_EVT_WIDGET_DELETE /* deferred delete of widget */
/*12*/	, CWT_EVT_WIDGET_ENTER  /* mouse enters widget's boundaries */
/*13*/ 	, CWT_EVT_WIDGET_LEAVE  /*  */
/*14*/	, CWT_EVT_INFOCUS
/*15*/	, CWT_EVT_OUTFOCUS
/*16*/	, CWT_EVT_CHILD
/*17*/	, CWT_EVT_WINDOW_ACTIVATE

/*xx*/	, CWT_EVT_USER
};
typedef enum CwtEventType CwtEventType;
#endif


struct CwtEvent {
	CwtStack *handler_stack;
	void (*destructor)(struct CwtEvent*);
};
typedef struct CwtEvent CwtEvent;
typedef struct CwtEvent* CwtEventPtr;

struct CwtEventSource {
	 void (*poll)(void);
};
typedef struct CwtEventSource CwtEventSource;
typedef struct CwtEventSource* CwtEventSourcePtr;


typedef BOOL (*CwtEventHandler)(CwtEventPtr);


#endif /* __CWT_EVENT_TYPES_H__ */
