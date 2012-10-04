/*
 * sockdev.h
 *
 *  Created on: 26.06.2012
 *      Author: wladt
 */

#ifndef __CWT_IO_SOCKETDEV_H__
#define __CWT_IO_SOCKETDEV_H__

#include <cwt/types.h>
#include <cwt/io/channel.h>
#include <cwt/net/socket.h>
#include <cwt/net/inet.h>

EXTERN_C_BEGIN
DLL_API_EXPORT CwtIODevice* cwtSocketDeviceOpen(CwtSocket *sockfd);
EXTERN_C_END

#endif /* __CWT_IO_SOCKETDEV_H__ */
