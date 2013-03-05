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
		, m_buffer(BufSize, 0)
		, m_headerDone(false)
		, m_remainChar(Char::MaxCodePoint) {}
public:
	Utf8Decoder(InputStream *is, Char replacement = Char::ReplacementChar)
		: StreamDecoder(is, replacement)
		, m_buffer(BufSize, 0)
		, m_headerDone(false)
		, m_remainChar(Char::MaxCodePoint) {}
	virtual ~Utf8Decoder() {}
	virtual ssize_t read(Char chars[], size_t len);
private:
	ByteArray m_buffer;
	bool      m_headerDone;
	Char      m_remainChar;
};


#ifdef __COMMENT__
class Utf8TextCodec : public TextCodec {
public:
	Utf8TextCodec() {}
	virtual ~Utf8TextCodec() {}

protected:
	virtual String decode(const char *bytes, size_t len, CodecState *state);
	virtual ByteArray encode(const Char *chars, size_t len, CodecState *state);
};
#endif

CWT_NS_END

#endif /* __CWT_UTF8CODEC_P_HPP__ */
