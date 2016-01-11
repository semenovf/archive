/*
 * inet_socket.hpp
 *
 *  Created on: Jan 11, 2016
 *      Author: wladt
 */

#ifndef __PFS_IO_INET_SOCKET_HPP__
#define __PFS_IO_INET_SOCKET_HPP__

#include <pfs/io/device.hpp>

namespace pfs { namespace io {


struct inet_socket {};

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
struct open_params<inet_socket>
{
	uint32_t addr;
	int32_t  port;

	open_params ();
	open_params (uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint16_t port);
	open_params (uint8_t a, uint8_t b, uint16_t c, uint16_t port);
	open_params (uint8_t a, uint32_t b, uint16_t port);
	open_params (uint32_t a , uint16_t port);
	open_params (const string & s);
	open_params (const string & s, uint16_t port);
};



//	static const int default_create_perms = fs::perm_user_read
//			| fs::perm_user_write
//			| fs::perm_group_read
//			| fs::perm_other_read;
//
//	fs::path path;
//	device::open_mode_flags oflags;
//	int permissions;

//	open_params (const fs::path & s, device::open_mode_flags of, int perms)
//		: path(s)
//		, oflags(of)
//		, permissions(perms)
//	{}
//
//	open_params (const fs::path & s, device::open_mode_flags of)
//		: path(s)
//		, oflags(of)
//		, permissions(default_create_perms)
//	{}
//
//	open_params (const fs::path & s)
//		: path(s)
//		, oflags(device::read_write)
//		, permissions(default_create_perms)
//	{}



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
bool open_device<file> (device & d, const open_params<file> & op, error_code * ex);

}} // pfs::io

#endif /* __PFS_IO_INET_SOCKET_HPP__ */
