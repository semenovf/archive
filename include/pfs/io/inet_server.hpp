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

struct inet_server {};

/**
 * @struct pfs::io::tcp_server
 * @brief TCP server implementation.
 * @see pfs::io::server.
 */
struct tcp_server {};

struct udp_server {};


// Helper structure for tcp/udp servers
//
template <>
struct open_params<inet_server>
{
	net::inet4_addr addr;
	uint16_t port;
	device::open_mode_flags oflags;
   
    open_params (net::inet4_addr a, uint16_t p, device::open_mode_flags of)
        : addr(a)
		, port(p)
		, oflags(of)
	{}
};


template <>
struct open_params<tcp_server> : public open_params<inet_server>
{
    typedef open_params<inet_server> base_class;
/*
	the maximum length to which the queue of pending connections for  sockfd  may
	grow.   If  a  connection  request arrives when the queue is full, the client may receive an error with an
	indication of ECONNREFUSED or, if the underlying protocol supports  retransmission,  the  request  may  be
	ignored so that a later reattempt at connection succeeds.
*/
	int npendingconn;

	open_params ()
        : base_class(net::inet4_addr(), 0, 0)
		, npendingconn(0)
	{}

	open_params (net::inet4_addr a, uint16_t p, int backlog, device::open_mode_flags of)
        : base_class(a, p, of)
		, npendingconn(backlog)
	{}

	open_params (net::inet4_addr a, uint16_t p, int backlog)
        : base_class(a, p, 0)
		, npendingconn(backlog)
	{}
};

template <>
struct open_params<udp_server> : public open_params<inet_server>
{
    typedef open_params<inet_server> base_class;

	open_params ()
        : base_class(net::inet4_addr(), 0, 0)
	{}

	open_params (net::inet4_addr a, uint16_t p, device::open_mode_flags of)
        : base_class(a, p, of)
	{}

	open_params (net::inet4_addr a, uint16_t p)
        : base_class(a, p, 0)
	{}
};

template <>
server open_server<tcp_server> (const open_params<tcp_server> & op, error_code & ex);

template <>
server open_server<udp_server> (const open_params<udp_server> & op, error_code & ex);

}} // pfs::io

#endif /* __PFS_IO_INET_SERVER_HPP__ */
