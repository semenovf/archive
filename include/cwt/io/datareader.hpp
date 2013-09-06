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
#include <cwt/bytearray.hpp>

CWT_NS_BEGIN

namespace io {

class DLL_API DataReader : public Reader<Device, NullByteCodec>
{
	typedef Reader<Device, NullByteCodec> base_class;

	CWT_DENY_COPY(DataReader);

public:
	typedef Reader<Device, NullByteCodec>::iterator iterator;

public:
	DataReader(shared_ptr<Device> dev) : base_class(dev, shared_ptr<NullByteCodec>(new NullByteCodec)) {}

	Device * device() const { return this->producer(); }
	const ByteArray & data() const { return base_class::m_outputBuffer; }

	ByteArray readAll()
	{
		return this->read(device()->available());
	}

};

} // namespace io

CWT_NS_END


#endif /* __CWT_IO_DATAREADER_HPP__ */
