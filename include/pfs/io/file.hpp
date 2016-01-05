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
struct file;

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
		, oflags(device::ReadWrite)
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
bool open_device<file> (device & d, const open_params<file> & op, error_code * ex);

}} // pfs::io


#if __COMMENT__

#include <pfs/byte_string.hpp>
#include <pfs/fs/path.hpp>
#include <pfs/io/device.hpp>

namespace pfs { namespace io {

class DLL_API file : public bits::device
{
public:
	typedef bits::device::native_handle_type native_handle_type;

	enum permission {
		  ReadOwner  = 0x4000 // The file is readable by the owner of the file.
		, WriteOwner = 0x2000 // The file is writable by the owner of the file.
		, ExeOwner	 = 0x1000 // The file is executable by the owner of the file.
		, ReadUser	 = 0x0400 // The file is readable by the user.
		, WriteUser	 = 0x0200 // The file is writable by the user.
		, ExeUser	 = 0x0100 // The file is executable by the user.
		, ReadGroup	 = 0x0040 // The file is readable by the group.
		, WriteGroup = 0x0020 // The file is writable by the group.
		, ExeGroup	 = 0x0010 // The file is executable by the group.
		, ReadOther	 = 0x0004 // The file is readable by anyone.
		, WriteOther = 0x0002 // The file is writable by anyone.
		, ExeOther	 = 0x0001 // The file is executable by anyone.
	};

public:
	file () : device() {}
	file (int fd);
	file (const string & path, int32_t oflags = io::device::ReadWrite);
	file (const pfs::fs::path & p, int32_t oflags = io::device::ReadWrite);
	virtual ~file () {}

	bool open (const string & path, int32_t oflags = io::device::ReadWrite);
	bool open (const pfs::fs::path & p, int32_t oflags = io::device::ReadWrite);

	size_t size () const;
//	bool set_permissions (int32_t perms); // XXX DEPRECATED
	void rewind ();
	size_t offset () const;

	void set_offset (size_t off);

//	// XXX DEPRECATED
//	string readAll ()
//	{
//		return string::fromUtf8(read(available()));
//	}
//
//	// XXX DEPRECATED
//	void read_all (string & s)
//	{
//		s = string::fromUtf8(read(available()));
//	}

//	void read_all (byte_string & bs)
//	{
//		bs = read(available());
//	}
};

}} // pfs::io

#endif

#endif /* __PFS_IO_FILE_HPP__ */
