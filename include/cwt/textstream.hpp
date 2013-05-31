/**
 * @file   textsream.hpp
 * @author wladt
 * @date   Mar 1, 2013 10:59:37 AM
 *
 * @brief
 */

#ifndef __CWT_TEXTSTREAM_HPP__
#define __CWT_TEXTSTREAM_HPP__

#include <cwt/cwt.h>
#include <cwt/bytearray.hpp>
#include <cwt/string.hpp>
#include <cwt/mt.hpp>
#include <cwt/iodevice.hpp>
#include <cwt/textcodec.hpp>

CWT_NS_BEGIN

class DLL_API TextInputStream {
private:
	static const size_t ChunkSize = 512;
public:
	TextInputStream() : m_isString(false), m_dev(NULL), m_decoder(NULL) {}
	TextInputStream(IODevice *dev) : m_isString(false), m_dev(dev), m_decoder(NULL) {}
	TextInputStream(String *s) : m_isString(true), m_pos(0), m_string(s) {}
	~TextInputStream() {}

	bool isNull() const {
		return m_isString
			? m_string != NULL ? false : true
			: m_dev != NULL ? false : true;
	}
	String read(size_t len);
	String readAll();
	String readLine(bool with_nl = false);
	String readLine(const String &endLine, bool with_nl = false);
	String readLine(const String endLines[], int count, bool with_nl = false);
	Char   getChar();
	void   ungetChar(Char ch);

	void setDecoder(Decoder *decoder) { m_decoder = decoder; }

protected:
	bool m_isString;
	union {
		struct {
			IODevice *m_dev;
			Decoder  *m_decoder;
		};
		struct {
			size_t  m_pos;
			String *m_string;
		};
	};
	String m_remainChars;
};

class DLL_API TextOutputStream {
public:
	TextOutputStream() : m_isString(false), m_dev(NULL), m_encoder(NULL)  {}
	TextOutputStream(IODevice *dev) : m_isString(false), m_dev(dev), m_encoder(NULL)  {}
	TextOutputStream(String *s) : m_isString(true), m_string(s) {}
	~TextOutputStream() {}
	bool    isNull() const { return m_dev != NULL ? false : true; }
	ssize_t write(const String &s);

	void setEncoder(Encoder *encoder) { m_encoder = encoder; }

protected:
	bool m_isString;
	union {
		struct {
			IODevice *m_dev;
			Encoder  *m_encoder;
		};
		String *m_string;
	};

};

inline Char TextInputStream::getChar()
{
	String ch = read(1);
	return ch.length() > 0 ? ch[0] : Char();
}

// TODO need more effective algorithm for returning a char into the stream.
inline void TextInputStream::ungetChar(Char ch)
{
	m_remainChars.prepend(ch);
}

CWT_NS_END

#endif /* __CWT_TEXTSTREAM_HPP__ */
