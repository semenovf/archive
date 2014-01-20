/*
 * writer.hpp
 *
 *  Created on: Jul 11, 2013
 *      Author: wladt
 */

#ifndef __CWT_IO_WRITER_HPP__
#define __CWT_IO_WRITER_HPP__

#include <cwt/io/io.hpp>
#include <pfs/shared_ptr.hpp>

namespace cwt { namespace io {

template <typename _Consumer, typename _Encoder>
class writer_traits
{
public:
	typedef typename _Encoder::istring_type    istring_type;
	typedef typename _Encoder::ostring_type    ostring_type;

	static bool convert (_Encoder & encoder, ostring_type & output, const istring_type & input, size_t & remain)
		{ return encoder.convert(output, input, remain); }
	static ssize_t write (_Consumer & consumer, const ostring_type & o)
		{ return consumer.write(o); }
	static istring_type istring_right (const istring_type & s, size_t n)
		{ return s.right(n); }

	static void istring_clear (istring_type & s) { s.clear(); }
	static void ostring_clear (ostring_type & s) { s.clear(); }

	static bool istring_is_empty (const istring_type & s) { return s.isEmpty(); }
	static void istring_append (istring_type & s, const istring_type & other) { s.append(other); }
};


template <typename _Consumer, typename _Encoder>
class writer
{
public:
	typedef writer_traits<_Consumer, _Encoder> writer_traits_type;
	typedef typename writer_traits_type::istring_type  istring_type;
	typedef typename writer_traits_type::ostring_type  ostring_type;

private:
	_Consumer &   m_consumerRef;
	_Encoder &    m_encoderRef;
	istring_type  m_inputBuffer;
	ostring_type  m_outputBuffer;

public:
	writer (_Consumer & consumerRef, _Encoder & encoderRef)
		: m_consumerRef(consumerRef)
		, m_encoderRef(encoderRef)
	{}

	bool isError() { return m_consumerRef->isError(); }
	ssize_t write (const istring_type & input);

	_Consumer * consumer() const { return & m_consumerRef; }
};


template <typename _Consumer, typename _Encoder>
inline ssize_t writer<_Consumer, _Encoder>::write(const istring_type & input)
{
	size_t remain = 0;

	writer_traits_type::ostring_clear(m_outputBuffer);

	if (!writer_traits_type::istring_is_empty(m_inputBuffer)) {
		writer_traits_type::istring_append(m_inputBuffer, input);

		if (!writer_traits_type::convert(m_encoderRef, m_outputBuffer, m_inputBuffer, remain))
			return ssize_t(-1);
	} else {
		if (!writer_traits_type::convert(m_encoderRef, m_outputBuffer, input, remain))
			return ssize_t(-1);
	}

	ssize_t written = writer_traits_type::write(m_consumerRef, m_outputBuffer);

	if (written > 0) {
		if (remain > 0) {
			if (!writer_traits_type::istring_is_empty(m_inputBuffer))
				m_inputBuffer = writer_traits_type::istring_right(m_inputBuffer, remain);
			else
				m_inputBuffer = writer_traits_type::istring_right(input, remain);
		} else {
			writer_traits_type::istring_clear(m_inputBuffer);
		}
	}

	return written;
}

}} // cwt::io

#endif /* __CWT_IO_WRITER_HPP__ */
