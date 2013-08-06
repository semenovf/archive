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
class DLL_API Utf8UcsCodec
{
public:
	static const T Null            = T(0x0000);
	static const T ReplacementChar = T(0xfffd);
	static const T MaxCodePoint    = MaxCP;
public:
	typedef char      orig_char_type;
	typedef T         dest_char_type;
	typedef Vector<T> vector_type;

	Utf8UcsCodec(T replacement = ReplacementChar)
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

typedef Utf8UcsCodec<uint16_t, 0xffff>   Utf8Ucs2Codec;
typedef Utf8UcsCodec<uint32_t, 0x10ffff> Utf8Ucs4Codec;

extern template ssize_t Utf8Ucs2Codec::convert(dest_char_type output[], size_t osize, const orig_char_type input[], size_t isize, size_t * remain);
extern template ssize_t Utf8Ucs4Codec::convert(dest_char_type output[], size_t osize, const orig_char_type input[], size_t isize, size_t * remain);

template <typename T, uint32_t MaxCP>
class DLL_API UcsUtf8Codec
{
public:
	static const T MaxCodePoint        = MaxCP;
	static const char ReplacementChar  = '?';
public:
	typedef T          orig_char_type;
	typedef char       dest_char_type;
	typedef Utf8String vector_type;

	UcsUtf8Codec(bool ignoreHeader = true, char replacement = ReplacementChar)
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

typedef UcsUtf8Codec<uint16_t, 0xffff>   Ucs2Utf8Codec;
typedef UcsUtf8Codec<uint32_t, 0x10ffff> Ucs4Utf8Codec;

extern template ssize_t Ucs2Utf8Codec::convert(dest_char_type output[], size_t osize, const orig_char_type input[], size_t isize, size_t * remain);
extern template ssize_t Ucs4Utf8Codec::convert(dest_char_type output[], size_t osize, const orig_char_type input[], size_t isize, size_t * remain);


class DLL_API Utf8BytesConverter
{
/*
public:
	typedef char orig_char_type;
	typedef char dest_char_type;
*/

protected:
	Utf8BytesConverter(char replacement = Utf8String::ReplacementChar) : m_state() { m_state.replacementChar = replacement; }

public:
	virtual ~Utf8BytesConverter() {}
	virtual ssize_t convert(char output[], size_t osize, const char input[], size_t isize, size_t * remain) = 0;
	bool isValid() const { return m_state.invalidChars == 0; }

protected:
	Utf8String::ConvertState m_state;
};

class DLL_API Utf8Utf8Converter : public Utf8BytesConverter
{
public:
	Utf8Utf8Converter(char replacement = Utf8String::ReplacementChar) : Utf8BytesConverter(replacement) { }
	virtual ssize_t convert(char output[], size_t osize, const char input[], size_t isize, size_t * remain);
};

/*
class DLL_API Utf8Latin1Converter : public Utf8BytesConverter
{
public:
	Utf8Latin1Converter(char replacement = Utf8String::ReplacementChar) : Utf8BytesConverter(replacement) { }
	virtual ssize_t convert(char output[], size_t osize, const char input[], size_t isize, size_t * remain);
};
*/

class DLL_API Latin1Utf8Converter : public Utf8BytesConverter
{
public:
	Latin1Utf8Converter(char replacement = Utf8String::ReplacementChar) : Utf8BytesConverter(replacement) { }
	virtual ssize_t convert(char output[], size_t osize, const char input[], size_t isize, size_t * remain);
};

class DLL_API Utf8Codec
{
public:
	typedef char orig_char_type;
	typedef char dest_char_type;
	typedef Utf8String vector_type;

	Utf8Codec(Utf8BytesConverter * converter) : m_converter(converter) {}

	ssize_t convert(dest_char_type output[], size_t osize, const orig_char_type input[], size_t isize, size_t * remain)
	{
		return m_converter->convert(output, osize, input, isize, remain);
	}
	bool isValid() const { return m_converter->isValid(); }

private:
	Utf8BytesConverter * m_converter;
};

} // namespace io

CWT_NS_END

#endif /* __CWT_UTF8CODEC_HPP__ */
