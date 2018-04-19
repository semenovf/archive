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

typedef struct _CwtApp {
	void (*main)(void);
	/*BOOL (*selectEvents)(UINT acceptEvents);*/
	BOOL (*registerEventSource)(CwtEventSource*);
} CwtApp;

EXTERN_C_BEGIN
DLL_API_EXPORT CwtApp* cwtApp(void);
EXTERN_C_END

#endif /* __CWT_APP_H__ */
