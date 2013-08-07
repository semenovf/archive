/*
 * utf8codec.cpp
 *
 *  Created on: Jul 18, 2013
 *      Author: wladt
 */


#include "../include/cwt/io/utf8codec.hpp"
#include <cwt/utf8string.hpp>
#include <cwt/unicode.hpp>

CWT_NS_BEGIN

namespace io {

/*
template class Utf8UcsCodec<uint16_t, 0xffff>;
template class Utf8UcsCodec<uint32_t, 0x10ffff>;
*/

template ssize_t Ucs2Encoder::convert(dest_char_type output[], size_t osize, const orig_char_type input[], size_t isize, size_t * remain);
template ssize_t Ucs4Encoder::convert(dest_char_type output[], size_t osize, const orig_char_type input[], size_t isize, size_t * remain);
template ssize_t Ucs2Decoder::convert(dest_char_type output[], size_t osize, const orig_char_type input[], size_t isize, size_t * remain);
template ssize_t Ucs4Decoder::convert(dest_char_type output[], size_t osize, const orig_char_type input[], size_t isize, size_t * remain);

template <typename T, uint32_t MaxCP>
ssize_t UcsEncoder<T, MaxCP>::convert(T output[], size_t osize, const char input[], size_t isize, size_t * remain)
{
	CWT_ASSERT(remain);

	if (!osize || !isize)
		return 0;

	const char *pbytes = input;
	const char *pbytesEnd = input + isize;
	T *pchars = output;
	T *pcharsEnd = output + osize;
	*remain = isize;

	if (!m_headerDone) {
		if (isize == 1 && byte_t(input[0]) == 0xef) {
			return 0;
		} if (isize == 2 && byte_t(input[0]) == 0xef && byte_t(input[1]) == 0xbb) {
			return 0;
		} if (isize >= 3 && byte_t(input[0]) == 0xef && byte_t(input[1]) == 0xbb && byte_t(input[2]) == 0xbf) {
			// starts with a byte order mark
			pbytes += 3;
		}
	}
	m_headerDone = true;

	for (; pbytes < pbytesEnd && pchars < pcharsEnd; ++pbytes) {
		uint32_t uc = 0;
		uint32_t min_uc = 0; // for 'Overlong' encodings recognition
		int n = Utf8String::decodeBytes(pbytes, size_t(pbytesEnd - pbytes), uc, min_uc);

		// Code units too few
		if (n == -2) {
			break;
		}

		// Error, add replacement char, one byte will be skipped.
		if (n == -1) {
			*pchars++ = m_replacementChar;
			++m_invalidCount;
			continue;
		}

		// Skip the BOM
		if (uc == 0xFEFF) {
			pbytes += n; // n = 2 for BOM (0xEF, 0xBB, 0xBF)
			continue;
		}

		if ((uc < min_uc)                        // overlong sequence
				|| Unicode::isSurrogate(uc)      // UTF16 surrogate
				/*|| Unicode::isNonCharacter(uc)*/   // non-character
				|| uc > MaxCodePoint) {          // non-character

			// As for n == -1
			*pchars++ = m_replacementChar;
			++m_invalidCount;
			continue;
		}

		*pchars++ = T(uc);
		pbytes += n;
	}

	*remain = size_t(pbytesEnd - pbytes);

	return ssize_t(pchars - output);
}

template <typename T, uint32_t MaxCP>
ssize_t UcsDecoder<T, MaxCP>::convert(char output[], size_t osize, const T input[], size_t isize, size_t * remain)
{
	CWT_ASSERT(remain);

	char *pbytes = output;
	char *pbytesEnd = output + osize;
	const T *pchars = input;
	const T *pcharsEnd = input + isize;
	*remain = isize;

	if (!m_headerDone) {
		if (osize > 2) {
			*pbytes++ = 0xef;
			*pbytes++ = 0xbb;
			*pbytes++ = 0xbf;
			m_headerDone = true;
		} else {
			return 0;
		}
	}

	while (pbytes < pbytesEnd && pchars < pcharsEnd) {

		uint32_t ucs4 = uint32_t(*pchars);

	    if (/*Unicode::isNonCharacter(ucs4) || */ucs4 > MaxCodePoint) {
	    	*pbytes++ = m_replacementChar;
	        ++m_invalidCount;
	        continue;
	    } else {
			ssize_t n = Utf8String::encodeUcs4(pbytes, size_t(pbytesEnd - pbytes), ucs4);
			if (n < 0) // not enough space
				break;
			pbytes += n;
	    }
	    ++pchars;
	}

	*remain = size_t(pcharsEnd - pchars);
	return ssize_t(pbytes - output);
}


ssize_t Utf8NullDecoder::convert(char output[], size_t osize, const char input[], size_t isize, size_t * remain)
{
	CWT_ASSERT(remain);

	Utf8String s = Utf8String::fromUtf8(input, CWT_MIN(isize, osize), nullptr, & m_state);
	size_t r = s.size(); // size in bytes;
	CWT_ASSERT(r <= CWT_SSIZE_MAX);
	memcpy(output, s.data(), r);
	*remain = isize - r;
	return ssize_t(r);
}

ssize_t Utf8NullEncoder::convert(char output[], size_t osize, const char input[], size_t isize, size_t * remain)
{
	CWT_ASSERT(remain);

	Utf8String s = Utf8String::fromUtf8(input, CWT_MIN(isize, osize), nullptr, & m_state);
	size_t r = s.size(); // size in bytes;
	CWT_ASSERT(r <= CWT_SSIZE_MAX);
	memcpy(output, s.data(), r);
	*remain = isize - r;
	return ssize_t(r);
}

ssize_t Latin1Decoder::convert(char output[], size_t osize, const char input[], size_t isize, size_t * remain)
{
	CWT_ASSERT(remain);

	Utf8String s = Utf8String::fromLatin1(input, CWT_MIN(isize, osize), nullptr, & m_state);
	size_t r = s.size(); // size in bytes;
	CWT_ASSERT(r <= CWT_SSIZE_MAX);
	memcpy(output, s.data(), r);
	*remain = isize - r;
	return ssize_t(r);
}

} // namespace io

CWT_NS_END
