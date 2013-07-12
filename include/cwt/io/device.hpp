/*
 * device.hpp
 *
 *  Created on: Jul 12, 2013
 *      Author: wladt
 */

#ifndef __CWT_IO_DEVICE_HPP__
#define __CWT_IO_DEVICE_HPP__

#include <cwt/cwt.hpp>
#include <cwt/vector.hpp>
#include <cwt/errorable.hpp>

CWT_NS_BEGIN

namespace io {

class DLL_API Device : public Errorable
{
public:
	static const size_t ChunkSize = 512;
	typedef char char_type;
	typedef Vector<char_type> vector_type;

protected:
	Device() {;}

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

public:
	virtual int  close  () = 0;
	virtual bool opened () const = 0;
	virtual void flush  () = 0;

	size_t       available() const       { return bytesAvailable(); }
	bool         atEnd    () const       { return bytesAvailable() == ssize_t(0); }
	vector_type  read     (size_t size);
	ssize_t      write    (const vector_type & bytes);
};

inline Device::vector_type Device::read (size_t size)
{
	vector_type r;
	r.reserve(size);
	ssize_t rb = readBytes(r.data(), size);
	if (rb > 0)
		r.resize(size_t(rb));
	return r;
}

inline ssize_t Device::write(const Vector<char> & bytes)
{
	if (bytes.size() > 0)
		return writeBytes(bytes.data(), bytes.size());
	return 0;
}


} // namespace io

CWT_NS_END

#endif /* __CWT_IO_DEVICE_HPP__ */
