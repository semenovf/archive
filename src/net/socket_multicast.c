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


CwtSocket* __socket_openMSocket (BOOL is_nonblocking)
{
	return __socket_openTypified(Cwt_McastSocket, is_nonblocking);
}


static BOOL __socket_listenOrConnect(CwtSocket *sd, const CWT_CHAR *inetAddr, UINT16 port, const CWT_CHAR *inetMCastAddr, BOOL is_listener)
{
	BOOL ok = FALSE;

	char *inetAddrLatin1;
	char *inetMCastAddrLatin1;
	CwtMcastSocket *msd;

	CWT_ASSERT(sd);

	msd = (CwtMcastSocket*)sd;

	if( Cwt_McastSocket != sd->type ) {
		cwtLoggerNS()->error(_Tr("expected multicast socket for listen"));
		return FALSE;
	}

	inetAddrLatin1      = cwtTextCodecNS()->toLatin1(inetAddr, cwtStrNS()->strlen(inetAddr));
	inetMCastAddrLatin1 = cwtTextCodecNS()->toLatin1(inetMCastAddr, cwtStrNS()->strlen(inetMCastAddr));

	ok = FALSE;

	if( is_listener ) {
		do {
			int rc;
			int reuse = 1;
			struct sockaddr_in localSock;

			/* Enable SO_REUSEADDR to allow multiple instances of this */
		    /* application to receive copies of the multicast datagrams. */
			rc = setsockopt(msd->sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse));
			if (rc < 0) {
				cwtLoggerNS()->error(_Tr("setting SO_REUSEADDR error (%d)"), __socket_errno);
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
				cwtLoggerNS()->error(_Tr("binding datagram socket error (%d)"), __socket_errno);
				break;
			}

			/* Join the multicast group 226.1.1.1 on the local 203.106.93.94 */
			/* interface. Note that this IP_ADD_MEMBERSHIP option must be */
			/* called for each local interface over which the multicast */
			/* datagrams are to be received. */

			msd->group.mreq.imr_multiaddr.s_addr = inet_addr(inetMCastAddrLatin1);
			msd->group.mreq.imr_interface.s_addr = inet_addr(inetAddrLatin1);

			rc = setsockopt(msd->sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&msd->group.mreq, sizeof(msd->group.mreq));
			if (rc < 0) {
				cwtLoggerNS()->error(_Tr("adding multicast group error (%d)"), __socket_errno);
				break;
			}

			msd->is_listener = TRUE;
			ok = TRUE;
		} while(FALSE);
	} else {
		do {
			struct in_addr localInterface;

			/* Initialize the group sockaddr structure with a */
			/* group address of 225.1.1.1 and port 5555. */
			memset((char *) &msd->group.sockaddr, 0, sizeof(msd->group.sockaddr));
			msd->group.sockaddr.sin_family = AF_INET;
			msd->group.sockaddr.sin_addr.s_addr = inet_addr(inetMCastAddrLatin1); /*inet_addr("226.1.1.1");*/
			msd->group.sockaddr.sin_port = htons(port);

			/* Set local interface for outbound multicast datagrams. */
			/* The IP address specified must be associated with a local, */
			/* multicast capable interface. */
			localInterface.s_addr = inet_addr(inetAddrLatin1); /*inet_addr("192.168.0.198");*/

		    if(setsockopt(msd->sockfd, IPPROTO_IP, IP_MULTICAST_IF, (char *)&localInterface, sizeof(localInterface)) < 0) {
		    	cwtLoggerNS()->error(_Tr("setting local interface error (%d)"), __socket_errno);
		        break;
		    }

		    ok = TRUE;
		} while (FALSE);
	}

	CWT_FREE(inetAddrLatin1);
	CWT_FREE(inetMCastAddrLatin1);

	return ok;
}


BOOL __socket_listenMSocket(CwtSocket *sd, const CWT_CHAR *inetAddr, UINT16 port, const CWT_CHAR *inetMCastAddr)
{
	return __socket_listenOrConnect(sd, inetAddr, port, inetMCastAddr, TRUE);
}



BOOL __socket_connectMSocket (CwtSocket *sd, const CWT_CHAR *inetAddr, UINT16 port, const CWT_CHAR *inetMCastAddr)
{
	return __socket_listenOrConnect(sd, inetAddr, port, inetMCastAddr, FALSE);
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
	ssize_t bw;

	CWT_ASSERT(sd);
	CWT_ASSERT(sz <= CWT_INT_MAX);

	if (!sd->is_listener) {
		bw = sendto(msd->sockfd, buf, (int)sz, 0
			, (struct sockaddr*)&msd->group.sockaddr
			, sizeof(msd->group.sockaddr));
	} else {
		/* FIXME need to implement */
	}
    /*return bw;*/

	return -1;
}
