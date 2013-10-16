/*
 * hostaddr.cpp
 *
 *  Created on: Oct 14, 2013
 *      Author: wladt
 */

#include <cwt/safeformat.hpp>
//#include <netinet/in.h>
//#include <arpa/inet.h>

CWT_NS_BEGIN

namespace io {

#ifdef __COMMENT__
void HostAddress::setPath (const String & path)
{
	m_hostname = path;
}

void HostAddress::setAddress (const String & hostname)
{
	m_hostname = hostname;
}

void HostAddress::setAddress (uint32_t ipv4)
{
	//m_type = HostAddress::IPv4;
	m_ipv4 = ipv4;
	m_hostname  = _F("%d.%d.%d.%d")
			% ((ipv4 >> 24) & 0xFF)
			% ((ipv4 >> 16) & 0xFF)
			% ((ipv4 >> 8)  & 0xFF)
			% (ipv4  & 0xFF);

}
#endif

} // namespace io

CWT_NS_END
