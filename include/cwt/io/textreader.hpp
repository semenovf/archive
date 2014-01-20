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

class DLL_API text_reader : public reader<cwt::io::device, utf8decoder>
{
private:
	typedef reader<cwt::io::device, utf8decoder> base_class;
	static utf8nulldecoder DefaultUtf8Decoder;

	PFS_DENY_COPY(text_reader);

public:
	typedef reader<cwt::io::device, utf8decoder>::iterator iterator;

public:
	text_reader (cwt::io::device & dev, utf8decoder & decoder) : base_class(dev, decoder) {}
	text_reader (cwt::io::device & dev) : base_class(dev, DefaultUtf8Decoder) {}

	cwt::io::device * device () const { return & this->producer(); }

	pfs::utf8string readAll ()
	{
		return read(device()->available());
	}
};

}} // cwt::io

#endif /* __CWT_IO_TEXTREADER_HPP__ */
