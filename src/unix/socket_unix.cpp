/*
 * socket_posix.cpp
 *
 *  Created on: Oct 14, 2013
 *      Author: wladt
 */

#include "../include/cwt/io/socket.hpp"
#include <cwt/logger.hpp>
#include <cwt/safeformat.hpp>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cerrno>

/*

#include <cwt/logger.hpp>
#include <cerrno>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
*/

CWT_NS_BEGIN

namespace io {

struct Socket::Impl
{
	Impl() : isListener(false), socktype(Socket::DefaultSocketType), hostAddr(), port(0), sockfd(-1) {}

	bool open (int oflags);
	bool listenTcpSocket ();  // for server socket
	bool connectTcpSocket (); // for client socket

	bool               isListener;
	Socket::SocketType socktype;
	HostAddress        hostAddr;
	uint16_t           port;
	int                sockfd;
};

extern bool init_inet_sockaddr (struct sockaddr_in & sockaddr, const HostAddress & hostAddr, uint16_t port);

bool Socket::Impl::open (int oflags)
{
	int type = 0;
	int domain = 0;

	if ((oflags & SocketMask) == TcpSocket) {
		type |= SOCK_STREAM;
		domain = AF_INET;
		this->socktype = TcpSocket;
	} else if ((oflags & SocketMask) == LocalSocket) {
		type |= SOCK_STREAM;
		domain = AF_UNIX;
		this->socktype = LocalSocket;
	} else if ((oflags & SocketMask) == UdpSocket) {
		type |= SOCK_DGRAM;
		domain = AF_INET;
		this->socktype = UdpSocket;
	} else if ((oflags & SocketMask) == LocalDgramSocket) {
		type |= SOCK_DGRAM;
		domain = AF_UNIX;
		this->socktype = LocalDgramSocket;
	} else {
		Logger::error(_Tr("Unspecified socket type: must be stream, datagram or raw"));
		return false;
	}

	if (oflags & Device::NonBlocking)
		type |= SOCK_NONBLOCK;

	this->sockfd = socket(domain, type, 0);

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

bool Socket::Impl::listenTcpSocket ()
{
	int rc = 0;

    /* http://publib.boulder.ibm.com/infocenter/iseries/v5r3/topic/rzab6/rzab6xconoserver.htm
     *
     * The setsockopt() function is used to allow the local address to
     * be reused when the server is restarted before the required wait
     * time expires
     */
	int reuse = 1;
	rc = ::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *) & reuse, sizeof(reuse));
	if (rc < 0)	{
		Logger::error(errno, _Tr("Setting the socket to reusable address mode (SO_REUSEADDR) failed"));
		return false;
	}

	struct sockadd_in sockaddr;

	if (!init_inet_sockaddr(& sockaddr, hostAddr, port))
		return false;

	/* Bind the socket. */
	rc = bind(sockfd, reinterpret_cast<struct sockaddr *>(sockaddr), sizeof(sockaddr));
	if( rc < 0 ) {
		Logger::error(errno, _Tr("bind failed"));
		return false;
	}

	rc = ::listen(sockfd, 10);
	if (rc < 0) {
		Logger::error(errno, _Tr("Failed to listen connections on socket"));
		return false;
	}
	return true;
}

bool Socket::Impl::connectTcpSocket ()
{
	int rc = 0;

	struct sockadd_in sockaddr;

	if (!init_inet_sockaddr(& sockaddr, hostAddr, port))
		return false;

	rc = connect(sockfd, reinterpret_cast<struct sockaddr *>(& sockaddr), sizeof(sockaddr));

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
		Logger::error(errno, _Fr("Connection to %s:%u failed")
			% hostAddr.toString()
			% port);
		return false;
	}

	return true;
}

Socket::Socket() : pimpl(new Socket::Impl)
{}

bool Socket::opened() const
{
	return pimpl->sockfd >= 0;
}

void Socket::flush()
{
	// TODO need to implement

	//	if (m_fd > 0) { // as for File
	//		::fsync(m_fd);
	//	}

}

bool Socket::open(const String & path, int32_t oflags)
{
	pimpl->hostAddr.setPath(path);
	return pimpl->open(oflags);
}

bool Socket::open (const String & hostname, uint16_t port, int32_t oflags)
{
	pimpl->hostAddr.setAddress(hostname);
	pimpl->port = port;
	return pimpl->open(oflags);
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

	if (pimpl->socktype == Socket::TcpSocket)
		CWT_VERIFY_ERRNO(shutdown(pimpl->sockfd, SHUT_RDWR) == 0, errno);

	if (pimpl->sockfd > 0)
		CWT_VERIFY_ERRNO(::close(pimpl->sockfd) == 0, errno);

	pimpl->sockfd = -1;

	// Unlink local file for listener
	if (pimpl->isListener
			&& (pimpl->socktype == Socket::LocalSocket
					|| pimpl->socktype == Socket::LocalDgramSocket)) {

		CWT_VERIFY_ERRNO(unlink(pimpl->hostAddr.hostname().c_str()) == 0, errno);
	}

	return r;
}

size_t Socket::bytesAvailable() const
{
	int nbytes = 0;
	CWT_ASSERT(pimpl->sockfd >= 0 );
	ioctl(pimpl->sockfd, FIONREAD, & nbytes);
	return nbytes;
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

} // namespace io

CWT_NS_END
