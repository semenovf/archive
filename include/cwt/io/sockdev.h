/*
 * sockdev.h
 *
 *  Created on: 26.06.2012
 *      Author: wladt
 */

#ifndef __CWT_IO_SOCKETDEV_H__
#define __CWT_IO_SOCKETDEV_H__

#include <cwt/io/channel.h>

typedef enum CwtSocketType {
	  Cwt_TcpSocket
	, Cwt_UdpSocket
	, Cwt_MCSocket_Sender
	, Cwt_MCSocket_Receiver
} CwtSocketType;

#define CWT_MCSOCKET_SENDER   TRUE
#define CWT_MCSOCKET_RECEIVER FALSE

EXTERN_C_BEGIN

/*
CwtIODevicePtr cwtLocalSocketDeviceOpen(void);
CwtIODevicePtr cwtTcpSocketDeviceOpen(const CHAR *hostName, UINT16 port);
*/

CwtIODevice* cwtUdpSocketDeviceOpen(const CWT_CHAR *inetAddr, UINT16 port);
CwtIODevice* cwtTcpSocketDeviceOpen(const CWT_CHAR *inetAddr, UINT16 port);

DLL_API_EXPORT CwtIODevice* cwtMulticastSocketDeviceOpen(
	  const CWT_CHAR *inetAddr
	, UINT16 port
	, const CWT_CHAR *inetMCastAddr
	, BOOL isSender);

EXTERN_C_END

#endif /* __CWT_IO_SOCKETDEV_H__ */
