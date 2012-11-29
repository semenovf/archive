/*
 * socket_local_unix.c
 *
 *  Created on: 29.09.2012
 *      Author: wladt
 */

#include "../socket_p.h"
#include <string.h>
#include <cwt/str.h>
#include <cwt/logger.h>
#include <cwt/txtcodec.h>

/*FIXME need to implement*/
static BOOL __initListener(CwtLocalSocket *sd_local
		, const CWT_CHAR *path)
{
	CWT_UNUSED(sd_local);
	CWT_UNUSED(path);
	return FALSE;
}


/*FIXME need to implement*/
static BOOL __initClient(CwtLocalSocket *sd_local
		, const CWT_CHAR *path)
{
	CWT_UNUSED(sd_local);
	CWT_UNUSED(path);
	return FALSE;
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


/*FIXME need to implement*/
CwtSocket* __socket_acceptLocalSocket(CwtSocket *sd)
{
	CWT_UNUSED(sd);
	return NULL;
}

/*FIXME need to implement*/
ssize_t __socket_readLocalSocket(CwtSocket *sd, BYTE *buf, size_t sz)
{
	CWT_UNUSED(sd);
	CWT_UNUSED(buf);
	CWT_UNUSED(sz);
	return (ssize_t)-1;
}


/*FIXME need to implement*/
ssize_t __socket_writeLocalSocket(CwtSocket *sd, const BYTE *buf, size_t sz)
{
	CWT_UNUSED(sd);
	CWT_UNUSED(buf);
	CWT_UNUSED(sz);
	return (ssize_t)-1;
}


/*FIXME need to implement*/
CWT_CHAR*  __socket_localPath(CwtSocket *sd)
{
	CWT_UNUSED(sd);
	return NULL;
}
