/**
 * @file   file.hpp
 * @author wladt
 * @date   Feb 15, 2013 9:06:20 AM
 *
 * @brief
 */

#ifndef __CWT_FILE_HPP__
#define __CWT_FILE_HPP__


#include <cwt/cwt.h>
#include <cwt/iodevice.hpp>

CWT_NS_BEGIN

/*
class File
{
public:
	File(const String & name);
	~File();
	bool copy(const String & newName);
	bool exists() const;
	bool open(OpenMode mode, FileHandleFlags handleFlags = DontCloseHandle);
	bool remove();
	bool rename(const String & newName);
	void setFileName(const String & name);

private:
	int m_fd;
};
*/
class DLL_API File : public IODevice {
	CWT_IODEVICE_PIMPL(File);
public:
	File();
	File(const File &other);
	bool open(const char *path, int oflags);
};

CWT_NS_END


#endif /* __CWT_FILE_HPP__ */
