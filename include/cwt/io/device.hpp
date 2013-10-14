/*
 * device.hpp
 *
 *  Created on: Jul 12, 2013
 *      Author: wladt
 */

#ifndef __CWT_IO_DEVICE_HPP__
#define __CWT_IO_DEVICE_HPP__

#include <cwt/cwt.hpp>
#include <cwt/array.hpp>
#include <cwt/bytearray.hpp>
#include <cwt/string.hpp>

CWT_NS_BEGIN

namespace io {

class DLL_API Device
{
public:
	typedef char char_type;

protected:
	Device() : m_inputBuffer(nullptr), m_inputBufferSize(0) {;}

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
	virtual ssize_t readBytes      (char bytes[], size_t n) = 0;
	virtual ssize_t writeBytes     (const char bytes[], size_t n) = 0;
	virtual size_t  bytesAvailable () const = 0;
	void closeDevice() { close(); }
public:
	virtual ~Device()
	{
		if (m_inputBuffer)
			delete[] m_inputBuffer;
		m_inputBuffer = nullptr;
		m_inputBufferSize = 0;
		closeDevice();
	}

	virtual bool close  () = 0;
	virtual bool opened () const = 0;
	virtual void flush  () = 0;

	size_t       available() const       { return bytesAvailable(); }
	bool         atEnd    () const       { return bytesAvailable() == ssize_t(0); }
	ssize_t      read     (char bytes[], size_t n) { return readBytes(bytes, n); }
	ssize_t      read     (ByteArray & ba, size_t n);
	ssize_t      write    (const char bytes[], size_t n) { return writeBytes(bytes, n); }
	ssize_t      write    (const Array<char_type> bytes, size_t n) { return writeBytes(bytes.data(), n); }
	ssize_t      write    (const ByteArray & bytes, size_t n) { return writeBytes(bytes.data(), CWT_MIN(n, bytes.size())); }
	ssize_t      write    (const ByteArray & bytes) { return writeBytes(bytes.data(), bytes.size()); }
private:
	char * m_inputBuffer;
	size_t m_inputBufferSize;
};

} // namespace io

CWT_NS_END

#endif /* __CWT_IO_DEVICE_HPP__ */
