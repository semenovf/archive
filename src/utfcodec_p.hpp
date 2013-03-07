/**
 * @file   utf8codec_p.hpp
 * @author wladt
 * @date   Mar 1, 2013 11:14:20 AM
 *
 * @brief
 */

#ifndef __CWT_UTF8CODEC_P_HPP__
#define __CWT_UTF8CODEC_P_HPP__

#include "../include/cwt/textcodec.hpp"

CWT_NS_BEGIN

class Utf8Decoder : public StreamDecoder {
	static const size_t BufSize = 512;
protected:
	Utf8Decoder()
		: StreamDecoder()
		, m_buffer()
		, m_headerDone(false)
		, m_remainChar(Char::MaxCodePoint) { m_buffer.reserve(BufSize); }
public:
	Utf8Decoder(InputStream &is, Char replacement = Char::ReplacementChar)
		: StreamDecoder(is, replacement)
		, m_buffer()
		, m_headerDone(false)
		, m_remainChar(Char::MaxCodePoint) { m_buffer.reserve(BufSize); }
	virtual ~Utf8Decoder() {}
	virtual ssize_t read(Char chars[], size_t len);
private:
	ByteArray m_buffer;
	bool      m_headerDone;
	Char      m_remainChar;
};

class Utf8Encoder : public StreamEncoder {
protected:
	Utf8Encoder()
		: StreamEncoder()
		, m_headerDone(false)
		, m_hiSurrogate(-1) {}
public:
	Utf8Encoder(OutputStream &os, bool ignoreHeader = true, char replacement = '?')
		: StreamEncoder(os, ignoreHeader, replacement)
		, m_headerDone(ignoreHeader ? true : false)
		, m_hiSurrogate(-1) {}
	virtual ~Utf8Encoder() {}
	virtual ssize_t write(const Char chars[], size_t sz);

private:
	bool m_headerDone;
	int  m_hiSurrogate;
};


CWT_NS_END

#endif /* __CWT_UTF8CODEC_P_HPP__ */
