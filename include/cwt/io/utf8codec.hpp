/*
 * codec_utf8.hpp
 *
 *  Created on: Jul 18, 2013
 *      Author: wladt
 */

#ifndef __CWT_UTF8CODEC_HPP__
#define __CWT_UTF8CODEC_HPP__

#include <cwt/unicode.hpp>
#include <cwt/utf8string.hpp>
#include <cwt/shared_ptr.hpp>
#include <cwt/vector.hpp>

CWT_NS_BEGIN

namespace io {

template <typename T, uint32_t MaxCP>
class DLL_API UcsEncoder
{
public:
	static const T Null            = T(0x0000);
	static const T ReplacementChar = T(0xfffd);
	static const T MaxCodePoint    = MaxCP;
public:
	typedef char      orig_char_type;
	typedef T         dest_char_type;
	typedef Vector<T> vector_type;

	UcsEncoder(T replacement = ReplacementChar)
		: m_replacementChar(replacement)
		, m_headerDone(false)
		, m_invalidCount(0) {}

	ssize_t convert(dest_char_type output[], size_t osize, const orig_char_type input[], size_t isize, size_t * remain);
	bool isValid() const { return m_invalidCount == size_t(0); }
	size_t invalidCount() { return m_invalidCount; }

private:
	T      m_replacementChar;
	bool   m_headerDone;
	size_t m_invalidCount;
};

typedef UcsEncoder<uint16_t, 0xffff>   Ucs2Encoder;
typedef UcsEncoder<uint32_t, 0x10ffff> Ucs4Encoder;

extern template ssize_t Ucs2Encoder::convert(dest_char_type output[], size_t osize, const orig_char_type input[], size_t isize, size_t * remain);
extern template ssize_t Ucs4Encoder::convert(dest_char_type output[], size_t osize, const orig_char_type input[], size_t isize, size_t * remain);

template <typename T, uint32_t MaxCP>
class DLL_API UcsDecoder
{
public:
	static const T MaxCodePoint        = MaxCP;
	static const char ReplacementChar  = '?';
public:
	typedef T          orig_char_type;
	typedef char       dest_char_type;
	typedef Utf8String vector_type;

	UcsDecoder(bool ignoreHeader = true, char replacement = ReplacementChar)
		: m_ignoreHeader(ignoreHeader)
		, m_replacementChar(replacement)
		, m_headerDone(ignoreHeader ? true : false)
		, m_invalidCount(0) {}

	ssize_t convert(dest_char_type output[], size_t osize, const orig_char_type input[], size_t isize, size_t * remain);
	bool isValid() const { return m_invalidCount == size_t(0); }
	size_t invalidCount() { return m_invalidCount; }

private:
	bool    m_ignoreHeader;
	char    m_replacementChar;
	bool    m_headerDone;
	size_t  m_invalidCount;
};

typedef UcsDecoder<uint16_t, 0xffff>   Ucs2Decoder;
typedef UcsDecoder<uint32_t, 0x10ffff> Ucs4Decoder;

extern template ssize_t Ucs2Decoder::convert(dest_char_type output[], size_t osize, const orig_char_type input[], size_t isize, size_t * remain);
extern template ssize_t Ucs4Decoder::convert(dest_char_type output[], size_t osize, const orig_char_type input[], size_t isize, size_t * remain);


class DLL_API Utf8Decoder
{
public:
	typedef char orig_char_type;
	typedef char dest_char_type;
	typedef Utf8String vector_type;

public:
	Utf8Decoder(UChar replacement = UChar::ReplacementChar) : m_state() { m_state.replacementChar = replacement; }
	virtual ~Utf8Decoder() {}
	virtual ssize_t convert(char output[], size_t osize, const char input[], size_t isize, size_t * remain);
	bool isValid() const { return m_state.invalidChars == 0; }

protected:
	Utf8String::ConvertState m_state;
};


class DLL_API Utf8Encoder
{
public:
	typedef char orig_char_type;
	typedef char dest_char_type;
	typedef ByteArray vector_type;

public:
	Utf8Encoder() : m_state() { m_state.replacementChar = UChar::ReplacementChar; }
	Utf8Encoder(char replacement) : m_state() { m_state.replacementChar = replacement; }
	virtual ~Utf8Encoder() {}
	virtual ssize_t convert(char output[], size_t osize, const char input[], size_t isize, size_t * remain);
	bool isValid() const { return m_state.invalidChars == 0; }
protected:
	Utf8String::ConvertState m_state;
};


typedef Utf8Decoder Utf8NullDecoder;

/*class DLL_API Utf8NullDecoder : public Utf8Decoder
{
public:
	Utf8NullDecoder(char replacement = Utf8String::ReplacementChar) : Utf8Decoder(replacement) { }
	virtual ssize_t convert(char output[], size_t osize, const char input[], size_t isize, size_t * remain);
};*/

class DLL_API Latin1Decoder : public Utf8Decoder
{
public:
	Latin1Decoder(UChar replacement = UChar::ReplacementChar) : Utf8Decoder(replacement) { }
	virtual ssize_t convert(char output[], size_t osize, const char input[], size_t isize, size_t * remain);
};

typedef Utf8Encoder Utf8NullEncoder;
/*
class DLL_API Utf8NullEncoder : public Utf8Encoder
{
public:
	Utf8NullEncoder(char replacement = Utf8String::ReplacementChar) : Utf8Encoder(replacement) { }
	virtual ssize_t convert(char output[], size_t osize, const char input[], size_t isize, size_t * remain);
};
*/

} // namespace io

CWT_NS_END

#endif /* __CWT_UTF8CODEC_HPP__ */
