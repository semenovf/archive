/*
 * reader.hpp
 *
 *  Created on: Jul 11, 2013
 *      Author: wladt
 */

#ifndef __CWT_IO_READER_HPP__
#define __CWT_IO_READER_HPP__

#include <cwt/io/io.hpp>
//#include <cwt/deque.hpp>
#include <cwt/shared_ptr.hpp>
#include <cwt/errorable.hpp>
//#include <algorithm> // std::copy
//#include <iterator>  // std::back_inserter

CWT_NS_BEGIN

namespace io {

template <typename char_type>
class Buffer : public Vector<char_type>
{
public:
	Buffer(size_t size) : Vector<char_type>(), m_head(0) { this->reserve(size); }
	size_t count() const { return this->size() - m_head; }

	void shift(size_t n)
	{
		m_head += n;
		if (m_head >= this->size()) {
			this->clear();
			m_head = 0;
		}
	}

	void unshift()
	{
		if (m_head > 0) {
			this->remove(0, m_head);
			m_head = 0;
		}
	}

private:
	size_t m_head;
};


template <typename P, typename C>
class Reader
{
public:
	typedef typename C::orig_char_type  orig_char_type;
	typedef typename C::dest_char_type  dest_char_type;
	typedef typename C::dest_char_type  char_type;
	typedef Vector<dest_char_type>      vector_type;

public:
	Reader(shared_ptr<P> producer) : m_producer(producer), m_decoder(new C) {}
	Reader(shared_ptr<P> producer, shared_ptr<C> decoder) : m_producer(producer), m_decoder(decoder) {}

	bool    isError  () { return m_producer->isError(); }
	bool    atEnd    () { return m_producer->atEnd() && m_bufferT2.isEmpty(); }
	bool    get      (char_type & ch);
	bool    unget    (char_type ch);
	vector_type read (size_t size);

/*
protected:
	bool readToInternalBuffer();
*/

private:
	shared_ptr<P>           m_producer;
	shared_ptr<C>           m_decoder;
	Buffer<orig_char_type>  m_bufferT1;
	Buffer<dest_char_type>  m_bufferT2;
};

#ifdef __COMMENT__

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
#endif

template <typename P, typename C>
typename Reader<P, C>::vector_type Reader<P, C>::read (size_t size)
{
	vector_type r;

	// If internal buffer has previous read chars, append they to result
	if (m_bufferT2.count() > 0) {
		size_t sz = CWT_MIN(size, m_bufferT2.size());
		r.append(m_bufferT2.left(sz));
		m_bufferT2.shift(sz);
	}

	while (r.size() < size) {
		m_bufferT1.unshift(); // m_bufferT::m_head is zero now

		if (m_bufferT1.count() < P::ChunkSize / 2) {
			m_bufferT1.reserve(P::ChunkSize);
			ssize_t nbytes = m_producer->read(P::ChunkSize - m_bufferT1.count());
			if (nbytes <= 0) {
				if (m_producer->isError()) {
					r.clear();
				}
				break;
			}
			m_bufferT1.resize(size_t(nbytes) + m_bufferT1.count());
		}

		vector_type decoded;
		ssize_t remain = 0;
		decoded = m_decoder->convert(m_bufferT1, & remain);

		if (remain < 0) { // error
			m_producer->addError(_Tr("decoding failed"));
			r.clear();
			break;
		}

		if (remain > 0) {
			m_bufferT1.shift(bytes.right(size_t(remain)));
		}

		bytes.clear();
		r.append(decoded);
	}

	if (r.size() > size) {
		m_bufferT2.append(r.right(r.size() - size));
		r.remove(size, r.size() - size); // truncate
	}

	return r;
}

// FIXME need more effective algorithm
template <typename P, typename C>
bool Reader<P, C>::get(Reader<P, C>::char_type & ch)
{
	Vector<char_type> v = read(1);
	if (v.isEmpty())
		return false;
	ch = v.at(0);
	return true;
}

template <typename P, typename C>
bool Reader<P, C>::unget (Reader<P, C>::char_type ch)
{
	m_bufferT2.prepend(ch);
	return true;
}

enum ReadLineStatus {
	  ReadLine_Error    =    -2
	, ReadLine_Overflow =    -1
	, ReadLine_Intermediate = 0
	, ReadLine_AtEnd    =     1
	, ReadLine_Success  =     2
};


template <typename R>
class LineReader
{
	typedef typename R::char_type char_type;
private:
	LineReader() {}

public:
	LineReader(R * reader, size_t maxSize = 256);
	ReadLineStatus readLine(Vector<char_type> & line);
	void setEndLine(const Vector<char_type> & endl) { m_ends.clear(); m_ends.append(endl); }
	void addEndLine(const Vector<char_type> & endl) { m_ends.append(endl); }

private:
	size_t m_maxSize;
	R *    m_reader;
	Vector<Vector<char_type> >  m_ends;
};

/**
 *
 * @param reader
 * @param endIsSuccess Interpret the end of the reader as Success status, otherwise as Intermediate.
 * @param maxSize
 */
template <typename R>
inline LineReader<R>::LineReader(R * reader, size_t maxSize)
	: m_maxSize(maxSize)
	, m_reader(reader)
{
	//setEndLine(defaultEndLine);
}

template <typename R>
ReadLineStatus LineReader<R>::readLine(Vector<char_type> & line)
{
	char_type ch;
	ReadLineStatus rc = ReadLine_Intermediate;

	if (line.size() >= m_maxSize) {
		rc = ReadLine_Overflow;
	} else {
		size_t n = 0;

		while (m_reader->get(ch)) {
			line.append(ch);

			for (size_t i = 0; i < m_ends.size(); ++i) {
				if (line.endsWith(m_ends[i])) {
					n = m_ends.size();
					break;
				}
			}

			if (n) {
				line.resize(line.size() - n);
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
		rc = ReadLine_AtEnd;
	}

	return rc;
}

} // namespace io

CWT_NS_END

#endif /* __CWT_IO_READER_HPP__ */
