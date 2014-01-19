/*
 * tcpserver.hpp
 *
 *  Created on: Oct 16, 2013
 *      Author: wladt
 */

#ifndef __CWT_NET_TCPSERVER_HPP__
#define __CWT_NET_TCPSERVER_HPP__

#include <cwt/io/inetsocket.hpp>

namespace cwt { namespace net {

class TcpServer
{
	CWT_PIMPL_IMPL(TcpServer);

public:
	TcpServer ();
	bool open (const String hostname, uint16_t port, int32_t oflags = io::Device::ReadWrite);
	bool listen (const String hostname, uint16_t port, int32_t oflags = io::Device::ReadWrite)
	{
		return open(hostname, port, oflags);
	}
	io::TcpSocket * accept ();
	bool opened () const;
};

}} // cwt::net

#endif /* __CWT_NET_TCPSERVER_HPP__ */
