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
	static const size_t DefaultChunkSize = 512;

	typedef typename C::orig_char_type  orig_char_type;
	typedef typename C::dest_char_type  char_type;
	typedef typename C::vector_type     vector_type;

public:
	Reader (shared_ptr<P> producer, size_t chunkSize = DefaultChunkSize)
		: m_chunkSize (chunkSize)
	    , m_producer  (producer)
		, m_decoder   (new C)
		, m_buffer    (new orig_char_type[chunkSize])
		, m_remain    (0)
	{}

	Reader (shared_ptr<P> producer, shared_ptr<C> decoder, size_t chunkSize = DefaultChunkSize)
		: m_chunkSize (chunkSize)
		, m_producer  (producer)
		, m_decoder   (decoder)
		, m_buffer    (new orig_char_type[chunkSize])
		, m_remain    (0)
	{}

	~Reader()      { delete m_buffer; }
	bool isError() { return m_producer->isError(); }
	ssize_t read (char_type chars[], size_t size);
	bool atEnd    () const { return m_producer->atEnd(); }

	P * producer() const { return m_producer.get(); }

protected:
	size_t           m_chunkSize; // counted in original chars
	shared_ptr<P>    m_producer;
	shared_ptr<C>    m_decoder;
	orig_char_type * m_buffer;
	size_t           m_remain;    // counted in original chars
};

template <typename P, typename C>
ssize_t Reader<P, C>::read (Reader<P, C>::char_type chars[], size_t size)
{
	size_t r = 0;
	CWT_ASSERT(size <= CWT_SSIZE_MAX);

	while (r < size) {
		ssize_t nbytes = m_producer->read(m_buffer + m_remain, m_chunkSize - m_remain);

		if (nbytes < 0)
			return ssize_t(-1);

		nbytes += m_remain;

		if (!nbytes)
			break;

		ssize_t nchars = m_decoder->convert(chars + r, size - r, m_buffer, size_t(nbytes), & m_remain);

		if (nchars < 0)
			return ssize_t(-1);

		if (!nchars)
			break;

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

// MT-Safe
template <typename R>
class BufferedReader
{
public:
	static const size_t MaxChunkSize = 512;
	static const bool ResetCursor    = true;
	static const bool NotResetCursor = false;

	typedef typename R::char_type char_type;
	typedef typename R::vector_type vector_type;

private:
	BufferedReader() {}

public:
    class iterator;
    friend class iterator;

	class iterator {
		friend class BufferedReader;
		BufferedReader * m_reader;
		char_type        m_value;

	public:
		iterator() : m_reader(nullptr) {}
		iterator(BufferedReader & reader) : m_reader(& reader) { ++*this; }
		iterator(const iterator & x) : m_reader(x.m_reader), m_value(x.m_value) {}
		~iterator() {}

		const char_type & operator * () const { return   m_value; }
		const char_type * operator-> () const { return & m_value; }

        bool operator  == (const iterator & o) const { return m_reader == o.m_reader; }
        bool operator  != (const iterator & o) const { return !(m_reader == o.m_reader); }

		iterator & operator ++ () {
		    if (m_reader && m_reader->atLeast(1)) {
		    	m_value = m_reader->m_buffer[m_reader->m_cursor++];
		    } else {
		    	m_reader = nullptr;
		    }
		    return *this;
		}

		iterator operator ++ (int) {
		    iterator r = *this;
		    ++*this;
		    return r;
		}

		bool unget(char_type ch);
	};

public:
	BufferedReader(shared_ptr<R> reader)
		: m_reader(reader)
		, m_buffer()
		, m_cursor(0) {}
	vector_type read (size_t maxSize);
	bool canReadUntil(const vector_type & end, size_t maxSize, typename vector_type::const_iterator * it = nullptr);
	bool canReadUntil(const vector_type ends[], size_t count, size_t maxSize, typename vector_type::const_iterator * it = nullptr, ssize_t * pend_index = nullptr);
	vector_type readUntil(const vector_type & end, bool * ok, size_t maxSize);
	vector_type readUntil(const vector_type ends[], size_t count, bool * ok, size_t maxSize);
	bool atEnd    () const { return m_reader->atEnd(); }
	R * reader() const { return m_reader.get(); }

protected:
	typename vector_type::const_iterator findFirstEnding(const vector_type ends[], size_t count, ssize_t & end_index);
	/*bool fillBuffer(size_t lowMaxLength, bool resetCursor);*/ // TODO obsolete
	ssize_t fillBuffer(size_t maxSize);
	bool atLeast(size_t count);

protected:
	shared_ptr<R> m_reader;
	vector_type   m_buffer;
	size_t        m_cursor;
};


/**
 * @brief Fill the internal buffer with the data from reader
 *
 * @param maxSize Internal buffer need to contain near maxSize characters or less.
 * @return total  Number of characters read from reader or -1 if error occurred.
 */
template <typename R>
ssize_t BufferedReader<R>::fillBuffer(size_t maxSize)
{
	CWT_ASSERT(!m_reader->isError());

	ssize_t r = 0;
	char_type tmp[MaxChunkSize];

	while (m_buffer.length() < maxSize) {
		ssize_t nchars = m_reader->read(tmp, MaxChunkSize);

		if (nchars < 0) {
			return ssize_t(-1);
		}

		if (!nchars)
			break;

		m_buffer.append(tmp, size_t(nchars));
		r += nchars;
	}

	return r;
}

template <typename R>
bool BufferedReader<R>::atLeast(size_t count)
{
	if (m_cursor + count <= m_buffer.length()) {
		if (m_cursor > m_buffer.length() * 3 / 4) {
			m_buffer.remove(0, m_cursor);
			m_cursor = 0;
		}
		return true;
	}

	if (m_cursor > 0 && m_cursor >= m_buffer.length()) {
		m_buffer.clear();
		m_cursor = 0;
	}

	if (fillBuffer(m_cursor + count) < 0) {
		return false;
	}

	if (m_cursor + count <= m_buffer.length())
		return true;

	return false;
}

template <typename R>
typename BufferedReader<R>::vector_type BufferedReader<R>::read (size_t maxSize)
{
	BufferedReader<R>::vector_type r;

	if (atLeast(maxSize)) {
		r = m_buffer.right(maxSize);
		m_cursor += maxSize;
	} else if (m_buffer.length() > m_cursor) {
		r = m_buffer.right(m_buffer.length() - m_cursor);
		m_buffer.clear();
		m_cursor = 0;
	} else {
		;
	}

	return r;
}

template <typename R>
bool BufferedReader<R>::iterator::unget(char_type ch)
{
	CWT_ASSERT(m_reader);

	if (m_reader->m_cursor > 0 && m_reader->m_cursor >= m_buffer.length()) {
		m_reader->m_buffer[--m_buffer->m_cursor] = ch;
		return true;
	}

	if (!m_reader->m_cursor) {
		m_reader->m_buffer.prepend(ch);
		return true;
	}

	return false;
}


template <typename R>
typename BufferedReader<R>::vector_type::const_iterator BufferedReader<R>::findFirstEnding(
	  const vector_type ends[]
	, size_t count
	, ssize_t & end_index)
{
	CWT_ASSERT(count <= CWT_SSIZE_MAX);
	typename vector_type::const_iterator itBegin = m_buffer.cbegin() + m_cursor;
	end_index = -1;

	for (size_t i = 0; i < count; ++i) {
		typename vector_type::const_iterator it;
		it = search(itBegin, m_buffer.cend(), ends[i].cbegin(), ends[i].cend());
		if (it != m_buffer.cend()) {
			end_index = ssize_t(i);
			return it;
		}
	}
	return m_buffer.cend();
}

template <typename R>
inline bool BufferedReader<R>::canReadUntil(const BufferedReader<R>::vector_type & end
	, size_t maxSize
	, typename vector_type::const_iterator * pit)
{
	const vector_type ends[1] = { end };
	return canReadUntil(ends, 1, maxSize, pit);
}

template <typename R>
bool BufferedReader<R>::canReadUntil(const BufferedReader<R>::vector_type ends[]
	, size_t count
	, size_t maxSize
	, typename vector_type::const_iterator * pit
	, ssize_t * pend_index)
{
	CWT_ASSERT(maxSize > 0);
	size_t size = 0;
	ssize_t end_index = -1;
	typename vector_type::const_iterator it = m_buffer.cend();
	typename vector_type::const_iterator itEnd = m_buffer.cend();

	while (it == itEnd
			&& size < maxSize
			&& fillBuffer(size + CWT_MIN(MaxChunkSize, maxSize - size)) >= 0) {
		it = findFirstEnding(ends, count, end_index);
		size += CWT_MIN(MaxChunkSize, maxSize - size);
	}

	if (pit)
		*pit = it;

	if (pend_index)
		*pend_index = end_index;

	return it == itEnd ? false : true;
}


template <typename R>
inline typename BufferedReader<R>::vector_type BufferedReader<R>::readUntil(
	  const BufferedReader<R>::vector_type & end
	, bool * pok
	, size_t maxSize)
{
	const vector_type ends[1] = { end };
	return readUntil(ends, 1, pok, maxSize);
}

template <typename R>
typename BufferedReader<R>::vector_type BufferedReader<R>::readUntil(
	  const BufferedReader<R>::vector_type ends[]
	, size_t count
	, bool * pok
	, size_t maxSize)
{
	vector_type r;
	bool ok = false;
	ssize_t end_index = -1;
	typename vector_type::const_iterator it;

	if (canReadUntil(ends, count, maxSize, & it, & end_index)) {
		size_t lineLength = m_buffer.length(m_buffer.cbegin() + m_cursor, it);
		r = m_buffer.left(lineLength);
		m_buffer.remove(0, lineLength + ends[end_index].length());
		ok = true;
	}

	if (r.isEmpty()) {
		r = m_buffer;
		m_buffer.clear();

		if (atEnd())
			ok = true;
	}

	if (pok)
		*pok = ok;

	return r;
}

} // namespace io

CWT_NS_END

#endif /* __CWT_IO_READER_HPP__ */
