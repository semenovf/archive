/*
 * socket_tcpudp.c
 *
 *  Created on: 18.11.2011
 *      Author: user
 */

#include "socket_p.h"
#include <string.h>
#if defined(CWT_CC_MSC)
#elif defined(CWT_CC_GNUC)
#	include <unistd.h>
#endif
#include <cwt/str.h>
#include <cwt/logger.h>

static BOOL __initListener(CwtTcpSocket *sd_tcp
		, const CWT_CHAR *inetAddr
		, UINT16 port)
{
	int rc;
	int reuse = 1;

    /* http://publib.boulder.ibm.com/infocenter/iseries/v5r3/topic/rzab6/rzab6xconoserver.htm
     *
     * The setsockopt() function is used to allow the local address to
     * be reused when the server is restarted before the required wait
     * time expires
     */
	rc = setsockopt(sd_tcp->sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse));
	if (rc < 0)	{
		cwt_logger_ns()->error(_Tr("setting the socket to reusable address mode (SO_REUSEADDR) failed")
			_CWT_SOCKET_LOG_FMTSUFFIX
			, _CWT_SOCKET_LOG_ARGS);
		return FALSE;
	}


	if (!__socket_initSockAddrIn(&sd_tcp->sockaddr, inetAddr, port))
		return FALSE;

	/* Bind the socket. */
	rc = bind(sd_tcp->sockfd, (SOCKADDR*)&sd_tcp->sockaddr, sizeof(sd_tcp->sockaddr));
	if( rc < 0 ) {
		cwt_logger_ns()->error(_Tr("bind failed")
			_CWT_SOCKET_LOG_FMTSUFFIX
			, _CWT_SOCKET_LOG_ARGS);
		return FALSE;
	}

	rc = listen(sd_tcp->sockfd, 10);
	if (rc < 0) {
		cwt_logger_ns()->error(_Tr("changing socket state to listening mode failed")
			_CWT_SOCKET_LOG_FMTSUFFIX
			, _CWT_SOCKET_LOG_ARGS);
		return FALSE;
	}

	sd_tcp->is_listener = TRUE;
	return TRUE;
}

static BOOL __initClient(CwtTcpSocket *sd_tcp
		, const CWT_CHAR *inetAddr
		, UINT16 port)
{
	int rc;

	if (!__socket_initSockAddrIn(&sd_tcp->sockaddr, inetAddr, port))
		return FALSE;

	rc = connect(sd_tcp->sockfd, (struct sockaddr *)&sd_tcp->sockaddr, sizeof(sd_tcp->sockaddr));

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
		cwt_logger_ns()->error(_Tr("connection to %s:%u failed")
			_CWT_SOCKET_LOG_FMTSUFFIX
			, inetAddr
			, port
			, _CWT_SOCKET_LOG_ARGS);
		return FALSE;
	}

	return TRUE;
}


static CwtSocket* __socket_openTcpSocketHelper(
		const CWT_CHAR *inetAddr
		, UINT16 port
		, BOOL is_listener
		, BOOL is_nonblocking)
{
	BOOL ok = FALSE;
    CwtSocket *sd;

	sd = __socket_openTypified(Cwt_TcpSocket, is_nonblocking);

	if (sd) {
		ok = is_listener
			? __initListener((CwtTcpSocket *)sd, inetAddr, port)
			: __initClient((CwtTcpSocket *)sd, inetAddr, port);

		if (!ok) {
			cwt_socket_ns()->close(sd);
			sd = NULL;
		}
	}

	return sd;
}


CwtSocket* __socket_openTcpSocket(
		const CWT_CHAR *inetAddr
		, UINT16 port
		, BOOL is_nonblocking)
{
	return __socket_openTcpSocketHelper(
			inetAddr
			, port
			, FALSE
			, is_nonblocking);
}


CwtSocket* __socket_openTcpServerSocket(
		const CWT_CHAR *inetAddr
		, UINT16 port
		, BOOL is_nonblocking)
{
	return __socket_openTcpSocketHelper(
			inetAddr
			, port
			, TRUE
			, is_nonblocking);
}


CwtSocket* __socket_acceptTcpSocket(CwtSocket *sd)
{
	SOCKET client;
	CwtTcpSocket *sd_tcp;
	socklen_t socklen;

	CWT_ASSERT(sd);
	CWT_ASSERT(Cwt_TcpSocket == sd->type);

	sd_tcp = CWT_MALLOCT(CwtTcpSocket);
	socklen = sizeof(sd_tcp->sockaddr);

	client = accept(sd->sockfd, (struct sockaddr*)&sd_tcp->sockaddr, &socklen);

	if (client > 0) {
		sd_tcp->sockfd      = client;
		sd_tcp->type        = sd->type;
		sd_tcp->is_listener = FALSE;

		__socket_nsockets_opened++;

		return (CwtSocket*)sd_tcp;
	}

	CWT_FREE(sd_tcp);
	cwt_logger_ns()->error(_Tr("accepting connection failed")
		_CWT_SOCKET_LOG_FMTSUFFIX
		, _CWT_SOCKET_LOG_ARGS);

	return NULL;
}


ssize_t __socket_readTcpSocket(CwtSocket *sd, CwtByteArray *ba, size_t n)
{
	CwtByteArrayNS *ba_ns = cwt_bytearray_ns();
	ssize_t br;

	CWT_ASSERT(sd);

	n = CWT_MIN(n, CWT_INT_MAX);
	ba_ns->reserve(ba, ba_ns->size(ba) + n);
	br = recv(sd->sockfd, ba_ns->data(ba) + ba_ns->size(ba), (int)n, 0);

	if( br < 0 ) {
		cwt_logger_ns()->error(_Tr("receiving data error")
			_CWT_SOCKET_LOG_FMTSUFFIX
			, _CWT_SOCKET_LOG_ARGS);
	} else {
		ba_ns->resize(ba, ba_ns->size(ba) + br);
	}

	return (ssize_t)br;
}


ssize_t __socket_writeTcpSocket(CwtSocket *sd, const BYTE *buf, size_t sz)
{
	ssize_t bw;

	CWT_ASSERT(sd);
	CWT_ASSERT(sz <= CWT_INT_MAX);

	bw = send(sd->sockfd, buf, (int)sz, 0);
    return bw;
}
