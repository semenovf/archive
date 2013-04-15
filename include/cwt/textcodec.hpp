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

CWT_NS_BEGIN

class Decoder {
protected:
	Decoder(Char replacement = Char::ReplacementChar)
		: m_replacement(replacement) {}

	virtual String convertToUnicode(const char *bytes, size_t len) = 0;

public:
	virtual ~Decoder() { }
	virtual String toUnicode(const char *bytes, size_t len) { return convertToUnicode(bytes, len); }

	static Decoder* forName(const char* charset, Char replacement = Char::ReplacementChar);

protected:
	Char m_replacement;

protected:
	static CWT_DEFAULT_MT_POLICY g_mutex;
};


class Encoder {
protected:
	Encoder(bool ignoreHeader = true, char replacement = '?')
		: m_replacement(replacement) { CWT_UNUSED(ignoreHeader); }

	virtual ByteArray convertFromUnicode(const Char *chars, size_t len) = 0;
public:
	virtual ~Encoder() { ; }
	ByteArray fromUnicode(const Char *chars, size_t len) { return convertFromUnicode(chars, len); }

	static Encoder* forName(const char* charset, bool ignoreHeader = true, char replacement = '?');

protected:
	char          m_replacement;

protected:
	static CWT_DEFAULT_MT_POLICY g_mutex;
};

CWT_NS_END

#endif /* __CWT_TEXTCODEC_HPP__ */
