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

/*
typedef enum _CwtSocketDeviceFlag {
	  Cwt_SocketDevice_Client
	, Cwt_SocketDevice_MSender = Cwt_SocketDevice_Client
	, Cwt_SocketDevice_Listener = 0x0001
	, Cwt_SocketDevice_MReceiver = Cwt_SocketDevice_Listener
	, Cwt_SocketDevice_NonBlocking = 0x0002
} CwtSocketDeviceFlag;
*/


EXTERN_C_BEGIN
DLL_API_EXPORT CwtIODevice* cwtSocketDeviceOpen(CwtSocket *sockfd);
/*
DLL_API_EXPORT CwtIODevice* cwtLocalSocketDeviceOpen(const CWT_CHAR *path, UINT32 flags);
DLL_API_EXPORT CwtIODevice* cwtUdpSocketDeviceOpen(const CWT_CHAR *inetAddr, UINT16 port, UINT32 flags);
DLL_API_EXPORT CwtIODevice* cwtTcpSocketDeviceOpen(const CWT_CHAR *inetAddr, UINT16 port, UINT32 flags);
DLL_API_EXPORT CwtIODevice* cwtMcastSocketDeviceOpen(const CWT_CHAR *inetAddr, UINT16 port, const CWT_CHAR *inetMcastAddr, UINT32 flags);
*/
EXTERN_C_END

#endif /* __CWT_IO_SOCKETDEV_H__ */
