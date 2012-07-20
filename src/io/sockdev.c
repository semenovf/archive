/*
 * filedev.c
 *
 *  Created on: 18.11.2011
 *      Author: user
 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <cwt/unistd.h>
#include <cwt/logger.h>
#include <cwt/str.h>
#include <cwt/io/sockdev.h>
#include <string.h> /* TODO include after <cwt/str.h> to avoid function/methods conflicts */

#if defined(CWT_CC_MSC)
#	include <winsock2.h>
#	include <ws2tcpip.h>
#	define __sockdev_errno WSAGetLastError()
#	define __closesocket closesocket
#elif defined(CWT_CC_GNUC)
#	include <sys/ioctl.h>
#	include <sys/socket.h>
#	include <netinet/in.h>
#	include <arpa/inet.h>
#	define __sockdev_errno errno
#	define __closesocket CWT_ISO_CPP_NAME(close)
    typedef int SOCKET;
	typedef struct sockaddr SOCKADDR;
#else
#	error "Not implemented"
#endif


#ifndef CWT_CC_MSC

#else

#endif

static size_t __cwt_nsockets_opened = (size_t)0;
static BOOL   __cwt_is_sockets_allowed = FALSE;

static void    __cwtSocketClose(CwtIODevice*);
static size_t  __cwtSocketBytesAvailable(CwtIODevice*);
static ssize_t __cwtSocketRead(CwtIODevice*, BYTE*, size_t);
static ssize_t __cwtSocketWrite(CwtIODevice*, const BYTE*, size_t);

typedef struct CwtSocketDevice {
	CwtIODevice   base;
	CwtSocketType type;
	SOCKET sockfd;
} CwtSocketDevice;

typedef struct CwtMulticastSocketDevice {
	CwtSocketDevice sockbase;
	union {
		struct sockaddr_in sockaddr;  /* group sockaddr for sender */
		struct ip_mreq     mreq;      /* multicast group for receiver */
	} group;
} CwtMulticastSocketDevice;


static BOOL __cwtAllowSockets(void)
{
	if( !__cwt_is_sockets_allowed ) {

#ifdef CWT_CC_MSC
		WORD version;
		WSADATA wsaData;
		int res;

		version = MAKEWORD(2, 2);
		res = WSAStartup(MAKEWORD(2, 2), &wsaData);

		if( res != NO_ERROR ) {
			printf_error(_Tr("WSAStartup(): %d"), res);
			return FALSE;
		}

		/* Confirm that the WinSock DLL supports 2.2.*/
		/* Note that if the DLL supports versions greater    */
		/* than 2.2 in addition to 2.2, it will still return */
		/* 2.2 in wVersion since that is the version we      */
		/* requested.                                        */

		if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
			/* Tell the user that we could not find a usable */
			/* WinSock DLL.                                  */
			printf_error(_Tr("could not find a usable version of Winsock.dll, required version 2.2 or greater"));
			WSACleanup();
			return FALSE;
		}
#endif
	}

	__cwt_is_sockets_allowed = TRUE;
	return TRUE;
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
	CwtStrNS *strNS = cwtStrNS();

	SOCKET sockfd;
	struct sockaddr_in sockaddr;
	CwtSocketDevice *sockdev;

	if( !__cwtAllowSockets() )
		return NULL;

    /* Create a datagram socket on which to send. */
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if( sockfd < 0 ) {
 		printf_error(_Tr("failed to open multicast socket [%d]"), __sockdev_errno);
		return NULL;
    }


    /* The sockaddr_in structure specifies the address family,
     * IP address, and port for the socket that is being bound.
     */
    {
		char *inetAddr_ = strNS->toLatin1(inetAddr);
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
	sockdev->type                = Cwt_UdpSocket;
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
		char *inetAddr_      = strNS->toLatin1(inetAddr);
		char *inetMCastAddr_ = strNS->toLatin1(inetMCastAddr);

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

		char *inetAddr_      = strNS->toLatin1(inetAddr);
		char *inetMCastAddr_ = strNS->toLatin1(inetMCastAddr);


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



static void __cwtSocketClose(CwtIODevice *dev)
{
	CwtSocketDevice *sockdev;

	CWT_ASSERT(dev);
	sockdev = (CwtSocketDevice*)dev;

	if( sockdev->sockfd > 0 ) {

		/* leave multicast socket group */
		if( sockdev->type == Cwt_MCSocket_Receiver ) {
			CwtMulticastSocketDevice *mcsockdev = (CwtMulticastSocketDevice*)sockdev;
			if( setsockopt (sockdev->sockfd, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char *)&mcsockdev->group.mreq, sizeof(mcsockdev->group.mreq)) < 0 ) {
		        printf_error(_Tr("dropping multicast group error (%d)"), __sockdev_errno);
			}
		}

		CWT_ASSERT(__cwt_nsockets_opened > 0 );
		__closesocket(sockdev->sockfd);
		sockdev->sockfd = -1;
		__cwt_nsockets_opened--;
	}

	if( __cwt_nsockets_opened == 0 && __cwt_is_sockets_allowed ) {
#ifdef CWT_CC_MSC
		WSACleanup();
#endif
		__cwt_is_sockets_allowed = FALSE;
	}

	CWT_FREE(sockdev);
}

static size_t __cwtSocketBytesAvailable(CwtIODevice *dev)
{
	CwtSocketDevice *sockdev;
	size_t nbytes = 0;
	/*BYTE buf[64];*/

	CWT_ASSERT(dev);

	sockdev = (CwtSocketDevice*)dev;

#ifdef CWT_CC_MSC
	ioctlsocket(sockdev->sockfd, FIONREAD, (u_long*)&nbytes);
#else
	ioctl(sockdev->sockfd, FIONREAD, (char*)&nbytes);
#endif

	/*printf_debug("bytes available: %ul", nbytes);*/
	return nbytes;
}

static ssize_t __cwtSocketRead(CwtIODevice *dev, BYTE* buf, size_t sz)
{
	CwtSocketDevice *sockdev;
	ssize_t br;

	CWT_ASSERT(dev);

	sockdev = (CwtSocketDevice*)dev;

	sz = CWT_MIN(sz, __cwtSocketBytesAvailable(dev));
	CWT_ASSERT(sz <= CWT_INT_MAX);

	br = recv(sockdev->sockfd, buf, (int)sz, 0);

#ifdef CWT_CC_MSC
	if( br == SOCKET_ERROR ) {
		printf_error(_Tr("receiving data error: %d (0x%0X)"), __sockdev_errno, __sockdev_errno);
	}
#endif

	return (ssize_t)br;
}

static ssize_t __cwtSocketWrite(CwtIODevice *dev, const BYTE* buf, size_t sz)
{
	CwtSocketDevice *sockdev;
	ssize_t bw;

	CWT_ASSERT(dev);
	CWT_ASSERT(sz <= CWT_INT_MAX);

	sockdev = (CwtSocketDevice*)dev;
	if( sockdev->type == Cwt_MCSocket_Sender ) {
		CwtMulticastSocketDevice *mcsockdev = (CwtMulticastSocketDevice*)dev;
		bw = sendto(sockdev->sockfd, buf, (int)sz, 0
			, (struct sockaddr*)&mcsockdev->group.sockaddr
			, sizeof(mcsockdev->group.sockaddr));
	} else {
		bw = send(sockdev->sockfd, buf, (int)sz, 0);
	}
    return bw;
}


