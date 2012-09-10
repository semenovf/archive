/*
 * socket_p.c
 *
 *  Created on: 30.08.2012
 *      Author: user
 */

#include "socket_p.h"
#include <cwt/logger.h>
#include <cwt/str.h>

SOCKET __socket_openNative(CwtSocketType socketType)
{
	SOCKET sockfd;
	int nativeType;
	int domain = AF_INET;

	if( !__socket_allowSockets() ) {
		cwtLoggerNS()->error(_Tr("network sockets not allowed in this system"));
		return -1;
	}

	domain = socketType != Cwt_LocalSocket
			? AF_INET
			: AF_UNIX;

	nativeType == (socketType == Cwt_TcpSocket || socketType == Cwt_LocalSocket)
			? SOCK_STREAM
			: SOCK_DGRAM;

	sockfd = socket(domain, nativeType, 0);
    if( sockfd < 0 ) {
    	cwtLoggerNS()->error(_Tr("failed to open socket [%d]"), __socket_errno);
		return -1;
    }

    return sockfd;
}


CwtSocket* __socket_openTypified(CwtSocketType socketType, BOOL is_nonblocking)
{
	SOCKET sockfd;
	CwtSocket *sd;

	sockfd = __socket_openNative(socketType);

	if( !sockfd )
		return NULL;

	if( is_nonblocking ) {
		if (!__socket_setNonBlockingNative(sockfd, is_nonblocking)) {
			cwtLoggerNS()->error(_Tr("set/unset socket non-blocking mode failed"));
			__socket_closeNative(sockfd);
			return NULL;
		}
	}

	switch(socketType) {
	case Cwt_LocalSocket:
		sd = (CwtSocket*)CWT_MALLOC(CwtLocalSocket);
		cwtStrNS()->bzero(sd, sizeof(CwtLocalSocket));
		break;
	case Cwt_McastSocket:
		sd = (CwtSocket*)CWT_MALLOC(CwtMcastSocket);
		cwtStrNS()->bzero(sd, sizeof(CwtMcastSocket));
		break;
	case Cwt_UdpSocket:
		sd = (CwtSocket*)CWT_MALLOC(CwtUdpSocket);
		cwtStrNS()->bzero(sd, sizeof(CwtUdpSocket));
		break;
	case Cwt_TcpSocket:
	default:
		sd = (CwtSocket*)CWT_MALLOC(CwtTcpSocket);
		cwtStrNS()->bzero(sd, sizeof(CwtTcpSocket));
		break;
	}

	sd->sockfd      = sockfd;
	sd->type        = socketType;
	sd->is_listener = FALSE;


	__socket_nsockets_opened++;

	return (CwtSocket*)sd;
}



