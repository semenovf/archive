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
#include <cwt/outputstream.hpp>
#include <cwt/reader.hpp>
#include <cwt/writer.hpp>

CWT_NS_BEGIN

class StreamDecoder : public Reader {
protected:
	StreamDecoder() : m_is(NULL) {}
	StreamDecoder(InputStream &is, Char replacement = Char::ReplacementChar)
		: m_is(&is)
		, m_replacement(replacement) {}
public:
	virtual ~StreamDecoder() { close(); }
	virtual bool    isNull() const { return m_is ? true : false; }
	virtual void 	close() { if(m_is) m_is->close(); m_is = NULL; }
	virtual ssize_t read(Char chars[], size_t len)  = 0;

	static StreamDecoder* forName(const char* charset, InputStream &is, Char replacement = Char::ReplacementChar);
	static StreamDecoder* forName(const ByteArray &charset, InputStream &is, Char replacement = Char::ReplacementChar);

protected:
	InputStream *m_is;
	Char         m_replacement;

protected:
	static CWT_DEFAULT_MT_POLICY g_mutex;
};


class StreamEncoder : public Writer {
protected:
	StreamEncoder() : m_os(NULL), m_replacement('?') {}
	StreamEncoder(OutputStream &os, bool ignoreHeader = true, char replacement = '?')
		: m_os(&os)
		, m_replacement(replacement) { CWT_UNUSED(ignoreHeader); }
public:
	virtual ~StreamEncoder() { close(); }
	virtual bool    isNull() const { return m_os ? true : false; }
	virtual void 	close() { if(m_os) m_os->close(); m_os = NULL; }
	virtual void    flush() { m_os->flush(); }
	virtual ssize_t write(const Char chars[], size_t sz) = 0;

	static StreamEncoder* forName(const char* charset, OutputStream &os, bool ignoreHeader = true, char replacement = '?');
	static StreamEncoder* forName(const ByteArray &charset, OutputStream &os, bool ignoreHeader = true, char replacement = '?');

protected:
	OutputStream *m_os;
	char          m_replacement;

protected:
	static CWT_DEFAULT_MT_POLICY g_mutex;
};

inline StreamDecoder* StreamDecoder::forName(const ByteArray &charset
		, InputStream &is
		, Char replacement)
{
	return forName(charset.data(), is, replacement);
}

inline StreamEncoder* StreamEncoder::forName(const ByteArray &charset
		, OutputStream &os
		, bool ignoreHeader
		, char replacement)
{
	return forName(charset.data(), os, ignoreHeader, replacement);
}

CWT_NS_END

#endif /* __CWT_TEXTCODEC_HPP__ */
