/*
 * inet_server.hpp
 *
 *  Created on: Jan 13, 2016
 *      Author: wladt
 */

#ifndef __PFS_IO_INET_SERVER_HPP__
#define __PFS_IO_INET_SERVER_HPP__

#include <pfs/io/inet_socket.hpp>
#include <pfs/io/server.hpp>
#include <pfs/net/inet4_addr.hpp>

namespace pfs { namespace io {

/**
 * @struct pfs::io::tcp_server
 * @brief TCP server implementation.
 * @see pfs::io::server.
 */
struct tcp_server {};


template <>
struct open_params<tcp_server>
{
	net::inet4_addr addr;
	uint16_t port;
	device::open_mode_flags oflags;

/*
	the maximum length to which the queue of pending connections for  sockfd  may
	grow.   If  a  connection  request arrives when the queue is full, the client may receive an error with an
	indication of ECONNREFUSED or, if the underlying protocol supports  retransmission,  the  request  may  be
	ignored so that a later reattempt at connection succeeds.
*/
	int npendingconn;

	open_params ()
		: port(0)
		, oflags(0)
		, npendingconn(0)
	{}

	open_params (net::inet4_addr a, uint16_t p, int backlog, device::open_mode_flags of)
		: addr(a)
		, port(p)
		, oflags(of)
		, npendingconn(backlog)
	{}

	open_params (net::inet4_addr a, uint16_t p, int backlog)
		: addr(a)
		, port(p)
		, oflags(0)
		, npendingconn(backlog)
	{}
};

template <>
error_code open_server<tcp_server> (server & d
		, const open_params<tcp_server> & op);

}} // pfs::io

#endif /* __PFS_IO_INET_SERVER_HPP__ */
