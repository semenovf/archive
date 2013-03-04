/**
 * @file   inputstreamreader.hpp
 * @author wladt
 * @date   Mar 4, 2013 12:16:56 PM
 *
 * @brief
 */

#ifndef __CWT_INPUTSTREAMREADER_HPP__
#define __CWT_INPUTSTREAMREADER_HPP__

#include <cwt/cwt.h>
#include <cwt/inputstream.hpp>
#include <cwt/textcodec.hpp>
#include <cwt/bytearray.hpp>

CWT_NS_BEGIN

class InputStreamReader {
	static const size_t BufSize = 256;
protected:
	InputStreamReader() : m_is(NULL), m_codec(NULL) {}
public:
	InputStreamReader(InputStream &in) : m_is(&in), m_codec(TextCodec::forLocale()), m_bytes(BufSize, 0), m_chars(BufSize, 0) {}
	InputStreamReader(InputStream &in, TextCodec &codec) : m_is(&in), m_codec(&codec), m_bytes(BufSize, 0), m_chars(BufSize, 0) {}
	virtual ~InputStreamReader() { close(); }
	virtual bool    isNull() const { return !m_is ? true : m_is->isNull() ? true : false; }
	virtual void 	close() { if(m_is) m_is->close(); m_is = NULL; }
	virtual ssize_t read(Char chars[], size_t sz);
private:
	InputStream *m_is;
	TextCodec   *m_codec;
	ByteArray    m_bytes;
	String       m_chars;
	TextCodec::CodecState m_state;
};

CWT_NS_END

#endif /* __CWT_INPUTSTREAMREADER_HPP__ */
