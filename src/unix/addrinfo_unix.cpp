/*
 * addrinfo_unix.cpp
 *
 *  Created on: Oct 14, 2013
 *      Author: wladt
 */

#include "../../include/cwt/net/hostinfo.hpp"
#include <cwt/logger.hpp>
#include <cwt/safeformat.hpp>
#include <netdb.h>

#if (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 1) \
		|| defined(_XOPEN_SOURCE) || defined(_POSIX_SOURCE)
#	define CWT_HAVE_GETADDRINFO 1
#else
#	include <cwt/mt.hpp>

//Since glibc 2.8: _BSD_SOURCE || _SVID_SOURCE || _GNU_SOURCE
#	if defined(__GNUC_PREREQ) && __GNUC_PREREQ(2,8) && (defined(_BSD_SOURCE) || defined(_SVID_SOURCE) || defined(_GNU_SOURCE)))
#		define CWT_HAVE_HSTRERROR 1
#	endif
#endif


CWT_NS_BEGIN

namespace net {

struct addrinfo_iterator_data
{
#ifdef CWT_HAVE_GETADDRINFO
	addrinfo_iterator_data() : result(nullptr), next(nullptr) {}
	~addrinfo_iterator_data();
	struct addrinfo * result;
	struct addrinfo * next;
#endif
};

addrinfo_iterator_data::~addrinfo_iterator_data ()
{
#ifdef CWT_HAVE_GETADDRINFO
	if (result) {
		freeaddrinfo(result);
		result = nullptr;
		next = nullptr;
	}
#endif
}


addrinfo_iterator::addrinfo_iterator()
	: m_data(new addrinfo_iterator_data)
{}

addrinfo_iterator addrinfo_iterator::end()
{
	return addrinfo_iterator();
}

addrinfo_iterator addrinfo_iterator::begin (const String & hostname)
{
	addrinfo_iterator it;

#ifdef CWT_HAVE_GETADDRINFO
	const char * node = hostname.c_str();
	const char * service = nullptr; // service is no matter
	struct addrinfo hints;
	struct addrinfo * result;

	memset(& hints, 0, sizeof(struct addrinfo));
	hints.ai_flags     = (hostname.isEmpty() ? AI_PASSIVE : 0) | AI_CANONNAME;
	hints.ai_family    = AF_UNSPEC;    /* Allow IPv4 or IPv6, i.e. any address family */
	hints.ai_socktype  = 0;            /* Any type */
	hints.ai_protocol  = 0;            /* Any protocol */
	hints.ai_canonname = nullptr;
	hints.ai_addr      = nullptr;
	hints.ai_next      = nullptr;

	int rc = getaddrinfo (node , service, & hints, & result);
	if (rc) {
		Logger::error(_Fr("Failed to get address info: %s: %s")
				% hostname
				% gai_strerror(rc));
		return end();
	}

	shared_ptr<addrinfo_iterator_data> d(new addrinfo_iterator_data);
	d->result = result;
	d->next = result;
	it.m_data.swap(d);

#else
/*
	static Mutex addrinfo_iterator_mutex;
	AutoLock(addrinfo_iterator_mutex);

	struct hostent * phost;

	phost = gethostbyname(hostname.c_str());

	if (phost == nullptr) {
#	ifdef CWT_HAVE_HSTRERROR
		Logger::error(_Fr("%s: host not found: %s")
				% hostname
				% hstrerror(h_error));
#	else
		Logger::error(_Fr("%s: host not found") % hostname);
#endif
		return end();
	}

	memcpy(& sockaddr.sin_addr, phost->h_addr, sizeof(sockaddr.sin_addr));
*/
#endif
	return it;
}

bool addrinfo_iterator::operator == (const addrinfo_iterator & it)
{
	return m_data->next == it.m_data->next;
}

addrinfo_iterator & addrinfo_iterator::operator ++ ()
{
	m_data->next = m_data->next->ai_next;
	return *this;
}

/**
 * @fn String addrinfo_iterator::canonicalName () const
 *
 * @brief Returns official name of the host.
 *
 * @return Official name of the host.
 */
String addrinfo_iterator::canonicalName () const
{
	// Only field of the first of the addrinfo structures in the returned list
	// is set to point to the official name of the host.
	if (m_data->result)
		return String(m_data->result->ai_canonname);
	return String();
}

uint16_t addrinfo_iterator::port () const
{
	if (m_data->next) {
		struct sockaddr_in * saddr = reinterpret_cast<struct sockaddr_in *>(m_data->next->ai_addr);
		return saddr->sin_port;
	}
	return 0;
}

uint32_t addrinfo_iterator::ipv4 () const
{
	if (m_data->next) {
		struct sockaddr_in * saddr = reinterpret_cast<struct sockaddr_in *>(m_data->next->ai_addr);
		return ntohl(saddr->sin_addr.s_addr);
	}
	return 0;
}

String addrinfo_iterator::ipv4name () const
{
	if (m_data->next) {
		uint32_t ip = ipv4();
		return 	_F("%d.%d.%d.%d")
				% ((ip >> 24) & 0xFF)
				% ((ip >> 16) & 0xFF)
				% ((ip >> 8)  & 0xFF)
				% (ip  & 0xFF);
	}
	return String();
}

} // namespace io

CWT_NS_END
