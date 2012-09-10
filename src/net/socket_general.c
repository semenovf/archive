/*
 * socket_tcpudp.c
 *
 *  Created on: 18.11.2011
 *      Author: user
 */

#include "socket_p.h"
#include <string.h>
#include <unistd.h>
#include <cwt/str.h>
#include <cwt/logger.h>
#include <cwt/txtcodec.h>

extern size_t __socket_bytesAvailable (CwtSocket *sd);

static BOOL __initSockAddrIn(struct sockaddr_in *saddr, const CWT_CHAR *inetAddr, UINT16 port)
{
	BOOL ok = TRUE;

	cwtStrNS()->bzero(saddr, sizeof(*saddr));
	saddr->sin_family      = AF_INET;
	saddr->sin_port        = htons(port);

	if (!inetAddr) {
		saddr->sin_addr.s_addr = htonl(INADDR_ANY);
	} else {
		char *inetAddrLatin1;
		inetAddrLatin1 = cwtTextCodecNS()->toLatin1(inetAddr, cwtStrNS()->strlen(inetAddr));

		if( !inet_aton(inetAddrLatin1, &saddr->sin_addr) ) {
			do {
				struct hostent *phost;

				phost = gethostbyname(inetAddrLatin1);

				if (phost == (struct hostent *)NULL) {
					cwtLoggerNS()->error(_Tr("%s: host not found"), inetAddr);
					/*printf("h_errno = %d\n", h_errno);*/
					ok = FALSE;
					break;
				}

				memcpy(&saddr->sin_addr, phost->h_addr, sizeof(saddr->sin_addr));

			} while(FALSE);
		}
		CWT_FREE(inetAddrLatin1);
	}

	return ok;
}


static BOOL __initSockAddrUn(struct sockaddr_un *saddr, const CWT_CHAR *path)
{
	char *pathLatin1;
	BOOL ok = FALSE;

	if( !path || cwtStrNS()->strlen(path) == 0 ) {
		cwtLoggerNS()->error(_Tr("local socket path is empty"));
		return FALSE;
	}

	pathLatin1 = cwtTextCodecNS()->toLatin1(path, cwtStrNS()->strlen(path));

	if( strlen(pathLatin1) >= sizeof(saddr->sun_path)) {
		cwtLoggerNS()->error(_Tr("local socket path long"));
	} else {
		ok = TRUE;
	}
	CWT_FREE(pathLatin1);
	return ok;
}

static BOOL __socket_bindTcpUdpSocket(CwtSocket *sd, const CWT_CHAR *inetAddr, UINT16 port)
{
	int rc;

    /* The sockaddr_in structure specifies the address family,
     * IP address, and port for the socket that is being bound.
     */
	struct sockaddr_in sockaddr;

	if (!__initSockAddrIn(&sockaddr, inetAddr, port))
		return FALSE;

	/* Bind the socket. */
	rc = bind(sd->sockfd, (SOCKADDR*)&sockaddr, sizeof(sockaddr));
	if( rc < 0 ) {
		cwtLoggerNS()->error(_Tr("bind failed")
			_CWT_SOCKET_LOG_FMTSUFFIX
			, _CWT_SOCKET_LOG_ARGS);
		return FALSE;
	}

	/* It is not important to what socket to cast.
	 * CwtTcpSocket and CwtUdpSocket both have sockaddr member
	 */
	memcpy(&((CwtTcpSocket*)sd)->sockaddr, &sockaddr, sizeof(sockaddr));
	return TRUE;
}

static BOOL __socket_bindLocalSocket(CwtSocket *sd, const CWT_CHAR *path)
{
	int rc;
	size_t len;

    /* The sockaddr_in structure specifies the address family,
     * IP address, and port for the socket that is being bound.
     */
	struct sockaddr_un sockaddr;

	if (!__initSockAddrUn(&sockaddr, path))
		return FALSE;

	/* Bind the socket. */
	len = strlen(sd_local->sockaddr.sun_path) + sizeof(sd_local->sockaddr.sun_family);
	/*rc = connect(sd_local->sockfd, (struct sockaddr *)&sd_local->sockaddr, len);*/
	rc = bind(sd->sockfd, (SOCKADDR*)&sockaddr, sizeof(sockaddr));
	if( rc < 0 ) {
		cwtLoggerNS()->error(_Tr("bind failed")
			_CWT_SOCKET_LOG_FMTSUFFIX
			, _CWT_SOCKET_LOG_ARGS);
		return FALSE;
	}

	/* It is not important to what socket to cast.
	 * CwtTcpSocket and CwtUdpSocket both have sockaddr member
	 */
	memcpy(&((CwtTcpSocket*)sd)->sockaddr, &sockaddr, sizeof(sockaddr));
	return TRUE;
}

static BOOL __socket_connectLocalSocket(CwtSocket *sd, const CWT_CHAR *path)
{
	CwtLocalSocket *sd_local;
	size_t len;
	int rc;

	sd_local = (CwtLocalSocket *)sd;
	CWT_ASSERT(Cwt_LocalSocket == sd->type);

	if( __initSockAddrUn(&sd_local->sockaddr, path) ) {
		len = strlen(sd_local->sockaddr.sun_path) + sizeof(sd_local->sockaddr.sun_family);
		rc = connect(sd_local->sockfd, (struct sockaddr *)&sd_local->sockaddr, len);

		if (rc == EINPROGRESS) {
#if CWT_HAVE_POLL
			;
#endif
		}

		if (rc < 0) {
			cwtLoggerNS()->error(_Tr("connection to '%s' failed")
				_CWT_SOCKET_LOG_FMTSUFFIX
				, path
				, _CWT_SOCKET_LOG_ARGS);
			return FALSE;
		}

	}

	return TRUE;
}

static BOOL __socket_listenLocalSocket(CwtSocket *sd, const CWT_CHAR *path)
{
	int rc;

	CWT_ASSERT(sd);

	CWT_ASSERT(Cwt_LocalSocket == sd->type);

	if (!__socket_bindLocalSocket(sd, path))
		return FALSE;

	rc = listen(sd->sockfd, 10);
	if (rc < 0) {
		cwtLoggerNS()->error(_Tr("changing socket state to listening mode failed")
			_CWT_SOCKET_LOG_FMTSUFFIX
			, _CWT_SOCKET_LOG_ARGS);
		return FALSE;
	}

	sd->is_listener = TRUE;
	return TRUE;
}

static BOOL __socket_connectTcpUdpSocket(CwtSocket *sd, const CWT_CHAR *inetAddr, UINT16 port)
{
	CwtTcpSocket *sd_tcp;
	int rc;

	sd_tcp = (CwtTcpSocket *)sd;

	if (!__initSockAddrIn(&sd_tcp->sockaddr, inetAddr, port))
		return FALSE;

	if( Cwt_TcpSocket == sd->type ) {
		rc = connect(sd->sockfd, (struct sockaddr *)&sd_tcp->sockaddr, sizeof(sd_tcp->sockaddr));

		/*
			rc == EINPROGRESS:
			The socket is nonblocking and the connection cannot be completed immediately.
			It is possible to select(2) or poll(2) for completion by selecting
			the socket for writing. After select(2) indicates writability,
			use getsockopt(2) to read the SO_ERROR option at level SOL_SOCKET
			to determine whether connect() completed successfully (SO_ERROR is zero) or
			unsuccessfully (SO_ERROR is one of the usual error codes listed here,
			explaining the reason for the failure).
		*/
		if (rc == EINPROGRESS) {
#if CWT_HAVE_POLL
			;
#endif
		}

		if (rc < 0) {
			cwtLoggerNS()->error(_Tr("connection to %s:%u failed")
				_CWT_SOCKET_LOG_FMTSUFFIX
				, inetAddr
				, port
				, _CWT_SOCKET_LOG_ARGS);
			return FALSE;
		}
	}

	return TRUE;
}

/*
 * @fn BOOL CwtSocket::listen(CwtSocket *sd, const CWT_CHAR *inetAddr, UINT16 port)
 *
 * @brief Changes socket state into listening mode.
 *
 * @param sd Socket instance.
 * @param inetAddr IP address. If NULL any address selected.
 * @param port Socket port. If equals to 0 any available port selected.
 * @return
 */
static BOOL __socket_listenTcpUdpSocket(CwtSocket *sd, const CWT_CHAR *inetAddr, UINT16 port)
{
	int rc;

	CWT_ASSERT(sd);

	if (!(Cwt_TcpSocket == sd->type || Cwt_UdpSocket == sd->type)) {
		cwtLoggerNS()->error(_Tr("expected TCP or UDP socket for listen"));
		return FALSE;
	}


/*#ifdef CWT_OS_UNIX*/
    /* http://publib.boulder.ibm.com/infocenter/iseries/v5r3/topic/rzab6/rzab6xconoserver.htm
     *
     * The setsockopt() function is used to allow the local address to
     * be reused when the server is restarted before the required wait
     * time expires
     */
	if (Cwt_TcpSocket == sd->type) {
		int reuse = 1;
		rc = setsockopt(sd->sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse));
		if (rc < 0)	{
			cwtLoggerNS()->error(_Tr("setting the socket to reusable address mode (SO_REUSEADDR) failed")
				_CWT_SOCKET_LOG_FMTSUFFIX
				, _CWT_SOCKET_LOG_ARGS);
			return FALSE;
		}
	}
/*#endif*/

	if (!__socket_bindTcpUdpSocket(sd, inetAddr, port))
		return FALSE;


	if (Cwt_TcpSocket == sd->type) {
		rc = listen(sd->sockfd, 10);
		if (rc < 0) {
			cwtLoggerNS()->error(_Tr("changing socket state to listening mode failed")
				_CWT_SOCKET_LOG_FMTSUFFIX
				, _CWT_SOCKET_LOG_ARGS);
			return FALSE;
		}
	}

	sd->is_listener = TRUE;
	return TRUE;
}

static CwtSocket* __socket_openSocketHelper(CwtSocketType socketType, const CWT_CHAR *inetAddr, UINT16 port, BOOL is_nonblocking)
{
	CwtSocket *sd;
	BOOL ok;
	sd = __socket_openTypified(socketType, is_nonblocking);

	if( sd ) {
		switch(socketType) {
		case Cwt_LocalSocket:
			ok = __socket_connectLocalSocket(sd, inetAddr);
			break;
		case Cwt_TcpSocket:
		case Cwt_UdpSocket:
			ok = __socket_connectTcpUdpSocket(sd, inetAddr, port);
			break;
		default:
			CWT_ASSERT(FALSE);
			break;
		}
	}

	if (!ok) {
		cwtSocketNS()->close(sd);
		sd = NULL;
	}

	return sd;
}


static CwtSocket* __socket_openServerSocketHelper(CwtSocketType socketType, const CWT_CHAR *inetAddr, UINT16 port, BOOL is_nonblocking)
{
/*
	CwtSocket *sd;
	sd = __socket_openTypified(Cwt_LocalSocket, is_nonblocking);
	if( sd ) {
		if( !__socket_listenLocalSocket(sd, path) ) {
			cwtSocketNS()->close(sd);
			sd = NULL;
		}
	}
	return sd;
*/

	CwtSocket *sd;
	BOOL ok;
	sd = __socket_openTypified(socketType, is_nonblocking);

	if( sd ) {
		switch(socketType) {
		case Cwt_LocalSocket:
			ok = __socket_listenLocalSocket(sd, inetAddr);
			break;
		case Cwt_TcpSocket:
		case Cwt_UdpSocket:
			ok = __socket_listenTcpUdpSocket(sd, inetAddr, port);
			break;
		default:
			CWT_ASSERT(FALSE);
			break;
		}
	}

	if (!ok) {
		cwtSocketNS()->close(sd);
		sd = NULL;
	}
	return sd;
}


CwtSocket* __socket_openLocalSocket(const CWT_CHAR *path, BOOL is_nonblocking)
{
	return __socket_openSocketHelper(Cwt_LocalSocket, path, 0, is_nonblocking);
}

CwtSocket* __socket_openTcpSocket(const CWT_CHAR *inetAddr, UINT16 port, BOOL is_nonblocking)
{
	return __socket_openSocketHelper(Cwt_TcpSocket, inetAddr, port, is_nonblocking);
}

CwtSocket* __socket_openUdpSocket(const CWT_CHAR *inetAddr, UINT16 port, BOOL is_nonblocking)
{
	return __socket_openSocketHelper(Cwt_UdpSocket, inetAddr, port, is_nonblocking);
}


CwtSocket* __socket_openLocalServerSocket(const CWT_CHAR *path, BOOL is_nonblocking)
{
	return __socket_openServerSocketHelper(Cwt_LocalSocket, path, 0, is_nonblocking);
}


CwtSocket* __socket_openTcpServerSocket(const CWT_CHAR *inetAddr, UINT16 port, BOOL is_nonblocking)
{
	return __socket_openServerSocketHelper(Cwt_TcpSocket, inetAddr, port, is_nonblocking);
}

CwtSocket* __socket_openUdpServerSocket(const CWT_CHAR *inetAddr, UINT16 port, BOOL is_nonblocking)
{
	return __socket_openServerSocketHelper(Cwt_UdpSocket, inetAddr, port, is_nonblocking);
}



CwtSocket* __socket_accept(CwtSocket *sd)
{
	CWT_ASSERT(sd);
	if (Cwt_TcpSocket == sd->type) {
		SOCKET client;

		client = accept(sd->sockfd, NULL, NULL);

		if (client < 0) {
			cwtLoggerNS()->error(_Tr("accepting connection failed")
				_CWT_SOCKET_LOG_FMTSUFFIX
				, _CWT_SOCKET_LOG_ARGS);
		} else {
			CwtTcpSocket *sd_tcp;

			sd_tcp = CWT_MALLOC(CwtTcpSocket);

			sd_tcp->sockfd      = client;
			sd_tcp->type        = sd->type;
			sd_tcp->is_listener = FALSE;

			__socket_nsockets_opened++;

			return (CwtSocket*)sd_tcp;
		}
	} else {
		ssize_t rc;
		int flags;
		struct sockaddr_in senderAddr;
		UINT senderSize = sizeof(senderAddr);
		BYTE buf[1];

		/* Peeks at the incoming data. The data is copied into the buffer
		 * but is not removed from the input queue.
		 *
		 * See http://msdn.microsoft.com/en-us/library/windows/desktop/ms740120%28v=vs.85%29.aspx
		 */
		flags = MSG_PEEK;
		rc = recvfrom(sd->sockfd, buf, sizeof(buf), flags,
				(struct sockaddr *)&senderAddr,
				&senderSize);

		if( rc < 0 ) {
			printf_error(_Tr("failed to accept UDP socket")
				_CWT_SOCKET_LOG_FMTSUFFIX
				, _CWT_SOCKET_LOG_ARGS);
		} else {
			CwtUdpSocket *sd_udp;

			sd_udp = CWT_MALLOC(CwtUdpSocket);
			sd_udp->sockfd      = dup(sd->sockfd);
			sd_udp->type        = sd->type;
			sd_udp->is_listener = FALSE;

			cwtStrNS()->memcpy(&sd_udp->sockaddr, &senderAddr, sizeof(senderAddr));

			__socket_nsockets_opened++;

			return (CwtSocket*)sd_udp;
		}
	}

	return NULL;
}

/**
 * @fn BOOL CwtSocketNS::connect(CwtSocket *sd, const CWT_CHAR *inetAddr, UINT16 port, BOOL is_nonblocking)
 *
 * @param sd
 * @param inetAddr
 * @param port
 * @param is_nonblocking
 * @return
 */
/*
BOOL __socket_connect(CwtSocket *sd, const CWT_CHAR *inetAddr, UINT16 port)
{
	CWT_ASSERT(sd);

	return __socket_connectTcpUdpSocket(sd, inetAddr, port);
}
*/


ssize_t __socket_readUdpSocket(CwtSocket *sd, BYTE *buf, size_t sz)
{
	ssize_t br;
	struct sockaddr_in senderAddr;
	UINT senderSize = sizeof(senderAddr);

	br = recvfrom(sd->sockfd, buf, sz, 0,
			(struct sockaddr *)&senderAddr,
			&senderSize);

	if( br < 0 ) {
		cwtLoggerNS()->error(_Tr("receiving data error")
			_CWT_SOCKET_LOG_FMTSUFFIX
			, _CWT_SOCKET_LOG_ARGS);
	}

	return br;
}

ssize_t __socket_readTcpSocket(CwtSocket *sd, BYTE *buf, size_t sz)
{
	ssize_t br;

	CWT_ASSERT(sd);

	sz = CWT_MIN(sz, __socket_bytesAvailable(sd));
	sz = CWT_MIN(sz, CWT_INT_MAX);

	br = recv(sd->sockfd, buf, (int)sz, 0);

	if( br < 0 ) {
		printf_error(_Tr("receiving data error")
			_CWT_SOCKET_LOG_FMTSUFFIX
			, _CWT_SOCKET_LOG_ARGS);
	}
	return (ssize_t)br;
}

ssize_t __socket_writeUdpSocket(CwtSocket *sd, const BYTE *buf, size_t sz)
{
	ssize_t bw;
	CwtUdpSocket *sd_udp = (CwtUdpSocket*)sd;

	bw = sendto(sd->sockfd, buf, sz, 0,
		(struct sockaddr *)&sd_udp->sockaddr,
		sizeof(sd_udp->sockaddr));

	if( bw < 0 ) {
		cwtLoggerNS()->error(_Tr("sending data error")
			_CWT_SOCKET_LOG_FMTSUFFIX
			, _CWT_SOCKET_LOG_ARGS);
	}

	return bw;
}

ssize_t __socket_writeTcpSocket(CwtSocket *sd, const BYTE *buf, size_t sz)
{
	ssize_t bw;

	CWT_ASSERT(sd);
	CWT_ASSERT(sz <= CWT_INT_MAX);

	bw = send(sd->sockfd, buf, (int)sz, 0);
    return bw;
}
