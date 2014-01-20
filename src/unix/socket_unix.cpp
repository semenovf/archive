/*
 * socket_unix.cpp
 *
 *  Created on: Oct 16, 2013
 *      Author: wladt
 */

#include "socket_unix.hpp"
#include "../../include/cwt/io/device.hpp"
#include "../../include/cwt/net/hostinfo.hpp"
#include <cwt/logger.hpp>
#include <arpa/inet.h>
#include <unistd.h>
#include <cerrno>
#include <sys/types.h>
#include <sys/ioctl.h>

namespace cwt { namespace io {

size_t native_socket::bytesAvailable() const
{
	int nbytes = 0;
	PFS_ASSERT(sockfd >= 0 );
	int rc = 0;
	PFS_VERIFY_ERRNO((rc = ioctl(sockfd, FIONREAD, & nbytes)) == 0, errno);
	return nbytes;
}


bool native_socket::open (int socktype, int proto, int32_t oflags)
{
	if (oflags & device::NonBlocking)
		socktype |= SOCK_NONBLOCK;

	PFS_VERIFY_ERRNO((sockfd = ::socket(AF_INET, socktype, proto)) >= 0, errno);

	return sockfd >= 0 ? true : false;
}

bool native_socket::close()
{
	int rc = 0;
	PFS_VERIFY_ERRNO((rc = ::close(sockfd)) == 0, errno);
	sockfd = -1;
	return rc == 0;
}

bool native_inet_socket::open (const pfs::string & hostname, uint16_t port, int socktype, int proto, int32_t oflags)
{
	if (native_socket::open(socktype, proto, oflags)) {
		memset(& saddr, sizeof(saddr), 0);
		saddr.sin_family = AF_INET;
		saddr.sin_port   = htons(port);

		if (hostname.isEmpty()) {
			saddr.sin_addr.s_addr = htonl(INADDR_ANY);
		} else {
			cwt::net::addrinfo_iterator it = cwt::net::addrinfo_iterator::begin(hostname);

			if (it != cwt::net::addrinfo_iterator::end()) {
				saddr.sin_addr.s_addr = htonl(it.ipv4());
			} else {
				pfs::string errmsg(hostname);
				errmsg << ": can't resolve host name";
				PFS_ERROR(errmsg.c_str());
				PFS_VERIFY_ERRNO(::close(sockfd) == 0, errno);
				sockfd = -1;
				return false;
			}
		}
	}

	return true;
}

}} // cwt::io
