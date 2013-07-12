/*
 * reader.hpp
 *
 *  Created on: Jul 11, 2013
 *      Author: wladt
 */

#ifndef __CWT_IO_READER_HPP__
#define __CWT_IO_READER_HPP__

#include <cwt/io/io.hpp>
#include <cwt/deque.hpp>
#include <cwt/shared_ptr.hpp>
#include <cwt/errorable.hpp>
#include <algorithm> // std::copy
#include <iterator>  // std::back_inserter

CWT_NS_BEGIN

namespace io {

template <typename P>
class Producer
{
public:
	typedef typename P::char_type char_type;
	typedef Vector<char_type>     vector_type;

	Producer(shared_ptr<P> p) : m_p(p) {}
	bool    isError  () { return m_p->isError(); }
	bool    atEnd    () { return m_p->atEnd(); }
	vector_type read (size_t size) { return m_p->read(size); }

private:
	shared_ptr<P> m_p;
};

template <typename P, typename T>
class Reader : public Errorable
{
public:
	typedef typename P::char_type pchar_type; // Producer's character type
	typedef Vector<T> vector_type;
	typedef Producer<P> producer_type;
	typedef Codec<pchar_type, T> decoder_type;
	typedef FilterChain<T> filter_chain_type;

public:
	Reader(shared_ptr<P> producer);

	Reader(shared_ptr<P> producer
			, shared_ptr<decoder_type> decoder);

	Reader(shared_ptr<P> producer
			, shared_ptr<decoder_type> decoder
			, shared_ptr<filter_chain_type> filterChain);

	bool    isError () { return m_producer->isError() || isError(); }
	bool    atEnd   () { return m_producer->atEnd() && m_bufferT2.isEmpty(); }
	bool    get     (T & ch);
	bool    unget   (T ch);
	vector_type read (size_t size);

private:
	producer_type m_producer;
	shared_ptr<decoder_type>      m_decoder;
	shared_ptr<filter_chain_type> m_filterChain;
	Deque<pchar_type>  m_bufferT1;
	Deque<T>           m_bufferT2;
};


template <typename P, typename T>
inline Reader<P, T>::Reader(shared_ptr<P> producer)
	: m_producer(producer)
	, m_decoder()
	, m_filterChain()
{
}

template <typename P, typename T>
inline Reader<P, T>::Reader(
		  shared_ptr<P> producer
		, shared_ptr<decoder_type> decoder)
	: m_producer(producer)
	, m_decoder(decoder)
	, m_filterChain()
{
}

template <typename P, typename T>
inline Reader<P, T>::Reader(
		  shared_ptr<P> producer
		, shared_ptr<decoder_type> decoder
		, shared_ptr<filter_chain_type> filterChain)
	: m_producer(producer)
	, m_decoder(decoder)
	, m_filterChain(filterChain)
{
}


template <typename T>
inline void __append_vector(Deque<T> & d, Vector<T> v)
{
	//std::copy(v.cbegin(), v.cend(), std::back_inserter(d)); // I have no const_reference for Vector

	typename Vector<T>::const_iterator it = v.cbegin();
	typename Vector<T>::const_iterator itEnd = v.cend();
	while (it != itEnd) {
		d.append(*it);
		++it;
	}
}

template <typename T>
inline void __append_deque(Vector<T> &v, Deque<T> d)
{
//	std::copy(d.cbegin(), d.cend(), std::back_inserter(v));
	typename Deque<T>::const_iterator it = d.cbegin();
	typename Deque<T>::const_iterator itEnd = d.cend();
	while (it != itEnd) {
		v.append(*it);
		++it;
	}

}


template <typename P, typename T>
typename Reader<P, T>::vector_type Reader<P, T>::read (size_t size)
{
	vector_type r;

	// If internal buffer has previous read chars, append they to result
	if (m_bufferT2.size() > 0) {
		size_t sz = CWT_MIN(size, m_bufferT2.size());
		__append_deque(r, m_bufferT2.left(sz));
		m_bufferT2.remove(0, sz);
	}

	Vector<pchar_type> bytes;

	while (r.size() < size) {
		if (!m_bufferT1.isEmpty()) {
			__append_deque(bytes, m_bufferT1);
			m_bufferT1.clear();
		}

		if (bytes.isEmpty()) {
			bytes = m_producer.read(P::ChunkSize);
		}

		if (bytes.isEmpty()) {
			if (m_producer.isError()) {
				r.clear();
			}
			break;
		}

		vector_type decoded;

		if (m_decoder.get()) {
			ssize_t nonconverted = 0;
			decoded = m_decoder->convert(bytes, & nonconverted);

			if (nonconverted < 0) { // error
				addError(_Tr("Decoding failed"));
				break;
			}

			if (nonconverted > 0) {
				__append_vector(m_bufferT1, bytes.right(size_t(nonconverted)));
			}
		} else {
			if (sizeof(pchar_type) != sizeof(T)) {
				addError(_Tr("Decoder must be specified"));
				break;
			}
			decoded = bytes;
		}

		if (m_filterChain.get()) {
			bool ok = true;
			decoded = m_filterChain->process(decoded, &ok);

			if (!ok) {
				addError(_Tr("Filtering failed"));
				break;
			}
		}

		bytes.clear();
		r.append(decoded);
	}

	if (r.size() > size) {
		__append_vector(m_bufferT2, r.right(r.size() - size));
		r.remove(size, r.size() - size);
	}

	return r;
}

template <typename P, typename T>
bool Reader<P, T>::get(T & ch)
{
	if (m_bufferT2.isEmpty()) {
		m_bufferT2 = read(256);
	}

	if (m_bufferT2.isEmpty())
		return false;

	ch = m_bufferT2[0];
	m_bufferT2.remove(0, 1);
}

template <typename P, typename T>
bool Reader<P, T>::unget (T ch)
{
	m_bufferT2.prepend(ch);
	return true;
}

enum ReadLineStatus {
	  ReadLine_Success
	, ReadLine_Intermediate
	, ReadLine_AtEnd
	, ReadLine_Overflow
	, ReadLine_Error
};

#ifdef __COMMENT__

template <typename T>
struct Line
{
	void append(const T &value);
	void resize(size_t size);
	bool endsWith (Line<T> & end) const;
};

template <typename T1, typename T2>
class LineReader
{
	static const Line<T2> defaultEndLine;

	typedef Reader<T1, T2> reader_type;
private:
	LineReader() {}

public:
	LineReader(shared_ptr<reader_type> reader, size_t maxSize = 256);
	ReadLineStatus readLine(Line<T2> & line);
	void setEndLine(const Line<T2> & endl) { m_ends.clear(); m_ends.append(endl); }
	void addEndLine(const Line<T2> & endl) { m_ends.append(endl); }

private:
	size_t                  m_maxSize;
	shared_ptr<reader_type> m_reader;
	Vector<Line<T2> >       m_ends;
};

/**
 *
 * @param reader
 * @param endIsSuccess Interpret the end of the reader as Success status, otherwise as Intermediate.
 * @param maxSize
 */
template <typename T1, typename T2>
inline LineReader<T1, T2>::LineReader(shared_ptr<Reader<T1,T2> > reader, size_t maxSize)
	: m_maxSize(maxSize)
	, m_reader(reader)
{
	setEndLine(defaultEndLine);
}

template <typename T1, typename T2>
ReadLineStatus LineReader<T1, T2>::readLine(Line<T2> & line)
{
	T2 ch;
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
#endif
} // namespace io

CWT_NS_END

#endif /* __CWT_IO_READER_HPP__ */
