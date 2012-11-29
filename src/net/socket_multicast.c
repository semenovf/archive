/*
 * socket_multicast.c
 *
 *  Created on: 18.11.2011
 *      Author: user
 */

#include "socket_p.h"
#include <string.h>
#include <cwt/str.h>
#include <cwt/logger.h>
#include <cwt/txtcodec.h>


static BOOL __initListener(CwtMcastSocket *msd
		, const char *inetAddrLatin1
		, UINT16 port
		, const char *inetMcastAddrLatin1)
{
	BOOL ok = FALSE;

	do {
		int rc;
		int reuse = 1;
		struct sockaddr_in localSock;

		/* Enable SO_REUSEADDR to allow multiple instances of this */
	    /* application to receive copies of the multicast datagrams. */
		rc = setsockopt(msd->sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse));
		if (rc < 0) {
			cwt_logger_ns()->error(_Tr("setting SO_REUSEADDR error")
					_CWT_SOCKET_LOG_FMTSUFFIX
					, _CWT_SOCKET_LOG_ARGS);
			break;
		}

		/* Bind to the proper port number with the IP address */
		/* specified as INADDR_ANY. */
		memset((char *) &localSock, 0, sizeof(localSock));
		localSock.sin_family = AF_INET;
		localSock.sin_port = htons(port);
		localSock.sin_addr.s_addr = htonl(INADDR_ANY);

		rc = bind(msd->sockfd, (struct sockaddr*)&localSock, sizeof(localSock));
		if (rc < 0) {
			cwt_logger_ns()->error(_Tr("binding datagram socket error")
					_CWT_SOCKET_LOG_FMTSUFFIX
					, _CWT_SOCKET_LOG_ARGS);
			break;
		}

		/* Join the multicast group 226.1.1.1 on the local 203.106.93.94 */
		/* interface. Note that this IP_ADD_MEMBERSHIP option must be */
		/* called for each local interface over which the multicast */
		/* datagrams are to be received. */

		msd->group.mreq.imr_multiaddr.s_addr = inet_addr(inetMcastAddrLatin1);
		msd->group.mreq.imr_interface.s_addr = inet_addr(inetAddrLatin1);

		rc = setsockopt(msd->sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&msd->group.mreq, sizeof(msd->group.mreq));
		if (rc < 0) {
			cwt_logger_ns()->error(_Tr("adding multicast group error (%d)")
				_CWT_SOCKET_LOG_FMTSUFFIX
				, _CWT_SOCKET_LOG_ARGS);
			break;
		}

		msd->is_listener = TRUE;
		ok = TRUE;
	} while(FALSE);

	return ok;
}


static BOOL __initClient(
		CwtMcastSocket *msd
		, const char *inetAddrLatin1
		, UINT16 port
		, const char *inetMcastAddrLatin1)
{
	struct in_addr localInterface;

	/* Initialize the group sockaddr structure with a */
	/* group address of 225.1.1.1 and port 5555. */
	memset((char *) &msd->group.sockaddr, 0, sizeof(msd->group.sockaddr));
	msd->group.sockaddr.sin_family = AF_INET;
	msd->group.sockaddr.sin_addr.s_addr = inet_addr(inetMcastAddrLatin1); /*inet_addr("226.1.1.1");*/
	msd->group.sockaddr.sin_port = htons(port);

	/* Set local interface for outbound multicast datagrams. */
	/* The IP address specified must be associated with a local, */
	/* multicast capable interface. */
	localInterface.s_addr = inet_addr(inetAddrLatin1); /*inet_addr("192.168.0.198");*/

	if(setsockopt(msd->sockfd, IPPROTO_IP, IP_MULTICAST_IF, (char *)&localInterface, sizeof(localInterface)) < 0) {
		cwt_logger_ns()->error(_Tr("setting local interface error (%d)")
				_CWT_SOCKET_LOG_FMTSUFFIX
				, _CWT_SOCKET_LOG_ARGS);
		return FALSE;
	}

	return TRUE;
}

static CwtSocket* __socket_openMcastSocketHelper(
		const CWT_CHAR *inetAddr
		, UINT16 port
		, const CWT_CHAR *inetMcastAddr
		, BOOL is_listener
		, BOOL is_nonblocking)
{
	BOOL ok = FALSE;
    CwtSocket *sd;

	sd = __socket_openTypified(Cwt_McastSocket, is_nonblocking);

	if (sd) {
		char *inetAddrLatin1;
		char *inetMcastAddrLatin1;

		inetAddrLatin1      = cwt_textcodec_ns()->toLatin1(inetAddr, cwt_str_ns()->strLen(inetAddr));
		inetMcastAddrLatin1 = cwt_textcodec_ns()->toLatin1(inetMcastAddr, cwt_str_ns()->strLen(inetMcastAddr));

		if( is_listener ) {
			ok = __initListener((CwtMcastSocket *)sd
					, inetAddrLatin1
					, port
					, inetMcastAddrLatin1);
		} else {
			ok = __initClient((CwtMcastSocket *)sd
					, inetAddrLatin1
					, port
					, inetMcastAddrLatin1);
		}
		CWT_FREE(inetAddrLatin1);
		CWT_FREE(inetMcastAddrLatin1);
		if (!ok) {
			cwt_socket_ns()->close(sd);
			sd = NULL;
		}
	}

	return sd;
}


CwtSocket* __socket_openMcastSocket(
		const CWT_CHAR *inetAddr
		, UINT16 port
		, const CWT_CHAR *inetMcastAddr
		, BOOL is_nonblocking)
{
	return __socket_openMcastSocketHelper(
			inetAddr
			, port
			, inetMcastAddr
			, FALSE
			, is_nonblocking);
}


CwtSocket* __socket_openMcastServerSocket(
		const CWT_CHAR *inetAddr
		, UINT16 port
		, const CWT_CHAR *inetMcastAddr
		, BOOL is_nonblocking)
{
	return __socket_openMcastSocketHelper(
			inetAddr
			, port
			, inetMcastAddr
			, TRUE
			, is_nonblocking);
}


CwtSocket* __socket_acceptMcastSocket(CwtSocket *sd)
{
	/* FIXME need to implement */
	CWT_UNUSED(sd);
/*	SOCKET client;
	ssize_t rc;
	int flags;
	UINT senderSize = sizeof(struct sockaddr_in);
	BYTE buf[1];
	CwtUdpSocket *sd_udp;

	CWT_ASSERT(sd);
	CWT_ASSERT(Cwt_UdpSocket == sd->type);

	sd_udp = CWT_MALLOC(CwtUdpSocket);

	 Peeks at the incoming data. The data is copied into the buffer
	 * but is not removed from the input queue.
	 *
	 * See http://msdn.microsoft.com/en-us/library/windows/desktop/ms740120%28v=vs.85%29.aspx

	flags = MSG_PEEK;
	rc = recvfrom(sd->sockfd, buf, sizeof(buf), flags,
			(struct sockaddr *)&sd_udp->sockaddr,
			&senderSize);

	if( rc >= 0 ) {
		sd_udp->sockfd      = dup(sd->sockfd);
		sd_udp->type        = sd->type;
		sd_udp->is_listener = FALSE;

		__socket_nsockets_opened++;

		return (CwtSocket*)sd_udp;
	}

	CWT_FREE(sd_udp);
	cwtLoggerNS()->error(_Tr("failed to accept UDP socket")
		_CWT_SOCKET_LOG_FMTSUFFIX
		, _CWT_SOCKET_LOG_ARGS);*/

	return NULL;
}


ssize_t __socket_readMcastSocket(CwtSocket *sd, BYTE *buf, size_t sz)
{
	CWT_UNUSED3(sd, buf, sz);
	/* FIXME need to implement */
	return -1;
}

ssize_t __socket_writeMcastSocket(CwtSocket *sd, const BYTE *buf, size_t sz)
{
	CwtMcastSocket *msd = (CwtMcastSocket*)sd;
	ssize_t bw = (ssize_t)-1;

	CWT_ASSERT(sd);
	CWT_ASSERT(sz <= CWT_INT_MAX);

	if (!sd->is_listener) {
		bw = sendto(msd->sockfd, buf, (int)sz, 0
			, (struct sockaddr*)&msd->group.sockaddr
			, sizeof(msd->group.sockaddr));
	} else {
		/* FIXME need to implement */
	}

    return bw;
}
