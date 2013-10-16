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

CWT_NS_BEGIN

size_t NativeSocket::bytesAvailable() const
{
	int nbytes = 0;
	CWT_ASSERT(sockfd >= 0 );
	int rc = 0;
	CWT_VERIFY_ERRNO((rc = ioctl(sockfd, FIONREAD, & nbytes)) == 0, errno);
	return nbytes;
}


bool NativeSocket::open (int socktype, int proto, int32_t oflags)
{
	if (oflags & io::Device::NonBlocking)
		socktype |= SOCK_NONBLOCK;

	CWT_VERIFY_ERRNO((sockfd = ::socket(AF_INET, socktype, proto)) >= 0, errno);

	return sockfd >= 0 ? true : false;
}

bool NativeSocket::close()
{
	int rc = 0;
	CWT_VERIFY_ERRNO((rc = ::close(sockfd)) == 0, errno);
	sockfd = -1;
	return rc == 0;
}

bool NativeInetSocket::open (const String & hostname, uint16_t port, int socktype, int proto, int32_t oflags)
{
	if (NativeSocket::open(socktype, proto, oflags)) {
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
				Logger::error(_Fr("Can't resolve host name: %s") % hostname);
				CWT_VERIFY_ERRNO(::close(sockfd) == 0, errno);
				sockfd = -1;
				return false;
			}
		}
	}

	return true;
}

CWT_NS_END
