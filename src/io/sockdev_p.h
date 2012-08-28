/*
 * sockdev_p.h
 *
 *  Created on: 28.08.2012
 *      Author: user
 */

#ifndef __CWT_SOCKDEV_P_H__
#define __CWT_SOCKDEV_P_H__

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <cwt/unistd.h>
#include <cwt/logger.h>
#include <cwt/str.h>
#include <cwt/txtcodec.h>
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


extern BOOL    __cwtAllowSockets(void);
extern void    __cwtSocketClose(CwtIODevice*);
extern size_t  __cwtSocketBytesAvailable(CwtIODevice*);
extern ssize_t __cwtSocketRead(CwtIODevice*, BYTE*, size_t);
extern ssize_t __cwtSocketWrite(CwtIODevice*, const BYTE*, size_t);

extern size_t __cwt_nsockets_opened;


#endif /* __CWT_SOCKDEV_P_H__ */
