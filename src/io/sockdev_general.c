/*
 * sockdev.c
 *
 *  Created on: 18.11.2011
 *      Author: user
 */

#include "sockdev_p.h"

/* helper function */
static CwtIODevice* __inetSocketDeviceOpen(CwtSocketType socketType, const CWT_CHAR *inetAddr, UINT16 port)
{
	CwtStrNS *strNS = cwtStrNS();
	SOCKET sockfd;
	struct sockaddr_in sockaddr;
	CwtSocketDevice *sockdev;

	if( !__cwtAllowSockets() )
		return NULL;

    /* Create a datagram socket on which to send. */
	sockfd = socket(AF_INET, socketType == Cwt_TcpSocket ? SOCK_STREAM : SOCK_DGRAM , 0);

    if( sockfd < 0 ) {
 		printf_error(_Tr("failed to open socket [%d]"), __sockdev_errno);
		return NULL;
    }


    /* The sockaddr_in structure specifies the address family,
     * IP address, and port for the socket that is being bound.
     */
    {
		char *inetAddr_ = cwtTextCodecNS()->toLatin1(inetAddr, strNS->strlen(inetAddr));
		sockaddr.sin_family = AF_INET;
		sockaddr.sin_addr.s_addr = inetAddr != NULL ? inet_addr(inetAddr_) : INADDR_ANY;
		sockaddr.sin_port = port;
		CWT_FREE(inetAddr_);
    }

    /* Bind the socket. */
	if( bind(sockfd, (SOCKADDR*)&sockaddr, sizeof(sockaddr)) < 0 ) {
		print_error(_Tr("bind failed")); /*, WSAGetLastError());*/
		__closesocket(sockfd);
		return NULL;
	}

    sockdev = CWT_MALLOC(CwtSocketDevice);

	sockdev->sockfd              = sockfd;
	sockdev->type                = socketType;
	sockdev->base.close          = __cwtSocketClose;
	sockdev->base.bytesAvailable = __cwtSocketBytesAvailable;
	sockdev->base.read           = __cwtSocketRead;
	sockdev->base.write          = __cwtSocketWrite;

	sockdev->base.readBegin      = NULL;
	sockdev->base.readCommit     = NULL;
	sockdev->base.readRollback   = NULL;

	__cwt_nsockets_opened++;

	return (CwtIODevice*)sockdev;
}
/**
 * Opens UDP socket device
 *
 * @param inetAddr ip address. If NULL any address selected
 * @param port port. If equals to 0 any available port selected
 * @return UDP socket device instance or NULL if error occured
 */
CwtIODevice* cwtUdpSocketDeviceOpen(const CWT_CHAR *inetAddr, UINT16 port)
{
	return __inetSocketDeviceOpen(Cwt_UdpSocket, inetAddr, port);
}


/**
 * Opens TCP socket device
 *
 * @param inetAddr ip address. If NULL any address selected
 * @param port port. If equals to 0 any available port selected
 * @return UDP socket device instance or NULL if error occured
 */
CwtIODevice* cwtTcpSocketDeviceOpen(const CWT_CHAR *inetAddr, UINT16 port)
{
	return __inetSocketDeviceOpen(Cwt_TcpSocket, inetAddr, port);
}


