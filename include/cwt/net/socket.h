/*
 * socket.h
 *
 *  Created on: 29.08.2012
 *      Author: wladt
 *
 *
 *  Socket Engine
 */

#ifndef __CWT_SOCKET_H__
#define __CWT_SOCKET_H__

#include <cwt/types.h>

#if defined(CWT_CC_MSC)
#elif defined(CWT_CC_GNUC)
    typedef int SOCKET;
#else
#	error "Not implemented"
#endif

typedef enum CwtSocketType {
	  Cwt_LocalSocket
	, Cwt_TcpSocket
	, Cwt_UdpSocket
	, Cwt_McastSocket
} CwtSocketType;

#define _CWT_SOCKET_BASE 	                      \
	CwtSocketType type;                           \
	SOCKET        sockfd; /* socket descriptor */ \
	BOOL          is_listener;


typedef struct _CwtSocket {
	_CWT_SOCKET_BASE
} CwtSocket;

typedef struct _CwtSocketNS {
	CwtSocket*       (*openLocalSocket)      (const CWT_CHAR *path, BOOL is_nonblocking);
	CwtSocket*       (*openLocalServerSocket)(const CWT_CHAR *path, BOOL is_nonblocking);
	CwtSocket*       (*openUdpSocket)        (const CWT_CHAR *inetAddr, UINT16 port, BOOL is_nonblocking);
	CwtSocket*       (*openUdpServerSocket)  (const CWT_CHAR *inetAddr, UINT16 port, BOOL is_nonblocking);
	CwtSocket*       (*openTcpSocket)        (const CWT_CHAR *inetAddr, UINT16 port, BOOL is_nonblocking);
	CwtSocket*       (*openTcpServerSocket)  (const CWT_CHAR *inetAddr, UINT16 port, BOOL is_nonblocking);
	CwtSocket*       (*openMcastSocket)      (BOOL is_nonblocking);
	CwtSocket*       (*openMcastServerSocket)(BOOL is_nonblocking);
	/*BOOL             (*listen)               (CwtSocket*);*/
	/*BOOL             (*listenMcastSocket)(CwtSocket*, const CWT_CHAR *inetAddr, UINT16 port, const CWT_CHAR *inetMCastAddr);*/
	/*BOOL             (*connect)        (CwtSocket*, const CWT_CHAR *inetAddr, UINT16 port);*/
	/*BOOL             (*connectMcastSocket)(CwtSocket*, const CWT_CHAR *inetAddr, UINT16 port, const CWT_CHAR *inetMCastAddr);*/
	CwtSocket*       (*accept)         (CwtSocket*);
	void             (*close)          (CwtSocket*);
	BOOL             (*setNonBlocking) (CwtSocket*, BOOL is_nonblocking);

	size_t           (*bytesAvailable) (CwtSocket*);
	ssize_t          (*read)           (CwtSocket*, BYTE* buf, size_t sz);
	ssize_t          (*write)          (CwtSocket*, const BYTE *buf, size_t sz);
	CWT_CHAR*        (*inetAddr)       (CwtSocket*);
	UINT16           (*inetPort)       (CwtSocket*);
} CwtSocketNS;


EXTERN_C_BEGIN
DLL_API_EXPORT CwtSocketNS* cwtSocketNS(void);
EXTERN_C_END


#endif /* __CWT_SOCKET_H__ */
