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

	static void istring_remove(istring_type & s, size_t pos, size_t n)
		{ s.remove(pos, n); }

	static bool convert(Decoder & decoder, ostring_type & output, const istring_type & input, size_t & remain)
		{ return decoder.convert(output, input, remain); }

	static void istring_clear(istring_type & s)
		{ s.clear(); }

	static ostring_type ostring_left(const ostring_type & s, size_t n)
		{ return s.left(n); }

	static size_t istring_length(const istring_type & s) { return s.length(); }
	static size_t ostring_length(const ostring_type & s) { return s.length(); }
	static void   ostring_prepend(ostring_type & s, char_type c) { s.prepend(c); }

	static bool canReadLine (ostring_type & s, const ostring_type ends[], size_t count, size_t maxSize);
	static ostring_type readLine (ostring_type & s, const ostring_type ends[], size_t count, size_t maxSize);
};


template <typename P, typename Decoder>
bool ReaderTraits<P, Decoder>::canReadLine (
	  ostring_type & s
	, const ostring_type ends[]
	, size_t count
	, size_t maxSize)
{
	CWT_ASSERT(count > 0);
	CWT_ASSERT(maxSize > 1);

	typename ostring_type::const_iterator itBegin = s.cbegin();
	typename ostring_type::const_iterator it = itBegin;

	size_t i = 0;
	for (; i < count && it != s.cend(); ++i) {
		it = search(itBegin, s.cend(), ends[i].cbegin(), ends[i].cend());
	}

	return it == s.cend() ? false : true;
}

template <typename P, typename Decoder>
typename ReaderTraits<P, Decoder>::ostring_type ReaderTraits<P, Decoder>::readLine (
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
	static const size_t MaxChunkSize = 512;

	typedef ReaderTraits<P, Decoder>              reader_traits;
	typedef typename reader_traits::istring_type  istring_type;
	typedef typename reader_traits::ostring_type  ostring_type;
	typedef typename reader_traits::char_type     char_type;

public:
    class iterator;
    friend class iterator;

	class iterator {
		friend class Reader;
		Reader *  m_readerPtr;
		char_type m_value;

	public:
		iterator() : m_readerPtr(nullptr) {}
		iterator(Reader & reader);
		iterator(const iterator & x) : m_readerPtr(x.m_readerPtr), m_value(x.m_value) {}
		~iterator() {}
		char_type value() const               { return   m_value; }
		const char_type & operator * () const { return   m_value; }
		const char_type * operator-> () const { return & m_value; }

        bool operator       == (const iterator & o) const { return m_readerPtr == o.m_readerPtr; }
        bool operator       != (const iterator & o) const { return !(m_readerPtr == o.m_readerPtr); }
		iterator & operator ++ ();
		iterator operator   ++ (int);
		iterator & operator += (size_t index);
		iterator at (size_t index);
		bool canRead(size_t n);
	};

public:
	Reader (P & producerRef, Decoder & decoderRef)
		: m_producerRef    (producerRef)
		, m_decoderRef     (decoderRef)
		, m_inputBuffer ()
		, m_outputBuffer()
		, m_cursor      (0)
		, m_status      (1)
	{}

	ostring_type read  (size_t maxSize);
	bool        atEnd () const   { return reader_traits::ostring_length(m_outputBuffer) == 0 && m_status == 0; }
	bool        isError() const  { return m_status < 0; }
	P &         producer() const { return m_producerRef; }

	bool canReadLine(const ostring_type & end, size_t maxSize) {
		const ostring_type ends[1] = { end };
		return canReadLine(ends, 1, maxSize);
	}
	bool canReadLine(const ostring_type ends[], size_t count, size_t maxSize);

	ostring_type readLine(const ostring_type & end, size_t maxSize) {
		const ostring_type ends[1] = { end };
		return readLine(ends, 1, maxSize);
	}
	ostring_type readLine(const ostring_type ends[], size_t count, size_t maxSize);

protected:
	void    resetCursor    ();
	bool    readAndConvert (size_t maxSize);
	bool    canRead        (size_t maxSize);

protected:
	P &                  m_producerRef;
	Decoder &            m_decoderRef;
	istring_type         m_inputBuffer;
	ostring_type         m_outputBuffer;
	size_t               m_cursor;    // cursor for outputBuffer, used in iterator
	int                  m_status;    // -1 - error, 0 - at end, +1 - normal
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
	CWT_ASSERT(maxSize > 0);
	m_status = 1;

	ssize_t nread = reader_traits::read(m_producerRef, m_inputBuffer, maxSize);

	if (nread > 0) {
		size_t remain = 0;
		if (reader_traits::convert(m_decoderRef, m_outputBuffer, m_inputBuffer, remain)) {
			if (remain > 0) {
				reader_traits::istring_remove(m_inputBuffer, reader_traits::istring_length(m_inputBuffer) - remain, remain);
			} else {
				reader_traits::istring_clear(m_inputBuffer);
			}
		}
	} else if (nread < 0) {   // producer error
		m_status = -1;
	} else {                  // nread == 0, producer at end
		//if (reader_traits::ostring_length(m_outputBuffer) == 0) {
			if (reader_traits::istring_length(m_inputBuffer) == 0) { // reader at end
				m_status = 0;
			} else {              // not all characters converted
				m_status = -2;
			}
		//}
	}

	return m_status >= 0 ? true : false;
}


template <typename P, typename Decoder>
bool Reader<P, Decoder>::canRead(size_t maxSize)
{
	if (m_status < 0)
		return false;

	if (m_cursor != 0)
		resetCursor();

	if (maxSize > reader_traits::ostring_length(m_outputBuffer)) {
		if (!readAndConvert(maxSize < MaxChunkSize ? MaxChunkSize : maxSize)) {
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

	ostring_type r = reader_traits::ostring_left(m_outputBuffer, maxSize);
	reader_traits::ostring_remove(m_outputBuffer, 0, maxSize);
	return r;
}

template <typename P, typename Decoder>
bool Reader<P, Decoder>::canReadLine (
	  const Reader<P, Decoder>::ostring_type ends[]
	, size_t count
	, size_t maxSize)
{
	CWT_ASSERT(count > 0);
	CWT_ASSERT(maxSize > 1);

	resetCursor();

	if (!readAndConvert(maxSize)) {
		return false;
	}

	return reader_traits::canReadLine(m_outputBuffer, ends, count, maxSize);
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

	return reader_traits::readLine(m_outputBuffer, ends, count, maxSize);
}


template <typename P, typename Decoder>
inline Reader<P, Decoder>::iterator::iterator(Reader<P, Decoder> & reader)
	: m_readerPtr(& reader)
{
	if (!(m_readerPtr && canRead(1))) {
		m_readerPtr->read(1); // set status to end-of-file or error
		m_readerPtr = nullptr;
	} else {
		m_value = m_readerPtr->m_outputBuffer[m_readerPtr->m_cursor];
	}
}


template <typename P, typename Decoder>
typename Reader<P, Decoder>::iterator & Reader<P, Decoder>::iterator::operator ++ ()
{
    if (m_readerPtr) {
    	++m_readerPtr->m_cursor;
    	if (canRead(1)) {
    		m_value = m_readerPtr->m_outputBuffer[m_readerPtr->m_cursor];
    	} else {
    		m_readerPtr->read(1); // set status to end-of-file or error
    		m_readerPtr = nullptr;
    	}
    }
    return *this;
}

template <typename P, typename Decoder>
inline typename Reader<P, Decoder>::iterator Reader<P, Decoder>::iterator::operator ++ (int)
{
    iterator r = *this;
    ++*this;
    return r;
}

template <typename P, typename Decoder>
typename Reader<P, Decoder>::iterator & Reader<P, Decoder>::iterator::operator += (size_t count)
{
	if (m_readerPtr) {
		if (count > 0) {
			if (canRead(count + 1)) {
				m_readerPtr->m_cursor += count;
				m_value = m_readerPtr->m_outputBuffer[m_readerPtr->m_cursor];
			} else {
				m_readerPtr->read(count + 1); // set status to end-of-file or error
				m_readerPtr = nullptr;
			}
		}
	}
    return *this;
}

template <typename P, typename Decoder>
inline typename Reader<P, Decoder>::iterator Reader<P, Decoder>::iterator::at (size_t index)
{
	iterator r = *this;
	if (m_readerPtr) {
		if (index > 0) {
			if (canRead(index + 1)) {
				r.m_value = m_readerPtr->m_outputBuffer[m_readerPtr->m_cursor + index];
			} else {
				m_readerPtr->read(index + 1); // set status to end-of-file or error
				r.m_readerPtr = nullptr;
			}
		}
	}
    return r;
}

template <typename P, typename Decoder>
bool Reader<P, Decoder>::iterator::canRead(size_t n)
{
	if (m_readerPtr) {
		if (m_readerPtr->m_cursor + n <= reader_traits::ostring_length(m_readerPtr->m_outputBuffer))
			return true;
		return m_readerPtr->canRead(n);
	}
	return false;
}

} // namespace io

CWT_NS_END

#endif /* __CWT_IO_READER_HPP__ */
