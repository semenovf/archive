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

class TextStream {
private:
	static const size_t ChunkSize = 512;
public:
	TextStream() : m_dev(NULL), m_decoder(NULL), m_encoder(NULL)  {}
	TextStream(IODevice &dev) : m_dev(&dev), m_decoder(NULL), m_encoder(NULL)  {}
	~TextStream() {}
	bool    isNull() const { return m_dev != NULL ? false : true; }
	String  read(size_t len);
	ssize_t write(const String &s);
	String  readAll();

	void setDecoder(Decoder *decoder) { m_decoder = decoder; }
	void setEncoder(Encoder *encoder) { m_encoder = encoder; }

protected:
	IODevice *m_dev;
	Decoder  *m_decoder;
	Encoder  *m_encoder;
	String    m_remainChars;
};

CWT_NS_END

#endif /* __CWT_TEXTSTREAM_HPP__ */
