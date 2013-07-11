/*
 * reader.hpp
 *
 *  Created on: Jul 11, 2013
 *      Author: wladt
 */

#ifndef __CWT_IO_READER_HPP__
#define __CWT_IO_READER_HPP__

#include <cwt/io.hpp>
#include <cwt/shared_ptr.hpp>
#include <cwt/errorable.hpp>

CWT_NS_BEGIN

namespace io {

template <typename T>
struct Producer
{
	static size_t ChunkSize;
	bool    isError ();
	bool    atEnd   ();
	bool    get     (T & ch);
	bool    unget   (T ch);
	Vector<T> read  (size_t size);
};

template <typename T1, typename T2>
class Reader : public Errorable
{
	typedef Producer<T1>  producer_type;
	typedef Codec<T1, T2> decoder_type;
	typedef FilterChain<T2> filter_chain_type;

public:
	Reader(shared_ptr<producer_type> producer
			, shared_ptr<decoder_type> decoder
			, shared_ptr<filter_chain_type> filterChain);

	bool    isError () { return m_producer->isError(); }
	bool    atEnd   () { return m_producer->atEnd(); }
	bool    get     (T2 & ch);
	Vector<T2> read (size_t size);

private:
	shared_ptr<producer_type> m_producer;
	shared_ptr<decoder_type>  m_decoder;
	shared_ptr<filter_chain_type> m_filterChain
	Vector<T1> m_bufferT1;
	Vector<T2> m_bufferT2;
};

template <typename T1, typename T2>
inline Reader<T1,T2>::Reader(
		  shared_ptr<producer_type> producer
		, shared_ptr<decoder_type> decoder
		, shared_ptr<filter_chain_type> filterChain)
	: m_producer(producer)
	, m_decoder(decoder)
	, m_filterChain(filterChain)
{
}

template <typename T1, typename T2>
Vector<T2> Reader<T1,T2>::read (size_t size)
{
	Vector<T2> r;

	if (m_bufferT2.length() > 0) {
		r.append(m_bufferT2.substr(0, CWT_MIN(size, m_bufferT2.length())));
		m_bufferT2.remove(0, CWT_MIN(size, m_bufferT2.length()));
	}

	Vector<T1> bytes(m_bufferT1);
	m_bufferT1.clear();

	while (r.size() < size) {
		Vector<T1> bytes = m_producer->read(producer_type::ChunkSize);

		if (bytes.isEmpty() && (m_producer->atEnd() || m_producer->isError())) {
			break;
		}

		ssize_t nonconverted = 0;
		Vector<T2> decoded = m_decoder->convert(bytes, & nonconverted);

		if (nonconverted < 0) { // error
			addError(_Tr("Decoding failed"));
			break;
		}

		if (*m_filterChain) {
			bool ok = true;
			decoded = m_filterChain->process(decoded, &ok);

			if (!ok) {
				addError(_Tr("Filtering failed"));
				break;
			}
		}

		r.append(decoded);
	}

	if (r.size() > size) {
		m_bufferT2.append(r.substr(size));
		r.remove(size, r.size() - size);
	}

	return r;
}


enum ReadLineStatus {
	  ReadLine_Success
	, ReadLine_Intermediate
	, ReadLine_AtEnd
	, ReadLine_Overflow
	, ReadLine_Error
};

template <typename T>
struct Line
{
	void append(const T &value);
	void resize(size_t size);
	bool endsWith (Line<T> & end) const;
};

template <typename T>
class LineReader
{
	static const Line<T> defaultEndLine;

	typedef Reader<T> reader_type;
private:
	LineReader() {}

public:
	LineReader(shared_ptr<reader_type> reader, size_t maxSize = 256);
	ReadLineStatus readLine(Line<T> & line);
	void setEndLine(const Line<T> & endl) { m_ends.clear(); m_ends.append(endl); }
	void addEndLine(const Line<T> & endl) { m_ends.append(endl); }

private:
	size_t                 m_maxSize;
	shared_ptr<reader_type> m_reader;
	Vector<Line<T> >       m_ends;
};

/**
 *
 * @param reader
 * @param endIsSuccess Interpret the end of the reader as Success status, otherwise as Intermediate.
 * @param maxSize
 */
template <typename T>
inline LineReader<T>::LineReader(shared_ptr<Reader<T> > reader, size_t maxSize)
	: m_maxSize(maxSize)
	, m_reader(reader)
{
	setEndLine(defaultEndLine);
}

template <typename T>
ReadLineStatus LineReader<T>::readLine(Line<T> & line)
{
	T ch;
	ReadLineStatus rc = ReadLine_Intermediate;

	if (line.size() >= m_maxSize) {
		rc = ReadLine_Overflow;
	} else {
		size_t n = 0;

		while (m_reader->get(&ch)) {
			line.append(ch);

			for (size_t i = 0; i < m_ends.size(); ++i) {
				if (line.endsWith(m_ends[i])) {
					n = m_ends.size();
					break;
				}
			}

			if (n) {
				line.resize(line.length() - n);
				rc = ReadLine_Success;
				break;
			}

			if (line.size() >= m_maxSize) {
				rc = ReadLine_Overflow;
				break;
			}
		}
	}

	if (m_reader->isError()) {
		rc = ReadLine_Error;
	} else if (m_reader->atEnd()) {
		rc == ReadLine_AtEnd;
	}

	return rc;
}

} // namespace io

CWT_NS_END

#endif /* __CWT_IO_READER_HPP__ */
