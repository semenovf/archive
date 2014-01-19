/*
 * textwriter.hpp
 *
 *  Created on: Aug 13, 2013
 *      Author: wladt
 */

#ifndef __CWT_IO_TEXTWRITER_HPP__
#define __CWT_IO_TEXTWRITER_HPP__

#include <cwt/io/device.hpp>
#include <cwt/io/writer.hpp>
#include <cwt/io/utf8codec.hpp>

namespace cwt { namespace io {

class DLL_API TextWriter : public Writer<Device, Utf8Encoder>
{
private:
	typedef Writer<Device, Utf8Encoder> base_class;
	static Utf8NullEncoder DefaultUtf8Encoder;

	CWT_DENY_COPY(TextWriter);

public:
	TextWriter(Device & dev) : base_class(dev, DefaultUtf8Encoder) {}
	TextWriter(Device & dev, Utf8Encoder & encoder) : base_class(dev, encoder) {}

	Device * device() const { return consumer(); }
};

}} // cwt::io

#endif /* __CWT_IO_TEXTWRITER_HPP__ */
