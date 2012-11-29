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
#   define SHUT_READ  SD_RECEIVE
#   define SHUT_WRITE SD_SEND
#   define SHUT_RDWR  SD_BOTH
#	define EINPROGRESS        WSAEINPROGRESS
#	define EINTR              WSAEINTR /* The blocking Windows Socket 1.1 call was canceled through WSACancelBlockingCall. */
#elif defined(CWT_CC_GNUC)
#   include <errno.h>
#   include <unistd.h> /* for close() */
#	include <sys/ioctl.h>
#	include <sys/fcntl.h>
#	include <sys/socket.h>
#	include <netinet/in.h>
#	include <sys/un.h>
#	include <arpa/inet.h>
#   include <netdb.h>
#	define __socket_errno errno  /* FIXME need multi-threaded support */
#	define __socket_closeNative CWT_ISO_CPP_NAME(close)
	typedef struct sockaddr SOCKADDR;
#   define CWT_SHUTDOWN_READ  SHUT_READ
#   define CWT_SHUTDOWN_WRITE SHUT_WRITE
#   define CWT_SHUTDOWN_RDWR  SHUT_RDWR
#else
#	error "Not implemented"
#endif


/*
#ifdef CWT_CC_MSC
#else
	typedef int SOCKET;
#endif
*/


typedef struct _CwtLocalSocket {
	_CWT_SOCKET_BASE
#if defined(CWT_CC_MSC)

#elif defined(CWT_CC_GNUC)
	struct sockaddr_un sockaddr;
#endif
} CwtLocalSocket;

typedef struct _CwtTcpSocket {
	_CWT_SOCKET_BASE
	struct sockaddr_in sockaddr; /* for client socket - server address */
	                             /* for server socket - home address */
	                             /* for accepted client socket - it is not important */
} CwtTcpSocket;

typedef struct _CwtUdpSocket {
	_CWT_SOCKET_BASE
	struct sockaddr_in sockaddr; /* for client socket      - server address */
	                             /* for server socket      - home address */
	                             /* for accepted client socket - client address */
	BOOL is_master;              /*  */
} CwtUdpSocket;


/* Multicast socket */
typedef struct _CwtMcastSocket {
	_CWT_SOCKET_BASE

	union {
		struct sockaddr_in sockaddr;  /* group sockaddr for sender */
		struct ip_mreq     mreq;      /* multicast group for receiver */
	} group;
} CwtMcastSocket;

extern size_t __socket_nsockets_opened;
extern BOOL   __socket_is_sockets_allowed;

EXTERN_C_BEGIN

extern BOOL       __socket_allowSockets     (void);
extern BOOL       __socket_setNonBlockingNative (SOCKET, BOOL is_nonblocking);
extern CwtSocket* __socket_openTypified     (CwtSocketType socketType, BOOL is_nonblocking);
extern BOOL       __socket_initSockAddrIn   (struct sockaddr_in *saddr, const CWT_CHAR *inetAddr, UINT16 port);
extern CwtSocket* __socket_acceptLocalSocket(CwtSocket*);
extern CwtSocket* __socket_acceptTcpSocket  (CwtSocket*);
extern CwtSocket* __socket_acceptUdpSocket  (CwtSocket*);
extern CwtSocket* __socket_acceptMcastSocket(CwtSocket*);
extern ssize_t    __socket_readLocalSocket  (CwtSocket*, BYTE *buf, size_t sz);
extern ssize_t    __socket_readTcpSocket    (CwtSocket*, BYTE *buf, size_t sz);
extern ssize_t    __socket_readUdpSocket    (CwtSocket*, BYTE *buf, size_t sz);
extern ssize_t    __socket_readMcastSocket  (CwtSocket*, BYTE *buf, size_t sz);
extern ssize_t    __socket_writeLocalSocket (CwtSocket*, const BYTE *buf, size_t sz);
extern ssize_t    __socket_writeTcpSocket   (CwtSocket*, const BYTE *buf, size_t sz);
extern ssize_t    __socket_writeUdpSocket   (CwtSocket*, const BYTE *buf, size_t sz);
extern ssize_t    __socket_writeMcastSocket (CwtSocket*, const BYTE *buf, size_t sz);

#define _CWT_SOCKET_LOG_FMTSUFFIX _T(": %s (errno=%d)")
#define _CWT_SOCKET_LOG_ARGS      cwt_str_ns()->strError(__socket_errno), __socket_errno

#ifdef CWT_CC_MSC
extern int inet_aton(const char *cp, struct in_addr *inp);
#endif

EXTERN_C_END

#endif /* __CWT_SOCKET_P_H__ */
