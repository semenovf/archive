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

enum socket_set_option
{
//	  so_debug      = 0x00000000 // SO_DEBUG	1
//	, so_reuse_addr = 0x00000001 // SO_REUSEADDR	2
//	, so_type       = 0x00000002 // SO_TYPE		3
	// 0x00000004 //#define SO_ERROR	4
	// 0x00000008 //#define SO_DONTROUTE	5
	// 0x00000010 //#define SO_BROADCAST	6
	// 0x00000020 //#define SO_SNDBUF	7
	// 0x00000040 //#define SO_RCVBUF	8
	// 0x00000080 //#define SO_SNDBUFFORCE	32
	// 0x00000100 //#define SO_RCVBUFFORCE	33
    sso_keep_alive = 0x0200 // SO_KEEPALIVE	9
//#define SO_OOBINLINE	10
//#define SO_NO_CHECK	11
//#define SO_PRIORITY	12
//#define SO_LINGER	13
//#define SO_BSDCOMPAT	14
//#define SO_REUSEPORT	15
//#ifndef SO_PASSCRED /* powerpc only differs in these */
//#define SO_PASSCRED	16
//#define SO_PEERCRED	17
//#define SO_RCVLOWAT	18
//#define SO_SNDLOWAT	19
//#define SO_RCVTIMEO	20
//#define SO_SNDTIMEO	21
};

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
	uint32_t socketopts;

	open_params ()
		: addr()
		, port(0)
		, oflags(0)
		, socketopts(0)
	{}

	/**
	 * @param a IPv4 address.
	 * @param p Port number
	 * @param of Only device::non_blocking applicable.
	 * @param sso Socket set options (@see socket_set_option).
	 */
	open_params (net::inet4_addr a, uint16_t p, device::open_mode_flags of, uint32_t sso)
		: addr(a)
		, port(p)
		, oflags(of)
		, socketopts(sso)
	{}

	open_params (net::inet4_addr a, uint16_t p)
		: addr(a)
		, port(p)
		, oflags(0)
		, socketopts(0)
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
device open_device<tcp_socket> (const open_params<tcp_socket> & op, error_code & ex);

}} // pfs::io

#endif /* __PFS_IO_INET_SOCKET_HPP__ */
