/*
 * sockdev_multicast.c
 *
 *  Created on: 18.11.2011
 *      Author: user
 */

#include "sockdev_p.h"

CwtIODevice* cwtMulticastSocketDeviceOpen(
	  const CWT_CHAR *inetAddr
	, UINT16 port
	, const CWT_CHAR *inetMCastAddr
	, BOOL isSender)
{
	CwtStrNS *strNS = cwtStrNS();
	SOCKET sockfd;
	CwtMulticastSocketDevice *sockdev;

	if( !__cwtAllowSockets() )
		return NULL;

    /* Create a datagram socket on which to send. */
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if( sockfd < 0 ) {
 		printf_error(_Tr("failed to open multicast socket [%d]"), __sockdev_errno);
		return NULL;
    }


	if( isSender ) {
		struct in_addr localInterface;
		char *inetAddr_      = cwtTextCodecNS()->toLatin1(inetAddr, strNS->strlen(inetAddr));
		char *inetMCastAddr_ = cwtTextCodecNS()->toLatin1(inetMCastAddr, strNS->strlen(inetMCastAddr));

		sockdev = CWT_MALLOC(CwtMulticastSocketDevice);


		/* Initialize the group sockaddr structure with a */
		/* group address of 225.1.1.1 and port 5555. */
		memset((char *) &sockdev->group.sockaddr, 0, sizeof(sockdev->group.sockaddr));
		sockdev->group.sockaddr.sin_family = AF_INET;
		sockdev->group.sockaddr.sin_addr.s_addr = inet_addr(inetMCastAddr_); /*inet_addr("226.1.1.1");*/
		sockdev->group.sockaddr.sin_port = htons(port);

		/* Set local interface for outbound multicast datagrams. */
		/* The IP address specified must be associated with a local, */
		/* multicast capable interface. */
		localInterface.s_addr = inet_addr(inetAddr_); /*inet_addr("192.168.0.198");*/

		CWT_FREE(inetAddr_);
		CWT_FREE(inetMCastAddr_);

	    if(setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_IF, (char *)&localInterface, sizeof(localInterface)) < 0) {
	        printf_error(_Tr("setting local interface error (%d)"), __sockdev_errno);
	        __closesocket(sockfd);
	        CWT_FREE(sockdev);
	        return NULL;
	    }
	} else { /* CwtMCSocket_Client */
		/* Enable SO_REUSEADDR to allow multiple instances of this */
	    /* application to receive copies of the multicast datagrams. */
		int reuse = 1;
		struct sockaddr_in localSock;

		char *inetAddr_      = cwtTextCodecNS()->toLatin1(inetAddr, strNS->strlen(inetAddr));
		char *inetMCastAddr_ = cwtTextCodecNS()->toLatin1(inetMCastAddr, strNS->strlen(inetMCastAddr));


		if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse)) < 0) {
			printf_error(_Tr("setting SO_REUSEADDR error (%d)"), __sockdev_errno);
			__closesocket(sockfd);
			return NULL;
		}

	    /* Bind to the proper port number with the IP address */
	    /* specified as INADDR_ANY. */
	    memset((char *) &localSock, 0, sizeof(localSock));
	    localSock.sin_family = AF_INET;
	    localSock.sin_port = htons(port);
	    localSock.sin_addr.s_addr = INADDR_ANY;

		if(bind(sockfd, (struct sockaddr*)&localSock, sizeof(localSock))) {
	        printf_error(_Tr("binding datagram socket error (%d)"), __sockdev_errno);
	        __closesocket(sockfd);
	        return NULL;
	    }

	    /* Join the multicast group 226.1.1.1 on the local 203.106.93.94 */
	    /* interface. Note that this IP_ADD_MEMBERSHIP option must be */
	    /* called for each local interface over which the multicast */
	    /* datagrams are to be received. */
		sockdev = CWT_MALLOC(CwtMulticastSocketDevice);

	    sockdev->group.mreq.imr_multiaddr.s_addr = inet_addr(inetMCastAddr_);
	    sockdev->group.mreq.imr_interface.s_addr = inet_addr(inetAddr_);

		CWT_FREE(inetAddr_);
		CWT_FREE(inetMCastAddr_);

	    if(setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&sockdev->group.mreq, sizeof(sockdev->group.mreq)) < 0) {
	        printf_error(_Tr("adding multicast group error (%d)"), __sockdev_errno);
	        __closesocket(sockfd);
	        CWT_FREE(sockdev);
	        return NULL;
	    }
	}


	sockdev->sockbase.sockfd              = sockfd;
	((CwtSocketDevice*)sockdev)->type   = isSender ? Cwt_MCSocket_Sender : Cwt_MCSocket_Receiver;
	sockdev->sockbase.base.close          = __cwtSocketClose;
	sockdev->sockbase.base.bytesAvailable = __cwtSocketBytesAvailable;
	sockdev->sockbase.base.read           = __cwtSocketRead;
	sockdev->sockbase.base.write          = __cwtSocketWrite;

	sockdev->sockbase.base.readBegin      = NULL;
	sockdev->sockbase.base.readCommit     = NULL;
	sockdev->sockbase.base.readRollback   = NULL;

	__cwt_nsockets_opened++;

	return (CwtIODevice*)sockdev;
}

