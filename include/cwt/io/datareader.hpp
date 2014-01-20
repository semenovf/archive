/*
 * datareader.hpp
 *
 *  Created on: Aug 6, 2013
 *      Author: wladt
 */

#ifndef __CWT_IO_DATAREADER_HPP__
#define __CWT_IO_DATAREADER_HPP__

#include <cwt/io/device.hpp>
#include <cwt/io/reader.hpp>
#include <cwt/io/nullcodec.hpp>
#include <pfs/bytearray.hpp>

namespace cwt { namespace io {

class DLL_API data_reader : public reader<cwt::io::device, nullbyte_codec>
{
	typedef cwt::io::reader<cwt::io::device, nullbyte_codec> base_class;
	static nullbyte_codec DefaultByteDecoder;

	PFS_DENY_COPY(data_reader);

public:
	typedef reader<cwt::io::device, nullbyte_codec>::iterator iterator;

public:
	data_reader (cwt::io::device & dev) : base_class(dev, DefaultByteDecoder) {}

	cwt::io::device * device () const { return & this->producer(); }
	const pfs::bytearray & data () const { return base_class::m_outputBuffer; }

	pfs::bytearray readAll()
	{
		return this->read(device()->available());
	}

};

}} // cwt::io

#endif /* __CWT_IO_DATAREADER_HPP__ */
