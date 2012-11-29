/*
 * socket_local_unix.c
 *
 *  Created on: 29.09.2012
 *      Author: wladt
 */

#include "../socket_p.h"
#include <string.h>
#include <unistd.h>
#include <cwt/str.h>
#include <cwt/logger.h>
#include <cwt/txtcodec.h>

static BOOL __initSockAddrUn(struct sockaddr_un *saddr, const CWT_CHAR *path)
{
	char *utf8Path;
	BOOL ok = FALSE;

	if( !path || cwt_str_ns()->strLen(path) == 0 ) {
		cwt_logger_ns()->error(_Tr("local socket path is empty"));
		return FALSE;
	}

	utf8Path = cwt_textcodec_ns()->toUtf8(path, cwt_str_ns()->strLen(path));

	if( strlen(utf8Path) >= sizeof(saddr->sun_path)) {
		cwt_logger_ns()->error(_Tr("local socket path too long"));
	} else {
		saddr->sun_family = AF_UNIX;
		strcpy(saddr->sun_path, utf8Path);
		ok = TRUE;
	}
	CWT_FREE(utf8Path);
	return ok;
}

static BOOL __initListener(CwtLocalSocket *sd_local
		, const CWT_CHAR *path)
{
	int rc;
	socklen_t len;

	if (!__initSockAddrUn(&sd_local->sockaddr, path))
		return FALSE;

	/* Bind the socket. */
	len = strlen(sd_local->sockaddr.sun_path) + sizeof(sd_local->sockaddr.sun_family);
	rc = bind(sd_local->sockfd, (SOCKADDR*)&sd_local->sockaddr, len);

	if( rc < 0 ) {
		cwt_logger_ns()->error(_Tr("bind failed")
			_CWT_SOCKET_LOG_FMTSUFFIX
			, _CWT_SOCKET_LOG_ARGS);
		return FALSE;
	}

	rc = listen(sd_local->sockfd, 0);

	if (rc < 0) {
		cwt_logger_ns()->error(_Tr("changing socket state to listening mode failed")
			_CWT_SOCKET_LOG_FMTSUFFIX
			, _CWT_SOCKET_LOG_ARGS);
		return FALSE;
	}

	sd_local->is_listener = TRUE;
	return TRUE;
}


static BOOL __initClient(CwtLocalSocket *sd_local
		, const CWT_CHAR *path)
{
	size_t len;
	int rc;

	if( __initSockAddrUn(&sd_local->sockaddr, path) ) {
		len = strlen(sd_local->sockaddr.sun_path) + sizeof(sd_local->sockaddr.sun_family);
		rc = connect(sd_local->sockfd, (struct sockaddr *)&sd_local->sockaddr, len);

		if (rc == EINPROGRESS) {
#if CWT_HAVE_POLL
			;
#endif
		}

		if (rc < 0) {
			cwt_logger_ns()->error(_Tr("connection to '%s' failed")
				_CWT_SOCKET_LOG_FMTSUFFIX
				, path
				, _CWT_SOCKET_LOG_ARGS);
			return FALSE;
		}

	}

	return TRUE;
}


static CwtSocket* __socket_openLocalSocketHelper(
		const CWT_CHAR *path
		, BOOL is_listener
		, BOOL is_nonblocking)
{
	BOOL ok = FALSE;
    CwtSocket *sd;

	sd = __socket_openTypified(Cwt_LocalSocket, is_nonblocking);

	if (sd) {
		ok = is_listener
			? __initListener((CwtLocalSocket *)sd, path)
			: __initClient((CwtLocalSocket *)sd, path);

		if (!ok) {
			cwt_socket_ns()->close(sd);
			sd = NULL;
		}
	}

	return sd;
}



CwtSocket* __socket_openLocalSocket(const CWT_CHAR *path, BOOL is_nonblocking)
{
	return __socket_openLocalSocketHelper(
			path
			, FALSE
			, is_nonblocking);
}


CwtSocket* __socket_openLocalServerSocket(const CWT_CHAR *path, BOOL is_nonblocking)
{
	return __socket_openLocalSocketHelper(
			path
			, TRUE
			, is_nonblocking);
}


CwtSocket* __socket_acceptLocalSocket(CwtSocket *sd)
{
	SOCKET client;
	CwtLocalSocket *sd_local;
	socklen_t socklen;

	CWT_ASSERT(sd);
	CWT_ASSERT(Cwt_LocalSocket == sd->type);

	sd_local = CWT_MALLOC(CwtLocalSocket);
	socklen = sizeof(sd_local->sockaddr);

	client = accept(sd->sockfd, (struct sockaddr*)&sd_local->sockaddr, &socklen);

	if (client > 0) {
		sd_local->sockfd      = client;
		sd_local->type        = sd->type;
		sd_local->is_listener = FALSE;

		__socket_nsockets_opened++;

		return (CwtSocket*)sd_local;
	}

	CWT_FREE(sd_local);
	cwt_logger_ns()->error(_Tr("accepting connection failed")
		_CWT_SOCKET_LOG_FMTSUFFIX
		, _CWT_SOCKET_LOG_ARGS);

	return NULL;
}

ssize_t __socket_readLocalSocket(CwtSocket *sd, BYTE *buf, size_t sz)
{
	return __socket_readTcpSocket(sd, buf, sz);
}


ssize_t __socket_writeLocalSocket(CwtSocket *sd, const BYTE *buf, size_t sz)
{
	return __socket_writeTcpSocket(sd, buf, sz);
}


CWT_CHAR*  __socket_localPath(CwtSocket *sd)
{
	if( Cwt_LocalSocket == sd->type ) {
		CwtLocalSocket *sd_local = (CwtLocalSocket *)sd;
		size_t socklen = strlen(sd_local->sockaddr.sun_path);

		if( socklen >= sizeof(sd_local->sockaddr.sun_path) ) {
			cwt_logger_ns()->error(_Tr("local socket is invalid or it's path is wrong"));
			return NULL;
		}
		return cwt_textcodec_ns()->fromUtf8(sd_local->sockaddr.sun_path, socklen);
	}

	return NULL;
}
