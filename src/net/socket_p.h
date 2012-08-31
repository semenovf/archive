/*
 * socket_p.h
 *
 *  Created on: 29.08.2012
 *      Author: user
 */

#ifndef __CWT_SOCKET_P_H__
#define __CWT_SOCKET_P_H__

#include <cwt/net/socket.h>

#if defined(CWT_CC_MSC)
#	include <winsock2.h>
#	include <ws2tcpip.h>
#	define __socket_errno WSAGetLastError() /* FIXME need multi-threaded support */
#	define __socket_closeNative closesocket
#elif defined(CWT_CC_GNUC)
#   include <errno.h>
#   include <unistd.h> /* for close() */
#	include <sys/ioctl.h>
#	include <sys/socket.h>
#	include <netinet/in.h>
#	include <arpa/inet.h>
#   include <netdb.h>
#	define __socket_errno errno  /* FIXME need multi-threaded support */
#	define __socket_closeNative CWT_ISO_CPP_NAME(close)
    typedef int SOCKET;
	typedef struct sockaddr SOCKADDR;
#else
#	error "Not implemented"
#endif


/*
#ifdef CWT_CC_MSC
#else
	typedef int SOCKET;
#endif
*/


#define _SOCKET_BASE 	                          \
	CwtSocketType type;                           \
	SOCKET        sockfd; /* socket descriptor */ \
	BOOL          is_listener;


typedef struct _CwtSocket {
	_SOCKET_BASE
} CwtSocket;

typedef struct _CwtTcpSocket {
	_SOCKET_BASE
	struct sockaddr_in sockaddr; /* for client socket - server address */
	                             /* for server socket - home address */
	                             /* for accepted client socket - it is not important */
} CwtTcpSocket;

typedef struct _CwtUdpSocket {
	_SOCKET_BASE
	struct sockaddr_in sockaddr; /* for client socket      - server address */
	                             /* for server socket      - home address */
	                             /* for accepted client socket - client address */
} CwtUdpSocket;


/* Multicast socket */
typedef struct _CwtMSocket {
	_SOCKET_BASE

	union {
		struct sockaddr_in sockaddr;  /* group sockaddr for sender */
		struct ip_mreq     mreq;      /* multicast group for receiver */
	} group;
} CwtMSocket;

extern size_t __socket_nsockets_opened;
extern BOOL   __socket_is_sockets_allowed;

extern BOOL   __socket_allowSockets (void);
extern BOOL   __socket_setNonBlockingNative (SOCKET, BOOL is_nonblocking);
extern SOCKET __socket_openNative(CwtSocketType socketType);
extern CwtSocket* __socket_openTypified(CwtSocketType socketType, BOOL is_nonblocking);
extern ssize_t    __socket_readUdpSocket(CwtSocket*, BYTE *buf, size_t sz);
extern ssize_t    __socket_readTcpSocket(CwtSocket*, BYTE *buf, size_t sz);
extern ssize_t    __socket_readMSocket(CwtSocket*, BYTE *buf, size_t sz);
extern ssize_t    __socket_writeUdpSocket(CwtSocket*, const BYTE *buf, size_t sz);
extern ssize_t    __socket_writeTcpSocket(CwtSocket*, const BYTE *buf, size_t sz);
extern ssize_t    __socket_writeMSocket(CwtSocket*, const BYTE *buf, size_t sz);



#endif /* __CWT_SOCKET_P_H__ */
