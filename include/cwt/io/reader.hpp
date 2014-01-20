/*
 * reader.hpp
 *
 *  Created on: Jul 11, 2013
 *      Author: wladt
 */

#ifndef __CWT_IO_READER_HPP__
#define __CWT_IO_READER_HPP__

#include <cwt/io/io.hpp>
#include <pfs/shared_ptr.hpp>
#include <pfs/vector.hpp>

namespace cwt { namespace io {

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

template <typename _Producer, typename _Decoder>
class reader_traits
{
public:
	typedef typename _Decoder::istring_type    istring_type;
	typedef typename _Decoder::ostring_type    ostring_type;
	typedef typename ostring_type::char_type  char_type;

	static ssize_t read(_Producer & producer, istring_type & in, size_t maxSize)
		{ return producer.read(in, maxSize); }

	static void ostring_remove(ostring_type & s, size_t pos, size_t n)
		{ s.remove(pos, n); }

	static void istring_remove(istring_type & s, size_t pos, size_t n)
		{ s.remove(pos, n); }

	static bool convert(_Decoder & decoder, ostring_type & output, const istring_type & input, size_t & remain)
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


template <typename _P, typename _Decoder>
bool reader_traits<_P, _Decoder>::canReadLine (
	  ostring_type & s
	, const ostring_type ends[]
	, size_t count
	, size_t maxSize)
{
	PFS_ASSERT(count > 0);
	PFS_ASSERT(maxSize > 1);

	typename ostring_type::const_iterator itBegin = s.cbegin();
	typename ostring_type::const_iterator it = itBegin;

	size_t i = 0;
	for (; i < count && it != s.cend(); ++i) {
		it = search(itBegin, s.cend(), ends[i].cbegin(), ends[i].cend());
	}

	return it == s.cend() ? false : true;
}

template <typename _P, typename _Decoder>
typename reader_traits<_P, _Decoder>::ostring_type reader_traits<_P, _Decoder>::readLine (
	  ostring_type & s
	, const ostring_type ends[]
	, size_t count
	, size_t maxSize)
{
	ostring_type r;
	PFS_ASSERT(count > 0);
	PFS_ASSERT(maxSize > 1);

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
		r.append(1, ostring_type::EndOfLineChar);
		s.remove(0, len + ends[i-1].length());
	}

	return r;
}


template <typename _P, typename _Decoder>
class reader
{
public:
	static const size_t MaxChunkSize = 512;

	typedef reader_traits<_P, _Decoder> reader_traits_type;
	typedef typename reader_traits_type::istring_type  istring_type;
	typedef typename reader_traits_type::ostring_type  ostring_type;
	typedef typename reader_traits_type::char_type     char_type;

protected:
	_P &         m_producerRef;
	_Decoder &   m_decoderRef;
	istring_type m_inputBuffer;
	ostring_type m_outputBuffer;
	size_t       m_cursor;    // cursor for outputBuffer, used in iterator
	int          m_status;    // -1 - error, 0 - at end, +1 - normal

public:
    class iterator;
    friend class iterator;

	class iterator {
		friend class reader;
		reader *  m_readerPtr;
		char_type m_value;

	public:
		iterator () : m_readerPtr(nullptr) {}
		iterator (reader & reader);
		iterator (const iterator & x) : m_readerPtr(x.m_readerPtr), m_value(x.m_value) {}
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
		bool canRead (size_t n);
	};

public:
	reader (_P & producerRef, _Decoder & decoderRef)
		: m_producerRef    (producerRef)
		, m_decoderRef     (decoderRef)
		, m_inputBuffer ()
		, m_outputBuffer()
		, m_cursor      (0)
		, m_status      (1)
	{}

	ostring_type read  (size_t maxSize);
	bool         atEnd () const   { return reader_traits_type::ostring_length(m_outputBuffer) == 0 && m_status == 0; }
	bool         isError() const  { return m_status < 0; }
	_P &         producer() const { return m_producerRef; }

	bool canReadLine (const ostring_type & end, size_t maxSize) {
		const ostring_type ends[1] = { end };
		return canReadLine(ends, 1, maxSize);
	}
	bool canReadLine (const ostring_type ends[], size_t count, size_t maxSize);

	ostring_type readLine (const ostring_type & end, size_t maxSize) {
		const ostring_type ends[1] = { end };
		return readLine(ends, 1, maxSize);
	}
	ostring_type readLine (const ostring_type ends[], size_t count, size_t maxSize);

protected:
	void resetCursor ();
	bool readAndConvert (size_t maxSize);
	bool canRead (size_t maxSize);
};

template <typename _P, typename _Decoder>
inline void reader<_P, _Decoder>::resetCursor ()
{
	if (m_cursor != 0) {
		reader_traits_type::ostring_remove(m_outputBuffer, 0, m_cursor);
		m_cursor = 0;
	}
}

template <typename _P, typename _Decoder>
bool reader<_P, _Decoder>::readAndConvert (size_t maxSize)
{
	PFS_ASSERT(maxSize > 0);
	m_status = 1;

	ssize_t nread = reader_traits_type::read(m_producerRef, m_inputBuffer, maxSize);

	if (nread > 0) {
		size_t remain = 0;
		if (reader_traits_type::convert(m_decoderRef, m_outputBuffer, m_inputBuffer, remain)) {
			if (remain > 0) {
				reader_traits_type::istring_remove(m_inputBuffer, reader_traits_type::istring_length(m_inputBuffer) - remain, remain);
			} else {
				reader_traits_type::istring_clear(m_inputBuffer);
			}
		}
	} else if (nread < 0) {   // producer error
		m_status = -1;
	} else {                  // nread == 0, producer at end
		//if (reader_traits::ostring_length(m_outputBuffer) == 0) {
			if (reader_traits_type::istring_length(m_inputBuffer) == 0) { // reader at end
				m_status = 0;
			} else {              // not all characters converted
				m_status = -2;
			}
		//}
	}

	return m_status >= 0 ? true : false;
}


template <typename _P, typename _Decoder>
bool reader<_P, _Decoder>::canRead(size_t maxSize)
{
	if (m_status < 0)
		return false;

	if (m_cursor != 0)
		resetCursor();

	if (maxSize > reader_traits_type::ostring_length(m_outputBuffer)) {
		if (!readAndConvert(maxSize < MaxChunkSize ? MaxChunkSize : maxSize)) {
			return false;
		}
	}

	return reader_traits_type::ostring_length(m_outputBuffer) >= maxSize ? true : false;
}


template <typename _P, typename _Decoder>
typename reader<_P, _Decoder>::ostring_type reader<_P, _Decoder>::read (size_t maxSize)
{
	resetCursor();

	if (maxSize > reader_traits_type::ostring_length(m_outputBuffer)) {
		if (!readAndConvert(maxSize)) {
			return typename reader_traits_type::ostring_type();
		}
	}

	ostring_type r = reader_traits_type::ostring_left(m_outputBuffer, maxSize);
	reader_traits_type::ostring_remove(m_outputBuffer, 0, maxSize);
	return r;
}

template <typename _P, typename _Decoder>
bool reader<_P, _Decoder>::canReadLine (
	  const reader<_P, _Decoder>::ostring_type ends[]
	, size_t count
	, size_t maxSize)
{
	PFS_ASSERT(count > 0);
	PFS_ASSERT(maxSize > 1);

	resetCursor();

	if (!readAndConvert(maxSize)) {
		return false;
	}

	return reader_traits_type::canReadLine(m_outputBuffer, ends, count, maxSize);
}


template <typename _P, typename _Decoder>
typename reader<_P, _Decoder>::ostring_type reader<_P, _Decoder>::readLine (
	  const reader<_P, _Decoder>::ostring_type ends[]
	, size_t count
	, size_t maxSize)
{
	PFS_ASSERT(count > 0);
	PFS_ASSERT(maxSize > 1);

	resetCursor();

	if (!readAndConvert(maxSize)) {
		return ostring_type();
	}

	return reader_traits_type::readLine(m_outputBuffer, ends, count, maxSize);
}


template <typename _P, typename _Decoder>
inline reader<_P, _Decoder>::iterator::iterator (reader<_P, _Decoder> & reader)
	: m_readerPtr(& reader)
{
	if (!(m_readerPtr && canRead(1))) {
		m_readerPtr->read(1); // set status to end-of-file or error
		m_readerPtr = nullptr;
	} else {
		m_value = m_readerPtr->m_outputBuffer[m_readerPtr->m_cursor];
	}
}


template <typename _P, typename _Decoder>
typename reader<_P, _Decoder>::iterator & reader<_P, _Decoder>::iterator::operator ++ ()
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

template <typename _P, typename _Decoder>
inline typename reader<_P, _Decoder>::iterator reader<_P, _Decoder>::iterator::operator ++ (int)
{
    iterator r = *this;
    ++*this;
    return r;
}

template <typename _P, typename _Decoder>
typename reader<_P, _Decoder>::iterator & reader<_P, _Decoder>::iterator::operator += (size_t count)
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

template <typename _P, typename _Decoder>
inline typename reader<_P, _Decoder>::iterator reader<_P, _Decoder>::iterator::at (size_t index)
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

template <typename _P, typename _Decoder>
bool reader<_P, _Decoder>::iterator::canRead (size_t n)
{
	if (m_readerPtr) {
		if (m_readerPtr->m_cursor + n <= reader_traits_type::ostring_length(m_readerPtr->m_outputBuffer))
			return true;
		return m_readerPtr->canRead(n);
	}
	return false;
}

}} // cwt::io

#endif /* __CWT_IO_READER_HPP__ */
