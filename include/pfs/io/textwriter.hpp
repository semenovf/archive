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
#include <pfs/noncopyable.hpp>

namespace cwt { namespace io {

class DLL_API text_writer : public writer<cwt::io::device, utf8encoder>, pfs::noncopyable
{
private:
	typedef writer<cwt::io::device, utf8encoder> base_class;
	static utf8nullencoder DefaultUtf8Encoder;

public:
	text_writer (cwt::io::device & dev) : base_class(dev, DefaultUtf8Encoder) {}
	text_writer (cwt::io::device & dev, utf8encoder & encoder) : base_class(dev, encoder) {}

	cwt::io::device * device() const { return consumer(); }
};

}} // cwt::io

#endif /* __CWT_IO_TEXTWRITER_HPP__ */
