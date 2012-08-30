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

typedef enum CwtSocketType {
	  Cwt_LocalSocket
	, Cwt_TcpSocket
	, Cwt_UdpSocket
	, Cwt_MSocket
} CwtSocketType;

struct _CwtSocket;

typedef struct _CwtSocketNS {
	struct _CwtSocket*       (*openUdpSocket)  (BOOL is_nonblocking);
	struct _CwtSocket*       (*openTcpSocket)  (BOOL is_nonblocking);
	struct _CwtSocket*       (*openMSocket)    (BOOL is_nonblocking);
	BOOL                     (*listen)         (struct _CwtSocket*, const CWT_CHAR *inetAddr, UINT16 port);
	BOOL                     (*listenMSocket)  (struct _CwtSocket*, const CWT_CHAR *inetAddr, UINT16 port, const CWT_CHAR *inetMCastAddr);
	BOOL                     (*connect)        (struct _CwtSocket*, const CWT_CHAR *inetAddr, UINT16 port);
	BOOL                     (*connectMSocket) (struct _CwtSocket*, const CWT_CHAR *inetAddr, UINT16 port, const CWT_CHAR *inetMCastAddr);
	struct _CwtSocket*       (*accept)         (struct _CwtSocket*);
	void                     (*close)          (struct _CwtSocket*);
	BOOL                     (*setNonBlocking) (struct _CwtSocket*, BOOL is_nonblocking);

	size_t                   (*bytesAvailable) (struct _CwtSocket*);
	ssize_t                  (*read)           (struct _CwtSocket*, BYTE* buf, size_t sz);
	ssize_t                  (*write)          (struct _CwtSocket*, const BYTE *buf, size_t sz);
} CwtSocketNS;


EXTERN_C_BEGIN
DLL_API_EXPORT CwtSocketNS* cwtSocketNS(void);
EXTERN_C_END


#endif /* __CWT_SOCKET_H__ */
