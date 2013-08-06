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

CWT_NS_BEGIN

namespace io {

class DLL_API TextReader : public BufferedReader<Reader<Device, Utf8Codec> >
{
	static const size_t MaxLineLength = 4096;
	typedef Reader<Device, Utf8Codec>   base_reader;
	typedef BufferedReader<base_reader> base_class;

	CWT_DENY_COPY(TextReader);

public:
	typedef typename base_class::vector_type vector_type;

public:
	TextReader(Device * dev, Utf8BytesConverter * converter, size_t chunkSize = base_reader::DefaultChunkSize)
		: base_class(new base_reader(dev, new io::Utf8Codec(converter), chunkSize))
		{}
	//Device * device() const { return m_producer.get(); }

	Utf8String readLine(const Utf8String & end = Utf8String("\n", 1), size_t maxLength = MaxLineLength)
	{
		return readUntil(end, maxLength);
	}

	Utf8String readLine(const Utf8String ends[], size_t count, size_t maxLength = MaxLineLength)
	{
		return readUntil(ends, count, maxLength);
	}
};

} // namespace io

CWT_NS_END

#endif /* __CWT_IO_TEXTREADER_HPP__ */
