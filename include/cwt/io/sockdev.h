/*
 * sockdev.h
 *
 *  Created on: 26.06.2012
 *      Author: wladt
 */

#ifndef __CWT_IO_SOCKETDEV_H__
#define __CWT_IO_SOCKETDEV_H__

#include <cwt/io/channel.h>
#include <cwt/net/inet.h>

#define CWT_MCSOCKET_SENDER   TRUE
#define CWT_MCSOCKET_RECEIVER FALSE

EXTERN_C_BEGIN

/* CwtIODevicePtr cwtLocalSocketDeviceOpen(void); */
DLL_API_EXPORT CwtIODevice* cwtUdpSocketDeviceOpen(const CWT_CHAR *inetAddr, UINT16 port, BOOL is_listener);
DLL_API_EXPORT CwtIODevice* cwtTcpSocketDeviceOpen(const CWT_CHAR *inetAddr, UINT16 port, BOOL is_listener);
DLL_API_EXPORT CwtIODevice* cwtMSocketDeviceOpen(const CWT_CHAR *inetAddr, UINT16 port, const CWT_CHAR *inetMCastAddr, BOOL is_listener);

EXTERN_C_END

#endif /* __CWT_IO_SOCKETDEV_H__ */
