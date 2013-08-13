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

class DLL_API TextReader : public BufferedReader<Reader<Device, Utf8Decoder> >
{
public:
	static const size_t MaxLineLength = 4096;

private:
	typedef Reader<Device, Utf8Decoder> base_reader;
	typedef BufferedReader<base_reader> base_class;

	CWT_DENY_COPY(TextReader);

public:
	typedef typename base_class::vector_type vector_type;

public:
	TextReader(shared_ptr<Device> dev, shared_ptr<Utf8Decoder> decoder, size_t chunkSize = base_reader::DefaultChunkSize)
		: base_class(shared_ptr<base_reader> (new base_reader(dev, decoder, chunkSize)))
		{}

	TextReader(shared_ptr<Device> dev, size_t chunkSize = base_reader::DefaultChunkSize)
		: base_class(shared_ptr<base_reader> (new base_reader(dev, shared_ptr<Utf8Decoder>(new Utf8NullDecoder), chunkSize)))
		{}

	Device * device() const { return reader()->producer(); }

	Utf8String readLine(const Utf8String & end = Utf8String("\n"), bool * ok = nullptr, size_t maxLength = MaxLineLength)
	{
		return readUntil(end, ok, maxLength);
	}

	Utf8String readLine(const Utf8String ends[], size_t count, bool * ok = nullptr, size_t maxLength = MaxLineLength)
	{
		return readUntil(ends, count, ok, maxLength);
	}
};

} // namespace io

CWT_NS_END

#endif /* __CWT_IO_TEXTREADER_HPP__ */
