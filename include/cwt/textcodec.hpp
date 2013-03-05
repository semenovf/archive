/**
 * @file   textcodec.hpp
 * @author wladt
 * @date   Mar 1, 2013 10:59:37 AM
 *
 * @brief
 */

#ifndef __CWT_TEXTCODEC_HPP__
#define __CWT_TEXTCODEC_HPP__

#include <cwt/cwt.h>
#include <cwt/bytearray.hpp>
#include <cwt/string.hpp>
#include <cwt/mt.hpp>
#include <cwt/inputstream.hpp>
#include <cwt/reader.hpp>

CWT_NS_BEGIN

class StreamDecoder : public Reader {
protected:
	StreamDecoder() : m_is(NULL) {}
	StreamDecoder(InputStream *is, Char replacement = Char::ReplacementChar)
		: m_is(is)
		, m_replacement(replacement) {}
public:
	virtual ~StreamDecoder() { close(); }
	virtual bool    isNull() const { return m_is ? true : false; }
	virtual void 	close() { if(m_is) m_is->close(); m_is = NULL; }
	virtual ssize_t read(Char chars[], size_t len)  = 0;

	static StreamDecoder* forName(const char* charset, InputStream *is, Char replacement = Char::ReplacementChar);
	static StreamDecoder* forName(const ByteArray &charset, InputStream *is, Char replacement = Char::ReplacementChar);

protected:
	InputStream *m_is;
	Char         m_replacement;

protected:
	static CWT_DEFAULT_MT_POLICY g_mutex;
};

inline StreamDecoder* StreamDecoder::forName(const ByteArray &charset
		, InputStream *is
		, Char replacement)
{
	return forName(charset.data(), is, replacement);
}

/*
class StreamEncoder {

};
*/

#ifdef __COMMENT__

class TextCodec {
public:
	TextCodec() : m_is(NULL) {}
	TextCodec(InputStream *is) : m_is(is) {}
	virtual ~TextCodec() {}

protected:
	//virtual String    decode(const char *bytes, size_t len, CodecState *state) = 0;
	//virtual ByteArray encode(const Char *chars, size_t len, CodecState *state) = 0;
	virtual ssize_t   decode(const Char *chars, size_t offset, size_t len) = 0;
	virtual ssize_t   encode(const char *bytes, size_t offset, size_t len) = 0;

public:
	static TextCodec* forName(const char* name);
	static TextCodec* forName(const ByteArray &name) { return forName(name.data()); }
	static TextCodec* forLocale();

protected:
	static CWT_DEFAULT_MT_POLICY g_mutex;
};
#endif
CWT_NS_END

#endif /* __CWT_TEXTCODEC_HPP__ */
