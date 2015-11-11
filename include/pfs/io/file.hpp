/*
 * file.hpp
 *
 *  Created on: Jul 12, 2013
 *      Author: wladt
 */

#ifndef __PFS_IO_FILE_HPP__
#define __PFS_IO_FILE_HPP__

#include <pfs/byte_string.hpp>
#include <pfs/fs/path.hpp>
#include <pfs/io/device.hpp>


namespace pfs { namespace io {

class DLL_API file : public device
{
    typedef device base_class;

public:
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
	file (const string & path, int32_t oflags = ReadWrite);
	file (const pfs::fs::path & p, int32_t oflags = ReadWrite);
	virtual ~file () {}

	bool open (const string & path, int32_t oflags = ReadWrite);
	bool open (const pfs::fs::path & p, int32_t oflags = ReadWrite);

	size_t size () const;
	bool setPermissions (int32_t perms); // XXX DEPRECATED
	void rewind ();
	size_t offset () const;
	void setOffset (size_t off);

	// XXX DEPRECATED
	string readAll ()
	{
		return string::fromUtf8(read(available()));
	}

	// XXX DEPRECATED
	void read_all (string & s)
	{
		s = string::fromUtf8(read(available()));
	}

	void read_all (byte_string & bs)
	{
		bs = read(available());
	}

};

}} // pfs::io

#endif /* __PFS_IO_FILE_HPP__ */
