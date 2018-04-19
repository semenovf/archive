/*
 * socket.c
 *
 *  Created on: 29.08.2012
 *      Author: user
 */


#include "../socket_p.h"
#include <cwt/net/socket.h>
#include <cwt/logger.h>

BOOL __socket_allowSockets(void)
{
	if( !__socket_is_sockets_allowed ) {
		CwtLoggerNS *logger = cwt_logger_ns();

		WORD version;
		WSADATA wsaData;
		int res;

		version = MAKEWORD(2, 2);
		res = WSAStartup(MAKEWORD(2, 2), &wsaData);

		if( res != NO_ERROR ) {
			logger->error(_Tr("WSAStartup(): %d"), res);
			return FALSE;
		}

		/* Confirm that the WinSock DLL supports 2.2.*/
		/* Note that if the DLL supports versions greater    */
		/* than 2.2 in addition to 2.2, it will still return */
		/* 2.2 in wVersion since that is the version we      */
		/* requested.                                        */

		if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
			/* Tell the user that we could not find a usable */
			/* WinSock DLL.                                  */
			logger->error(_Tr("could not find a usable version of Winsock.dll, required version 2.2 or greater"));
			WSACleanup();
			return FALSE;
		}
	}

	__socket_is_sockets_allowed = TRUE;
	return __socket_is_sockets_allowed;
}

BOOL __socket_setNonBlockingNative (SOCKET sockfd, BOOL is_nonblocking)
{
	return ioctlsocket(sockfd, FIONBIO, (u_long*)&is_nonblocking) < 0 ? FALSE : TRUE;
}
