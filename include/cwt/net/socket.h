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
	, Cwt_MSocket
} CwtSocketType;

#define _CWT_SOCKET_BASE 	                      \
	CwtSocketType type;                           \
	SOCKET        sockfd; /* socket descriptor */ \
	BOOL          is_listener;


typedef struct _CwtSocket {
	_CWT_SOCKET_BASE
} CwtSocket;

typedef struct _CwtSocketNS {
	struct _CwtSocket*       (*openUdpSocket)  (BOOL is_nonblocking);
	struct _CwtSocket*       (*openTcpSocket)  (BOOL is_nonblocking);
	struct _CwtSocket*       (*openMSocket)    (BOOL is_nonblocking);
	BOOL                     (*listen)         (CwtSocket*, const CWT_CHAR *inetAddr, UINT16 port);
	BOOL                     (*listenMSocket)  (CwtSocket*, const CWT_CHAR *inetAddr, UINT16 port, const CWT_CHAR *inetMCastAddr);
	BOOL                     (*connect)        (CwtSocket*, const CWT_CHAR *inetAddr, UINT16 port);
	BOOL                     (*connectMSocket) (CwtSocket*, const CWT_CHAR *inetAddr, UINT16 port, const CWT_CHAR *inetMCastAddr);
	struct _CwtSocket*       (*accept)         (CwtSocket*);
	void                     (*close)          (CwtSocket*);
	BOOL                     (*setNonBlocking) (CwtSocket*, BOOL is_nonblocking);

	size_t                   (*bytesAvailable) (CwtSocket*);
	ssize_t                  (*read)           (CwtSocket*, BYTE* buf, size_t sz);
	ssize_t                  (*write)          (CwtSocket*, const BYTE *buf, size_t sz);
} CwtSocketNS;


EXTERN_C_BEGIN
DLL_API_EXPORT CwtSocketNS* cwtSocketNS(void);
EXTERN_C_END


#endif /* __CWT_SOCKET_H__ */
