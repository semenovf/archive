/*
 * hostinfo_unix.cpp
 *
 *  Created on: Oct 14, 2013
 *      Author: wladt
 */

#include "../../include/cwt/io/socket.hpp"
#include <cwt/logger.hpp>
#include <cwt/safeformat.hpp>
#include <netdb.h>

#if (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 1) \
		|| defined(_XOPEN_SOURCE) || defined(_POSIX_SOURCE)
#	define CWT_HAVE_GETADDRINFO 1
#endif


CWT_NS_BEGIN
namespace io {

struct HostInfo::addrinfo_iterator_data
{
	addrinfo_iterator_data() : result(nullptr), next(nullptr) {}
	~addrinfo_iterator_data();
	struct addrinfo * result;
	struct addrinfo * next;
};

HostInfo::addrinfo_iterator_data::~addrinfo_iterator_data ()
{
	if (result) {
		freeaddrinfo(result);
		result = nullptr;
		next = nullptr;
	}
}

HostInfo::addrinfo_iterator::addrinfo_iterator()
	: m_data(new addrinfo_iterator_data)
{}


HostInfo::addrinfo_iterator HostInfo::endAddrInfo()
{
	return addrinfo_iterator();
}

HostInfo::addrinfo_iterator HostInfo::beginAddrInfo (const String & hostname)
{
	addrinfo_iterator it;

#ifdef CWT_HAVE_GETADDRINFO
	const char * node = hostname.c_str();
	const char * service = nullptr; // service is no matter
	struct addrinfo hints;
	struct addrinfo * result;

	memset(& hints, 0, sizeof(struct addrinfo));
	hints.ai_flags     = hostname.isEmpty() ? AI_PASSIVE : 0;
	hints.ai_family    = AF_UNSPEC;    /* Allow IPv4 or IPv6, i.e. any address family */
	hints.ai_socktype  = 0;            /* Any type */
	hints.ai_protocol  = 0;            /* Any protocol */
	hints.ai_canonname = nullptr;
	hints.ai_addr      = nullptr;
	hints.ai_next      = nullptr;

	int rc = getaddrinfo (node , service, & hints, & result);
	if (rc) {
		Logger::error(_Fr("Failed to get address info: %s") % hostname);
		return endAddrInfo();
	}

	shared_ptr<addrinfo_iterator_data> d(new addrinfo_iterator_data);
	d->result = result;
	d->next = result;
	it.m_data.swap(d);
#else
#	error "Need getaddrinfo"
#endif
	return it;
}

bool HostInfo::addrinfo_iterator::operator == (const addrinfo_iterator & it)
{
	return m_data->next == it.m_data->next;
}

HostInfo::addrinfo_iterator & HostInfo::addrinfo_iterator::operator ++ ()
{
	m_data->next = m_data->next->ai_next;
	return *this;
}

} // namespace io

CWT_NS_END
