/*
 * bufdev.h
 *
 *  Created on: 01.03.2012
 *      Author: wladt
 */

#ifndef __CWT_IO_BUFDEV_H__
#define __CWT_IO_BUFDEV_H__

#include <cwt/io/channel.h>

EXTERN_C_BEGIN

DLL_API_EXPORT CwtIODevicePtr cwtBufferDeviceOpen(void);
DLL_API_EXPORT CwtIODevicePtr cwtBufferDeviceOpenPeer(CwtIODevicePtr bufd);

EXTERN_C_END

#endif /* __CWT_IO_BUFDEV_H__ */
