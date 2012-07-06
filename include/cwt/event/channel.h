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

EXTERN_C_BEGIN

DLL_API_EXPORT CwtEventSourcePtr cwtEventChannelSource(void);
DLL_API_EXPORT void cwtEventChannelAddListener(CwtChannelPtr chan, BOOL (*reader)(CwtEventPtr));
DLL_API_EXPORT void cwtEventChannelRemoveListener(CwtChannelPtr chan);
DLL_API_EXPORT void cwtEventPeekChannel(CwtEventPtr pevt, CwtChannelPtr *pchan);

EXTERN_C_END



#endif /* __CWT_EVENT_CHANNEL_H__ */
