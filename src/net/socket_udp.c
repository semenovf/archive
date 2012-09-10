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

static BOOL __initListener(CwtTcpSocket *sd_tcp
		, const CWT_CHAR *inetAddr
		, UINT16 port)
{
	int rc;

	if (!__initSockAddrIn(&sd_tcp->sockaddr, inetAddr, port))
		return FALSE;

	/* Bind the socket. */
	rc = bind(sd_tcp->sockfd, (SOCKADDR*)&sd_tcp->sockaddr, sizeof(sd_tcp->sockaddr));
	if( rc < 0 ) {
		cwtLoggerNS()->error(_Tr("bind failed")
			_CWT_SOCKET_LOG_FMTSUFFIX
			, _CWT_SOCKET_LOG_ARGS);
		return FALSE;
	}

	sd_tcp->is_listener = TRUE;
	return TRUE;
}


static BOOL __initClient(CwtTcpSocket *sd_udp
		, const CWT_CHAR *inetAddr
		, UINT16 port)
{
	if (!__socket_initSockAddrIn(&sd_udp->sockaddr, inetAddr, port))
		return FALSE;

	return TRUE;
}


static CwtSocket* __socket_openUdpSocketHelper(
		const CWT_CHAR *inetAddr
		, UINT16 port
		, BOOL is_listener
		, BOOL is_nonblocking)
{
	BOOL ok = FALSE;
    CwtSocket *sd;

	sd = __socket_openTypified(Cwt_UdpSocket, is_nonblocking);

	if (sd) {
		char *inetAddrLatin1;

		inetAddrLatin1 = cwtTextCodecNS()->toLatin1(inetAddr, cwtStrNS()->strlen(inetAddr));

		if( is_listener ) {
			ok = __initListener((CwtTcpSocket *)sd, inetAddrLatin1, port);
		} else {
			ok = __initClient((CwtTcpSocket *)sd, inetAddrLatin1, port);
		}
		CWT_FREE(inetAddrLatin1);

		if (!ok) {
			cwtSocketNS()->close(sd);
			sd = NULL;
		}
	}

	return sd;
}


CwtSocket* __socket_openUdpSocket(
		const CWT_CHAR *inetAddr
		, UINT16 port
		, BOOL is_nonblocking)
{
	return __socket_openUdpSocketHelper(
			inetAddr
			, port
			, FALSE
			, is_nonblocking);
}


CwtSocket* __socket_openUdpServerSocket(
		const CWT_CHAR *inetAddr
		, UINT16 port
		, BOOL is_nonblocking)
{
	return __socket_openUdpSocketHelper(
			inetAddr
			, port
			, TRUE
			, is_nonblocking);
}


CwtSocket* __socket_acceptUdpSocket(CwtSocket *sd)
{
	SOCKET client;
	ssize_t rc;
	int flags;
	struct sockaddr_in senderAddr;
	UINT senderSize = sizeof(senderAddr);
	BYTE buf[1];

	CWT_ASSERT(sd);
	CWT_ASSERT(Cwt_UdpSocket == sd->type);

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
		cwtLoggerNS()->error(_Tr("failed to accept UDP socket")
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

	return NULL;
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
		cwtLoggerNS()->error(_Tr("receiving data error")
			_CWT_SOCKET_LOG_FMTSUFFIX
			, _CWT_SOCKET_LOG_ARGS);
	}

	return br;
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
