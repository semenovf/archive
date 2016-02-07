/*
 * file.hpp
 *
 *  Created on: Jul 12, 2013
 *      Author: wladt
 */

#ifndef __PFS_IO_FILE_HPP__
#define __PFS_IO_FILE_HPP__

#include <pfs/fs/path.hpp>
#include <pfs/io/device.hpp>

namespace pfs { namespace io {

/**
 * @struct pfs::io::file
 * @brief File device implementation.
 * @see pfs::io::device.
 */
struct file {};

template <>
struct open_params<file>
{
	static const int default_create_perms = fs::perm_user_read
			| fs::perm_user_write
			| fs::perm_group_read
			| fs::perm_other_read;

	fs::path path;
	device::open_mode_flags oflags;
	int permissions;

	open_params (const fs::path & s, device::open_mode_flags of, int perms)
		: path(s)
		, oflags(of)
		, permissions(perms)
	{}

	open_params (const fs::path & s, device::open_mode_flags of)
		: path(s)
		, oflags(of)
		, permissions(default_create_perms)
	{}

	open_params (const fs::path & s)
		: path(s)
		, oflags(read_write)
		, permissions(default_create_perms)
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
device open_device<file> (const open_params<file> & op, error_code & ex);

}} // pfs::io

#endif /* __PFS_IO_FILE_HPP__ */
