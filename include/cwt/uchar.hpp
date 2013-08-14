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
	static const uint32_t MaxCodePoint = 0x10ffff;
	static const uint32_t Null = 0x0000;

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
	static bool isLowSurrogate(const UChar & ucs4) { return ucs4 >= 0xdc00 && ucs4 <= 0xdfff; }

	/* High Surrogates: D800-DBFF
	 * Qt implementation: (ucs4 & 0xfffffc00) == 0xd800
	 * */
	static bool isHiSurrogate(const UChar & ucs4)      { return ucs4 >= 0xd800 && ucs4 <= 0xdbff; }
	static bool isSurrogate(const UChar & ucs4)        { return (ucs4 - 0xd800u < 2048u); }
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
