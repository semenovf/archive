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
#include <cwt/bytearray.hpp>

CWT_NS_BEGIN

namespace io {

struct NullByteCodec : public NullCodec<char>
{
	typedef NullCodec<char>::orig_char_type orig_char_type;
	typedef NullCodec<char>::dest_char_type dest_char_type;
	typedef ByteArray vector_type;
};

class DLL_API DataReader : public BufferedReader<Reader<Device, NullByteCodec> >
{
	static const size_t MaxLineLength = 4096;
	typedef Reader<Device, NullByteCodec> base_reader;
	typedef BufferedReader<base_reader>   base_class;

	CWT_DENY_COPY(DataReader);

public:
	typedef typename base_class::vector_type vector_type;

public:
	DataReader(Device * dev, size_t chunkSize = base_reader::DefaultChunkSize)
		: base_class(new base_reader(dev, new NullByteCodec, chunkSize))
		{}
	//Device * device() const { return m_producer.get(); }

	ByteArray readLine(const ByteArray & end = ByteArray("\n", 1), size_t maxLength = MaxLineLength)
	{
		return readUntil(end, maxLength);
	}

	ByteArray readLine(const ByteArray ends[], size_t count, size_t maxLength = MaxLineLength)
	{
		return readUntil(ends, count, maxLength);
	}
};

} // namespace io

CWT_NS_END


#endif /* __CWT_IO_DATAREADER_HPP__ */
