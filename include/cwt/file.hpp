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
#include <cwt/pimpl.hpp>

CWT_NS_BEGIN

class DLL_API File : public IODevice {
	CWT_PIMPL_IMPL(File);
protected:
	virtual ssize_t   readBytes(char bytes[], size_t n);
	virtual ssize_t   writeBytes(const char bytes[], size_t n);
	virtual ssize_t   bytesAvailable() const;
public:
	File();
	bool open(const char *path, int oflags);
	virtual int  close();
	virtual bool opened() const;

};

CWT_NS_END


#endif /* __CWT_FILE_HPP__ */
