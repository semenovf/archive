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

class DLL_API IODevice : public Errorable
{
public:
	static const size_t ChunkSize     = 512;
	static const size_t DefaultBufferSize = 512; // must be less than CWT_SIZE_MAX/2

protected:
	IODevice() : m_buffer(), m_head(0) {;}

public:
	enum OpenMode {
		  ReadOnly    = 0
		, WriteOnly   = 0x0001
		, ReadWrite   = 0x0002
		, WriteRead   = ReadWrite
		, NonBlocking = 0x0004
		, Unbuffered  = 0x0008
	};

	enum ReadLineStatus {
		  ReadLine_Overflow = -2
		, ReadLine_Error = -1
		, ReadLine_Ok = 0
		, ReadLine_Intermediate
	};

protected:
	virtual ssize_t readBytes(char bytes[], size_t n) = 0;
	virtual ssize_t writeBytes(const char bytes[], size_t n) = 0;
	virtual size_t  bytesAvailable() const = 0;
	ReadLineStatus  readLineData(const ByteArray endLines[], int count, ByteArray & bytes, size_t maxSize = CWT_INT_MAX);

public:
	virtual int  close() = 0;
	virtual bool opened() const = 0;
	virtual void flush() = 0;

	size_t       available() const                   { return m_buffer.size() - m_head + bytesAvailable(); }
	virtual bool atEnd() const                       { return bytesAvailable() == ssize_t(0); }
	ssize_t      read(char bytes[], size_t n);
	ssize_t      write(const char bytes[], size_t n);
	void         unread(const char bytes[], size_t n); // TODO must be removed (referenced from TextStream class)
	ByteArray    readAll();
	bool         getByte(char * byte);
	void         ungetByte(char byte);

	ReadLineStatus readLine(ByteArray & bytes, size_t maxSize = CWT_INT_MAX);
	ReadLineStatus readLine(const ByteArray & endLine, ByteArray & bytes, size_t maxSize = CWT_INT_MAX);
	ReadLineStatus readLine(const char * endLine, ByteArray & bytes, size_t maxSize = CWT_INT_MAX) { return readLine(ByteArray(endLine), bytes, maxSize); }
	ReadLineStatus readLine(const ByteArray endLines[], int count, ByteArray & bytes, size_t maxSize = CWT_INT_MAX);

private:
	// FIXME need shared
	ByteArray m_buffer;
	ssize_t   m_head;   // head position
};

inline IODevice::ReadLineStatus IODevice::readLine(const ByteArray & endLine, ByteArray & bytes, size_t maxSize)
{
	const ByteArray endLines[] = { endLine };
	return readLineData(endLines, 1, bytes, maxSize);
}

inline IODevice::ReadLineStatus IODevice::readLine(const ByteArray endLines[], int count, ByteArray & bytes, size_t maxSize)
{
	return readLineData(endLines, count, bytes, maxSize);
}


CWT_NS_END

#endif /* __CWT_IODEVICE_HPP__ */
