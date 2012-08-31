/*
 * socket_tcpudp.c
 *
 *  Created on: 18.11.2011
 *      Author: user
 */

#include "socket_p.h"
#include <string.h>
#include <cwt/str.h>
#include <cwt/logger.h>
#include <cwt/txtcodec.h>

extern size_t __socket_bytesAvailable (CwtSocket *sd);

static BOOL __socket_bindNative(CwtSocket *sd, const CWT_CHAR *inetAddr, UINT16 port)
{
	CwtStrNS *strNS = cwtStrNS();
	int rc;

    /* The sockaddr_in structure specifies the address family,
     * IP address, and port for the socket that is being bound.
     */
	char *inetAddrLatin1;
	struct sockaddr_in sockaddr;

	inetAddrLatin1 = cwtTextCodecNS()->toLatin1(inetAddr, strNS->strlen(inetAddr));
	memset(&sockaddr, 0, sizeof(sockaddr));
	sockaddr.sin_family      = AF_INET;
	sockaddr.sin_addr.s_addr = htonl(inetAddr != NULL ? inet_addr(inetAddrLatin1) : INADDR_ANY);
	sockaddr.sin_port        = htons(port);
	CWT_FREE(inetAddrLatin1);

	/* Bind the socket. */
	rc = bind(sd->sockfd, (SOCKADDR*)&sockaddr, sizeof(sockaddr));
	if( rc < 0 ) {
		cwtLoggerNS()->error(_Tr("bind failed")); /* TODO WSAGetLastError());*/
		return FALSE;
	}

	/* It is not important to what socket to cast.
	 * CwtTcpSocket and CwtUdpSocket both have sockaddr member
	 */
	memcpy(&((CwtTcpSocket*)sd)->sockaddr, &sockaddr, sizeof(sockaddr));
	return TRUE;
}


static BOOL __socket_connectNative(CwtSocket *sd, const CWT_CHAR *inetAddr, UINT16 port)
{
	CwtStrNS *strNS = cwtStrNS();
	int rc;

    /* The sockaddr_in structure specifies the address family,
     * IP address, and port for the socket that is being bound.
     */
	char *inetAddrLatin1;
	struct sockaddr_in serveraddr;

	inetAddrLatin1 = cwtTextCodecNS()->toLatin1(inetAddr, strNS->strlen(inetAddr));
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family      = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(inetAddr != NULL ? inet_addr(inetAddrLatin1) : INADDR_ANY);
	serveraddr.sin_port        = htons(port);

	if (serveraddr.sin_addr.s_addr == (unsigned long)INADDR_NONE) {
		struct hostent *phost;

		phost = gethostbyname(inetAddrLatin1);

		if (phost == (struct hostent *)NULL) {
			cwtLoggerNS()->error(_Tr("%s: host not found"), inetAddr);
			/*printf("h_errno = %d\n", h_errno);*/
			CWT_FREE(inetAddrLatin1);
			return FALSE;
		}

		memcpy(&serveraddr.sin_addr, phost->h_addr, sizeof(serveraddr.sin_addr));
	}
	CWT_FREE(inetAddrLatin1);

	if( Cwt_TcpSocket == sd->type ) {
		rc = connect(sd->sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
		if (rc < 0) {
			cwtLoggerNS()->error(_Tr("connection to %s:%u failed"), inetAddr, port);
			return FALSE;
		}
	}

	/* It is not important to what socket to cast.
	 * CwtTcpSocket and CwtUdpSocket both have sockaddr member */
	memcpy(&((CwtTcpSocket*)sd)->sockaddr, &serveraddr, sizeof(serveraddr));
	return TRUE;
}

static SOCKET __socket_acceptNative(SOCKET listener)
{
	SOCKET client;

	client = accept(listener, NULL, NULL);
	if (client < 0) {
		cwtLoggerNS()->error(_Tr("accepting connection failed"));
	}

	return client;
}


CwtSocket* __socket_openUdpSocket (BOOL is_nonblocking)
{
	return __socket_openTypified(Cwt_UdpSocket, is_nonblocking);
}

CwtSocket* __socket_openTcpSocket (BOOL is_nonblocking)
{
	return __socket_openTypified(Cwt_TcpSocket, is_nonblocking);
}


/**
 * @fn BOOL CwtSocket::listen(CwtSocket *sd, const CWT_CHAR *inetAddr, UINT16 port)
 *
 * @brief Changes socket state into listening mode.
 *
 * @param sd Socket instance.
 * @param inetAddr IP address. If NULL any address selected.
 * @param port Socket port. If equals to 0 any available port selected.
 * @return
 */
BOOL __socket_listen(CwtSocket *sd, const CWT_CHAR *inetAddr, UINT16 port)
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
			cwtLoggerNS()->error(_Tr("setting the socket to reusable address mode (SO_REUSEADDR) failed"));
			return FALSE;
		}
	}
/*#endif*/

	if (!__socket_bindNative(sd, inetAddr, port))
		return FALSE;


	if (Cwt_TcpSocket == sd->type) {
		rc = listen(sd->sockfd, 10);
		if (rc < 0) {
			cwtLoggerNS()->error(_Tr("changing socket state to listening mode failed"));
			return FALSE;
		}
	}

	sd->is_listener = TRUE;
	return TRUE;
}


CwtSocket* __socket_accept(CwtSocket *sd)
{
	CWT_ASSERT(sd);
	if (Cwt_TcpSocket == sd->type) {
		SOCKET client;
		client = __socket_acceptNative(sd->sockfd);
		if( client ) {
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
			printf_error(_Tr("failed to accept UDP socket: %d (0x%0X)"), __socket_errno, __socket_errno);
		} else {
			CwtUdpSocket *sd_udp;

			sd_udp = CWT_MALLOC(CwtUdpSocket);
			sd_udp->sockfd      = sd->sockfd;
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
BOOL __socket_connect(CwtSocket *sd, const CWT_CHAR *inetAddr, UINT16 port, BOOL is_nonblocking)
{
	CWT_ASSERT(sd);

	if (!__socket_setNonBlockingNative(sd->sockfd, is_nonblocking)) {
		cwtLoggerNS()->error(_Tr("set/unset socket non-blocking mode failed"));
		return FALSE;
	}

	return __socket_connectNative(sd, inetAddr, port);
}


ssize_t __socket_readUdpSocket(CwtSocket *sd, BYTE *buf, size_t sz)
{
	ssize_t br;
	struct sockaddr_in senderAddr;
	UINT senderSize = sizeof(senderAddr);

	br = recvfrom(sd->sockfd, buf, sz, 0,
			(struct sockaddr *)&senderAddr,
			&senderSize);

	if( br < 0 ) {
		printf_error(_Tr("receiving data error: %d (0x%0X)"), __socket_errno, __socket_errno);
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
		printf_error(_Tr("receiving data error: %d (0x%0X)"), __socket_errno, __socket_errno);
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
		printf_error(_Tr("sending data error: %d (0x%0X)"), __socket_errno, __socket_errno);
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
