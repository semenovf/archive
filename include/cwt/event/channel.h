/*
 * event/channel.h
 *
 *  Created on: 25.06.2012
 *      Author: wladt
 */

#ifndef __CWT_EVENT_CHANNEL_H__
#define __CWT_EVENT_CHANNEL_H__

#include <cwt/event/types.h>
#include <cwt/io/channel.h>

typedef struct _CwtEventChannelNS {
	CwtEventSource* (*source)        (void);
	void            (*addListener)   (CwtChannel *pchan, BOOL (*reader)(CwtEventPtr));
	void 			(*removeListener)(CwtChannel *pchan);
	void 			(*peekChannel)   (CwtEvent *pevt, CwtChannel **pchan);
} CwtEventChannelNS;


EXTERN_C_BEGIN
DLL_API_EXPORT CwtEventChannelNS* cwtEventChannelNS(void);
EXTERN_C_END

#endif /* __CWT_EVENT_CHANNEL_H__ */
