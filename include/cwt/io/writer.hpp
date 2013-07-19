/*
 * writer.hpp
 *
 *  Created on: Jul 11, 2013
 *      Author: wladt
 */

#ifndef __CWT_IO_WRITER_HPP__
#define __CWT_IO_WRITER_HPP__

#include <cwt/io/io.hpp>
#include <cwt/shared_ptr.hpp>
#include <cwt/vector.hpp>

CWT_NS_BEGIN

namespace io {


template <typename Consumer, typename Encoder>
class Writer
{
	static const size_t ChunkSize = 256;
public:
	typedef typename Encoder::orig_char_type  char_type;
	typedef typename Encoder::dest_char_type  dest_char_type;
	typedef Vector<char_type>                 vector_type;

public:
	Writer (shared_ptr<Consumer> consumer)
	    : m_consumer(consumer)
		, m_encoder(new Encoder)
	{}
	Writer (shared_ptr<Consumer> consumer, shared_ptr<Encoder> encoder)
		: m_consumer(consumer)
		, m_encoder(encoder)
	{}

	~Writer() { }
	bool isError() { return m_consumer->isError(); }
	ssize_t write (const char_type chars[], size_t size);
	ssize_t write (const vector_type & chars);

private:
	shared_ptr<Consumer>    m_consumer;
	shared_ptr<Encoder>     m_encoder;
};


template <typename Consumer, typename Encoder>
inline ssize_t Writer<Consumer, Encoder>::write(const Writer<Consumer, Encoder>::vector_type & chars)
{
	return this->write(chars.constData(), chars.size());
}

template <typename Consumer, typename Encoder>
inline ssize_t Writer<Consumer, Encoder>::write(const char_type chars[], size_t size)
{
	ssize_t ntotal = 0;
	size_t  remain = size;
	dest_char_type buffer[ChunkSize];

	while (remain) {
		ssize_t encoded = m_encoder->convert(buffer, ChunkSize, chars, size, & remain);

		if (encoded < 0)
			return ssize_t(-1);

		if (encoded == 0 && remain > 0)
			break;

		chars += size - remain;
		size -= size - remain;

		ssize_t written = m_consumer->write(buffer, encoded);

		if (written <= 0)
			break;

		ntotal += written;
	}

	CWT_ASSERT(ntotal <= CWT_SSIZE_MAX);
	return ntotal;
}


} // namespace io

CWT_NS_END

#endif /* __CWT_IO_WRITER_HPP__ */
