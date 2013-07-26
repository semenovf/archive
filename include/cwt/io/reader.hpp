/*
 * reader.hpp
 *
 *  Created on: Jul 11, 2013
 *      Author: wladt
 */

#ifndef __CWT_IO_READER_HPP__
#define __CWT_IO_READER_HPP__

#include <cwt/io/io.hpp>
#include <cwt/shared_ptr.hpp>
#include <cwt/vector.hpp>

CWT_NS_BEGIN

namespace io {

template<class ForwardIterator1, class ForwardIterator2>
  ForwardIterator1 search ( ForwardIterator1 first1, ForwardIterator1 last1,
                            ForwardIterator2 first2, ForwardIterator2 last2)
{
	if (first2==last2) return first1;

	while (first1!=last1) {
		ForwardIterator1 it1 = first1;
		ForwardIterator2 it2 = first2;
		while (*it1 == *it2) {
			++it1; ++it2;
			if (it2 == last2) return first1;
			if (it1 == last1) return last1;
		}
		++first1;
	}
	return last1;
}

template <typename P, typename C>
class Reader
{
public:
	typedef typename C::orig_char_type  orig_char_type;
	typedef typename C::dest_char_type  char_type;
	typedef Vector<char_type>           vector_type;

public:
	Reader (shared_ptr<P> producer, size_t chunkSize = 512)
		: m_chunkSize(chunkSize)
	    , m_producer(producer)
		, m_decoder(new C)
		, m_buffer(new orig_char_type[chunkSize])
		, m_remain(0)
	{}
	Reader (shared_ptr<P> producer, shared_ptr<C> decoder, size_t chunkSize = 512)
		: m_chunkSize(chunkSize)
		, m_producer(producer)
		, m_decoder(decoder)
		, m_buffer(new orig_char_type[chunkSize])
		, m_remain(0)
	{}

	~Reader() { delete m_buffer; }
	bool isError() { return m_producer->isError(); }
	ssize_t read (char_type chars[], size_t size);
	vector_type read (size_t size);

private:
	size_t                  m_chunkSize;
	shared_ptr<P>           m_producer;
	shared_ptr<C>           m_decoder;
	orig_char_type         *m_buffer;
	size_t                  m_remain;
};


template <typename P, typename C>
ssize_t Reader<P, C>::read (Reader<P, C>::char_type chars[], size_t size)
{
	size_t r = 0;
	CWT_ASSERT(size <= CWT_SSIZE_MAX);

	while (r < size) {
		ssize_t nbytes = m_producer->read(m_buffer + m_remain, m_chunkSize - m_remain);
		if (nbytes < 0)
			break;

		nbytes += m_remain;

		if (!nbytes)
			break;

		ssize_t nchars = m_decoder->convert(chars + r, size - r, m_buffer, size_t(nbytes), & m_remain);
		if (nchars < 0) {
			break;
		}
		r += nchars;

		if (m_remain) {
			if (m_remain > 10) {
				memmove(m_buffer, m_buffer + (nbytes - m_remain), m_remain * sizeof(orig_char_type));
			} else {
				for (size_t i = 0, j = nbytes - m_remain; i < m_remain; ++i, ++j) {
					m_buffer[i] = m_buffer[j];
				}
			}
		}
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

template <typename R>
class BufferedReader
{
public:
	typedef typename R::char_type char_type;
	typedef Vector<char_type> vector_type;
private:
	BufferedReader() {}

public:
	BufferedReader(shared_ptr<R> reader) : m_reader(reader), m_buffer(), m_cursor(0) {}
	bool canReadUntil(const vector_type & end, size_t maxSize);
	bool canReadUntil(const vector_type ends[], size_t count, size_t maxSize);
	vector_type readUntil(const vector_type & end, size_t maxSize);
	vector_type readUntil(const vector_type ends[], size_t count, size_t maxSize);
	bool get(char_type & ch);
	bool unget(char_type ch);

protected:
	typename vector_type::const_iterator findFirstEnding(const vector_type ends[], size_t count);
	bool fillBuffer(size_t maxSize, bool resetCursor);

private:
	shared_ptr<R> m_reader;
	vector_type   m_buffer;
	size_t        m_cursor;
};


template <typename R>
bool BufferedReader<R>::fillBuffer(size_t maxSize, bool resetCursor)
{
	if (!m_reader->isError()) {
		if (m_cursor > 0 && m_cursor >= m_buffer.size()) {
			m_buffer.clear();
			m_cursor = 0;
		}

		if (resetCursor && m_cursor > 0 && m_buffer.size() > 0) {
			m_buffer.remove(0, m_cursor);
			m_cursor = 0;
		}

		m_buffer.reserve(maxSize);
		ssize_t nchars = m_reader->read(m_buffer.data() + m_buffer.size(), maxSize - m_buffer.size());

		if (nchars < 0)
			return false;

		m_buffer.resize(m_buffer.size() + size_t(nchars));

		if (!m_buffer.size())
			return false;

		return true;
	}

	return false;
}

template <typename R>
inline bool BufferedReader<R>::get(char_type & ch)
{
	if (fillBuffer(256, false)) {
		ch = m_buffer.at(m_cursor++);
		return true;
	}
	return false;
}


template <typename R>
bool BufferedReader<R>::unget(char_type ch)
{
	if (!m_reader->isError()) {
		if (m_cursor > 0 && m_cursor >= m_buffer.size()) {
			m_buffer[--m_cursor] = ch;
			return true;
		}

		if (!m_cursor) {
			m_buffer.prepend(ch);
			return true;
		}
	}

	return false;
}

template <typename R>
typename BufferedReader<R>::vector_type::const_iterator BufferedReader<R>::findFirstEnding(const vector_type ends[], size_t count)
{
	for (size_t i = 0; i < count; ++i) {
		typename vector_type::const_iterator it;
		it = search(m_buffer.cbegin(), m_buffer.cend(), ends[i].cbegin(), ends[i].cend());
		if (it != m_buffer.cend()) {
			return it;
		}
	}
	return m_buffer.cend();
}

template <typename R>
inline bool BufferedReader<R>::canReadUntil(const BufferedReader<R>::vector_type & end, size_t maxSize)
{
	const vector_type ends[1] = { end };
	return canReadUntil(ends, 1, maxSize);
}

template <typename R>
inline bool BufferedReader<R>::canReadUntil(const BufferedReader<R>::vector_type ends[], size_t count, size_t maxSize)
{
	if (fillBuffer(maxSize, true)) {
		return findFirstEnding(ends, count) != m_buffer.cend();
	}
	return false;
}


template <typename R>
inline typename BufferedReader<R>::vector_type BufferedReader<R>::readUntil(
	  const BufferedReader<R>::vector_type & end
	, size_t maxSize)
{
	const vector_type ends[1] = { end };
	return readUntil(ends, 1, maxSize);
}

template <typename R>
typename BufferedReader<R>::vector_type BufferedReader<R>::readUntil(
	  const BufferedReader<R>::vector_type ends[]
	, size_t count
	, size_t maxSize)
{
	vector_type r;
	CWT_ASSERT(maxSize > 1);

	if (fillBuffer(maxSize, true)) {
		for (size_t i = 0; i < count; ++i) {
			typename vector_type::const_iterator it = findFirstEnding(ends, count);

			if (it != m_buffer.cend()) {
				r = m_buffer.left(it - m_buffer.cbegin());
				m_buffer.remove(0, it - m_buffer.cbegin() + ends[i].size());
				break;
			}
		}

		if (r.isEmpty()) {
			r = m_buffer;
			m_buffer.clear();
		}
	}

	return r;
}

} // namespace io

CWT_NS_END

#endif /* __CWT_IO_READER_HPP__ */
