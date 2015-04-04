/*
 * hostinfo.hpp
 *
 *  Created on: Oct 15, 2013
 *      Author: wladt
 */

#ifndef __CWT_NET_HOSTINFO_HPP__
#define __CWT_NET_HOSTINFO_HPP__

#include <pfs/shared_ptr.hpp>
#include <pfs/string.hpp>

namespace cwt { namespace net {

struct addrinfo_iterator_data;
class addrinfo_iterator
{
	pfs::shared_ptr<addrinfo_iterator_data> m_data;

public:
	addrinfo_iterator ();
	addrinfo_iterator (const addrinfo_iterator & it) : m_data(it.m_data) {}
	addrinfo_iterator & operator = (const addrinfo_iterator & it)
	{
		m_data = it.m_data;
		return *this;
	}

	bool operator == (const addrinfo_iterator & it);
	bool operator != (const addrinfo_iterator & it) { return !(*this == it); }
	addrinfo_iterator & operator ++ ();
	addrinfo_iterator   operator ++ (int)
	{
		addrinfo_iterator r(*this);
		this->operator ++();
		return r;
	}

	pfs::string canonicalName () const;
	pfs::string hostname () const { return canonicalName(); }
	pfs::string ipv4name () const;
	uint32_t ipv4 () const;
	uint16_t port () const;

	static addrinfo_iterator begin (const pfs::string & hostname);
	static addrinfo_iterator end ();
};

}} // cwt::net

#endif /* __CWT_NET_HOSTINFO_HPP__ */
