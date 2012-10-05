/*
 * app.h
 *
 *  Created on: Oct 5, 2012
 *      Author: wladt
 */

#ifndef __CWT_APP_H__
#define __CWT_APP_H__

#include <cwt/types.h>
#include <cwt/event/event.h>



typedef enum _CwtEventType {
	  Cwt_Event_Kbd         = 0x0001
	, Cwt_Event_PosixSignal = 0x0002
	, Cwt_Event_Clock       = 0x0004
	, Cwt_Event_Timer       = 0x0008
	, Cwt_Event_Channel     = 0x0010
};

typedef struct _CwtApp {
	void (*main)(void);
	BOOL (*selectEvents)(UINT acceptEvents);
	BOOL (*registerEventSource)(UINT acceptEvents);
} CwtApp;

EXTERN_C_BEGIN
DLL_API_EXPORT CwtApp* cwtApp(void);
EXTERN_C_END

#endif /* __CWT_APP_H__ */
