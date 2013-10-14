/*
 * file.hpp
 *
 *  Created on: Jul 12, 2013
 *      Author: wladt
 */

#ifndef __CWT_IO_FILE_HPP__
#define __CWT_IO_FILE_HPP__

#include <cwt/cwt.hpp>
#include <cwt/pimpl.hpp>
#include <cwt/io/device.hpp>

CWT_NS_BEGIN

namespace io {

class DLL_API File : public Device
{
	CWT_PIMPL_IMPL(File);
public:
	enum Permission {
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

protected:
	virtual ssize_t readBytes(char bytes[], size_t n);
	virtual ssize_t writeBytes(const char bytes[], size_t n);
	virtual size_t  bytesAvailable() const;

public:
	File();
	File(int fd);
	File(const String & path, int32_t oflags);
	virtual ~File() {}
	bool open(const String & path, int32_t oflags = ReadWrite);
	virtual bool close();
	virtual void flush();
	virtual bool opened() const;

	size_t size() const;
	bool setPermissions(int32_t perms);
	void rewind();
};

} // namespace io

CWT_NS_END

#endif /* __CWT_IO_FILE_HPP__ */
