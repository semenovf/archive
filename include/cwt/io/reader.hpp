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

template <typename Producer, typename Decoder>
class ReaderTraits
{
public:
	typedef typename Decoder::istring_type    istring_type;
	typedef typename Decoder::ostring_type    ostring_type;
	typedef typename ostring_type::char_type  char_type;

	static ssize_t read(Producer & producer, istring_type & in, size_t maxSize)
		{ return producer.read(in, maxSize); }

	static void ostring_remove(ostring_type & s, size_t pos, size_t n)
		{ s.remove(pos, n); }

	static bool convert(Decoder & decoder, ostring_type & output, const istring_type & input, size_t & remain)
		{ return decoder.convert(output, input, remain); }

	static void istring_clear(istring_type & s)
		{ return s.clear(); }

	static istring_type istring_right(const istring_type & s, size_t n)
		{ return s.right(n); }

	static ostring_type ostring_left(const ostring_type & s, size_t n)
		{ return s.left(n); }

	static size_t ostring_length(const ostring_type & s) { return s.length(); }
	static void   ostring_prepend(ostring_type & s, char_type c) { s.prepend(c); }

	static ostring_type readUntil (ostring_type & s, const ostring_type ends[], size_t count, size_t maxSize);
};


template <typename P, typename Decoder>
typename ReaderTraits<P, Decoder>::ostring_type ReaderTraits<P, Decoder>::readUntil (
	  ostring_type & s
	, const ostring_type ends[]
	, size_t count
	, size_t maxSize)
{
	ostring_type r;
	CWT_ASSERT(count > 0);
	CWT_ASSERT(maxSize > 1);

	typename ostring_type::const_iterator itBegin = s.cbegin();
	typename ostring_type::const_iterator it = itBegin;

	size_t i = 0;
	for (; i < count && it != s.cend(); ++i) {
		it = search(itBegin, s.cend(), ends[i].cbegin(), ends[i].cend());
	}

	if (it == s.cend()) { // End-of-line not found
		r = s.left(maxSize);
		s.remove(0, maxSize);
	} else { // End-of-line found
		size_t len = s.length(s.cbegin(), it);
		r = s.left(len);
		r.append(ostring_type::EndOfLine);
		s.remove(0, len + ends[i-1].length());
	}

	return r;
}


template <typename P, typename Decoder>
class Reader
{
public:
	typedef ReaderTraits<P, Decoder>              reader_traits;
	typedef typename reader_traits::istring_type  istring_type;
	typedef typename reader_traits::ostring_type  ostring_type;
	typedef typename reader_traits::char_type     char_type;

public:
    class iterator;
    friend class iterator;

	class iterator {
		friend class Reader;
		Reader * m_reader;
		char_type m_value;

	public:
		iterator() : m_reader(nullptr) {}
		iterator(Reader & reader) : m_reader(& reader) { ++*this; }
		iterator(const iterator & x) : m_reader(x.m_reader), m_value(x.m_value) {}
		~iterator() {}

		const char_type & operator * () const { return   m_value; }
		const char_type * operator-> () const { return & m_value; }

        bool operator  == (const iterator & o) const { return m_reader == o.m_reader; }
        bool operator  != (const iterator & o) const { return !(m_reader == o.m_reader); }

		iterator & operator ++ () {
		    if (m_reader && m_reader->canRead(1)) {
		    	m_value = m_reader->m_outputBuffer[m_reader->m_cursor++];
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
	Reader (shared_ptr<P> producer)
	    : m_producer    (producer)
		, m_decoder     (new Decoder)
		, m_inputBuffer ()
		, m_outputBuffer()
		, m_cursor      (0)
	{}

	Reader (shared_ptr<P> producer, shared_ptr<Decoder> decoder)
		: m_producer    (producer)
		, m_decoder     (decoder)
		, m_inputBuffer ()
		, m_outputBuffer()
		, m_cursor      (0)
	{}

	ostring_type read  (size_t maxSize);
	bool        atEnd () const { return m_producer->atEnd(); }
	P *         producer() const { return m_producer.get(); }

	ostring_type readLine(const ostring_type & end, size_t maxSize) {
		const ostring_type ends[1] = { end };
		return readLine(ends, 1, maxSize);
	}
	ostring_type readLine(const ostring_type ends[], size_t count, size_t maxSize);

protected:
	void resetCursor    ();
	bool readAndConvert (size_t maxSize);
	bool canRead        (size_t maxSize);

protected:
	shared_ptr<P>        m_producer;
	shared_ptr<Decoder>  m_decoder;
	istring_type         m_inputBuffer;
	ostring_type         m_outputBuffer;
	size_t               m_cursor;    // cursor for outputBuffer
};


template <typename P, typename Decoder>
inline void Reader<P, Decoder>::resetCursor ()
{
	if (m_cursor != 0) {
		reader_traits::ostring_remove(m_outputBuffer, 0, m_cursor);
		m_cursor = 0;
	}
}

template <typename P, typename Decoder>
bool Reader<P, Decoder>::readAndConvert (size_t maxSize)
{
	ssize_t nread = reader_traits::read(*m_producer, m_inputBuffer, maxSize);

	if (nread > 0) {
		size_t remain = 0;
		if (reader_traits::convert(*m_decoder, m_outputBuffer, m_inputBuffer, remain)) {
			if (remain > 0) {
				m_inputBuffer = reader_traits::istring_right(m_inputBuffer, remain);
			} else {
				reader_traits::istring_clear(m_inputBuffer);
			}
		}
	}

	return nread >= 0 ? true : false;
}


template <typename P, typename Decoder>
bool Reader<P, Decoder>::canRead(size_t maxSize)
{
	resetCursor();
	if (maxSize > reader_traits::ostring_length(m_outputBuffer)) {
		if (!readAndConvert(maxSize)) {
			return false;
		}
	}

	return reader_traits::ostring_length(m_outputBuffer) >= maxSize ? true : false;
}


template <typename P, typename Decoder>
typename Reader<P, Decoder>::ostring_type Reader<P, Decoder>::read (size_t maxSize)
{
	resetCursor();

	if (maxSize > reader_traits::ostring_length(m_outputBuffer)) {
		if (!readAndConvert(maxSize)) {
			return typename reader_traits::ostring_type();
		}
	}

	return reader_traits::ostring_left(m_outputBuffer, maxSize);
}

template <typename P, typename Decoder>
typename Reader<P, Decoder>::ostring_type Reader<P, Decoder>::readLine (
	  const Reader<P, Decoder>::ostring_type ends[]
	, size_t count
	, size_t maxSize)
{
	CWT_ASSERT(count > 0);
	CWT_ASSERT(maxSize > 1);

	resetCursor();

	if (!readAndConvert(maxSize)) {
		return ostring_type();
	}

	return reader_traits::readUntil(m_outputBuffer, ends, count, maxSize);
}


template <typename P, typename Decoder>
bool Reader<P, Decoder>::iterator::unget(char_type ch)
{
	CWT_ASSERT(m_reader);

	if (m_reader->m_cursor > 0 && m_reader->m_cursor >= reader_traits::ostring_length(m_reader->m_outputBuffer)) {
		m_reader->m_outputBuffer[--m_outputBuffer->m_cursor] = ch;
		return true;
	}

	if (!m_reader->m_cursor) {
		reader_traits::ostring_prepend(m_reader->m_outputBuffer, ch);
		return true;
	}

	return false;
}

} // namespace io

CWT_NS_END

#endif /* __CWT_IO_READER_HPP__ */
