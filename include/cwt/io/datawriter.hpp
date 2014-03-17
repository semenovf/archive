/*
 * datawriter.hpp
 *
 *  Created on: Aug 12, 2013
 *      Author: wladt
 */

#ifndef __CWT_IO_DATAWRITER_HPP__
#define __CWT_IO_DATAWRITER_HPP__

#include <pfs/bytearray.hpp>
#include <pfs/string.hpp>
#include <cwt/io/device.hpp>
#include <cwt/io/writer.hpp>
#include <cwt/io/nullcodec.hpp>
#include <pfs/noncopyable.hpp>

namespace cwt { namespace io {

class DLL_API data_writer : public writer<cwt::io::device, nullbyte_codec>, pfs::noncopyable
{
	typedef writer<cwt::io::device, nullbyte_codec> base_class;
	static nullbyte_codec DefaultByteEncoder;

public:
	data_writer (cwt::io::device & dev) : base_class(dev, DefaultByteEncoder) {}

	cwt::io::device * device () const { return consumer(); }

	ssize_t write (const char chars[], size_t size) { return base_class::write(pfs::bytearray(chars, size)); }
	ssize_t write (const pfs::bytearray & bytes) { return base_class::write(bytes); }
	ssize_t write (const pfs::string & s) { return base_class::write(pfs::bytearray(s.data(), s.size())); }
};

}} // cwt::io

#endif /* __CWT_IO_DATAWRITER_HPP__ */
