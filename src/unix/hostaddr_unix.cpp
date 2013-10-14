/*
 * hostaddr_unix.cpp
 *
 *  Created on: Oct 14, 2013
 *      Author: wladt
 */

#include "../../include/cwt/io/socket.hpp"
#include <cwt/logger.hpp>
#include <cwt/safeformat.hpp>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

CWT_NS_BEGIN

namespace io {

String HostAddress::toString () const
{
	return m_hostname;
}


bool HostAddress::resolve ()
{
	bool r = true;

	// Translate hostname into IP address
	if (m_ipv4 == InvalidIpv4) {
		if (m_hostname.isEmpty())
			return false;
	}

/*
	struct hostent * phost;

	phost = gethostbyname(m_hostname.c_str());

	if (phost == (struct hostent *)NULL) {
		//Logger::error(_Fr("%s: host not found") % hostname);
		return false;
	}

			memcpy(& sockaddr.sin_addr, phost->h_addr, sizeof(sockaddr.sin_addr));

		} while(false);
	}*/
	return r;
}


bool init_inet_sockaddr (struct sockaddr_in & sockaddr, const HostAddress & /*hostAddr*/, uint16_t port)
{
	memset(& sockaddr, sizeof(sockaddr), 0);
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port   = htons(port);

/*	switch (hostAddr.type()) {
	case HostAddress::HostName:
		if (hostAddr.hostname().isEmpty()) {
			sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
		} else {
			const char * hostname = hostAddr.hostname().c_str();
			if( !inet_aton(hostname, & sockaddr.sin_addr) ) {
				do {
					struct hostent * phost;

					phost = gethostbyname(hostname);

					if (phost == (struct hostent *)NULL) {
						Logger::error(_Fr("%s: host not found") % hostname);
						return false;
					}

					memcpy(& sockaddr.sin_addr, phost->h_addr, sizeof(sockaddr.sin_addr));

				} while(false);
			}
		}
		break;
	case HostAddress::IPv4:
		sockaddr.sin_addr.s_addr = htonl(hostAddr.ipv4());
		break;
	case HostAddress::IPv6:
		Logger::error("IPv6 address unsupported yet");
		return false;
	case HostAddress::LocalPath: // for local socket (AF_UNIX domain for POSIX sockets)
	default:
		Logger::error("Unsupported address type");
		return false;
	}*/

	return true;
}

} // namespace io

CWT_NS_END
