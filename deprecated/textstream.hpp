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
#include <cwt/errorable.hpp>
#include <cwt/textcodec.hpp>

CWT_NS_BEGIN

class DLL_API TextInputStream  : public Errorable
{
private:
	static const size_t ChunkSize = 512;

protected:
	IODevice::ReadLineStatus readLineData(const String endLines[], int count, String & s, size_t maxSize = CWT_INT_MAX);

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

	IODevice::ReadLineStatus readLine(String & s, size_t maxSize = CWT_INT_MAX);
	IODevice::ReadLineStatus readLine(const String & endLine, String & s, size_t maxSize = CWT_INT_MAX);
	IODevice::ReadLineStatus readLine(const Char * endLine, String & s, size_t maxSize = CWT_INT_MAX) { return readLine(String(endLine), s, maxSize); }
	IODevice::ReadLineStatus readLine(const String endLines[], int count, String & s, size_t maxSize = CWT_INT_MAX);

	bool getChar(Char & ch);
	void ungetChar(Char ch);

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

inline bool TextInputStream::getChar(Char & ch)
{
	String s = read(1);

	if (s.length() > 0 ) {
		ch = s[0];
		return true;
	}

	ch = Char();
	return false;
}

// TODO need more effective algorithm for returning a char into the stream.
inline void TextInputStream::ungetChar(Char ch)
{
	m_remainChars.prepend(ch);
}

inline IODevice::ReadLineStatus TextInputStream::readLine(const String & endLine, String & s, size_t maxSize)
{
	const String endLines[] = { endLine };
	return readLineData(endLines, 1, s, maxSize);
}

inline IODevice::ReadLineStatus TextInputStream::readLine(const String endLines[], int count, String & s, size_t maxSize)
{
	return readLineData(endLines, count, s, maxSize);
}


CWT_NS_END

#endif /* __CWT_TEXTSTREAM_HPP__ */
