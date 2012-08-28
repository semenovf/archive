/*
 * sockdev.c
 *
 *  Created on: 18.11.2011
 *      Author: user
 */

#include "sockdev_p.h"

size_t __cwt_nsockets_opened = (size_t)0;
static BOOL   __cwt_is_sockets_allowed = FALSE;

BOOL __cwtAllowSockets(void)
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


void __cwtSocketClose(CwtIODevice *dev)
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

size_t __cwtSocketBytesAvailable(CwtIODevice *dev)
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

ssize_t __cwtSocketRead(CwtIODevice *dev, BYTE* buf, size_t sz)
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

ssize_t __cwtSocketWrite(CwtIODevice *dev, const BYTE* buf, size_t sz)
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


