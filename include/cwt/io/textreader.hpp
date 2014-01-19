/*
 * textreader.hpp
 *
 *  Created on: Aug 6, 2013
 *      Author: wladt
 */

#ifndef __CWT_IO_TEXTREADER_HPP__
#define __CWT_IO_TEXTREADER_HPP__

#include <cwt/io/device.hpp>
#include <cwt/io/reader.hpp>
#include <cwt/io/utf8codec.hpp>

namespace cwt { namespace io {

class DLL_API TextReader : public Reader<Device, Utf8Decoder>
{
private:
	typedef Reader<Device, Utf8Decoder> base_class;
	static Utf8NullDecoder DefaultUtf8Decoder;

	CWT_DENY_COPY(TextReader);
public:
	typedef Reader<Device, Utf8Decoder>::iterator iterator;

public:
	TextReader (Device & dev, Utf8Decoder & decoder) : base_class(dev, decoder) {}
	TextReader (Device & dev) : base_class(dev, DefaultUtf8Decoder) {}

	Device * device () const { return & this->producer(); }

	pfs::utf8string readAll ()
	{
		return read(device()->available());
	}
};

}} // cwt::io

#endif /* __CWT_IO_TEXTREADER_HPP__ */
