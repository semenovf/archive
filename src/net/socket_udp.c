/*
 * socket_tcpudp.c
 *
 *  Created on: 18.11.2011
 *      Author: user
 */

#include "socket_p.h"
#include <string.h>
#include <cwt/unistd.h>
#include <cwt/str.h>
#include <cwt/logger.h>

static BOOL __initListener(CwtUdpSocket *sd_udp
		, const CWT_CHAR *inetAddr
		, UINT16 port)
{
	int rc;

	if (!__socket_initSockAddrIn(&sd_udp->sockaddr, inetAddr, port))
		return FALSE;

	/* Bind the socket. */
	rc = bind(sd_udp->sockfd, (SOCKADDR*)&sd_udp->sockaddr, sizeof(sd_udp->sockaddr));
	if( rc < 0 ) {
		cwt_logger_ns()->error(_Tr("bind failed")
			_CWT_SOCKET_LOG_FMTSUFFIX
			, _CWT_SOCKET_LOG_ARGS);
		return FALSE;
	}

	sd_udp->is_listener = TRUE;
	return TRUE;
}


static BOOL __initClient(CwtUdpSocket *sd_udp
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
		ok = is_listener
			? __initListener((CwtUdpSocket *)sd, inetAddr, port)
			: __initClient((CwtUdpSocket *)sd, inetAddr, port);

		if (!ok) {
			cwt_socket_ns()->close(sd);
			sd = NULL;
		} else {
			((CwtUdpSocket*)sd)->is_master = TRUE;
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
	ssize_t rc;
	int flags;
	socklen_t socklen = sizeof(struct sockaddr_in);
	BYTE buf[1];
	CwtUdpSocket *sd_udp;

	CWT_ASSERT(sd);
	CWT_ASSERT(Cwt_UdpSocket == sd->type);

	sd_udp = CWT_MALLOCT(CwtUdpSocket);

	/* Peeks at the incoming data. The data is copied into the buffer
	 * but is not removed from the input queue.
	 *
	 * See http://msdn.microsoft.com/en-us/library/windows/desktop/ms740120%28v=vs.85%29.aspx
	 */
	flags = MSG_PEEK;
	rc = recvfrom(sd->sockfd, buf, sizeof(buf), flags,
			(struct sockaddr *)&sd_udp->sockaddr,
			&socklen);

	if( rc >= 0 ) {
		sd_udp->sockfd      = sd->sockfd;
		sd_udp->is_master   = FALSE;
		sd_udp->type        = sd->type;
		sd_udp->is_listener = FALSE;

		__socket_nsockets_opened++;

		return (CwtSocket*)sd_udp;
	}

	CWT_FREE(sd_udp);

	/*FIXME MT unsafe*/
	if (EINTR == errno) {
		cwt_logger_ns()->warn(_Tr("UDP socket acception interrupted by signal"));
	} else {
		cwt_logger_ns()->error(_Tr("accepting connection failed")
			_CWT_SOCKET_LOG_FMTSUFFIX
			, _CWT_SOCKET_LOG_ARGS);
	}

	return NULL;
}


ssize_t __socket_readUdpSocket(CwtSocket *sd, BYTE *buf, size_t sz)
{
	ssize_t br;
	struct sockaddr_in senderAddr;
	UINT senderSize = sizeof(senderAddr);

	br = recvfrom(sd->sockfd, buf
#ifdef CWT_CC_MSC
			, (int)CWT_MIN(sz, CWT_INT_MAX)
#else
			, sz
#endif
			, 0,
			(struct sockaddr *)&senderAddr,
			&senderSize);

	if( br < 0 ) {
		cwt_logger_ns()->error(_Tr("receiving data error")
			_CWT_SOCKET_LOG_FMTSUFFIX
			, _CWT_SOCKET_LOG_ARGS);
	}

	return br;
}


ssize_t __socket_writeUdpSocket(CwtSocket *sd, const BYTE *buf, size_t sz)
{
	ssize_t bw;
	CwtUdpSocket *sd_udp = (CwtUdpSocket*)sd;

#ifdef CWT_CC_MSC
	CWT_ASSERT(sz <= (size_t)CWT_INT_MAX);
#endif

	bw = sendto(sd->sockfd, buf
#ifdef CWT_CC_MSC
		, (int)sz
#else
		, sz
#endif
		, 0
		, (struct sockaddr *)&sd_udp->sockaddr,
		sizeof(sd_udp->sockaddr));

	if( bw < 0 ) {
		cwt_logger_ns()->error(_Tr("sending data error")
			_CWT_SOCKET_LOG_FMTSUFFIX
			, _CWT_SOCKET_LOG_ARGS);
	}

	return bw;
}
