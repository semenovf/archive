/*
 * inetsocket_unix.cpp
 *
 *  Created on: Oct 14, 2013
 *      Author: wladt
 */

#include "inetsocket_unix.hpp"
#include <cwt/logger.hpp>
#include <cwt/safeformat.hpp>
#include <netinet/in.h>
#include <cerrno>
#include <cstring>

CWT_NS_BEGIN

namespace io {

InetSocket::InetSocket() : pimpl(new InetSocket::Impl)
{}

uint32_t InetSocket::ipv4 () const
{
	if (opened()) {
		return ntohl(pimpl->saddr.sin_addr.s_addr);
	}
	return 0;
}

String InetSocket::hostname () const
{
	return ipv4name(); // TODO need resolve name by IP
}

String InetSocket::ipv4name () const
{
	if (opened()) {
		uint32_t ip = ipv4();
		return 	_F("%d.%d.%d.%d")
				% ((ip >> 24) & 0xFF)
				% ((ip >> 16) & 0xFF)
				% ((ip >> 8)  & 0xFF)
				% (ip  & 0xFF);
	}
	return String();
}


uint16_t InetSocket::port () const
{
	return opened()
		? pimpl->saddr.sin_port
		: 0;
}

size_t InetSocket::bytesAvailable() const
{
	return pimpl->bytesAvailable();
}


bool InetSocket::deviceIsOpened () const
{
	return pimpl->opened();
}

#ifdef __COMMENT__

//extern bool init_inet_sockaddr (struct sockaddr_in & sockaddr, const HostAddress & hostAddr, uint16_t port);

bool Socket::Impl::open (int oflags)
{
	int type = 0;
	int domain = 0;
	int proto = 0;

	if ((oflags & SocketMask) == TcpSocket) {
		type |= SOCK_STREAM;
		domain = AF_INET;
		proto = PF_INET;
		this->socktype = TcpSocket;
	} else if ((oflags & SocketMask) == LocalSocket) {
		type |= SOCK_STREAM;
		domain = AF_LOCAL;
		proto = PF_LOCAL;
		this->socktype = LocalSocket;
	} else if ((oflags & SocketMask) == UdpSocket) {
		type |= SOCK_DGRAM;
		domain = AF_INET;
		proto = PF_INET;
		this->socktype = UdpSocket;
	} else if ((oflags & SocketMask) == LocalDgramSocket) {
		type |= SOCK_DGRAM;
		domain = AF_LOCAL;
		proto = PF_LOCAL;begin
		this->socktype = LocalDgramSocket;
	} else {
		Logger::error(_Tr("Unspecified socket type: must be stream, datagram or raw"));
		return false;
	}

	if (oflags & Device::NonBlocking)
		type |= SOCK_NONBLOCK;

	this->sockfd = socket(domain, type, proto);

	if (this->sockfd < 0) {
		Logger::error(errno, _Tr("Failed to open socket"));
		return false;
	}

	bool rc = true;

	if (oflags & Listener)
		this->isListener = true;

	// TODO add UDP and Local socket support
	if (this->isListener) {
		if (this->socktype == TcpSocket)
			rc = listenTcpSocket();
	} else {
		if (this->socktype == TcpSocket)
			rc = connectTcpSocket();
	}

	if (!rc) {
		::close(sockfd);
		sockfd = -1;
		return false;
	}

	return true;
}

bool Socket::close ()
{
	bool r = true;

	if (pimpl->sockfd <= 0)
		return true;

	/* leave multicast receiver socket group */
/*
	if( Cwt_McastSocket == sd->type && sd->is_listener ) {
		CwtMcastSocket *msd = (CwtMcastSocket*)sd;
		if( setsockopt (msd->sockfd, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char *)&msd->group.mreq, sizeof(msd->group.mreq)) < 0 ) {
			__logger->error(_Tr("Dropping multicast group")
				_CWT_SOCKET_LOG_FMTSUFFIX
				, _CWT_SOCKET_LOG_ARGS);
		}
	}
*/

	if (pimpl->sockfd > 0)
		CWT_VERIFY_ERRNO(::close(pimpl->sockfd) == 0, errno);

	pimpl->sockfd = -1;

	// Unlink local file for listener
	if (pimpl->isListener
			&& (pimpl->socktype == Socket::LocalSocket
					|| pimpl->socktype == Socket::LocalDgramSocket)) {

		//CWT_VERIFY_ERRNO(unlink(pimpl->hostAddr.hostname().c_str()) == 0, errno);
	}

	return r;
}

// for TCP and LocalSocket
ssize_t Socket::readBytes (char bytes[], size_t n)
{
	ssize_t r;

	CWT_ASSERT(pimpl->sockfd >= 0);

	n = CWT_MIN(n, CWT_INT_MAX);
	r = recv(pimpl->sockfd, bytes, n, 0);

	if (r < 0) {
		Logger::error(errno, _Tr("Receive data error"));
	}

	return r;
}

// for TCP and LocalSocket
ssize_t Socket::writeBytes(const char bytes[], size_t n)
{
	ssize_t r;
	CWT_ASSERT(pimpl->sockfd >= 0);

	r = send(pimpl->sockfd, bytes, n, 0);

	if (r < 0) {
		Logger::error(errno, _Tr("Send data error"));
	}

    return r;
}
#endif

} // namespace io

CWT_NS_END
