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
#include <cwt/datastream.hpp>
#include <cwt/textcodec.hpp>

CWT_NS_BEGIN

class TextStream {
private:
	static const size_t ChunkSize = 512;
public:
	TextStream() : m_ds(NULL), m_decoder(NULL), m_encoder(NULL)  {}
	TextStream(DataStream &ds) : m_ds(&ds), m_decoder(NULL), m_encoder(NULL)  {}
	~TextStream() {}
	bool    isNull() const { return m_ds && !m_ds->isNull() ? false : true; }
	String read(size_t len);
	ssize_t write(const String &s);
	String  readAll();

	void setDecoder(Decoder *decoder) { m_decoder = decoder; }
	void setEncoder(Encoder *encoder) { m_encoder = encoder; }

protected:
	DataStream *m_ds;
	Decoder *m_decoder;
	Encoder *m_encoder;
};

CWT_NS_END

#endif /* __CWT_TEXTSTREAM_HPP__ */
