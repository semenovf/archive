/*
 * datawriter.hpp
 *
 *  Created on: Aug 12, 2013
 *      Author: wladt
 */

#ifndef __CWT_IO_DATAWRITER_HPP__
#define __CWT_IO_DATAWRITER_HPP__

#include <cwt/bytearray.hpp>
#include <cwt/string.hpp>
#include <cwt/io/device.hpp>
#include <cwt/io/writer.hpp>
#include <cwt/io/nullcodec.hpp>

CWT_NS_BEGIN

namespace io {

class DLL_API DataWriter : public Writer<Device, NullByteCodec>
{
	typedef Writer<Device, NullByteCodec> base_class;
	static NullByteCodec DefaultByteEncoder;

	CWT_DENY_COPY(DataWriter);

public:
	DataWriter(Device & dev) : base_class(dev, DefaultByteEncoder) {}

	Device * device() const { return consumer(); }

	ssize_t write (const char chars[], size_t size) { return base_class::write(ByteArray(chars, size)); }
	ssize_t write (const ByteArray & bytes) { return base_class::write(bytes); }
	ssize_t write (const String & s) { return base_class::write(ByteArray(s.data(), s.size())); }
};

} // namespace io

CWT_NS_END

#endif /* __CWT_IO_DATAWRITER_HPP__ */
