/*
 * hostinfo.hpp
 *
 *  Created on: Oct 15, 2013
 *      Author: wladt
 */

#ifndef __CWT_NET_HOSTINFO_HPP__
#define __CWT_NET_HOSTINFO_HPP__

#include <cwt/shared_ptr.hpp>
#include <cwt/string.hpp>

CWT_NS_BEGIN

namespace net {

struct addrinfo_iterator_data;
class addrinfo_iterator
{
	shared_ptr<addrinfo_iterator_data> m_data;

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

	String canonicalName () const;
	String hostname () const { return canonicalName(); }
	String ipv4name () const;
	uint32_t ipv4 () const;
	uint16_t port () const;

	static addrinfo_iterator begin (const String & hostname);
	static addrinfo_iterator end ();
};

} // namespace net

CWT_NS_END

#endif /* __CWT_NET_HOSTINFO_HPP__ */
