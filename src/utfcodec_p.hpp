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

class Utf8Decoder : public Decoder {
	static const size_t BufSize = 512;
protected:
	virtual String convertToUnicode(const char *bytes, size_t len) = 0;
public:
	Utf8Decoder(Char replacement = Char::ReplacementChar)
		: Decoder(replacement)
		, m_buffer()
		, m_headerDone(false)
		, m_remainChar(Char::MaxCodePoint) { m_buffer.reserve(BufSize); }

private:
	ByteArray m_buffer;
	bool      m_headerDone;
	Char      m_remainChar;
};

class Utf8Encoder : public Encoder {
protected:
	virtual ByteArray convertFromUnicode(const Char *chars, size_t len) = 0;
public:
	Utf8Encoder(bool ignoreHeader = true, char replacement = '?')
		: Encoder(ignoreHeader, replacement)
		, m_headerDone(ignoreHeader ? true : false)
		, m_hiSurrogate(-1) {}
private:
	bool m_headerDone;
	int  m_hiSurrogate;
};

CWT_NS_END

#endif /* __CWT_UTF8CODEC_P_HPP__ */
