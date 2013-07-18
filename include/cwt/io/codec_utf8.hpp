/*
 * codec_utf8.hpp
 *
 *  Created on: Jul 18, 2013
 *      Author: wladt
 */

#ifndef __CWT_IO_CODEC_UTF8_HPP__
#define __CWT_IO_CODEC_UTF8_HPP__

#include <cwt/cwt.hpp>

CWT_NS_BEGIN

namespace io {

template <typename T, uint32_t NullChar, uint32_t ReplChar, uint32_t MaxCP>
class DLL_API Utf8UcsCodec
{
public:
	static const T Null            = T(NullChar);
	static const T ReplacementChar = T(ReplChar);
	static const T MaxCodePoint    = T(MaxCP);
public:
	typedef char orig_char_type;
	typedef T    dest_char_type;

	Utf8UcsCodec(T replacement = ReplacementChar)
		: m_replacementChar(replacement)
		, m_headerDone(false)
		, m_remainChar(MaxCodePoint)
		, m_invalidCount(0) {}

	ssize_t convert(dest_char_type output[], size_t osize, const orig_char_type input[], size_t isize, size_t * remain);
	bool isValid() const { return m_invalidCount == size_t(0); }

private:
	T      m_replacementChar;
	bool   m_headerDone;
	T      m_remainChar;
	size_t m_invalidCount;
};

typedef Utf8UcsCodec<uint16_t, 0x0000, 0xfffd, 0xffff>   Utf8Ucs2Codec;
typedef Utf8UcsCodec<uint32_t, 0x0000, 0xfffd, 0x10ffff> Utf8Ucs4Codec;

extern template ssize_t Utf8Ucs2Codec::convert(dest_char_type output[], size_t osize, const orig_char_type input[], size_t isize, size_t * remain);
extern template ssize_t Utf8Ucs4Codec::convert(dest_char_type output[], size_t osize, const orig_char_type input[], size_t isize, size_t * remain);

template <typename T>
class DLL_API UcsUtf8Codec
{
public:
	typedef T    orig_char_type;
	typedef char dest_char_type;

	UcsUtf8Codec(bool ignoreHeader = true, char replacement = '?')
		: m_ignoreHeader(ignoreHeader)
		, m_replacementChar(replacement)
		, m_headerDone(ignoreHeader ? true : false)
		, m_invalidCount(0) {}

	ssize_t convert(dest_char_type output[], size_t osize, const orig_char_type input[], size_t isize, size_t * remain);
	bool isValid() const { return m_invalidCount == size_t(0); }

private:
	bool    m_ignoreHeader;
	char    m_replacementChar;
	bool    m_headerDone;
	size_t  m_invalidCount;
};

typedef UcsUtf8Codec<uint16_t> Ucs2Utf8Codec;
typedef UcsUtf8Codec<uint32_t> Ucs4Utf8Codec;

extern template ssize_t Ucs2Utf8Codec::convert(dest_char_type output[], size_t osize, const orig_char_type input[], size_t isize, size_t * remain);
extern template ssize_t Ucs4Utf8Codec::convert(dest_char_type output[], size_t osize, const orig_char_type input[], size_t isize, size_t * remain);

} // namespace io

CWT_NS_END


#endif /* __CWT_IO_CODEC_UTF8_HPP__ */
