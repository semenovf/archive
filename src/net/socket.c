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
#include <cwt/str.h>
#include <cwt/net/socket.h>
#include <cwt/logger.h>
#include <cwt/txtcodec.h>
#include <cwt/fs.h>
#include <string.h>


extern CwtSocket* __socket_openLocalSocket      (const CWT_CHAR *path, BOOL is_nonblocking);
extern CwtSocket* __socket_openLocalServerSocket(const CWT_CHAR *path, BOOL is_nonblocking);
extern CwtSocket* __socket_openUdpSocket        (const CWT_CHAR *inetAddr, UINT16 port, BOOL is_nonblocking);
extern CwtSocket* __socket_openUdpServerSocket  (const CWT_CHAR *inetAddr, UINT16 port, BOOL is_nonblocking);
extern CwtSocket* __socket_openTcpSocket        (const CWT_CHAR *inetAddr, UINT16 port, BOOL is_nonblocking);
extern CwtSocket* __socket_openTcpServerSocket  (const CWT_CHAR *inetAddr, UINT16 port, BOOL is_nonblocking);
extern CwtSocket* __socket_openMcastSocket      (const CWT_CHAR *inetAddr, UINT16 port, const CWT_CHAR *inetMCastAddr, BOOL is_nonblocking);
extern CwtSocket* __socket_openMcastServerSocket(const CWT_CHAR *inetAddr, UINT16 port, const CWT_CHAR *inetMCastAddr, BOOL is_nonblocking);

static CwtSocket* __socket_accept         (CwtSocket*);
static void       __socket_close          (CwtSocket*);
static BOOL       __socket_setNonBlocking (CwtSocket*, BOOL is_nonblocking);

       size_t     __socket_bytesAvailable (CwtSocket*);
static ssize_t    __socket_read           (CwtSocket*, BYTE *buf, size_t sz);
static ssize_t    __socket_write          (CwtSocket*, const BYTE *buf, size_t sz);
static CWT_CHAR*  __socket_inetAddr       (CwtSocket*);
static UINT16     __socket_inetPort       (CwtSocket*);
extern CWT_CHAR*  __socket_localPath      (CwtSocket*);


typedef CwtSocket* (*_socket_accept_f)(CwtSocket*);
typedef ssize_t    (*_socket_write_f)(CwtSocket*, const BYTE *buf, size_t sz);
typedef ssize_t    (*_socket_read_f) (CwtSocket*, BYTE *buf, size_t sz);

static _socket_accept_f __socket_acceptTypified[] = {
	  __socket_acceptLocalSocket
	, __socket_acceptTcpSocket
	, __socket_acceptUdpSocket
	, __socket_acceptMcastSocket
};

static _socket_write_f __socket_writeTypified[] = {
	  __socket_writeLocalSocket
	, __socket_writeTcpSocket
	, __socket_writeUdpSocket
	, __socket_writeMcastSocket
};

static _socket_read_f __socket_readTypified[] = {
	  __socket_readLocalSocket
	, __socket_readTcpSocket
	, __socket_readUdpSocket
	, __socket_readMcastSocket
};


static CwtSocketNS __cwtSocketNS = {
	  __socket_openLocalSocket
	, __socket_openLocalServerSocket
	, __socket_openUdpSocket
	, __socket_openUdpServerSocket
	, __socket_openTcpSocket
	, __socket_openTcpServerSocket
	, __socket_openMcastSocket
	, __socket_openMcastServerSocket
	, __socket_accept
	, __socket_close
	, __socket_setNonBlocking

	, __socket_bytesAvailable
	, __socket_read
	, __socket_write

	, __socket_inetAddr
	, __socket_inetPort
	, __socket_localPath
};

BOOL   __socket_is_sockets_allowed = FALSE;
size_t __socket_nsockets_opened = (size_t)0;

static CwtLoggerNS *__logger = NULL;

DLL_API_EXPORT CwtSocketNS* cwt_socket_ns(void)
{
	if( !__logger ) {
		__logger = cwt_logger_ns();
	}

	return &__cwtSocketNS;
}

static CwtSocket* __socket_accept(CwtSocket *sd)
{
	CWT_ASSERT(sd);
	return __socket_acceptTypified[sd->type](sd);
}

/**
 * @fn void CwtSocketNS::close(CwtSocket *sd)
 *
 * @brief Close socket and free allocated resources.
 *
 * @param sd Socket.
 */
static void __socket_close (CwtSocket *sd)
{
	CWT_ASSERT(sd);

	if (sd->sockfd > 0) {

		/* leave multicast receiver socket group */
		if( Cwt_McastSocket == sd->type && sd->is_listener ) {
			CwtMcastSocket *msd = (CwtMcastSocket*)sd;
			if( setsockopt (msd->sockfd, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char *)&msd->group.mreq, sizeof(msd->group.mreq)) < 0 ) {
		        __logger->error(_Tr("Dropping multicast group")
		        	_CWT_SOCKET_LOG_FMTSUFFIX
		        	, _CWT_SOCKET_LOG_ARGS);
			}
		}

		if (Cwt_TcpSocket == sd->type) {
			if (shutdown(sd->sockfd, SHUT_RDWR) != 0) {
				__logger->error(_Tr("Failed to shutdown socket")
					_CWT_SOCKET_LOG_FMTSUFFIX
					, _CWT_SOCKET_LOG_ARGS);
			}
		}

		CWT_ASSERT(__socket_nsockets_opened > 0 );

		if (sd->type == Cwt_UdpSocket) {
			if (((CwtUdpSocket*)sd)->is_master)
				__socket_closeNative(sd->sockfd);
		} else {
			__socket_closeNative(sd->sockfd);
		}

/*
		if ( !(sd->type == Cwt_UdpSocket && ((CwtUdpSocket*)sd)->is_master) )
			__socket_closeNative(sd->sockfd);
*/

		sd->sockfd = -1;

		if (Cwt_LocalSocket == sd->type && sd->is_listener) {
			CWT_CHAR *localPath = __cwtSocketNS.localPath(sd);
			if( !cwt_filesystem_ns()->unlink(localPath) ) {
				__logger->error(_Tr("Unable to unlink local socket: %s")
					_CWT_SOCKET_LOG_FMTSUFFIX
					, localPath
					, _CWT_SOCKET_LOG_ARGS);
			}
			CWT_FREE(localPath);
		}
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


/**
 * @fn CWT_CHAR* CwtSocketNS::inetAddr(CwtSocket *sd)
 *
 * @brief Converts the Internet host address in, given in network byte order,
 *        to a string in IPv4 dotted-decimal notation.
 *
 * @param sd Socket handler.
 * @return Internet address in IPv4 dotted-decimal notation or @c NULL if
 *         socket is not associated with Internet address (see notes).
 *
 * @note This function acceptable for Cwt_TcpSocket, Cwt_UdpSocket and Cwt_MSocket
 * type sockets.
 */
static CWT_CHAR*  __socket_inetAddr(CwtSocket *sd)
{
	CWT_CHAR *inetAddr = NULL;
	char *inetAddrLatin1 = NULL;
	struct in_addr *in;

	switch (sd->type) {
	case Cwt_UdpSocket:
	case Cwt_TcpSocket:
		in = &((CwtUdpSocket*)sd)->sockaddr.sin_addr;
		break;
	case Cwt_McastSocket:
		/* FIXME must be implemented */
		return NULL;
	case Cwt_LocalSocket:
		return NULL;
	default:
		CWT_ASSERT(FALSE);
		break;
	}

	/* FIXME MT-unsafe, The string is returned in a statically
	 * allocated buffer, which subsequent calls will overwrite*/
	inetAddrLatin1 = inet_ntoa(*in);
	inetAddr = cwt_textcodec_ns()->fromLatin1(inetAddrLatin1, strlen(inetAddrLatin1));

	return inetAddr;
}

static UINT16 __socket_inetPort(CwtSocket *sd)
{
	UINT16 port = 0;

	switch (sd->type) {
	case Cwt_TcpSocket:
	case Cwt_UdpSocket:
		port = ntohs(((CwtUdpSocket*)sd)->sockaddr.sin_port);
		break;
	case Cwt_McastSocket:
		/* FIXME must be implemented */
		return 0;
	case Cwt_LocalSocket:
		return 0;
	default:
		CWT_ASSERT(FALSE);
		break;
	}

	return port;
}
