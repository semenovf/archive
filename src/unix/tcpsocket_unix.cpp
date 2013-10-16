/*
 * tcpsocket_unix.cpp
 *
 *  Created on: Oct 15, 2013
 *      Author: wladt
 */

#include "inetsocket_unix.hpp"
#include <cwt/logger.hpp>
#include <cwt/safeformat.hpp>
#include <cerrno>
#include <sys/socket.h>


CWT_NS_BEGIN

namespace io {

bool TcpSocket::open (const String & hostname, uint16_t port, int32_t oflags)
{
	if (pimpl->open(hostname, port, SOCK_STREAM, 0, oflags)) {
		int rc = 0;

		CWT_VERIFY_ERRNO((rc = ::connect(pimpl->sockfd
				, reinterpret_cast<struct sockaddr *>(& pimpl->saddr)
				, sizeof(pimpl->saddr))) == 0, errno);

		if (rc == 0)
			return true;

		/* TODO
 		    errno == EINPROGRESS:
			The socket is nonblocking and the connection cannot be completed immediately.
			It is possible to select(2) or poll(2) for completion by selecting
			the socket for writing. After select(2) indicates writability,
			use getsockopt(2) to read the SO_ERROR option at level SOL_SOCKET
			to determine whether connect() completed successfully (SO_ERROR is zero) or
			unsuccessfully (SO_ERROR is one of the usual error codes listed here,
			explaining the reason for the failure).
		*/
		if (rc != 0 && errno == EINPROGRESS) { // TODO implement
#if CWT_HAVE_POLL
			;
#endif
		}

		Logger::error(errno, _Fr("Connection to %s:%u failed")
				% hostname
				% port);
		pimpl->close();
	}

	return false;
}

ssize_t TcpSocket::readBytes (char bytes[], size_t n)
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

ssize_t TcpSocket::writeBytes(const char bytes[], size_t n)
{
	ssize_t r;
	CWT_ASSERT(pimpl->sockfd >= 0);

	r = send(pimpl->sockfd, bytes, n, 0);

	if (r < 0) {
		Logger::error(errno, _Tr("Send data error"));
	}

    return r;
}


bool TcpSocket::closeDevice ()
{
	if (pimpl->sockfd < 0)
		return true;

	int rc = 0;
	CWT_VERIFY_ERRNO((rc = shutdown(pimpl->sockfd, SHUT_RDWR)) == 0, errno);
	pimpl->close();
	return rc == 0;
}

} // namespace io

CWT_NS_END


