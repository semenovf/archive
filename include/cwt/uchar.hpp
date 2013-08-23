/**
 * uchar.hpp
 *
 *  Created on: Aug 14, 2013
 *      Author: wladt
 *
 * @brief Unicode code point
 */

#ifndef __CWT_UCHAR_HPP__
#define __CWT_UCHAR_HPP__

#include <cwt/cwt.hpp>

CWT_NS_BEGIN

//typedef uint32_t UChar;

class DLL_API UChar
{
public:
	static const uint32_t MaxCodePoint    = 0x10ffff;
	static const uint32_t Null            = 0x0000;
	static const uint32_t ReplacementChar = uint32_t(0x0000FFFD);
	static const uint32_t MaxBMP          = uint32_t(0x0000FFFF);

	static const uint32_t HiSurrogateStart  = uint32_t(0xD800);
	static const uint32_t HiSurrogateEnd    = uint32_t(0xDBFF);
	static const uint32_t LowSurrogateStart = uint32_t(0xDC00);
	static const uint32_t LowSurrogateEnd   = uint32_t(0xDFFF);

public:
	UChar() : m_value(Null) {}
	UChar(char latin1) : m_value(latin1) { CWT_ASSERT(byte_t(latin1) <= 127); }
	UChar(uint32_t ucs4) : m_value(ucs4) { }
	UChar(const UChar & ucs4) : m_value(ucs4.m_value) { }
	operator uint32_t () const { return m_value; }
	operator uint32_t & () { return m_value; }
	uint32_t value () const { return m_value; }
	uint32_t ucs4 () const { return m_value; }
	uint32_t unicode () const { return m_value; }

	bool isSpace() const;

	/* Low Surrogates: DC00-DFFF
	 * Qt implementation: (ucs4 & 0xfffffc00) == 0xdc00;
	 * */
	static bool isLowSurrogate(const UChar & ucs4) { return ucs4 >= LowSurrogateStart && ucs4 <= LowSurrogateEnd; }

	/* High Surrogates: D800-DBFF
	 * Qt implementation: (ucs4 & 0xfffffc00) == 0xd800
	 * */
	static bool isHiSurrogate(const UChar & ucs4)      { return ucs4 >= HiSurrogateStart && ucs4 <= HiSurrogateEnd; }
	static bool isSurrogate(const UChar & ucs4)        { return (ucs4 - HiSurrogateStart < 2048u); }
	static bool isValid(const UChar & ch, const UChar & min_uc = UChar());

private:
	uint32_t m_value;
};

inline bool UChar::isValid(const UChar & ch, const UChar & min_uc)
{
	return (ch >= min_uc) // overlong
			&& (ch != 0xFEFF) // the BOM
			&& (!isSurrogate(ch))
			&& (ch <= MaxCodePoint);
}

CWT_NS_END

#endif /* __CWT_UCHAR_HPP__ */
