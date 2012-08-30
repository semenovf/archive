/*
 * socket.c
 *
 *  Created on: 29.08.2012
 *      Author: user
 */

/* Sources:
 * Multicast over TCP/IP HOWTO
 * 		http://tldp.org/HOWTO/Multicast-HOWTO.html
 * Socket programming:
 * 		http://publib.boulder.ibm.com/infocenter/iseries/v5r3/topic/rzab6/rzab6soxoverview.htm
 * Socket application design recommendations:
 * 		http://publib.boulder.ibm.com/infocenter/iseries/v5r3/topic/rzab6/rzab6designrec.htm
 * A connection-oriented server example:
 * 		http://publib.boulder.ibm.com/infocenter/iseries/v5r3/topic/rzab6/rzab6xconoserver.htm
 * A connectionless server example:
 * 		http://publib.boulder.ibm.com/infocenter/iseries/v5r3/topic/rzab6/rzab6xconlessserver.htm
 * A connection-oriented client example:
 * 		http://publib.boulder.ibm.com/infocenter/iseries/v5r3/topic/rzab6/rzab6xconoclient.htm
 * A connectionless client example:
 * 		http://publib.boulder.ibm.com/infocenter/iseries/v5r3/topic/rzab6/rzab6xconlessclient.htm
 * Introduction to non-blocking I/O:
 * 		http://www.kegel.com/dkftpbench/nonblocking.html
 */



#include "socket_p.h"
#include <cwt/net/socket.h>
#include <cwt/logger.h>

extern CwtSocket* __socket_openUdpSocket  (BOOL is_nonblocking);
extern CwtSocket* __socket_openTcpSocket  (BOOL is_nonblocking);
extern CwtSocket* __socket_openMSocket    (BOOL is_nonblocking);
extern BOOL       __socket_listen         (CwtSocket*, const CWT_CHAR *inetAddr, UINT16 port);
extern BOOL       __socket_listenMSocket  (CwtSocket*, const CWT_CHAR *inetAddr, UINT16 port, const CWT_CHAR *inetMCastAddr);
extern BOOL       __socket_connect        (CwtSocket*, const CWT_CHAR *inetAddr, UINT16 port);
extern BOOL       __socket_connectMSocket (CwtSocket*, const CWT_CHAR *inetAddr, UINT16 port, const CWT_CHAR *inetMCastAddr);
extern CwtSocket* __socket_accept         (CwtSocket*);
static void       __socket_close          (CwtSocket*);
static BOOL       __socket_setNonBlocking (CwtSocket*, BOOL is_nonblocking);

       size_t     __socket_bytesAvailable (CwtSocket*);
static ssize_t    __socket_read           (CwtSocket*, BYTE *buf, size_t sz);
static ssize_t    __socket_write          (CwtSocket*, const BYTE *buf, size_t sz);


typedef ssize_t  (*_socket_write_f)(CwtSocket*, const BYTE *buf, size_t sz);
typedef ssize_t  (*_socket_read_f) (CwtSocket*, BYTE *buf, size_t sz);

static _socket_write_f __socket_writeTypified[] = {
	  NULL
	, __socket_writeTcpSocket
	, __socket_writeUdpSocket
	, __socket_writeMSocket
};

static _socket_read_f __socket_readTypified[] = {
	  NULL
	, __socket_readTcpSocket
	, __socket_readUdpSocket
	, __socket_readMSocket
};


static CwtSocketNS __cwtSocketNS = {
	  __socket_openUdpSocket
	, __socket_openTcpSocket
	, __socket_openMSocket
	, __socket_listen
	, __socket_listenMSocket
	, __socket_connect
	, __socket_connectMSocket
	, __socket_accept
	, __socket_close
	, __socket_setNonBlocking

	, __socket_bytesAvailable
	, __socket_read
	, __socket_write
};

BOOL   __socket_is_sockets_allowed = FALSE;
size_t __socket_nsockets_opened = (size_t)0;

static CwtLoggerNS *__logger = NULL;

DLL_API_EXPORT CwtSocketNS* cwtSocketNS(void)
{
	if( !__logger ) {
		__logger = cwtLoggerNS();
	}

	return &__cwtSocketNS;
}

static void __socket_close (CwtSocket *sd)
{
	CWT_ASSERT(sd);

	if (sd->sockfd > 0) {

		/* leave multicast receiver socket group */
		if( sd->type == Cwt_MSocket && sd->is_listener ) {
			CwtMSocket *msd = (CwtMSocket*)sd;
			if( setsockopt (msd->sockfd, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char *)&msd->group.mreq, sizeof(msd->group.mreq)) < 0 ) {
		        __logger->error(_Tr("dropping multicast group error (%d)"), __socket_errno);
			}
		}

		CWT_ASSERT(__socket_nsockets_opened > 0 );
		__socket_closeNative(sd->sockfd);
		sd->sockfd = -1;
		__socket_nsockets_opened--;
	}

	if( __socket_nsockets_opened == 0 && __socket_is_sockets_allowed ) {
#ifdef CWT_CC_MSC
		WSACleanup();
#endif
		__socket_is_sockets_allowed = FALSE;
	}

	CWT_FREE(sd);
}


static BOOL __socket_setNonBlocking (CwtSocket *sd, BOOL is_nonblocking)
{
	CWT_ASSERT(sd);

	if( !__socket_setNonBlockingNative(sd->sockfd, is_nonblocking) ) {
		__logger->error(_Tr("set/unset socket non-blocking mode failed"));
		return FALSE;
	}
	return TRUE;
}

size_t __socket_bytesAvailable (CwtSocket *sd)
{
	size_t nbytes = 0;

	CWT_ASSERT(sd);

#ifdef CWT_CC_MSC
	ioctlsocket(sd->sockfd, FIONREAD, (u_long*)&nbytes);
#else
	ioctl(sd->sockfd, FIONREAD, (int*)&nbytes);
#endif

	return nbytes;
}


static ssize_t __socket_read(CwtSocket *sd, BYTE *buf, size_t sz)
{
	CWT_ASSERT(sd);
	return __socket_readTypified[sd->type](sd, buf, sz);
}


static ssize_t __socket_write(CwtSocket *sd, const BYTE *buf, size_t sz)
{
	CWT_ASSERT(sd);
	return __socket_writeTypified[sd->type](sd, buf, sz);
}
