/**
 * @file   iodevice.hpp
 * @author wladt
 * @date   Apr 11, 2013 2:58:04 PM
 *
 * @brief
 */

#ifndef __CWT_IODEVICE_HPP__
#define __CWT_IODEVICE_HPP__

#include <cwt/cwt.h>
#include <cwt/bytearray.hpp>
#include <cwt/errorable.hpp>
#include <cwt/shared_ptr.hpp>

CWT_NS_BEGIN

class DataStream;

class IODevice : public Errorable
{
public:
	static const size_t ChunkSize = 512;
	static const size_t BufferMaxSize = 512;

protected:
	IODevice() {;}

public:
	enum OpenMode {
		  ReadOnly    = 0
		, WriteOnly   = 0x0001
		, ReadWrite   = 0x0002
		, WriteRead   = ReadWrite
		, NonBlocking = 0x0004
		, Unbuffered  = 0x0008
	};

protected:
	virtual ssize_t readBytes(char bytes[], size_t n) = 0;
	virtual ssize_t writeBytes(const char bytes[], size_t n) = 0;
	virtual size_t  bytesAvailable() const = 0;

public:
	virtual int  close() = 0;
	virtual bool opened() const = 0;
	virtual void flush() = 0;

	size_t       available() const                   { return m_buffer.size() + bytesAvailable(); }
	virtual bool atEnd() const                       { return available() == ssize_t(0); }
	ssize_t      read(char bytes[], size_t n);
	ssize_t      write(const char bytes[], size_t n);

private:
	ByteArray m_buffer; // TODO need shared

	friend class DataStream;
};

CWT_NS_END

#endif /* __CWT_IODEVICE_HPP__ */
