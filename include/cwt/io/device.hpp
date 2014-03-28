/*
 * device.hpp
 *
 *  Created on: Jul 12, 2013
 *      Author: wladt
 */

#ifndef __CWT_IO_DEVICE_HPP__
#define __CWT_IO_DEVICE_HPP__

#include <pfs/array.hpp>
#include <pfs/bytearray.hpp>
#include <pfs/string.hpp>
#include <cwt/errorable.hpp>

namespace cwt { namespace io {

class DLL_API device : public errorable
{
	char * m_inputBuffer;
	size_t m_inputBufferSize;

public:
	typedef char char_type;

protected:
	device() : m_inputBuffer(nullptr), m_inputBufferSize(0) {;}

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
	virtual bool    closeDevice    ()  = 0;
	virtual bool    deviceIsOpened () const = 0;
	virtual void    flushDevice    () = 0;

public:
	virtual ~device()
	{
		if (m_inputBuffer)
			delete[] m_inputBuffer;
		m_inputBuffer = nullptr;
		m_inputBufferSize = 0;
	}

	bool         opened   () const       { return deviceIsOpened(); }
	void         flush    ()             { flushDevice(); }
	bool         close    ()             { return closeDevice(); }
	size_t       available() const       { return bytesAvailable(); }
	bool         atEnd    () const       { return bytesAvailable() == ssize_t(0); }
	ssize_t      read     (char bytes[], size_t n) { return readBytes(bytes, n); }
	ssize_t      read     (pfs::bytearray & ba, size_t n);
	ssize_t      write    (const char bytes[], size_t n) { return writeBytes(bytes, n); }
	ssize_t      write    (const pfs::bytearray & bytes, size_t n) { return writeBytes(bytes.data(), pfs::min(n, bytes.size())); }
	ssize_t      write    (const pfs::bytearray & bytes) { return writeBytes(bytes.data(), bytes.size()); }
};

}} // cwt::io

#endif /* __CWT_IO_DEVICE_HPP__ */
