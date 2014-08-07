/*
 * tcpsocket_unix.cpp
 *
 *  Created on: Oct 15, 2013
 *      Author: wladt
 */

#include "inetsocket_unix.hpp"
#include <cwt/platform.hpp>
#include <pfs/safeformat.hpp>
#include <cerrno>
#include <sys/socket.h>


namespace cwt { namespace io {

bool tcp_socket::open (const pfs::string & hostname, uint16_t port, int32_t oflags)
{
	if (_pimpl->open(hostname, port, SOCK_STREAM, 0, oflags)) {
		int rc = 0;

		CWT_VERIFY_ERRNO_X(0 == (rc = ::connect(_pimpl->sockfd
				, reinterpret_cast<struct sockaddr *>(& _pimpl->saddr)
				, sizeof(_pimpl->saddr))), errno);

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

		pfs::string errmsg;
		errmsg << hostname << pfs::string(1, ':')
			<< pfs::string::number(port)
			<< _Tr(": connection failure");
		this->addSystemError(errno, errmsg);
		_pimpl->close();
	}

	return false;
}

ssize_t tcp_socket::readBytes (char bytes[], size_t n)
{
	ssize_t r;

	PFS_ASSERT(_pimpl->sockfd >= 0);

	n = pfs::min(n, size_t(pfs::max_type<int>()));
	r = recv(_pimpl->sockfd, bytes, n, 0);

	if (r < 0) {
		pfs::string errmsg;
		errmsg << _Tr("receive data error");
		this->addSystemError(errno, errmsg);
	}

	return r;
}

ssize_t tcp_socket::writeBytes (const char bytes[], size_t n)
{
	ssize_t r;
	PFS_ASSERT(_pimpl->sockfd >= 0);

	r = send(_pimpl->sockfd, bytes, n, 0);

	if (r < 0) {
		pfs::string errmsg;
		errmsg << _Tr("send data error");
		this->addSystemError(errno, errmsg);
	}

    return r;
}


bool tcp_socket::closeDevice ()
{
	if (_pimpl->sockfd < 0)
		return true;

	int rc = 0;
	CWT_VERIFY_ERRNO_X(0 == (rc = shutdown(_pimpl->sockfd, SHUT_RDWR)), errno);
	_pimpl->close();
	return rc == 0;
}

}} // cwt::io

