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

class DLL_API DataReader : public BufferedReader<Reader<Device, NullByteCodec> >
{
	static const size_t MaxLineLength = 4096;

	typedef Reader<Device, NullByteCodec> base_reader;
	typedef BufferedReader<base_reader>   base_class;

	CWT_DENY_COPY(DataReader);

public:
	typedef typename base_class::vector_type vector_type;

public:
	DataReader(shared_ptr<Device> dev, size_t chunkSize = base_reader::DefaultChunkSize)
		: base_class(shared_ptr<base_reader>(new base_reader(dev, shared_ptr<NullByteCodec>(new NullByteCodec), chunkSize)))
		{}

	Device * device() const { return reader()->producer(); }

	ByteArray readLine(const ByteArray & end = ByteArray("\n"), bool * ok = nullptr, size_t maxLength = MaxLineLength)
	{
		return readUntil(end, ok, maxLength);
	}

	ByteArray readLine(const ByteArray ends[], size_t count, bool * ok = nullptr, size_t maxLength = MaxLineLength)
	{
		return readUntil(ends, count, ok, maxLength);
	}
};

} // namespace io

CWT_NS_END


#endif /* __CWT_IO_DATAREADER_HPP__ */
