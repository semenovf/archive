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

namespace cwt { namespace io {

inet_socket::inet_socket() : _pimpl(new inet_socket::impl)
{}

uint32_t inet_socket::ipv4 () const
{
	if (opened()) {
		return ntohl(_pimpl->saddr.sin_addr.s_addr);
	}
	return 0;
}

pfs::string inet_socket::hostname () const
{
	return ipv4name(); // TODO need resolve name by IP
}

pfs::string inet_socket::ipv4name () const
{
	if (opened()) {
		uint32_t ip = ipv4();
		pfs::string r;
		r << pfs::string::number(((ip >> 24) & 0xFF))
			<< pfs::string(1, '.') << pfs::string::number(((ip >> 16) & 0xFF))
			<< pfs::string(1, '.') << pfs::string::number(((ip >> 8)  & 0xFF))
			<< pfs::string(1, '.') << pfs::string::number((ip  & 0xFF));
		return r;
	}
	return pfs::string();
}


uint16_t inet_socket::port () const
{
	return opened()
		? _pimpl->saddr.sin_port
		: 0;
}

size_t inet_socket::bytesAvailable () const
{
	return _pimpl->bytesAvailable();
}


bool inet_socket::deviceIsOpened () const
{
	return _pimpl->opened();
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

}} // cwt::io
