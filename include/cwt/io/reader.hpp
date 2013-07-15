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

#ifdef __COMMENT__
template <typename char_type>
class Buffer
{
public:
	Buffer(size_t size) : m_buffer(new char_type[size]), m_size(size), m_head(0) { }
	~Buffer() { delete[] m_buffer; }
	size_t count() const { return m_size - m_head; }

/*	void shift(size_t n)
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
	}*/

private:
	char_type  *m_buffer;
	size_t      m_size;
	size_t      m_head;
};
#endif

template <typename P, typename C>
class Reader
{
public:
	typedef typename C::orig_char_type  orig_char_type;
	typedef typename C::dest_char_type  char_type;
	typedef Vector<char_type>           vector_type;

public:
	Reader(shared_ptr<P> producer)
		: m_producer(producer)
		, m_decoder()
		, m_buffer(new orig_char_type[m_producer->chunkSize()])
		, m_remain(0)
	{}
	Reader(shared_ptr<P> producer, shared_ptr<C> decoder)
		: m_producer(producer)
		, m_decoder(decoder)
		, m_buffer(new orig_char_type[m_producer->chunkSize()])
		, m_remain(0)
	{}

	~Reader() { delete m_buffer; }

	ssize_t read (char_type chars[], size_t size);
	vector_type read (size_t size);

private:
	shared_ptr<P>           m_producer;
	shared_ptr<C>           m_decoder;
	orig_char_type         *m_buffer;
	size_t                  m_remain;
};


template <typename P, typename C>
ssize_t Reader<P, C>::read (Reader<P, C>::char_type chars[], size_t size)
{
	size_t r = 0;
	size_t chunkSize = m_producer->chunkSize();

	CWT_ASSERT(size <= CWT_SSIZE_MAX);

	while (r <= size) {
		ssize_t nbytes = m_producer->read(m_buffer + m_remain, chunkSize - m_remain);
		if (nbytes <= 0)
			break;

		ssize_t nchars = m_decoder->convert(chars + r, size - r, m_buffer, size_t(nbytes), & m_remain);
		if (nchars <= 0) {
			break;
		}
		r += nchars;

		// TODO if m_remain < 10 do byte-to-byte copy instead of memmove
		memmove(m_buffer, m_buffer + nbytes - m_remain, m_remain * sizeof(orig_char_type));
	}

	return ssize_t(r);
}

template <typename P, typename C>
inline typename Reader<P, C>::vector_type Reader<P, C>::read (size_t size)
{
	vector_type r;
	r.reserve(size);
	ssize_t nchars = this->read(r.data(), size);
	if (nchars > 0)
		r.resize(size_t(nchars));
	return r;
}

// FIXME need more effective algorithm
/*
template <typename P, typename C>
bool Reader<P, C>::get(Reader<P, C>::char_type & ch)
{
	Vector<char_type> v = read(1);
	if (v.isEmpty())
		return false;
	ch = v.at(0);
	return true;
}
*/

enum ReadLineStatus {
	  ReadLine_Error    =    -2
	, ReadLine_Overflow =    -1
	, ReadLine_Intermediate = 0
	, ReadLine_AtEnd    =     1
	, ReadLine_Success  =     2
};

#ifdef __COMMENT__
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
#endif
} // namespace io

CWT_NS_END

#endif /* __CWT_IO_READER_HPP__ */
