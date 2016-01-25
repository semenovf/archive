/*
 * inet_socket.hpp
 *
 *  Created on: Jan 11, 2016
 *      Author: wladt
 */

#ifndef __PFS_IO_INET_SOCKET_HPP__
#define __PFS_IO_INET_SOCKET_HPP__

#include <pfs/io/device.hpp>
#include <pfs/net/inet4_addr.hpp>

namespace pfs { namespace io {

/**
 * @struct pfs::io::tcp_socket
 * @brief TCP socket device implementation.
 * @see pfs::io::device.
 */
struct tcp_socket {};

/**
 * @struct pfs::io::udp_socket
 * @brief UDP socket device implementation.
 * @see pfs::io::device.
 */
struct udp_socket {};

template <>
struct open_params<tcp_socket>
{
	net::inet4_addr addr;
	uint16_t port;
	device::open_mode_flags oflags;

	open_params ()
		: addr()
		, port(0)
		, oflags(0)
	{}

	/**
	 * @param a IPv4 address.
	 * @param p Port number
	 * @param of Only device::non_blocking applicable.
	 */
	open_params (net::inet4_addr a, uint16_t p, device::open_mode_flags of)
		: addr(a)
		, port(p)
		, oflags(of)
	{}

	open_params (net::inet4_addr a, uint16_t p)
		: addr(a)
		, port(p)
		, oflags(0)
	{}
};

/**
 * @fn bool open_device<file> (device & d, const open_params<file> & op)
 *
 * @brief Open file device.
 *
 * @param d File device to open.
 * @param op Open device parameters.
 * 		@li open_params(size_t n, uint32_t oflags)
 * 		@li open_params(byte_t * p, size_t n, uint32_t oflags)
 * 		@li open_params(char * p, size_t n, uint32_t oflags)
 *
 * @return @c true if open is successful, @c false otherwise
 *         (i.e. file device is already opened).
 */
template <>
device open_device<tcp_socket> (const open_params<tcp_socket> & op, error_code * ex);

}} // pfs::io

#endif /* __PFS_IO_INET_SOCKET_HPP__ */
