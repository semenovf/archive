/*
 * tcpserver.hpp
 *
 *  Created on: Oct 16, 2013
 *      Author: wladt
 */

#ifndef __CWT_NET_TCPSERVER_HPP__
#define __CWT_NET_TCPSERVER_HPP__

#include <cwt/io/inetsocket.hpp>

namespace cwt { namespace io {

class tcp_server
{
	PFS_PIMPL_DECL(tcp_server, impl);

public:
	tcp_server ();
	bool open (const pfs::string hostname, uint16_t port, int32_t oflags = device::ReadWrite);
	bool listen (const pfs::string hostname, uint16_t port, int32_t oflags = device::ReadWrite)
	{
		return open(hostname, port, oflags);
	}
	tcp_socket * accept ();
	bool opened () const;
};

}} // cwt::net

#endif /* __CWT_NET_TCPSERVER_HPP__ */
