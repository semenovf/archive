/*
 * socket_p.c
 *
 *  Created on: 30.08.2012
 *      Author: user
 */

#include "socket_p.h"
#include <cwt/logger.h>
#include <cwt/str.h>
#include <cwt/txtcodec.h>

CwtSocket* __socket_openTypified(CwtSocketType socketType, BOOL is_nonblocking)
{
	SOCKET sockfd;
	CwtSocket *sd;

	int nativeType;
	int domain = AF_INET;

	if( !__socket_allowSockets() ) {
		cwt_logger_ns()->error(_Tr("network sockets not allowed in this system"));
		return NULL;
	}

	domain = socketType != Cwt_LocalSocket
			? AF_INET
			: AF_UNIX;

	nativeType = (socketType == Cwt_TcpSocket || socketType == Cwt_LocalSocket)
			? SOCK_STREAM
			: SOCK_DGRAM;

	sockfd = socket(domain, nativeType, 0);
    if( sockfd < 0 ) {
    	cwt_logger_ns()->error(_Tr("failed to open socket")
    			_CWT_SOCKET_LOG_FMTSUFFIX
    			, _CWT_SOCKET_LOG_ARGS);
		return NULL;
    }

	if( is_nonblocking ) {
		if (!__socket_setNonBlockingNative(sockfd, is_nonblocking)) {
			cwt_logger_ns()->error(_Tr("set/unset socket non-blocking mode failed"));
			__socket_closeNative(sockfd);
			return NULL;
		}
	}

	switch(socketType) {
	case Cwt_LocalSocket:
		sd = (CwtSocket*)CWT_MALLOC(CwtLocalSocket);
		cwt_bzero(sd, sizeof(CwtLocalSocket));
		break;
	case Cwt_McastSocket:
		sd = (CwtSocket*)CWT_MALLOC(CwtMcastSocket);
		cwt_bzero(sd, sizeof(CwtMcastSocket));
		break;
	case Cwt_UdpSocket:
		sd = (CwtSocket*)CWT_MALLOC(CwtUdpSocket);
		cwt_bzero(sd, sizeof(CwtUdpSocket));
		break;
	case Cwt_TcpSocket:
	default:
		sd = (CwtSocket*)CWT_MALLOC(CwtTcpSocket);
		cwt_bzero(sd, sizeof(CwtTcpSocket));
		break;
	}

	sd->sockfd      = sockfd;
	sd->type        = socketType;
	sd->is_listener = FALSE;


	__socket_nsockets_opened++;

	return (CwtSocket*)sd;
}


BOOL __socket_initSockAddrIn(struct sockaddr_in *saddr, const CWT_CHAR *inetAddr, UINT16 port)
{
	BOOL ok = TRUE;

	cwt_bzero(saddr, sizeof(*saddr));
	saddr->sin_family      = AF_INET;
	saddr->sin_port        = htons(port);

	if (!inetAddr) {
		saddr->sin_addr.s_addr = htonl(INADDR_ANY);
	} else {
		char *inetAddrLatin1;
		inetAddrLatin1 = cwt_textcodec_ns()->toLatin1(inetAddr, cwt_str_ns()->strLen(inetAddr));

		if( !inet_aton(inetAddrLatin1, &saddr->sin_addr) ) {
			do {
				struct hostent *phost;

				phost = gethostbyname(inetAddrLatin1);

				if (phost == (struct hostent *)NULL) {
					cwt_logger_ns()->error(_Tr("%s: host not found"), inetAddr);
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

