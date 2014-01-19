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

class DLL_API DataReader : public Reader<Device, NullByteCodec>
{
	typedef Reader<Device, NullByteCodec> base_class;
	static NullByteCodec DefaultByteDecoder;

	CWT_DENY_COPY(DataReader);

public:
	typedef Reader<Device, NullByteCodec>::iterator iterator;

public:
	DataReader(Device & dev) : base_class(dev, DefaultByteDecoder) {}

	Device * device() const { return & this->producer(); }
	const ByteArray & data() const { return base_class::m_outputBuffer; }

	ByteArray readAll()
	{
		return this->read(device()->available());
	}

};

}} // cwt::io

#endif /* __CWT_IO_DATAREADER_HPP__ */
